/**
   \file sgclient.cc
   SuperGizmo client implementation.
*/

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "sgclient.h"
#include "messaging.h"


void sgClientUninit(ClientInstance_t *ClientInstance)
{
  if (ClientInstance && ClientInstance->SocketDescriptor != -1)
    {
      printf("Closing socket %d\n", ClientInstance->SocketDescriptor);
      close(ClientInstance->SocketDescriptor);
    }
}

void sgClientError(ClientInstance_t *ClientInstance, const char *message)
{
  perror(message);

  sgClientUninit(ClientInstance);

  exit(EXIT_FAILURE);
}

void sgClientPrintUsage(void)
{
  printf("Usage: sgclient host:port\n");
  printf(" host: Valid IPv4 address, for example %s\n", DEFAULT_HOST);
  printf(" port: %hu-%hu\n\n", PORT_MIN, PORT_MAX);
}

bool sgClientInit(ClientInstance_t *ClientInstance)
{
  if (ClientInstance)
    {
      ClientInstance->SocketDescriptor = socket(AF_INET, SOCK_DGRAM,
                                                IPPROTO_UDP);

      if (ClientInstance->SocketDescriptor == -1)
        {
          perror("Could not open socket\n");
          return false;
        }

      printf("Opened socket %d\n", ClientInstance->SocketDescriptor);

      return true;
    }

  return false;
}

bool sgClientHandleMsg(char *Buffer, const uint8_t Bytes)
{
  bool Success = false;

  if (Buffer && Bytes >= sizeof(SgSimpleMsg_t))
    {
      SgSimpleMsg_t *pMsg = (SgSimpleMsg_t *) Buffer;

      switch (pMsg->MsgId)
        {
        case MSG_PONG:
          {
            printf("Received PONG reply\n");
            Success = true;
          }
          break;

        default:
          printf("Unknown message received\n");
        }
    }
  else
    {
      printf("Invalid data received\n");
    }

  return Success;
}

bool sgClientValidateArguments(uint16_t A, uint16_t B, uint16_t C,
                               uint16_t D, uint16_t Port)
{
  bool valid = true;

  if (A > 255 || B > 255 || C > 255 || D > 255 ||
      Port < PORT_MIN)
    {
      valid = false;
    }

  return valid;
}

/**
   \brief Main message handler.

   \param[in] ClientInstance Pointer to client instance

   \return TRUE if message exchange was successful.
*/
bool sgClient(ClientInstance_t *ClientInstance)
{
  bool Connected = false;

  if (ClientInstance && ClientInstance->SocketDescriptor != -1)
    {
      struct sockaddr_in ServerAddr;
      socklen_t Len = sizeof(ServerAddr);
      struct timeval TimeVal;
      char Buffer[MSG_MAX];
      size_t Sent = sgSimpleMessage(Buffer, MSG_PING, NULL, 0);
      ssize_t Bytes;

      printf("Sending PING request\n");

      memset(&ServerAddr, 0, Len);

      memset(&ServerAddr, 0, sizeof(ServerAddr));

      ServerAddr.sin_family = AF_INET;
      ServerAddr.sin_port = htons(ClientInstance->TargetPort);

      if (inet_aton(ClientInstance->TargetHost, &ServerAddr.sin_addr) == 0)
        {
          printf("Could not identify server\n");
          return false;
        }

      if (sendto(ClientInstance->SocketDescriptor, Buffer, Sent,
                 0, (struct sockaddr *) &ServerAddr, Len) == -1)
        {
          printf("Could not send request\n");
          return false;
        }

      TimeVal.tv_sec = CLIENT_TIMEOUT_SEC;
      TimeVal.tv_usec = CLIENT_TIMEOUT_USEC;

      if (setsockopt(ClientInstance->SocketDescriptor, SOL_SOCKET, SO_RCVTIMEO,
                     &TimeVal,sizeof(TimeVal)) < 0)
        {
          printf("Unable to set timeout");
          return false;
        }

      Bytes = recvfrom(ClientInstance->SocketDescriptor, Buffer,
                       MSG_MAX, 0, (struct sockaddr *) &ServerAddr,
                       &Len);

      if (Bytes == -1)
        {
          printf("Request timed out\n");
        }
      else
        {
          Connected = sgClientHandleMsg(Buffer, Bytes);
        }
    }

  return Connected;
}
