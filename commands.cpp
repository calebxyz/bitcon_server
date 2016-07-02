#include "commands.h"
#include "ui_commands.h"
#include "servermanager.h"

#define EXE_LAMBDA auto exe([this](int32_t ind, const QString& cmd, const QString& args)->bool \
                        {return this->runCommand(ind, cmd, args);});

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
    auto data = std::move(CServerManager::getReference().getTableData());

    initCombo(ui->comboBox, data);
    initCombo(ui->comboBox_2, data);

    QDialog::show();
}

void CCommands::initCombo(QComboBox* combo, CServerManager::TServTable &data)
{
    combo->clear();
    combo->addItem("None", QVariant(-1));

    for (auto& outer : data)
    {
        if (combo->findText(outer.second["Name"]) == -1)
        {
            if (outer.second["Status"] == CServerManager::ACTIVE)
            {
                combo->addItem(outer.second["Name"], QVariant(outer.first));
            }
        }
    }
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
        runCommand(ind, "getbalance", "");
    }

}

void CCommands::on_pushButton_2_clicked()
{
    hide();
}

void CCommands::on_pushButton_3_clicked()
{
    auto ind(ui->comboBox->currentData().toInt());

    auto exe([this](int32_t ind, const QString& cmd, const QString& args)->bool
    {return this->runCommand(ind, cmd, args);});

    execute(ind, std::move(exe), false, QString("getnewaddress"), QString(""));
}

bool CCommands::runCommand(const uint32_t ind, const QString& cmd, const QString& args)
{
    bool rv(true);

    QByteArray rawJason;

    CServerManager::TStringMap resp(m_serverMng.sendMsg(ind, cmd, args, &rawJason));

    qDebug() << "raw Response: [ " << rawJason << " ]";

    auto res = (resp.begin());

    qDebug() << res->first << ": [ " << res->second << " ]";

    if (res->first != "Response")
    {
        rv = false;
    }

    m_respShower.showFullResp(res->first, res->second, cmd);

    return rv;
}

void CCommands::on_pushButton_4_clicked()
{
    auto ind(ui->comboBox->currentData().toInt());

    EXE_LAMBDA;

    execute(ind, std::move(exe), false, QString("getpeerinfo"), QString(""));
}

void CCommands::on_pushButton_5_clicked()
{
    auto ind(ui->comboBox->currentData().toInt());

    QStringList commandAndArgs((ui->lineEdit->text()).split(" ")); //create List of command variants

    QString cmd = commandAndArgs[0];

    commandAndArgs.removeFirst();

    QString args = commandAndArgs.join(" ");

    EXE_LAMBDA;

    execute(ind, std::move(exe), false, cmd, args);
}
