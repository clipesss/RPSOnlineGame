#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void ServerStartUp();
    void on_updateVars_clicked();
    void on_getConnections_clicked();
    void checkAndProccesingRequestClient(int descriptor);

private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    QTcpSocket *socket;
    QTcpSocket *client1;
    QTcpSocket *client2;

    int client1Descriptor;
    int client2Descriptor;
    int connections = 0;

    int rpsClient1 = 0;
    int rpsClient2 = 0;

    int winner = 0;

};
#endif // MAINWINDOW_H
