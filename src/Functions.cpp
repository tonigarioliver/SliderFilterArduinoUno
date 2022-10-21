#include <Arduino.h>
#include <Functions.h>

void parseResponse(int numServers,String queries[], String server_output[], int *motionstatus,int *nextposition)
{
  for (int i = 0; i < numServers; i++)
  {
    char receivedChars[queries[i].length() + 1];
    strcpy(receivedChars, queries[i].c_str());
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
          server_output[i] = "<get,slider," + (String(num)) + ",status," + String(*motionstatus) + ">" + "\r\n";
        }
        else if (feature == "position")
        {
          server_output[i] = "<get,slider," + (String(num)) + ",position," + String(*nextposition) + ">" + "\r\n";
        }
      }
    }
    else if (action == "set")
    {
      if (parameter == "slider")
      {
        command = strtok(NULL, ",");
        *nextposition = String(command).toInt();
        server_output[i] = "<set,slider," + (String(num)) + ",position," + String(*nextposition) + ">" + "\r\n";
      }
    }
  }
}