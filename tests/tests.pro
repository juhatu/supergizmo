######################################################################
# Automatically generated by qmake (3.0) pe elokuuta 28 08:54:45 2015
######################################################################

TEMPLATE = app
TARGET = tests
INCLUDEPATH += . ../include

# Input
HEADERS += sgservertests.h sgclienttests.h messagingtests.h ../include/sgserver.h ../include/sgclient.h ../include/messaging.h
SOURCES += main.cpp sgserver_unit_tests.cpp sgclient_unit_tests.cpp messaging_unit_tests.cpp ../sgserver/sgserver.cc ../sgclient/sgclient.cc

QT += testlib
CONFIG += testcase c++11

QMAKE_CFLAGS += -Wall -Wextra -Werror -pedantic
QMAKE_CXXFLAGS += -Wall -Wextra -Werror -pedantic
