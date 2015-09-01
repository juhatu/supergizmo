/**
   \file main.cpp
   SuperGizmo unit test main routine
*/

#include <QtTest/QtTest>
#include "sgservertests.h"
#include "sgclienttests.h"
#include "messagingtests.h"

/**
   \brief Main testing routine for client and server.
*/
int main(int argc, char **argv)
{
  int RetVal = 0;

  (void) argc;
  (void) argv;

  /* Run all tests */
  RetVal |= RunServerTests();
  RetVal |= RunClientTests();
  RetVal |= RunMessagingTests();

  return RetVal;
}
