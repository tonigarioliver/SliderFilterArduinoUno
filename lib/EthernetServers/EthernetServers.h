#include <Arduino.h>
#include <Ethernet.h>

class EthernetServers
{
private:
    char *receivedChars;
    byte bufferSize = 127;
    boolean *newData;
    char *command = NULL;
    int numServers = 0;
    String *query;

public:
    EthernetServers(byte buffersize, int numservers);

    // getters and setters for server class
    boolean *getnewData();

    // methods
    void startServers(EthernetServer servers[], uint8_t macAddress[], byte ipAddress[]);
    String *servermsgreceive(EthernetServer servers[]);
    boolean existnewMessage();
    void sendreply(EthernetServer servers[], String reply[]);
};