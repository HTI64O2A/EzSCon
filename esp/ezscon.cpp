
#include <WebSockets.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

typedef void (* CB_POINTER)(String); 

struct KV {
	String key;
	CB_POINTER func;
};

class LazyMap {
private:
	int count = 0;
	KV funcs[];
public:
	void addEvent(String k, CB_POINTER);
	bool call(String key, String val);
};

void LazyMap::addEvent(String k, CB_POINTER function) {
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
	void setup(const char host[], int port);
	void loop();
	void addEvent(String k, CB_POINTER function);
	void eventHandler(WStype_t type, uint8_t * payload, size_t length);
	void dispacher(uint8_t *payload);
	template<typename T>
	void sendMessage(String event, T message);
	//void sendMessage(const char* event, const char* message);
};

void EZSCON::setup(const char host[], int port) {
	this->webSocket.begin(host, port);
	this->webSocket.onEvent([&](WStype_t type, uint8_t * payload, size_t length) { this->eventHandler(type, payload, length); });
	this->webSocket.setReconnectInterval(5000);
}

void EZSCON::loop() {
	this->webSocket.loop();
}

void EZSCON::addEvent(String k, CB_POINTER function) {
	this->map.addEvent(k, function);
}

void EZSCON::eventHandler(WStype_t type, uint8_t * payload, size_t length) {
	switch(type) {
		case WStype_TEXT:
			Serial.println("Conencted");
			this->dispacher(payload);
			break;
		// TODO other handling.
		default:
			break;
	}
}

void EZSCON::dispacher(uint8_t *payload) {
	char* json = (char*)payload;
	JsonObject& object = this->buf.parseObject(json);
	
	String event = object.get<String>("event");
	String value = object.get<String>("value");

	this->map.call(event, value);
}

template<typename T>
void EZSCON::sendMessage(String event, T message) {
	JsonObject& root = this->buf.createObject();
	String json;
	root["event"] = event;
	root["message"] = message;
	root.printTo(json);
	this->webSocket.sendTXT(json);
}
