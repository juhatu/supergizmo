/**
   \file messaging.h
   SuperGizmo messaging interface.
*/

#ifndef MESSAGING_H
#define MESSAGING_H

#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_PORT 12765

#define PORT_MIN 1024  /**< Minimum allowed port value */
#define PORT_MAX 65535 /**< Maximum allowed port value */
#define MSG_MAX 255    /**< Maximum message length including payload */
#define DATA_MAX 251   /**< Maximum message payload length */

enum
{
  MSG_PING = 1,
  MSG_PONG = 2
};

typedef struct SgSimpleMsg
{
  uint8_t MsgId;
  uint8_t MsgLen;
} SgSimpleMsg_t;

typedef SgSimpleMsg_t SgPingMsg_t;
typedef SgSimpleMsg_t SgPongMsg_t;

/**
   \brief Send simple message with optional data.
*/
static inline size_t sgSimpleMessage(char *Buffer, const uint8_t MsgId,
                                     const char *Data, const uint8_t Len)
{
  SgSimpleMsg_t *Msg = (SgSimpleMsg_t *) Buffer;
  size_t Written = 0;

  if (Msg && sizeof(SgSimpleMsg_t) + Len <= MSG_MAX)
    {
      Msg->MsgId = MsgId;
      Msg->MsgLen = Data ? Len : 0;
      Written = sizeof(*Msg);

      memcpy(&Buffer[Written], Data, Msg->MsgLen);

      Written += Msg->MsgLen;
    }

  return Written;
}

#endif // MESSAGING_H
