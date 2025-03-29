#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ServerStartUp();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ServerStartUp()
{
    server = new QTcpServer(this);
    if(server->listen(QHostAddress::Any,1234))
    {
        qDebug() << "Server start to listen.";
    }
    else
    {
        qDebug() << "Error to start listening.";
    }

    QObject::connect(server,&QTcpServer::newConnection,this,[&](){

        if(connections >= 3)
        {
            server->nextPendingConnection()->disconnect();
            connections = 2;
        }
        socket = server->nextPendingConnection();

        qDebug() << "New connect to the server: " << socket->localAddress();

        connections++;

        if(connections == 1)
        {
            client1 = socket;
            QObject::connect(client1,&QTcpSocket::disconnected, this,[&](){qDebug() << "Client1 Disconnect."; connections--;});
        }
        else if(connections == 2)
        {
            client2 = socket;
            QObject::connect(client2,&QTcpSocket::disconnected, this,[&](){qDebug() << "Client2 Disconnect."; connections--;});
        }
        if(connections == 2)
        {
            QObject::connect(client1,&QTcpSocket::readyRead,this,[&](){ qDebug() <<1; int descriptor = client1->socketDescriptor();  checkAndProccesingRequestClient(descriptor); });
            QObject::connect(client2,&QTcpSocket::readyRead,this,[&](){ qDebug() <<2; int descriptor = client2->socketDescriptor();  checkAndProccesingRequestClient(descriptor); });
        }



    });


}

void MainWindow::on_updateVars_clicked()
{
    ui->textConnections->setText("Connections: " + QString::number(connections));

}

void MainWindow::on_getConnections_clicked()
{
    if(connections == 1)
    {
        ui->connectionsList->setText("1. " + client1->localAddress().toString());
    }
    else if(connections == 2)
    {
        ui->connectionsList->setText("1. " + client1->localAddress().toString() + "\n" + "2. " + client2->localAddress().toString());
    }
}


void MainWindow::checkAndProccesingRequestClient(int descriptor)
{
    client1Descriptor = client1->socketDescriptor();
    client2Descriptor = client2->socketDescriptor();
    if(descriptor == client1Descriptor)
    {
        QByteArray data = client1->readAll();
        QString dataString = data;
        qDebug() << dataString;
        if(dataString == "#gameRequest")
        {
            qDebug() << 1;
            QByteArray gameRequestData = "#acceptGameRequest";
            client2->write(gameRequestData);

            QByteArray gameResponseData = "#waitingResponse";
            client1->write(gameResponseData);
            qDebug() << 2;
        }
        if(dataString == "#gameAccepted")
        {
            QByteArray gameAcceptedData = "#gameAccepted";
            client2->write(gameAcceptedData);
        }
        if(dataString == "#chooseWinner")
        {
            QByteArray gamegameChooseData = "#responseWinToServer";
            client1->write(gamegameChooseData);
        }
        else
        {

            bool ok;
            int number = data.toInt(&ok);
            if(ok)
            {
                rpsClient1 = number;
            }
        }
    }
    else if(descriptor == client2Descriptor)
    {
        QByteArray data = client2->readAll();
        QString dataString = data;

        if(dataString == "#gameRequest")
        {
            QByteArray gameRequestData = "#acceptGameRequest";
            client1->write(gameRequestData);

            QByteArray gameResponseData = "#waitingResponse";
            client2->write(gameResponseData);
        }
        if(dataString == "#gameAccepted")
        {
            QByteArray gameAcceptedData = "#gameAccepted";
            client1->write(gameAcceptedData);
        }
        if(dataString == "#chooseWinner")
        {
            QByteArray gameChooseData = "#responseWinToServer";
            client2->write(gameChooseData);
        }
        else
        {

            bool ok;
            int number = data.toInt(&ok);
            if(ok)
            {
                rpsClient2 = number;
            }
        }
    }


    if(rpsClient1 > 0 && rpsClient2 > 0)
    {
        if(rpsClient1 == 1)
        {
            if(rpsClient2 == 1)
            {
                QByteArray gameRepeatData1 = "#repeat";
                client1->write(gameRepeatData1);
                QByteArray gameRepeatData2 = "#repeat";
                client2->write(gameRepeatData2);
                rpsClient1 = 0;
                rpsClient2 = 0;
            }
            if(rpsClient2 == 2)
            {
                winner = 1;
                QByteArray gameData1 = "#win";
                client1->write(gameData1);
                QByteArray gameData2 = "#lose";
                client2->write(gameData2);
                rpsClient1 = 0;
                rpsClient2 = 0;
            }
            if(rpsClient2 == 3)
            {
                winner = 2;
                QByteArray gameData1 = "#lose";
                client1->write(gameData1);
                QByteArray gameData2 = "#win";
                client2->write(gameData2);
                rpsClient1 = 0;
                rpsClient2 = 0;
            }
        }
        if(rpsClient1 == 2)
        {
            if(rpsClient2 == 1)
            {
                winner = 2;
                QByteArray gameData1 = "#lose";
                client1->write(gameData1);
                QByteArray gameData2 = "#win";
                client2->write(gameData2);
                rpsClient1 = 0;
                rpsClient2 = 0;
            }
            if(rpsClient2 == 2)
            {
                QByteArray gameRepeatData1 = "#repeat";
                client1->write(gameRepeatData1);
                QByteArray gameRepeatData2 = "#repeat";
                client2->write(gameRepeatData2);
                rpsClient1 = 0;
                rpsClient2 = 0;
            }
            if(rpsClient2 == 3)
            {
                winner = 1;
                QByteArray gameData1 = "#win";
                client1->write(gameData1);
                QByteArray gameData2 = "#lose";
                client2->write(gameData2);
                rpsClient1 = 0;
                rpsClient2 = 0;
            }
        }
        if(rpsClient1 == 3)
        {
            if(rpsClient2 == 1)
            {
                winner = 1;
                QByteArray gameData1 = "#win";
                client1->write(gameData1);
                QByteArray gameData2 = "#lose";
                client2->write(gameData2);
                rpsClient1 = 0;
                rpsClient2 = 0;

            }
            if(rpsClient2 == 2)
            {
                winner = 2;
                QByteArray gameData1 = "#lose";
                client1->write(gameData1);
                QByteArray gameData2 = "#win";
                client2->write(gameData2);
                rpsClient1 = 0;
                rpsClient2 = 0;
            }
            if(rpsClient2 == 3)
            {
                QByteArray gameRepeatData1 = "#repeat";
                client1->write(gameRepeatData1);
                QByteArray gameRepeatData2 = "#repeat";
                client2->write(gameRepeatData2);
                rpsClient1 = 0;
                rpsClient2 = 0;
            }
        }
    }
}
