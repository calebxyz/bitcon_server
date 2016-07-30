#include "bitcoinserver.h"
#include "ui_bitcoinserver.h"
#include <qdebug.h>

BitcoinServer::BitcoinServer(QWidget *parent) :
    QMainWindow(parent),
    CLogable("BitcoinLogger"),
    ui(new Ui::BitcoinServer),
    m_manager(CServerManager::getReference())
{
    ui->setupUi(this);
}

BitcoinServer::~BitcoinServer()
{
    delete ui;
}

void BitcoinServer::on_pushButton_clicked()
{
    std::string errMsg("");
    LOGGER_HELPER(INFO, errMsg, "Exiting Application");
    m_manager.deleteAll();
    exit(0);
}

void BitcoinServer::on_pushButton_2_clicked()
{
    m_settings.show();
}

void BitcoinServer::on_pushButton_3_clicked()
{
   m_startServer.show();
}

void BitcoinServer::on_pushButton_4_clicked()
{
    m_stopServers.show();
}

void BitcoinServer::on_pushButton_5_clicked()
{
   m_show.show();
}

void BitcoinServer::on_pushButton_6_clicked()
{
    m_commands.show();
}
