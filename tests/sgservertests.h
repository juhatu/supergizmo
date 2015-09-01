/**
   \file sgservertests.h
   SuperGizmo server unit test class.
*/

#ifndef SGSERVERTESTS_H
#define SGSERVERTESTS_H

#include <QtTest/QtTest>



/**
   \brief Class containing all server-side unit tests.
*/
class SgServerTests: public QObject
{
  Q_OBJECT
private slots:
  void testArgBounds(void); /**< Test for valid port ranges */

  void testServerInit(void); /**< Test server initialization
                                using default port */

  void testHandleMsg(void); /**< Test server message handling */
};

/**
   \brief Run all server-side tests
*/
int RunServerTests(void);

#endif //SGSERVERTESTS_H
