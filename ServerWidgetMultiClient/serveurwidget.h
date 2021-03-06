#ifndef SERVEURWIDGET_H
#define SERVEURWIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QProcess>
#include <QList>
#include <QHostInfo>

namespace Ui {
class ServeurWidget;
}

class ServeurWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServeurWidget(QWidget *parent = 0);
    ~ServeurWidget();

private:
    Ui::ServeurWidget *ui;
    QTcpServer *socketEcouteServeur;
    QList<QTcpSocket *> socketDialogueClient;
    QList<QProcess *> process;

private slots:
    void onQTcpServer_newConnection();
    void onQTcpSocket_readyRead();
    void onQTcpSocket_disconnected();
    void on_pushButtonLancerServeur_clicked();
    void onQProcess_readyReadStandardOutput();
};

#endif // SERVEURWIDGET_H
