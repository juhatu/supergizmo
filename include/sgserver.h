/**
   \file sgserver_priv.h
   SuperGizmo server internal definitions.
 */

#ifndef SGSERVER_H
#define SGSERVER_H

#define ARG_FMT "%hu" /**< Input format for cmd argument */

typedef struct ServerInstance
{
  int SocketDescriptor; /**< Socket descriptor */
  uint16_t Port; /**< Local port of sgserver */
} ServerInstance_t;



/**
   \brief Uninitialize server instance.

   \param[in] ServerInstance Pointer to server instance
*/
void sgServerUninit(ServerInstance_t *ServerInstance);

/**
   \brief Print error message and exit.

   \param[in] ServerInstance Pointer to server instance
   \param[in] Message Error message to display
*/
void sgServerError(ServerInstance_t *ServerInstance, const char *Message);

/**
   \brief Print help message
*/
void sgServerPrintUsage(void);

/**
   \brief Initialize server instance and socket.

   \param[in] ServerInstance Pointer to server instance

   \return TRUE if initialization was successful.
*/
bool sgServerInit(ServerInstance_t *ServerInstance);

/**
   \brief Handle received message from client.

   \param[in] ServerInstance Pointer to server instance
   \param[in] Buffer Buffer containing the message to send/receive
   \param[in] Bytes Size of received message
   \param[in] ClientAddr Socket address of client
   \param[in] Len Size of the socket address

   \return TRUE if message was handled successfully.
*/
bool sgServerHandleMsg(ServerInstance_t *ServerInstance, char *Buffer,
                       const uint8_t Bytes, struct sockaddr *ClientAddr,
                       size_t Len);

/**
   \brief Validate command line arguments.

   \return TRUE if arguments are valid
*/
bool sgServerValidateArguments(uint16_t Port);

/**
   \brief Main message parsing loop.

   \param[in] ServerInstance Pointer to server instance
*/
void sgServer(ServerInstance_t *ServerInstance);

#endif // SGSERVER_H
