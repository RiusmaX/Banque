#ifndef BANQUEMAINWINDOW_H
#define BANQUEMAINWINDOW_H

#include <QMainWindow>
#include "serveurbanque.h"
#include "compteclient.h"
#include <QtNetwork/QNetworkInterface>

namespace Ui {
class BanqueMainWindow;
}

class BanqueMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit BanqueMainWindow(QWidget *parent = 0);
    ~BanqueMainWindow();
    
private:
    Ui::BanqueMainWindow *ui;
    ServeurBanque leServeur;
};

#endif // BANQUEMAINWINDOW_H
