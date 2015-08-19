/**
   \file sgclient_priv.h
   SuperGizmo client internal definitions.
*/

#ifndef SGCLIENT_PRIV_H
#define SGCLIENT_PRIV_H

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

#endif // SGCLIENT_PRIV_H
