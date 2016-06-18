#include "commands.h"
#include "ui_commands.h"

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
