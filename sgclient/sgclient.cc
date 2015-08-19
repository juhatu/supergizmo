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
#include <QApplication>
#include "sgclient_priv.h"
#include "messaging.h"
#include "mainwindow.h"

static ClientInstance_t sClientInstance;

/**
   \brief Uninitialize client instance.
*/
static void sgClientUninit(void)
{
  if (sClientInstance.SocketDescriptor != -1)
    {
      printf("Closing socket %d\n", sClientInstance.SocketDescriptor);
      close(sClientInstance.SocketDescriptor);
    }
}

/**
   \brief Print error message and exit.
*/
static void sgClientError(const char *message)
{
  perror(message);

  sgClientUninit();

  exit(EXIT_FAILURE);
}

/**
   Print help message.
*/
static void sgClientPrintUsage(void)
{
  printf("Usage: sgclient host:port\n");
  printf(" host: Valid IPv4 address, for example %s\n", DEFAULT_HOST);
  printf(" port: %hu-%hu\n\n", PORT_MIN, PORT_MAX);
}

/**
   \brief Initialize client instance and socket.
*/
static int sgClientInit(void)
{
  int SocketDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (SocketDescriptor == -1)
    {
      sgClientError("Could not open socket\n");
    }

  printf("Opened socket %d\n", SocketDescriptor);

  return SocketDescriptor;
}

/**
   \brief Handle received message from server.

   \param[in] Buffer Buffer containing received message
   \param[in] Bytes Size of received message

   \return TRUE is message was received successfully.
*/
static bool sgClientHandleMsg(char *Buffer, const uint8_t Bytes)
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

/**
   \brief Validate command line arguments.

   \param[in] A First (leftmost) digits of IPV4 address
   \param[in] B Second digits of IPV4 address
   \param[in] C Third digits of IPV4 address
   \param[in] D Fourth digits of IPV4 address
   \param[in] Port UDP port to use

   \return TRUE if arguments are valid
*/
static bool sgClientValidateArguments(uint16_t A, uint16_t B, uint16_t C,
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

   \return TRUE if message exchange was successful.
*/
bool sgClient(void)
{
  bool Connected = false;

  if (sClientInstance.SocketDescriptor != -1)
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
      ServerAddr.sin_port = htons(sClientInstance.TargetPort);

      if (inet_aton(sClientInstance.TargetHost, &ServerAddr.sin_addr) == 0)
        {
          printf("Could not identify server\n");
          return false;
        }

      if (sendto(sClientInstance.SocketDescriptor, Buffer, Sent,
                 0, (struct sockaddr *) &ServerAddr, Len) == -1)
        {
          printf("Could not send request\n");
          return false;
        }

      TimeVal.tv_sec = CLIENT_TIMEOUT_SEC;
      TimeVal.tv_usec = CLIENT_TIMEOUT_USEC;

      if (setsockopt(sClientInstance.SocketDescriptor, SOL_SOCKET, SO_RCVTIMEO,
                       &TimeVal,sizeof(TimeVal)) < 0)
        {
          printf("Unable to set timeout");
          return false;
        }

      Bytes = recvfrom(sClientInstance.SocketDescriptor, Buffer,
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

/**
   \brief Main routine for the SuperGizmo client.
*/
int main(int argc, char **argv)
{
  QApplication MainApp(argc, argv);
  MainWindow Window;
  int ret;

  if (argc != 2)
    {
      sgClientPrintUsage();

      return EXIT_SUCCESS;
    }
  else
    {
      uint16_t A, B, C, D, Port;

      memset(&sClientInstance, 0, sizeof(sClientInstance));
      sClientInstance.SocketDescriptor = -1;

      // Validate arguments
      if (sscanf(argv[1], ARG_FMT, &A, &B, &C, &D, &Port) != 5)
        {
          sgClientError("Invalid argument\n");
        }

      if (!sgClientValidateArguments(A, B, C, D, Port))
        {
          sgClientError("Invalid argument\n");
        }

      // Store target host and port
      snprintf(sClientInstance.TargetHost, MAX_HOST_LEN, HOST_FMT,
               A, B, C, D);
      sClientInstance.TargetPort = Port;
    }

   sClientInstance.SocketDescriptor = sgClientInit();

   Window.show();

   ret = MainApp.exec();

   sgClientUninit();

   return ret;
}
