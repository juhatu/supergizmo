######################################################################
# Automatically generated by qmake (3.0) ti elokuuta 18 18:26:37 2015
######################################################################

isEmpty(PREFIX) {
  PREFIX = /usr/local
}

TEMPLATE = app

TARGET = sgserver
target.path = $$PREFIX/bin

INCLUDEPATH += . ../include

INSTALLS += target

# Input
HEADERS += sgserver_priv.h
SOURCES += sgserver.cc