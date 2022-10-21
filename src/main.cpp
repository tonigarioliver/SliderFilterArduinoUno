#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetServers.h>
#include "define.h"
#include "Functions.h"

int motion_status;
int next_position = 0;
String *query;
boolean *newData;
////////////////Ethernet MAC for DHCP
uint8_t mac[] = {0xA8, 0x61, 0x0A, 0xAE, 0x7B, 0x79};
byte ip[] = {192, 168, 201, 27};
byte buffersize = 127;
EthernetServer servers[] = {EthernetServer(80), EthernetServer(8080), EthernetServer(22)};
int numServers = sizeof(servers) / sizeof(servers[0]);
EthernetServers Servers(buffersize, numServers);
String *serveroutput;

void setup()
{
  // put your setup code here, to run once:
  pinMode(inMotion, INPUT_PULLUP);
  pinMode(backward, OUTPUT);
  pinMode(forward, OUTPUT);
  pinMode(mode, OUTPUT);
  digitalWrite(backward, HIGH);
  digitalWrite(forward, HIGH);
  digitalWrite(mode, HIGH);
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  delay(3000);
  Servers.startServers(servers, mac, ip);
  delay(2000);
}

void loop()
{
  // put your main code here, to run repeatedly:
  motion_status = digitalRead(inMotion);
  next_position = 0;
  // put your main code here, to run repeatedly:
  query = Servers.servermsgreceive(servers);
  if (Servers.existnewMessage())
  {
    delete[] serveroutput;
    serveroutput = new String[numServers];
    parseResponse(numServers, query, serveroutput, &motion_status, &next_position);
    Servers.sendreply(servers, serveroutput);
    if (next_position == 0)
    {
      digitalWrite(backward, LOW);
      digitalWrite(forward, HIGH);
    }
    else
    {
      digitalWrite(backward, HIGH);
      digitalWrite(forward, LOW);
    }
  }
  digitalWrite(13, motion_status);
}