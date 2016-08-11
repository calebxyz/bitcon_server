#include "demo.h"
#include "ui_demo.h"
#include "servermanager.h"
#include "commands.h"
#include <thread>
#include <chrono>

//globals for demo usage
static std::unique_ptr<CCommands> g_commands(nullptr);

//using chrono literals
using namespace std::chrono_literals;

CDemo::CDemo(QWidget *parent) :
    QDialog(parent),
    CLogable("DemoLogger"),
    ui(new Ui::CDemo),
    m_serverManager(CServerManager::getReference()),
    m_miner(std::make_shared<SMiner>()),
    m_player(std::make_shared<SPlayer>()),
    m_logReader(std::make_shared<SLoggingTask>()),
    m_logFile("bitcoinServer.log"),
    m_stream(&m_logFile)
{
    ui->setupUi(this);
    //connect the logging task to the actual ui container
    QObject::connect(m_logReader.get(), SIGNAL(loggerTrigger(QString)), this, SLOT(drawLogger(QString)));
}

void CDemo::show()
{
    //init commands global
    m_logFile.open(QIODevice::ReadOnly|QIODevice::Text);
    m_logFile.seek(m_logFile.size()); //go to the end of the file

    if (!g_commands)
    {
        g_commands.reset(new CCommands);
    }

    std::string errMsg("");
    LOGGER_HELPER(INFO, errMsg,
    std::string("==================================================================\n\t\t") +
    "!!!!DEMO START!!!! \n" +
    "==================================================================");

    //start the logreader task
    m_logReaderTask = std::make_unique<std::thread>
    (std::thread([this]()
    {
        this->m_logReader->mainFunc(this->m_stream);
    }));

    m_logReader->start();

    m_logReaderTask->detach();

    QDialog::show();
}

CDemo::~CDemo()
{
    delete ui;
}

void CDemo::on_pushButton_clicked()
{
    m_miner->stop();
    m_player->stop();
    m_logReader->stop();
    hide();
}

void CDemo::on_pushButton_2_clicked()
{
    std::string errMsg("");
    auto servers = g_commands->getActiveServerList();

    if (servers.size() > 1)
    {
        //create miner and players
        auto miner = servers[0];
        servers.pop_front(); //remove the miner

        //create the sync point
        std::promise<void> p;
        auto f = p.get_future();

        //first step is to register all servers
        g_commands->regAllServer();

        //now create accounts for all servers
        g_commands->createAddForAll();

        //creating the demo taks
        m_minerTask = std::make_unique<std::thread>
        (std::thread([this, miner, servers, &p]()
        {
            (this->m_miner)->minerMain(miner, std::move(servers), std::move(p));
        }));

        m_playerTask = std::make_unique<std::thread>
        (std::thread([this, servers, &f]()
        {
            (this->m_player)->playerMain(this->m_miner, std::move(servers), std::move(f));
        }));

        m_minerTask->detach();
        m_playerTask->detach();

    }
    else
    {
        LOGGER_HELPER(ERROR, errMsg, "Not enugh active server for demo to run properly , need at least 2 server");
    }
}

void CDemo::SMiner::distCash()
{
    std::string errMsg("");

    auto balance = g_commands->getBalance(miner, false);
    LOGGER_HELPER(INFO, errMsg, "Distrebuting [ ", balance, " ] bitcoins between users");

    for (auto serv : players)
    {
        auto amount = CDemo::getAmount(balance);
        balance -= amount;
        LOGGER_HELPER(INFO, errMsg, QString("Sending [ "), amount, " ] coins to server [ ", serv , " ]");
        LOGGER_HELPER(INFO, errMsg, QString("Amount left"), balance, " ]");

        g_commands->sendCoins(miner, serv, amount, false);

        //exit loop if no more bitcoins left.
        if (balance <= 0)
        {
            return;
        }
    }
}

void CDemo::SMiner::init(qint32 a_miner, QVector<quint32> a_players)
{
    std::string errMsg("");

    LOGGER_HELPER(INFO, errMsg, "Starting init proccess");

    miner = a_miner;
    players = a_players;

    setCount();

    //mine 102 blocks and get 100 bitcoins
    g_commands->mine(miner, 102);

    LOGGER_HELPER(INFO, errMsg, "Mined 102 blocks and now the miner has a total value of 100 bitcoins");

    //distrebute cash between active players
    distCash();
}

