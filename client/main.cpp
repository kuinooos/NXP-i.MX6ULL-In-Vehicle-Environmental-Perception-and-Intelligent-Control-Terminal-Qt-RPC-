#include "mainwindow.h"

#include <QApplication>

#include "rpc_client.h"
int main(int argc, char *argv[])
{
    RPC_Client_Init();
    QApplication a(argc, argv);

    Login *login = new Login();
    MainWindow w(nullptr,login);
    w.show();


    return a.exec();
}
