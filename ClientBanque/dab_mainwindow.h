#ifndef DAB_MAINWINDOW_H
#define DAB_MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QMessageBox>

namespace Ui {
class DAB_MainWindow;
}

class DAB_MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit DAB_MainWindow(QWidget *parent = 0);
    ~DAB_MainWindow();
    void Envoi_demande_numero_compte(QChar commande);
    void Envoi_demande(QChar commande);
    void Envoi_demande_montant(QChar commande);
private slots:
    void on_changing_state();
    void on_RecupererResultatCommande();
    void on_pushButton_connexion_clicked();

    void on_pushButton_numero_compte_clicked();

    void on_pushButton_envoyer_clicked();

private:
    Ui::DAB_MainWindow *ui;
    QTcpSocket *socket;
    quint16 compteur;
};

#endif // DAB_MAINWINDOW_H
