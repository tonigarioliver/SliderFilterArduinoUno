#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetServers.h>

EthernetServers::EthernetServers(byte buffersize, int numServers)
{
    this->bufferSize = buffersize;
    this->numServers = numServers;

    delete receivedChars;
    this->receivedChars = new char[buffersize];

    delete newData;
    this->newData = new boolean[numServers];

    delete query;
    this->query = new String[numServers];
}

void EthernetServers::startServers(EthernetServer servers[], uint8_t macAddress[], byte ipAddress[])
{
    Ethernet.begin(macAddress, ipAddress);
    delay(1000);
    Serial.print(F("IP address: "));
    Serial.println(Ethernet.localIP());
    for (int i = 0; i < numServers; i++)
    {
        servers[i].begin();
        Serial.println("Server num: " + String(i) + " ready");
    }
}

String *EthernetServers::servermsgreceive(EthernetServer servers[])
{
    for (int i = 0; i < numServers; i++)
    {
        static boolean recvInProgress = false;
        static byte ndx = 0;
        char startMarker = '<';
        char endMarker = '>';
        char rc;
        EthernetClient client = servers[i].available();
        while (client.available() > 0 && newData[i] == false)
        {
            rc = client.read();
            Serial.print(rc);
            if (recvInProgress == true)
            {
                if (rc != endMarker)
                {
                    receivedChars[ndx] = rc;
                    ndx++;
                    if (ndx >= bufferSize)
                    {
                        ndx = bufferSize - 1;
                    }
                }
                else
                {
                    receivedChars[ndx] = '\0'; // terminate the string
                    query[i] = String(receivedChars);
                    recvInProgress = false;
                    ndx = 0;
                    newData[i] = true;
                }
            }
            else if (rc == startMarker)
            {
                recvInProgress = true;
            }
        }
    }
    return query;
}

boolean EthernetServers::existnewMessage()
{
    boolean exist = false;
    for (int i = 0; i < numServers; i++)
    {
        if (newData[i] == true)
        {
            exist = true;
        }
    }
    return exist;
}

void EthernetServers::sendreply(EthernetServer servers[], String reply[])
{
    if (existnewMessage())
    {
        for (int i = 0; i < numServers; i++)
        {
            EthernetClient client = servers[i].available();
            if (newData[i] == true)
            {
                client.print(reply[i]);
                newData[i] = false;
            }
        }
    }
}