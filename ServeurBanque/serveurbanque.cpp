#include "serveurbanque.h"

ServeurBanque::ServeurBanque(QObject *parent) :
    QTcpServer(parent),
    compteur(0)
{
    if(!connect(this,SIGNAL(newConnection()),this,SLOT(on_newConnection())))
    {
        box.setText("Erreur sur : SIGNAL(newConnection()),this,SLOT(on_newConnection()) // serveurbanque.cpp");
        box.exec();
    }
}

void ServeurBanque::on_newConnection()
{
    socket = (CompteClient*)nextPendingConnection();
    connect(socket,SIGNAL(disconnected()),this,SLOT(on_disconnect()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(on_readyRead()));
    envoyer_texte("Bonjour, entrez votre numéro de compte");
}

void ServeurBanque::envoyer_texte(QString texte)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << (quint16)0;
    out << texte;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket->write(block);
}

void ServeurBanque::on_readyRead()
{
    int numCompte; float solde;
    QString texte;
    QDataStream in(socket);
    if(socket->bytesAvailable() >= sizeof(compteur) && compteur==0)
    {    in >> compteur;
         qDebug() << QString::fromUtf8("recuperation compteur") ;
    }
    if(socket->bytesAvailable() >= compteur)
    {   in >> commande;
        qDebug() << QString::fromUtf8("recuperation commande") ;
        compteur = 0;
    switch(commande.toAscii())
    {
    case 'R':
        in >> montant;
        if(socket->Retirer(montant))
        {
            solde = socket->ObtenirSolde();
            texte = "Après retrait votre solde est de : ";
            texte += QString::number(solde);
            texte += " Euros";
            envoyer_texte(texte);
        }else {texte = "ERREUR : Votre solde est insuffisant";envoyer_texte(texte);}
        break;
    case 'D':
        in >> montant;
        socket->Deposer(montant);
        solde = socket->ObtenirSolde();
        texte = "Après dépôt votre solde est de : ";
        texte += QString::number(solde);
        texte += " Euros";
        envoyer_texte(texte);
        break;
    case 'S':
        solde = socket->ObtenirSolde();
        texte = "Votre solde est de : ";
        texte += QString::number(solde);
        texte += " Euros";
        envoyer_texte(texte);
        break;
    case 'N':
        in >> numCompte;
        socket->DefinirNumCompte(numCompte);
        texte = "Vous êtes connecté sur le compte n°";
        texte += QString::number(numCompte);
        envoyer_texte(texte);
        break;
    }
}

}

bool ServeurBanque::Start()
{
   bool test=false;
   qDebug() << QString::fromUtf8("START") ;
   if(listen(QHostAddress::Any,8888))
   {
       qDebug() << QString::fromUtf8("EN ECOUTE") ;
       test = true;
   }else {box.setText("ERREUR : l'écoute a échoué :( ");
          box.exec();
          close();
   }
   return test;
}

void ServeurBanque::on_disconnect()
{
    close();
}

void ServeurBanque::Stop()
{
    close();
}
