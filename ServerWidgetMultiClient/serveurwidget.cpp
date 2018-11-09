#include "serveurwidget.h"
#include "ui_serveurwidget.h"

ServeurWidget::ServeurWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServeurWidget)
{
    ui->setupUi(this);
    socketEcouteServeur = new QTcpServer;
    connect(socketEcouteServeur, &QTcpServer::newConnection, this, &ServeurWidget::onQTcpServer_newConnection);
}

ServeurWidget::~ServeurWidget()
{
    delete ui;
}

void ServeurWidget::onQTcpServer_newConnection()
{
    QTcpSocket *client;
    client = socketEcouteServeur->nextPendingConnection();
    connect(client,&QTcpSocket::readyRead,this,&ServeurWidget::onQTcpSocket_readyRead);
    socketDialogueClient.append(client);

    /*QProcess *processus;
    connect(processus,&QProcess::readyReadStandardOutput,this,&ServeurWidget::onQProcess_readyReadStandardOutput);
    process.append(processus);*/
}

void ServeurWidget::onQTcpSocket_readyRead()
{
    QByteArray data;
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    int index = socketDialogueClient.indexOf(client);
    data = socketDialogueClient.at(index)->readAll();
    ui->textEditClients->append(data);

    if(!strcmp(data, "u")) //Nom de l'utilisateur connecté
    {
        QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
        int index = socketDialogueClient.indexOf(client);
        QString user = getenv("USERNAME");
        socketDialogueClient.at(index)->write(user.toLatin1());
    }
    if(!strcmp(data, "c")) //Nom de la machine
    {
        QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
        int index = socketDialogueClient.indexOf(client);
        QString user = QHostInfo::localHostName();
        socketDialogueClient.at(index)->write(user.toLatin1());
    }
    if(!strcmp(data, "o")) //Type d'OS
    {
        QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
        int index = socketDialogueClient.indexOf(client);
        socketDialogueClient.at(index)->write(QSysInfo::buildAbi().toUtf8());
    }
    if(!strcmp(data, "a")) //Type de process
    {
        QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
        int index = socketDialogueClient.indexOf(client);
        socketDialogueClient.at(index)->write(QSysInfo::productType().toUtf8());
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
    QProcess *processus = qobject_cast<QProcess*>(sender());
    int index = process.indexOf(processus);
    QString reponse = process.at(index)->readAllStandardOutput();
    if(!reponse.isEmpty())
    {
        QString message = "Réponse envoyée à " + socketDialogueClient.at(index)->peerAddress().toString() + " : " + reponse;
        ui->textEditClients->append(message);
        socketDialogueClient.at(index)->write(reponse.toLatin1());
    }
}
