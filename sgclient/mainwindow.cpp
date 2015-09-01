#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sgclient.h"

// Stylesheets and texts for the StatusField widget
#define STYLESHEET_CONNECTED "QLabel {background-color : green; }"
#define STYLESHEET_NOT_CONNECTED "QLabel {background-color : red; }"

#define TEXT_CONNECTED "Connected"
#define TEXT_NOT_CONNECTED "Not connected"

MainWindow::MainWindow(ClientInstance_t *Instance, QWidget *parent) :
  QMainWindow(parent),
  ClientInstance(Instance),
  ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
   \brief Action for Connect button pressed
*/
void MainWindow::on_ConnectButton_clicked()
{
    bool Connected = sgClient(ClientInstance);

    ui->StatusField->setStyleSheet(
      Connected ? STYLESHEET_CONNECTED : STYLESHEET_NOT_CONNECTED);
    ui->StatusField->setText(Connected ? TEXT_CONNECTED : TEXT_NOT_CONNECTED);
}
