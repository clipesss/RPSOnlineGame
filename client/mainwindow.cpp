#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->acceptGame->hide();
    ui->waitingResponseText->hide();
    ClientStartUp();
}

MainWindow::~MainWindow()
{
    delete ui;
    socket->deleteLater();
}

void MainWindow::ClientStartUp()
{
    socket = new QTcpSocket(this);

    QObject::connect(socket,&QTcpSocket::connected,this,[&](){

        QObject::connect(socket,&QTcpSocket::readyRead,this,[&](){checkMessageFunction();});
    });
    QObject::connect(socket,&QTcpSocket::disconnected, this,[&](){socket->disconnectFromHost(); qDebug() << "Disconnect from the server.";});
    socket->connectToHost("193.233.254.67",1234);
}

void MainWindow::checkMessageFunction()
{
    QByteArray data = socket->readAll();
    QString stringData = data;


    if(stringData == "#acceptGameRequest")
    {
        ui->sendGameRequest->hide();
        ui->acceptGame->show();
    }
    if(stringData == "#waitingResponse")
    {
        ui->sendGameRequest->hide();
        ui->waitingResponseText->show();
    }
    if(stringData == "#gameAccepted")
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    if(stringData == "#responseWinToServer")
    {
        QByteArray request = QString::number(chooseRPS).toLatin1();
        socket->write(request);
    }
    if(stringData == "#lose")
    {
        ui->chooseText->setText("Вы проиграли.");
        ui->chooseText->setStyleSheet("QLabel#chooseText{background-color: red; color: black;border-radius: 10px;font: 14pt \"Segoe UI\";}");
    }
    if(stringData == "#win")
    {
        ui->chooseText->setText("Вы выиграли!");
        ui->chooseText->setStyleSheet("QLabel#chooseText{background-color: green; color: black;border-radius: 10px;font: 14pt \"Segoe UI\";}");
    }
    if(stringData == "#repeat")
    {
        ui->chooseText->setText("Ничья, повторите выбор.");
        ui->chooseText->setStyleSheet("QLabel#chooseText{background-color: yellow; color: black;border-radius: 10px;font: 14pt \"Segoe UI\";}");
    }
}
void MainWindow::on_sendGameRequest_clicked()
{
    QByteArray request = "#gameRequest";
    socket->write(request);
}


void MainWindow::on_acceptGame_clicked()
{
    QByteArray request = "#gameAccepted";
    socket->write(request);

    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_rockButton_clicked()
{
    chooseRPS = 1;
}


void MainWindow::on_scissorsButton_clicked()
{
    chooseRPS = 2;
}


void MainWindow::on_paperButton_clicked()
{
    chooseRPS = 3;
}


void MainWindow::on_acceptChoose_clicked()
{
    QByteArray request = "#chooseWinner";
    socket->write(request);
}

