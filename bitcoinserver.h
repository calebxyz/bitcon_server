#ifndef BITCOINSERVER_H
#define BITCOINSERVER_H

#include <QMainWindow>
#include "servermanager.h"
#include "settingswin.h"
#include "startservers.h"
#include "stopservers.h"
#include "show.h"
#include "commands.h"

namespace Ui {
class BitcoinServer;
}

class BitcoinServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit BitcoinServer(QWidget *parent = 0);
    ~BitcoinServer();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::BitcoinServer *ui;

    CServerManager& m_manager;

    CSettingsWin m_settings;

    CStartServers m_startServer;

    CStopServers m_stopServers;

    CShow m_show;

    CCommands m_commands;

};

#endif // BITCOINSERVER_H
