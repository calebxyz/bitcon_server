#include "commands.h"
#include "ui_commands.h"
#include "servermanager.h"
#include "argwindowmine.h"

#define EXE_LAMBDA QString reslt; \
                   auto exe([this, &reslt](int32_t ind, const QString& cmd, const QString& args)->bool \
                   {return this->runCommand(ind, cmd, args, reslt);});

CCommands::CCommands(QWidget *parent) :
    QDialog(parent),
    CLogable("CommandsLog"),
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
        if (combo->findText(outer.second[CServerManager::SERVER_NAME]) == -1)
        {
            if (outer.second[CServerManager::STATUS] == CServerManager::ACTIVE)
            {
                combo->addItem(outer.second[CServerManager::SERVER_NAME], QVariant(outer.first));
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

    EXE_LAMBDA;

    auto status = execute(ind, std::move(exe), false, QString("getbalance"), QString(""));

    if (status)
    {
        auto balance(reslt.split(CServerManager::RESP_SEPERATOR));
        m_serverMng.setBalance(ind, balance[0].toFloat());
    }

    /*if (ind > -1)
    {
        runCommand(ind, "getbalance", "");
    }*/

}

void CCommands::on_pushButton_2_clicked()
{
    hide();
}

void CCommands::on_pushButton_3_clicked()
{
    auto ind(ui->comboBox->currentData().toInt());

    EXE_LAMBDA;

    auto status = execute(ind, std::move(exe), false, QString("getnewaddress"), QString(""));

    if (status)
    {
        m_serverMng.setAddress(ind, reslt.split(CServerManager::RESP_SEPERATOR)[0]);
    }
}

bool CCommands::runCommand(const uint32_t ind, const QString& cmd, const QString& args, QString& reslt)
{
    bool rv(true);
    std::string errMsg("");

    QByteArray rawJason;

    CServerManager::TStringMap resp(m_serverMng.sendMsg(ind, cmd, args, &rawJason));

    //qDebug() << "raw Response: [ " << rawJason << " ]";
    LOGGER_HELPER(TRACE, errMsg, "Raw response from server: [ " , rawJason, " ]");

    auto res = (resp.begin());

    //qDebug() << res->first << ": [ " << res->second << " ]";
    LOGGER_HELPER(TRACE, errMsg, res->first , ": [ ", res->second, " ]");

    if (res->first != "Response")
    {
        rv = false;
    }

    reslt = res->second;

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

void CCommands::on_pushButton_6_clicked()
{
    static cargWindow argWin;

    auto ind(ui->comboBox->currentData().toInt());

    QString cmd("generate");

    auto subExeFunc([this, ind, cmd](const QString& args)->bool
    {
        EXE_LAMBDA;
        return execute(ind, std::move(exe), false, cmd, args);
    });

    argWin.show(std::move(subExeFunc), "Number Of Blocks");
}

void CCommands::on_pushButton_7_clicked()
{
    static cargWindow argWin;
    auto indSender(ui->comboBox->currentData().toInt());
    auto indRecvr(ui->comboBox_2->currentData().toInt());

    QString cmd("sendtoaddress");

    //only if the rcvr is selected
    if (indRecvr != -1)
    {
        QString add(m_serverMng.getAddress(indRecvr));
        add += " ";

        auto subExeFunc([this, indSender, cmd, add](const QString& args)->bool
        {
            EXE_LAMBDA;
            return execute(indSender, std::move(exe), false, cmd, add + args);
        });

        argWin.show(std::move(subExeFunc), "Number Of Coins");
    }
}

void CCommands::on_pushButton_8_clicked()
{
    auto sndrInd(ui->comboBox->currentData().toInt());
    auto rcvrInd(ui->comboBox_2->currentData().toInt());

    auto cmd("addnode");

    auto sndrIp(m_serverMng.getIp(sndrInd));
    auto rcvrIp(m_serverMng.getIp(rcvrInd));

    if (sndrIp != CServerManager::INVALID_IP && rcvrIp != CServerManager::INVALID_IP)
    {
        auto args = rcvrIp + " onetry";
        EXE_LAMBDA;
        execute(sndrInd, std::move(exe), false, cmd, args);
    }

}

void CCommands::on_pushButton_9_clicked()
{
    auto ind(ui->comboBox->currentData().toInt());

    auto cmd("getinfo");

    EXE_LAMBDA;

    execute(ind, std::move(exe), false, cmd, QString(""));
}

void CCommands::on_pushButton_10_clicked()
{
    auto ind(ui->comboBox_2->currentData().toInt());

    auto cmd("getinfo");

    EXE_LAMBDA;

    execute(ind, std::move(exe), false, cmd, QString(""));
}
