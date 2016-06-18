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
        ui->comboBox->addItem(outer.second["Name"], QVariant(outer.first));
    }

    QDialog::show();
}

void CCommands::on_comboBox_activated(int index)
{
    m_index = index;
}


