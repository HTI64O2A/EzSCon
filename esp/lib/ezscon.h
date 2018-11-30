
#ifdef EZCON_H
#define EZCON_H


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

#endif //EZCON_H