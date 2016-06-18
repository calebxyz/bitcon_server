#include "startservers.h"
#include "ui_startservers.h"

CStartServers::CStartServers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartServers)
{
    ui->setupUi(this);
}

CStartServers::~CStartServers()
{
    delete ui;
}

void CStartServers::on_pushButton_clicked()
{
    qDebug() << "Strting all";

    CServerManager::getReference().startAll();
}
