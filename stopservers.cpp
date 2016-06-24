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


void CStopServers::show()
{
    ui->comboBox->clear();

    auto data = std::move(CServerManager::getReference().getTableData());
    ui->comboBox->addItem("All", QVariant(-1));


    for (auto& outer : data)
    {
        if (ui->comboBox->findText(outer.second["Name"]) == -1)
        {
            qDebug() << "the status is: " <<  outer.second["Status"];
            if (outer.second["Status"] == CServerManager::ACTIVE)
            {
                ui->comboBox->addItem(outer.second["Name"], QVariant(outer.first));
            }
        }
    }

    QDialog::show();
}

void CStopServers::on_pushButton_clicked()
{
    auto currVal = ui->comboBox->currentData().toString();
    (currVal == "-1") ? currVal = "All" : "";

    qDebug() << "Stoping server: [ "  << currVal << "]";

    if (currVal == "All")
    {
        CServerManager::getReference().deleteAll();
    }
    else //we just need to start one server
    {
        CServerManager::getReference().deleteClient(currVal.toInt());
    }

    hide();
}
