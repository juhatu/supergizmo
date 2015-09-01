/**
   \file sgclient.h
   SuperGizmo client public interface.
*/

#ifndef SGCLIENT_H
#define SGCLIENT_H

#include <stdint.h>

#define HOST_FMT "%hu.%hu.%hu.%hu" /**< Input format for the host part */
#define PORT_FMT "%hu" /**< Input format for the port part */
#define ARG_FMT (HOST_FMT ":" PORT_FMT) /**< Input format for cmd argument */

#define CLIENT_TIMEOUT_SEC 5 /**< Reply timeout, seconds part */
#define CLIENT_TIMEOUT_USEC 0 /**< Reply timeout, microseconds part */

#define MAX_HOST_LEN (15) /**< IPv4 address without terminating NULL */

typedef struct ClientInstance
{
  int SocketDescriptor; /**< Socket descriptor */
  uint16_t TargetPort; /**< Target port of sgserver */
  char TargetHost[MAX_HOST_LEN + 1]; /**< Target IP of sgserver */
} ClientInstance_t;



/**
   \brief Uninitialize client instance.

   \param[in] ClientInstance Pointer to client instance
*/
void sgClientUninit(ClientInstance_t *ClientInstance);

/**
   \brief Print error message and exit.

   \param[in] ClientInstance Pointer to client instance
*/
void sgClientError(ClientInstance_t *ClientInstance, const char *message);

/**
   Print help message.
*/
void sgClientPrintUsage(void);

/**
   \brief Initialize client instance and socket.

   \param[in] ClientInstance Pointer to client instance

   \return TRUE if initialization was successful.
*/
bool sgClientInit(ClientInstance_t *ClientInstance);

/**
   \brief Handle received message from server.

   \param[in] Buffer Buffer containing received message
   \param[in] Bytes Size of received message

   \return TRUE is message was received successfully.
*/
bool sgClientHandleMsg(char *Buffer, const uint8_t Bytes);

/**
   \brief Validate command line arguments.

   \param[in] A First (leftmost) digits of IPV4 address
   \param[in] B Second digits of IPV4 address
   \param[in] C Third digits of IPV4 address
   \param[in] D Fourth digits of IPV4 address
   \param[in] Port UDP port to use

   \return TRUE if arguments are valid
*/
bool sgClientValidateArguments(uint16_t A, uint16_t B, uint16_t C,
                               uint16_t D, uint16_t Port);

/**
   \brief Main message handler.

   \param[in] ClientInstance Pointer to client instance

   \return TRUE if message exchange was successful.
*/
bool sgClient(ClientInstance_t *ClientInstance);

#endif // SGCLIENT_H
