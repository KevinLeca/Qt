#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QByteArray>

namespace Ui {
class ClientWidget;
}

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWidget(QWidget *parent = nullptr);
    ~ClientWidget();

private slots:
    /*----- Slots associés aux bouttons -----*/
    void on_pushButtonConnexion_clicked();
    void on_pushButtonOrdinateur_clicked();
    void on_pushButtonUtilisateur_clicked();
    void on_pushButtonOS_clicked();
    void on_pushButtonArchitecture_clicked();
    /*---------------------------------------*/

    /*----- Signaux -----*/
    void onQTcpSocket_connected();
    void onQTcpSocket_disconnected();
    void onQTcpSocket_error(QAbstractSocket::SocketError socketError);
    void onQTcpSocket_hostFound();
    void onQTcpSocket_stateChanged(QAbstractSocket::SocketState socketState);
    void onQTcpSocket_aboutToClose();
    void onQTcpSocket_bytesWritten(qint64 bytes);
    void onQTcpSocket_readChannelFinished();
    void onQTcpSocket_readyRead();
    /*--------------------*/

private:
    Ui::ClientWidget *ui;
    QTcpSocket *socketDeDialogueAvecServeur;
    QString typeDeDemande;

};

#endif // CLIENTWIDGET_H
