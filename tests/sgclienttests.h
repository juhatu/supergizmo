/**
   \file sgclienttests.h
   SuperGizmo client unit test class.
*/

#ifndef SGCLIENTTESTS_H
#define SGCLIENTTESTS_H

#include <QtTest/QtTest>



/**
   \brief Class containing all client-side unit tests.
*/
class SgClientTests: public QObject
{
  Q_OBJECT
private slots:
  void testArgBounds(void); /**< Test for valid input parameters */
  void testClientInit(void); /**< Test client initialization
                                  using default host and port */
  void testHandleMsg(void); /**< Test client message handling */
};

/**
   \brief Run all client-side tests
*/
int RunClientTests(void);

#endif //SGCLIENTTESTS_H
