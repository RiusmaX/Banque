#ifndef COMPTECLIENT_H
#define COMPTECLIENT_H

#include <QTcpSocket>
#include <QMessageBox>

class CompteClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit CompteClient(QObject *parent = 0);
    void DefinirNumCompte(int val);
    void Deposer(float val_deposee);
    bool Retirer(float val_retiree);
    float ObtenirSolde();
    int ObtenirNumCompte();
    
signals:
    
public slots:

private:
    int numCompte;
    float solde;
    
};

#endif // COMPTECLIENT_H
