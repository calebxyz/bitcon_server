#include "stopservers.h"
#include "ui_stopservers.h"
#include "servermanager.h"

CStopServers::CStopServers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CStopServers)
{
    ui->setupUi(this);
}

CStopServers::~CStopServers()
{
    delete ui;
}

void CStopServers::on_pushButton_clicked()
{
    qDebug() << "Stop all servers";

    CServerManager::getReference().deleteAll();

}
