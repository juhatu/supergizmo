/**
   \file sgserver_unit_tests.cpp
   SuperGizmo server unit tests.
*/

#include <QtTest/QtTest>
#include <sys/socket.h>
#include "sgservertests.h"
#include "sgserver.h"
#include "messaging.h"



void SgServerTests::testArgBounds(void)
{
  QVERIFY(sgServerValidateArguments(0) == false);
  QVERIFY(sgServerValidateArguments(PORT_MIN - 1) == false);
  QVERIFY(sgServerValidateArguments(PORT_MIN) == true);
  QVERIFY(sgServerValidateArguments(DEFAULT_PORT) == true);
  QVERIFY(sgServerValidateArguments(PORT_MAX) == true);
}

void SgServerTests::testServerInit(void)
{
  ServerInstance_t ServerInstance;

  ServerInstance.SocketDescriptor = -1;
  ServerInstance.Port = DEFAULT_PORT;

  QVERIFY(sgServerInit(NULL) == false);

  QVERIFY(sgServerInit(&ServerInstance) == true);

  QVERIFY(ServerInstance.SocketDescriptor != -1);

  sgServerUninit(&ServerInstance);
}

void SgServerTests::testHandleMsg(void)
{
  ServerInstance_t ServerInstance;
  char Buffer[MSG_MAX];
  SgSimpleMsg_t *pMsg = (SgSimpleMsg_t *) Buffer;
  struct sockaddr ClientAddr;
  socklen_t AddrLen = sizeof(ClientAddr);

  pMsg->MsgId = MSG_PONG;
  pMsg->MsgLen = 0;
  memset(&ClientAddr, 0, AddrLen);
  ServerInstance.SocketDescriptor = -1;
  ServerInstance.Port = DEFAULT_PORT;

  /* Test handling error */
  QVERIFY(sgServerHandleMsg(NULL, Buffer, sizeof(pMsg), &ClientAddr,
                            AddrLen) == false);

  QVERIFY(sgServerInit(&ServerInstance) == true);

  QVERIFY(ServerInstance.SocketDescriptor != -1);

  /* Test handling success */
  QVERIFY(sgServerHandleMsg(&ServerInstance, Buffer, sizeof(pMsg), &ClientAddr,
                            AddrLen) == true);

  sgServerUninit(&ServerInstance);
}

int RunServerTests(void)
{
  SgServerTests Tests;

  return QTest::qExec(&Tests);
}
