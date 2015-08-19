# supergizmo

Your friendly neighborhood client/server communication example utilizing UDP and
running on Linux.

## Introduction

The supergizmo suite consists of two applications:

* `sgserver` is the server application which is run from the command line.
* `sgclient` is the client application with a graphical user interface.

The client and server applications communicate using a simple PING/PONG message
exchange over UDP.

## Compile and install (requires Qt5 installation)

Run the following commands in the repository root:

```
qmake
make
sudo make install
```

Or you can use the precompiled .deb package.

## sgserver

sgserver is run from the command line. Without any arguments it starts listening
on the default UDP port 12765. Any port value between 1024 and 65535 can be
specified from the command line, for example `./sgserver 5000` will direct the
server to listen on port 5000 instead.

## sgclient

sgclient is run from the command line. It requires exactly one argument which is
of the form `<target IP>:<port>` where the IP and port are the host IP and port
in which sgserver is running. For example to connect to a server running on
localhost type `./sgclient 127.0.0.1:5000`.

The sgclient GUI has a push button for testing the connection to the server.
When the button is pushed the client sends a PING message to the server and
waits for a PONG response. The application will then display the connection
status in green (valid response was received) or red (response timed out or
was invalid).

The GUI for sgclient was done using [Qt 5.5](http://www.qt.io).

