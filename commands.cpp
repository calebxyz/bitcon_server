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
    ui->comboBox->clear();

    auto data = std::move(CServerManager::getReference().getTableData());
    ui->comboBox->addItem("None", QVariant(-1));

    for (auto& outer : data)
    {
        if (ui->comboBox->findText(outer.second["Name"]) == -1)
        {
            if (outer.second["Status"] == CServerManager::ACTIVE)
            {
                ui->comboBox->addItem(outer.second["Name"], QVariant(outer.first));
            }
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
    auto ind(ui->comboBox->currentData().toInt());

    if (ind > -1)
    {
        QByteArray rawJason;

        CServerManager::TStringMap resp(m_serverMng.sendMsg(ind, "getbalance", "", &rawJason));

        qDebug() << "raw Response: [ " << rawJason << " ]";

        for (auto& res : resp)
        {
            qDebug() << res.first << "  " << res.second;
            m_respShower.showFullResp(res.second);
        }
    }
}

void CCommands::on_pushButton_2_clicked()
{
    hide();
}
