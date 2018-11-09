#include "clientwidget.h"
#include "ui_clientwidget.h"

ClientWidget::ClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWidget)
{
    ui->setupUi(this);
    /*----- Instanciation de la socket de communication -----*/
    socketDeDialogueAvecServeur = new QTcpSocket(this);
    /*-------------------------------------------------------*/

    /*----- Association des événements aux slots -----*/
    connect(socketDeDialogueAvecServeur, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &ClientWidget::onQTcpSocket_error);
    connect( socketDeDialogueAvecServeur, &QTcpSocket::connected,    this, &ClientWidget::onQTcpSocket_connected);
    connect( socketDeDialogueAvecServeur, &QTcpSocket::disconnected, this, &ClientWidget::onQTcpSocket_disconnected);
    connect( socketDeDialogueAvecServeur, &QTcpSocket::hostFound,    this, &ClientWidget::onQTcpSocket_hostFound);
    connect( socketDeDialogueAvecServeur, &QTcpSocket::stateChanged, this, &ClientWidget::onQTcpSocket_stateChanged);
    connect( socketDeDialogueAvecServeur, &QTcpSocket::aboutToClose,  this, &ClientWidget::onQTcpSocket_aboutToClose);
    connect( socketDeDialogueAvecServeur, &QTcpSocket::bytesWritten, this, &ClientWidget::onQTcpSocket_bytesWritten);
    connect( socketDeDialogueAvecServeur, &QTcpSocket::readChannelFinished, this, &ClientWidget::onQTcpSocket_readChannelFinished);
    connect( socketDeDialogueAvecServeur, &QTcpSocket::readyRead, this, &ClientWidget::onQTcpSocket_readyRead);
    /*----- -----------------------------------------*/
}

ClientWidget::~ClientWidget()
{
    delete ui;
}

void ClientWidget::on_pushButtonConnexion_clicked()
{
    QString hostname = ui->lineEditIPServeur->text();
    QString port = ui->lineEditPortServeur->text();
    int portint = port.toInt();
    if(ui->pushButtonConnexion->text() == "Connexion"){
        socketDeDialogueAvecServeur->connectToHost(hostname, portint);
        ui->pushButtonConnexion->setText("Deconnexion");
    }
    else{
        ui->pushButtonConnexion->setText("Connexion");
        socketDeDialogueAvecServeur->disconnectFromHost();
    }
}
void ClientWidget::on_pushButtonOrdinateur_clicked()
{
    typeDeDemande = "c";
    socketDeDialogueAvecServeur->write(typeDeDemande.toLatin1());;
}
void ClientWidget::on_pushButtonUtilisateur_clicked()
{
    typeDeDemande = "u";
    socketDeDialogueAvecServeur->write(typeDeDemande.toLatin1());
}
void ClientWidget::on_pushButtonOS_clicked()
{
    typeDeDemande = "o";
    socketDeDialogueAvecServeur->write(typeDeDemande.toLatin1());
}
void ClientWidget::on_pushButtonArchitecture_clicked()
{
    typeDeDemande = "a";
    socketDeDialogueAvecServeur->write(typeDeDemande.toLatin1());
}

void ClientWidget::onQTcpSocket_connected()
{
    ui->lineEditPortServeur->setEnabled(0);
    ui->lineEditIPServeur->setEnabled(0);
    ui->groupBoxInfosClient->setEnabled(1);
    ui->textEditEtat->append("connected");
}
void ClientWidget::onQTcpSocket_disconnected()
{
    ui->lineEditPortServeur->setEnabled(1);
    ui->lineEditIPServeur->setEnabled(1);
    ui->groupBoxInfosClient->setEnabled(0);
    ui->textEditEtat->append("disconnected");
}
void ClientWidget::onQTcpSocket_error(QAbstractSocket::SocketError socketError)
{
    ui->textEditEtat->append(socketDeDialogueAvecServeur->errorString());
}
void ClientWidget::onQTcpSocket_hostFound()
{
    ui->textEditEtat->append("hostfound");
}
void ClientWidget::onQTcpSocket_stateChanged(QAbstractSocket::SocketState socketState)
{
    switch (socketState)
    {
        case QAbstractSocket::HostLookupState: ui->textEditEtat->append("The socket is performing a host name lookup"); break;
        case QAbstractSocket::ConnectingState: ui->textEditEtat->append("The socket has started establishing a connection"); break;
        case QAbstractSocket::ConnectedState: ui->textEditEtat->append("A connection is established"); break;
        case QAbstractSocket::ClosingState: ui->textEditEtat->append("The socket is about to close"); break;
        case QAbstractSocket::UnconnectedState: ui->textEditEtat->append("The socket is not connected"); break;
    }

}
void ClientWidget::onQTcpSocket_aboutToClose()
{
    ui->textEditEtat->append("The socket is about to close");
}
void ClientWidget::onQTcpSocket_bytesWritten(qint64 bytes)
{
    ui->textEditEtat->append("bytes written");
}
void ClientWidget::onQTcpSocket_readChannelFinished()
{
    ui->textEditEtat->append("read channel finnished");
}
void ClientWidget::onQTcpSocket_readyRead()
{
    QByteArray donnees = socketDeDialogueAvecServeur->readAll();
    if(typeDeDemande == 'o'){
        ui->lineEditOSOrdinateur->insert(donnees.data());
    }
    if(typeDeDemande == 'u'){
        ui->lineEditNomUtilisateur->insert(donnees.data());
    }
    if(typeDeDemande == 'c'){
        ui->lineEditNomOrdiDistant->insert(donnees.data());
    }
    if(typeDeDemande == 'a'){
        ui->lineEditArchiProcess->insert(donnees.data());
    }
}
