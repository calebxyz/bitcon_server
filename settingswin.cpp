#include "settingswin.h"
#include "ui_settingswin.h"
#include "qdebug.h"
#include "servermanager.h"

CSettingsWin::CSettingsWin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSettingsWin)
{
    ui->setupUi(this);
}

CSettingsWin::~CSettingsWin()
{
    delete ui;
}

void CSettingsWin::on_buttonBox_accepted()
{
    int numOfDockers = ((ui->lineEdit)->text()).toInt();

    for (int docker(0); docker < numOfDockers; ++docker)
    {
        CServerManager::getReference().registerClient();
    }

    qDebug() << "Created: " << numOfDockers << " Servers";

}

void CSettingsWin::on_buttonBox_rejected()
{
    qDebug() << "Canceled";

}

void CSettingsWin::on_lineEdit_editingFinished()
{
    m_numOfDockers = ((ui->lineEdit)->text()).toInt();
}
