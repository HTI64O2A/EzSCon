

#include <WebSockets.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

struct KV {
	String key;
	void (*func)();
};

class LazyMap {
private:
	int count = 0;
	KV funcs[];
public:
	void addEvent(String k, void (*function)(String));
	bool call(String key, String val);
};

void LazyMap::addEvent(String k, void (*function)(String)) {
	int n = this->count++;
	this->funcs[n].key = k;
	this->funcs[n].func = function;
}

bool LazyMap::call(String key, String val) {
	for(int i=0; i < this->count; i++) {
		if(this->funcs[i].key == key) {
			this->funcs[i].func(val);
			return true;
		}
	}
	return false;
}

class EZSCON {
private:
	LazyMap map;
	WebSocketsClient webSocket;
	StaticJsonBuffer<200> buf;
public:
	void setup(const char[] host, int port);
	void loop();
	void addEvent(String k, void (*function)(String));
	void eventHandler(WStype_t type, uint8_t * payload, size_t length);
	void dispacher(uint8_t *payload);
	void sendMessage(String event, String message);
	//void sendMessage(const char* event, const char* message);
}

void EZSCON::setup(const char[] host, int port) {
	this->webSocket.begin(host, port);
	this->webSocket.onEvent(this->eventHandler);
	this->webSocket.setReconnectInterval(5000);
}

void EZSCON::loop() {
	this->webSocket.loop();
}

void addEvent(String k, void (*function)(String)) {
	this->map.addEvent(k, function);
}

void EZSCON::eventHandler(WStype_t type, uint8_t * payload, size_t length) {
	switch(type) {
		case WStype_TEXT:
			Serial.println("Conencted");
			this->dispacher(payload);
			break;
		// TODO other handling.
		case default:
			break;
	}
}

void dispacher(uint8_t *payload) {
	char* json = (char*)payload;
	JsonObject& object = this->buf.parseObject(json);
	
	String event = object.get<String>("event");
	String value = object.get<String>("value");

	this->map.call(event, value);
}

void sendMessage(String event, String message) {
	JsonObject& root = this->buf.createObject();
	String json;
	root["event"] = event;
	root["message"] = Message;
	root.printTo(json);
	this->webSocket.sendTXT(json);
}
