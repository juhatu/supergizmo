/**
   \file main.cc
   SuperGizmo server implementation entry point.
*/

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "sgserver.h"
#include "messaging.h"

/**
   \brief Main routine for the SuperGizmo server.
*/
int main(int argc, char **argv)
{
  ServerInstance_t ServerInstance;

  memset(&ServerInstance, 0, sizeof(ServerInstance));
  ServerInstance.SocketDescriptor = -1;

  if (argc == 1)
    {
      ServerInstance.Port = DEFAULT_PORT;
    }
  else if (argc != 2)
    {
      sgServerPrintUsage();

      return EXIT_SUCCESS;
    }
  else
    {
      uint16_t Port;

      // Validate arguments
      if (sscanf(argv[1], ARG_FMT, &Port) != 1)
        {
          sgServerError(&ServerInstance, "Invalid argument\n");
        }

      if (!sgServerValidateArguments(Port))
        {
          sgServerError(&ServerInstance, "Invalid port value\n");
        }

      // Store port
      ServerInstance.Port = Port;
    }

  if (sgServerInit(&ServerInstance))
    {
      sgServer(&ServerInstance);
    }

  sgServerUninit(&ServerInstance);

  return EXIT_SUCCESS;
}
