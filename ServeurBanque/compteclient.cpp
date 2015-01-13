#include "compteclient.h"

CompteClient::CompteClient(QObject *parent) :
    QTcpSocket(parent)
{

}

void CompteClient::DefinirNumCompte(int val)
{
      numCompte=val;
      solde = 200;
}

void CompteClient::Deposer(float val_deposee)
{
    solde = solde + val_deposee;
}

bool CompteClient::Retirer(float val_retiree)
{
    bool test = true;
    if(solde - val_retiree < 0)
    {
        solde = solde + val_retiree;
        test = false;
    }else  solde = solde - val_retiree;
    return test;
}

float CompteClient::ObtenirSolde()
{
    return solde;
}

int CompteClient::ObtenirNumCompte()
{
    return numCompte;
}
