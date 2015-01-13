#ifndef SERVEURBANQUE_H
#define SERVEURBANQUE_H

#include <QTcpServer>
#include <QMessageBox>
#include <QHostAddress>
#include "compteclient.h"

class ServeurBanque : public QTcpServer
{
    Q_OBJECT
public:
    explicit ServeurBanque(QObject *parent = 0);
    
signals:
    
public slots:
    void on_newConnection();
    void on_readyRead();
    bool Start();
    void Stop();
    void on_disconnect();
    void envoyer_texte(QString texte);
private:
      QMessageBox box;
      CompteClient *socket;
      QChar commande;
      float montant;
      quint16 compteur;
};

#endif // SERVEURBANQUE_H
