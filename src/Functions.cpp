#include <Arduino.h>
#include <Functions.h>

void parseResponse(int numServers,String query[], String serveroutput[], int motion_status,int next_position)
{
  for (int i = 0; i < numServers; i++)
  {
    char receivedChars[query[i].length() + 1];
    strcpy(receivedChars, query[i].c_str());
    char *command = strtok(receivedChars, ",");
    String action = String(command);
    command = strtok(NULL, ",");
    String parameter = String(command);
    command = strtok(NULL, ",");
    int num = String(command).toInt();
    command = strtok(NULL, ",");
    String feature = String(command);
    if (action == "get")
    {
      command = strtok(NULL, ",");
      if (parameter == "slider")
      {
        if (feature == "status")
        {
          serveroutput[i] = "<get,slider," + (String(num)) + ",status," + String(motion_status) + ">" + "\r\n";
        }
        else if (feature == "position")
        {
          serveroutput[i] = "<get,slider," + (String(num)) + ",position," + String(next_position) + ">" + "\r\n";
        }
      }
    }
    else if (action == "set")
    {
      if (parameter == "slider")
      {
        command = strtok(NULL, ",");
        next_position = String(command).toInt();
        serveroutput[i] = "<set,slider," + (String(num)) + ",position," + String(next_position) + ">" + "\r\n";
      }
    }
  }
}