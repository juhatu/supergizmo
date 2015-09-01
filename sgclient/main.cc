#include <QApplication>
#include "sgclient.h"
#include "messaging.h"
#include "mainwindow.h"



/**
   \brief Main routine for the SuperGizmo client.
*/
int main(int argc, char **argv)
{
  ClientInstance_t ClientInstance;

  memset(&ClientInstance, 0, sizeof(ClientInstance));
  ClientInstance.SocketDescriptor = -1;

  if (argc != 2)
    {
      sgClientPrintUsage();

      return EXIT_SUCCESS;
    }
  else
    {
      uint16_t A, B, C, D, Port;

      memset(&ClientInstance, 0, sizeof(ClientInstance));
      ClientInstance.SocketDescriptor = -1;

      // Validate arguments
      if (sscanf(argv[1], ARG_FMT, &A, &B, &C, &D, &Port) != 5)
        {
          sgClientError(&ClientInstance, "Invalid argument\n");
        }

      if (!sgClientValidateArguments(A, B, C, D, Port))
        {
          sgClientError(&ClientInstance, "Invalid argument\n");
        }

      // Store target host and port
      snprintf(ClientInstance.TargetHost, MAX_HOST_LEN, HOST_FMT,
               A, B, C, D);
      ClientInstance.TargetPort = Port;
    }

  if (sgClientInit(&ClientInstance))
    {
      QApplication MainApp(argc, argv);
      MainWindow Window(&ClientInstance);
      int ret;

      Window.show();

      ret = MainApp.exec();

      sgClientUninit(&ClientInstance);

      return ret;
    }

  return EXIT_FAILURE;
}
