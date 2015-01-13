#include "dab_mainwindow.h"
#include "ui_dab_mainwindow.h"

DAB_MainWindow::DAB_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DAB_MainWindow),
    compteur(0)
{
    QMessageBox boite;
    socket = new QTcpSocket(this);
    ui->setupUi(this);
    if(!connect(socket,SIGNAL(readyRead()),this,SLOT(on_RecupererResultatCommande())))
    {
        boite.setText("Erreur interne sur SIGNAL(readyRead()),this,SLOT(on_RecupererResultatCommande())");
        boite.exec();
    }
    if(!connect(socket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(on_changing_state())))
    {
        boite.setText("Erreur interne sur SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(on_changing_state())");
        boite.exec();
    }
    ui->lineEdit_add_serv->setCursorPosition(0);
    ui->lineEdit_port_serv->setCursorPosition(0);
    ui->pushButton_connexion->setShortcut(Qt::Key_Enter);
}

void DAB_MainWindow::on_changing_state()
{
    if(socket->state()==QAbstractSocket::ConnectingState)
    {
        ui->listWidget_etat_connexion->addItem("Connexion au serveur...");
        ui->pushButton_connexion->setDisabled(true);
    }

    if(socket->error()==QAbstractSocket::ConnectionRefusedError)
    {
        ui->listWidget_etat_connexion->addItem("Erreur : Connexion refusée");
        ui->pushButton_connexion->setText("Connexion");
        ui->pushButton_connexion->setDisabled(false);
    }

    if(socket->state()==QAbstractSocket::ConnectedState)
    {
        ui->listWidget_etat_connexion->addItem("Connecté au serveur !");
        ui->pushButton_connexion->setText("Déconnexion");
        ui->pushButton_connexion->setDisabled(false);

    }
}

void DAB_MainWindow::on_pushButton_connexion_clicked()
{
    if(socket->state()==QAbstractSocket::ConnectedState)
    {
        socket->disconnectFromHost();
        ui->listWidget_etat_connexion->addItem("Déconnexion du serveur");
        ui->pushButton_connexion->setText("Connexion");
        ui->pushButton_connexion->setDisabled(false);

    }else{
            {
                QString add;
                add = ui->lineEdit_add_serv->text();
                quint16 port = ui->lineEdit_port_serv->text().toUInt();
                socket->connectToHost(add,port);
            }
        }
}

void DAB_MainWindow::on_RecupererResultatCommande()
{
    QString resultat="";
    QDataStream in(socket);
    if(socket->bytesAvailable() >= sizeof(compteur) && compteur == 0)
        in >> compteur;
    if(socket->bytesAvailable() >= compteur) {
        in >> resultat;
        compteur = 0;
    }
    ui->listWidget_etat_connexion->addItem("Données reçues du serveur");
    ui->lineEdit_message_banque->setText(resultat);
}


void DAB_MainWindow::Envoi_demande_numero_compte(QChar commande)
{
    QByteArray block;
    int numero_compte;
    numero_compte = ui->lineEdit_numero_compte->text().toInt();
    if(numero_compte>0)
    {
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << (quint16)0;
    out << commande ;
    out << numero_compte;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket->write(block);
    ui->listWidget_etat_connexion->addItem("Demande du numéro de compte");
    }else {
        ui->listWidget_etat_connexion->addItem("Erreur, veuillez entrer un numéro de compte");
    }
}

void DAB_MainWindow::Envoi_demande_montant(QChar commande)
{
    QByteArray block;
    float montant;
    QString texte="";
    montant = ui->lineEdit_montant->text().toFloat();
    if(montant>0)
    {
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << (quint16)0;
    out << commande ;
    out << montant;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket->write(block);
    switch(commande.toAscii())
    {
    case 'R':
        texte="Demande de retrait de : ";
        texte+=ui->lineEdit_montant->text();
        texte+="€";
        ui->listWidget_etat_connexion->addItem(texte);
        break;
    case 'D':
        texte="Demande de dépôt de : ";
        texte+=ui->lineEdit_montant->text();
        texte+="€";
        ui->listWidget_etat_connexion->addItem(texte);
        break;
    }
    }else {
        ui->listWidget_etat_connexion->addItem("Erreur, veuillez entrer un montant");
    }
}


void DAB_MainWindow::Envoi_demande(QChar commande)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << (quint16)0;
    out << commande ;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket->write(block);
    QString texte="Envoi de la commande |";
    texte+=commande;
    texte+="|au serveur";
    ui->listWidget_etat_connexion->addItem(texte);
}

void DAB_MainWindow::on_pushButton_numero_compte_clicked()
{
    QChar c = 'N';
    Envoi_demande_numero_compte(c);
}

void DAB_MainWindow::on_pushButton_envoyer_clicked()
{
    if(ui->radioButton_solde->isChecked())
    {
        Envoi_demande('S');
    }
    if(ui->radioButton_depot->isChecked())
    {
        Envoi_demande_montant('D');
    }
    if(ui->radioButton_retrait->isChecked())
    {
        Envoi_demande_montant('R');
    }
}

DAB_MainWindow::~DAB_MainWindow()
{
    delete ui;
    socket->deleteLater();
}

