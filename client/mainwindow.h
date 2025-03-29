#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QRandomGenerator>

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
    void ClientStartUp();
    void checkMessageFunction();
    void on_sendGameRequest_clicked();

    void on_acceptGame_clicked();

    void on_rockButton_clicked();

    void on_scissorsButton_clicked();

    void on_paperButton_clicked();

    void on_acceptChoose_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    int chooseRPS = 0;

};
#endif // MAINWINDOW_H
