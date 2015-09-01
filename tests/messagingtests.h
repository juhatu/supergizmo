/**
   \file messagingtests.h
   SuperGizmo messaging interface test class.
*/

#ifndef MESSAGINGTESTS_H
#define MESSAGINGTESTS_H

#include <QtTest/QtTest>



/**
   \brief Class containing all messaging interface unit tests.
*/
class MessagingTests: public QObject
{
  Q_OBJECT
private slots:
  void testSimpleMessage(void); /**< Test simple message creation */
};

/**
   \brief Run all messaging interface tests
*/
int RunMessagingTests(void);

#endif //MESSAGINGTESTS_H
