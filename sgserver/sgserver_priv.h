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

#endif // SGSERVER_H
