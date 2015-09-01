#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sgclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ClientInstance_t *Instance, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_ConnectButton_clicked();

private:
    ClientInstance_t *ClientInstance;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
