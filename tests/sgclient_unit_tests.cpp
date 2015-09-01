/**
   \file sgclient_unit_tests.cpp
   SuperGizmo client unit tests.
*/

#include <QtTest/QtTest>
#include <sys/socket.h>
#include <string.h>
#include "sgclienttests.h"
#include "sgclient.h"
#include "messaging.h"



void SgClientTests::testArgBounds(void)
{
  QVERIFY(sgClientValidateArguments(0, 0, 0, 0, 0) == false);
  QVERIFY(sgClientValidateArguments(0, 0, 0, 0, PORT_MIN - 1) == false);
  QVERIFY(sgClientValidateArguments(0, 0, 0, 0, PORT_MIN) == true);
  QVERIFY(sgClientValidateArguments(0, 0, 0, 0, DEFAULT_PORT) == true);
  QVERIFY(sgClientValidateArguments(0, 0, 0, 0, PORT_MAX) == true);

  QVERIFY(sgClientValidateArguments(0, 0, 0, 256, DEFAULT_PORT) == false);
  QVERIFY(sgClientValidateArguments(0, 0, 256, 0, DEFAULT_PORT) == false);
  QVERIFY(sgClientValidateArguments(0, 256, 0, 0, DEFAULT_PORT) == false);
  QVERIFY(sgClientValidateArguments(256, 0, 0, 0, DEFAULT_PORT) == false);
  QVERIFY(sgClientValidateArguments(255, 255, 255, 255, DEFAULT_PORT) == true);
}

void SgClientTests::testClientInit(void)
{
  ClientInstance_t ClientInstance;
  const char *TargetHost = "127.0.0.1";

  ClientInstance.SocketDescriptor = -1;
  memcpy(&ClientInstance.TargetHost, TargetHost, strlen(TargetHost) + 1);
  ClientInstance.TargetPort = DEFAULT_PORT;

  QVERIFY(sgClientInit(NULL) == false);

  QVERIFY(sgClientInit(&ClientInstance) == true);

  QVERIFY(ClientInstance.SocketDescriptor != -1);

  sgClientUninit(&ClientInstance);
}

void SgClientTests::testHandleMsg(void)
{
  char Buffer[MSG_MAX];
  SgSimpleMsg_t *pMsg = (SgSimpleMsg_t *) Buffer;

  pMsg->MsgId = MSG_PING;
  pMsg->MsgLen = 0;

  /* Test handling error */
  QVERIFY(sgClientHandleMsg(NULL, 0) == false);
  QVERIFY(sgClientHandleMsg(Buffer, sizeof(pMsg)) == false);

  pMsg->MsgId = MSG_PONG;

  /* Test handling success */
  QVERIFY(sgClientHandleMsg(Buffer, sizeof(pMsg)) == true);
}

int RunClientTests(void)
{
  SgClientTests Tests;

  return QTest::qExec(&Tests);
}
