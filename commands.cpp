#include "commands.h"
#include "ui_commands.h"
#include "servermanager.h"

CCommands::CCommands(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CCommands)
{
    ui->setupUi(this);
}

CCommands::~CCommands()
{
    delete ui;
}

void CCommands::show()
{
    auto data = std::move(m_serverMng.getTableData());

    for (auto& outer : data)
    {
        if (ui->comboBox->findText(outer.second["Name"]) == -1)
        {
            ui->comboBox->addItem(outer.second["Name"], QVariant(outer.first));
        }
    }

    QDialog::show();
}

void CCommands::on_comboBox_activated(int index)
{
    m_index = index;
}



void CCommands::on_pushButton_clicked()
{
    auto ind(ui->comboBox->currentData().toUInt());

    QByteArray rawJason;

    CServerManager::TStringMap resp(m_serverMng.sendMsg(ind, "getbalance", "", &rawJason));

    qDebug() << "raw Response: [ " << rawJason << " ]";
    //TODO: pare resp here
}
