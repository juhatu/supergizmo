/**
   \file sgserver.cc
   SuperGizmo server implementation.
*/

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "sgserver_priv.h"
#include "messaging.h"

static ServerInstance_t sServerInstance;

/**
   \brief Uninitialize server instance.
*/
static void sgServerUninit(void)
{
  if (sServerInstance.SocketDescriptor != -1)
    {
      printf("Closing socket %d\n", sServerInstance.SocketDescriptor);
      close(sServerInstance.SocketDescriptor);
    }
}

/**
   \brief Print error message and exit.
*/
static void sgServerError(const char *Message)
{
  perror(Message);

  sgServerUninit();

  exit(EXIT_FAILURE);
}

/**
   \brief Print help message
*/
static void sgServerPrintUsage(void)
{
  printf("Usage: sgserver [port]\n");
  printf(" port: %hu-%hu\n", PORT_MIN, PORT_MAX);
  printf(" If port is not given, uses default port %hu\n\n", DEFAULT_PORT);
}

/**
   \brief Initialize server instance and socket.
*/
static int sgServerInit(void)
{
  struct sockaddr_in ServerAddr;

  int SocketDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (SocketDescriptor == -1)
    {
      sgServerError("Could not open socket\n");
    }

  printf("Opened socket %d\n", SocketDescriptor);

  memset(&ServerAddr, 0, sizeof(ServerAddr));

  ServerAddr.sin_family = AF_INET;
  ServerAddr.sin_port = htons(sServerInstance.Port);
  ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(SocketDescriptor, (struct sockaddr *) &ServerAddr,
           sizeof(ServerAddr)) == -1)
    {
      sgServerError("Could not bind socket\n");
    }

  return SocketDescriptor;
}

/**
   \brief Handle received message from client.

   \param[in] Buffer Buffer containing the message to send/receive
   \param[in] Bytes Size of received message
   \param[in] ClientAddr Socket address of client
   \param[in] Len Size of the socke address
*/
static void sgServerHandleMsg(char *Buffer, const uint8_t Bytes,
                              struct sockaddr *ClientAddr, size_t Len)
{
  if (Buffer && Bytes >= sizeof(SgSimpleMsg_t))
    {
      SgSimpleMsg_t *pMsg = (SgSimpleMsg_t *) Buffer;

      switch (pMsg->MsgId)
        {
        case MSG_PING:
          {
            // PING received, send PONG back
            size_t Sent = sgSimpleMessage(Buffer, MSG_PONG, NULL, 0);

            printf("Received PING request\n");

            if (sendto(sServerInstance.SocketDescriptor, Buffer, Sent,
                       0, ClientAddr, Len) == -1)
              {
                sgServerError("Unable to send reply\n");
              }
          }
          break;

        default:
          sgServerError("Unknown message received\n");
        }
    }
  else
    {
      sgServerError("Invalid data received\n");
    }
}

/**
   \brief Validate command line arguments.

   \return TRUE if arguments are valid
*/
static bool sgServerValidateArguments(uint16_t Port)
{
  bool valid = true;

  if (Port < PORT_MIN)
    {
      valid = false;
    }

  return valid;
}

/**
   \brief Main message parsing loop.
*/
static void sgServer(void)
{
  if (sServerInstance.SocketDescriptor != -1)
    {
      struct sockaddr_in ClientAddr;
      socklen_t Len = sizeof(ClientAddr);
      char Buffer[MSG_MAX];

      printf("Waiting for data\n");

      for (;;)
        {
          memset(&ClientAddr, 0, Len);

          ssize_t Bytes = recvfrom(sServerInstance.SocketDescriptor, Buffer,
                                   MSG_MAX, 0, (struct sockaddr *) &ClientAddr,
                                   &Len);

          if (Bytes == -1)
            {
              sgServerError("Could not receive data\n");
            }

          sgServerHandleMsg(Buffer, Bytes, (struct sockaddr *) &ClientAddr,
                            Len);
        }
    }
}

/**
   \brief Main routine for the SuperGizmo server.
*/
int main(int argc, char **argv)
{
  if (argc == 1)
    {
      sServerInstance.Port = DEFAULT_PORT;
    }
  else if (argc != 2)
    {
      sgServerPrintUsage();

      return EXIT_SUCCESS;
    }
  else
    {
      uint16_t Port;

      memset(&sServerInstance, 0, sizeof(sServerInstance));

      // Validate arguments
      if (sscanf(argv[1], ARG_FMT, &Port) != 1)
        {
          sgServerError("Invalid argument\n");
        }

      if (!sgServerValidateArguments(Port))
        {
          sgServerError("Invalid port value\n");
        }

      // Store port
      sServerInstance.Port = Port;
    }

  sServerInstance.SocketDescriptor = sgServerInit();

  sgServer();

  sgServerUninit();

  return EXIT_SUCCESS;
}
