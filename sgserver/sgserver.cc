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
#include "sgserver.h"
#include "messaging.h"



void sgServerUninit(ServerInstance_t *ServerInstance)
{
  if (ServerInstance && ServerInstance->SocketDescriptor != -1)
    {
      printf("Closing socket %d\n", ServerInstance->SocketDescriptor);
      close(ServerInstance->SocketDescriptor);
    }
}

void sgServerError(ServerInstance_t *ServerInstance, const char *Message)
{
  perror(Message);

  sgServerUninit(ServerInstance);

  exit(EXIT_FAILURE);
}

void sgServerPrintUsage(void)
{
  printf("Usage: sgserver [port]\n");
  printf(" port: %hu-%hu\n", PORT_MIN, PORT_MAX);
  printf(" If port is not given, uses default port %hu\n\n", DEFAULT_PORT);
}

bool sgServerInit(ServerInstance_t *ServerInstance)
{
  if (ServerInstance)
    {
      struct sockaddr_in ServerAddr;

      ServerInstance->SocketDescriptor = socket(AF_INET, SOCK_DGRAM,
                                               IPPROTO_UDP);

      if (ServerInstance->SocketDescriptor == -1)
        {
          perror("Could not open socket\n");
          return false;
        }

      printf("Opened socket %d\n", ServerInstance->SocketDescriptor);

      memset(&ServerAddr, 0, sizeof(ServerAddr));

      ServerAddr.sin_family = AF_INET;
      ServerAddr.sin_port = htons(ServerInstance->Port);
      ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

      if (bind(ServerInstance->SocketDescriptor,
               (struct sockaddr *) &ServerAddr,
               sizeof(ServerAddr)) == -1)
        {
          perror("Could not bind socket\n");
          return false;
        }

      return true;
    }
  else
    {
      perror("Unknown server instance");
    }

  return false;
}

bool sgServerHandleMsg(ServerInstance_t *ServerInstance, char *Buffer,
                       const uint8_t Bytes, struct sockaddr *ClientAddr,
                       size_t Len)
{
  if (ServerInstance && Buffer && ClientAddr && Bytes >= sizeof(SgSimpleMsg_t))
    {
      SgSimpleMsg_t *pMsg = (SgSimpleMsg_t *) Buffer;

      switch (pMsg->MsgId)
        {
        case MSG_PING:
          {
            // PING received, send PONG back
            size_t Sent = sgSimpleMessage(Buffer, MSG_PONG, NULL, 0);

            printf("Received PING request\n");

            if (sendto(ServerInstance->SocketDescriptor, Buffer, Sent,
                       0, ClientAddr, Len) == -1)
              {
                perror("Unable to send reply\n");
              }
            else
              {
                return true;
              }
          }
          break;

        default:
          printf("Unknown message %u received\n", pMsg->MsgId);
          return true;
        }
    }
  else
    {
      perror("Unable to handle message\n");
    }

  return false;
}

bool sgServerValidateArguments(uint16_t Port)
{
  bool valid = true;

  if (Port < PORT_MIN)
    {
      valid = false;
    }

  return valid;
}

void sgServer(ServerInstance_t *ServerInstance)
{
  if (ServerInstance && ServerInstance->SocketDescriptor != -1)
    {
      struct sockaddr_in ClientAddr;
      socklen_t Len = sizeof(ClientAddr);
      char Buffer[MSG_MAX];

      printf("Waiting for data\n");

      for (;;)
        {
          memset(&ClientAddr, 0, Len);

          ssize_t Bytes = recvfrom(ServerInstance->SocketDescriptor, Buffer,
                                   MSG_MAX, 0, (struct sockaddr *) &ClientAddr,
                                   &Len);

          if (Bytes == -1)
            {
              sgServerError(ServerInstance, "Could not receive data\n");
            }

          if (!sgServerHandleMsg(ServerInstance, Buffer, Bytes,
                                 (struct sockaddr *) &ClientAddr, Len))
            {
              sgServerError(ServerInstance, "Message handling error\n");
            }
        }
    }
}