void CDemo::SMiner::minerMain(qint32 a_miner, QVector<quint32> a_players, std::promise<void> p)
{
    const auto SLEEP_TIME(2s);
    const auto MAX_MININGS(6);

    qint32 minings(1);
    std::string errMsg("");

    init(a_miner, std::move(a_players));

    //after init is done release the players
    p.set_value();

    LOGGER_HELPER(INFO, errMsg, "Miner started working");

    //while flase
    while (!stopMe.test_and_set())
    {
        if (operCount.load() >= currMaxOpers)
        {
            g_commands->mine(miner, 1); //mine one block
            reset();
            setCount();
            minings++;

            if (minings == MAX_MININGS)
            {
                distCash();
            }
        }
        else
        {
            std::this_thread::sleep_for(SLEEP_TIME);
        }

        stopMe.clear();
    }
}

void CDemo::SPlayer::init(CDemo::TMinerSptr a_miner, QVector<quint32> a_players)
{
    std::string errMsg("");
    LOGGER_HELPER(INFO, errMsg, "Starting init proccess");

    miner = a_miner;
    players = std::move(a_players);
    vectSize = players.size();

    LOGGER_HELPER(INFO, errMsg, QString("init players ["), players,
                                        "] Num of players [ ", vectSize, "]");

}

void CDemo::SPlayer::playerMain(TMinerSptr a_miner, QVector<quint32> a_players, std::future<void> f)
{

    const auto SLEEP_TIME(1s);
    std::string errMsg("");
    init(std::move(a_miner), std::move(a_players));

    LOGGER_HELPER(INFO, errMsg, "Players init proccess ok waiting for miner");
    f.wait();
    LOGGER_HELPER(INFO, errMsg, "Miner and players are now synced starting normal activity");

    //while flase
    while (!stopMe.test_and_set())
    {
        auto playerSndr = peekPlayer();
        auto playerRcvr = peekPlayer();

        auto balance = g_commands->getBalance(playerSndr, false);

        //only if this player has cash
        if (balance > 0.0009)
        {
            auto amount = CDemo::getAmount(balance);

            balance -= amount;

            LOGGER_HELPER(INFO, errMsg, QString("Sending [ "), amount, " ] coins  from [ ", playerSndr, "] to server [ ",
                          playerRcvr , " ]");
            LOGGER_HELPER(INFO, errMsg, QString("Amount left"), balance, " ]");

            g_commands->sendCoins(playerSndr, playerRcvr, amount, false);

            miner->add();
        }

        stopMe.clear();

        std::this_thread::sleep_for(SLEEP_TIME);
    }
}

void CDemo::on_pushButton_3_clicked()
{
    m_miner->stop();
    m_player->stop();
    m_logReader->stop();
}

qreal CDemo::getAmount(qreal& currBalance)
{
    auto half = static_cast<qint32>(currBalance / 2);

    qreal stoshi = (currBalance - static_cast<qint32>(currBalance)) / 2;

    auto intAmount = std::rand() % static_cast<qint32>(currBalance);

    qreal amount = static_cast<qreal>(intAmount) + stoshi;

    if (amount >= half)
    {
        amount /= 2; // give only half of what you wanted to give
    }

    return amount;
}

void SLoggingTask::mainFunc(QTextStream& stream)
{
    //read the first demo notice
    QString str = stream.readAll();
    emit loggerTrigger(str);

    while (m_runLogger)
    {
        str = stream.readLine(); //each time rad only one log enterance

        //only if theres somthing to print and its from the currect loggers print to screen
        if ((str.size() != 0) &&
           ((str.contains("PlayersLogger")) || (str.contains("MinerLogger")) ||
            (str.contains("DemoLogger"))) )
        {
            emit loggerTrigger(str);
            std::this_thread::sleep_for(2s); //sleep for 2 seconds
        }
    }

    //set it to run again
    m_runLogger = true;
}


//this is the slot that will be connected to the log reader task
void CDemo::drawLogger(QString str)
{
    static auto pTextEdit = ui->textEdit;

    pTextEdit->insertPlainText(str);
    auto cursor = pTextEdit->textCursor();
    cursor.setPosition(QTextCursor::End);
    pTextEdit->setTextCursor(cursor);
}
