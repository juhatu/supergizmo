/**
   \file messaging_unit_tests.cpp
   SuperGizmo messaging interface unit tests.
*/

#include <QtTest/QtTest>
#include "messagingtests.h"
#include "messaging.h"



void MessagingTests::testSimpleMessage(void)
{
  char Buffer[MSG_MAX];
  const uint8_t MsgId = MSG_PING;
  const char *Payload = "This is the payload";
  const uint8_t PayloadLen = strlen(Payload) + 1;
  const size_t ExpectedLen = sizeof(SgPingMsg_t) + PayloadLen;
  SgPingMsg_t *PingMsg = (SgPingMsg_t *) Buffer;

  memset(Buffer, 'X', MSG_MAX - 1);
  Buffer[MSG_MAX - 1] = '\0';

  /* Test handling error */
  QVERIFY(sgSimpleMessage(NULL, MsgId, Payload, PayloadLen) == 0);

  /* Test handling success */
  QVERIFY(sgSimpleMessage(Buffer, MsgId, Payload, PayloadLen) == ExpectedLen);
  QVERIFY(PingMsg->MsgId == MSG_PING);
  QVERIFY(PingMsg->MsgLen == PayloadLen);
  QVERIFY(memcmp(&Buffer[sizeof(*PingMsg)], Payload, PayloadLen) == 0);
}

int RunMessagingTests(void)
{
  MessagingTests Tests;

  return QTest::qExec(&Tests);
}
