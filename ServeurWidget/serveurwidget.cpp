#include "serveurwidget.h"
#include "ui_serveurwidget.h"
#include "QHostInfo"

ServeurWidget::ServeurWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServeurWidget)
{
    ui->setupUi(this);
    socketEcouteServeur = new QTcpServer;
    process = new QProcess;
    connect(process, &QProcess::readyReadStandardOutput, this, &ServeurWidget::onQProcess_readyReadStandardOutput);
    connect(socketEcouteServeur, &QTcpServer::newConnection, this, &ServeurWidget::onQTcpServer_newConnection);
}

ServeurWidget::~ServeurWidget()
{
    delete ui;
}

void ServeurWidget::onQTcpServer_newConnection()
{
    socketDialogueClient = socketEcouteServeur->nextPendingConnection();
    connect(socketDialogueClient, &QTcpSocket::readyRead, this, &ServeurWidget::onQTcpSocket_readyRead);
    ui->textEditClients->append("Client address : " + socketDialogueClient->peerAddress().toString());

}

void ServeurWidget::onQTcpSocket_readyRead()
{
    QByteArray data;
    data = socketDialogueClient->readAll();
    ui->textEditClients->append(data);

    if(data.toStdString()=="u") //Nom de l'utilisateur connecté
    {
        QString user = getenv("USERNAME");
        socketDialogueClient->write(user.toLatin1());
    }
    if(data.toStdString()=="c") //Nom de la machine
    {
        QString user = QHostInfo::localHostName();
        socketDialogueClient->write(user.toLatin1());
    }
    if(data.toStdString()=="o") //Type d'OS
    {
        process->start("uname", QStringList("-p"));
    }
    if(data.toStdString()=="a") //Type de process
    {
        process->start("uname");
    }
}

void ServeurWidget::onQTcpSocket_disconnected()
{

}

void ServeurWidget::on_pushButtonLancerServeur_clicked()
{
    ui->textEditClients->append("Server Widget");
    quint16 port = ui->spinBoxNumeroPort->value();
    if(socketEcouteServeur->listen(QHostAddress::Any,port))
    {
        ui->textEditClients->append("Listening on port : " + QString::number(port));
    }
    else
    {
        ui->textEditClients->append("Failed listening");
    }

    if(ui->pushButtonLancerServeur->text()=="Lancement serveur")
    {
        ui->spinBoxNumeroPort->setEnabled(0);
    }


}

void ServeurWidget::onQProcess_readyReadStandardOutput()
{
    QString reponse = process->readAllStandardOutput();
    if(!reponse.isEmpty())
    {
        QString message = "Réponse envoyée à " + socketDialogueClient->peerAddress().toString() + " : " + reponse;
        ui->textEditClients->append(message);
        socketDialogueClient->write(reponse.toLatin1());
    }
}
