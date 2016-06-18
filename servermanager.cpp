#include "servermanager.h"
#include <QProcess>
#include <algorithm>
#include <QDebug>

#define CLI_WRAPPER CServerManager::SCliWrap;

//constexpr define
using TDockCmdLine = char[255];

//constansts
constexpr TDockCmdLine DOCKER_NAME{"DockerServer%1"};
constexpr TDockCmdLine DOCKER_IMG{"bitcoin-regtest"};
constexpr TDockCmdLine DOCKER_LOCK{"/home/alexd/gak/docker-bitcoin-regtest/bitcoin-regtest"};
constexpr TDockCmdLine DOCKER_CMD{"run -t -p %1:8888 --name=%2 --hostname=%3"};
constexpr TDockCmdLine DOCKER_RM{"rm -f %1"};
constexpr TDockCmdLine RUN_DEMON{"bitcoind -regtest -server -rpcuser=test -rpcpassword=test -rpcallowip=0.0.0.0/0 -rpcport=8888 -printtoconsole"};
constexpr TDockCmdLine DOCKER_BUILD{"build -t %1 %2"};
constexpr TDockCmdLine DOCKER{"docker"};

//static definitions
unsigned int CServerManager::s_id = 0;
unsigned int CServerManager::s_port = 8888;

using namespace NBitcoinServer;

CServerManager::CServerManager(QObject *parent) : QObject(parent)
{}

CServerManager& CServerManager::getReference()
{
    static CServerManager ref;

    return ref;
}

void CServerManager::registerClient()
{
    m_cliMap.emplace(TCliPair(s_id, std::move(SCliWrap(s_id++, s_port++))));
}

void CServerManager::deleteClient(uint id)
{
    auto iter = m_cliMap.find(id);

    if (iter != m_cliMap.end())
    {
        (iter->second).remove();
        m_cliMap.erase(iter);
    }
}

void CServerManager::startClient(const uint id)
{
    auto iter = m_cliMap.find(id);

    if (iter != m_cliMap.end())
    {
        (iter->second).run();
    }
}

void CServerManager::startAll()
{
    forEach([](TCliPair& pair){(pair.second).run();});
}

void CServerManager::deleteAll()
{
    forEach([](TCliPair& pair){(pair.second).remove();});
}

QStringList CServerManager::toString()
{
    QStringList rv;

    forEach([&rv](TCliPair& pair){
        rv << std::move(pair.second.toString());
    });


    return std::move(rv);
}


CServerManager::TServTable CServerManager::getTableData()
{
    CServerManager::TServTable rv;
    int id(0);

    for (auto& cli : m_cliMap)
    {
       std::map<QString, QString> child;
       child.emplace(std::pair<QString,QString>("Name", (cli.second).getName()));
       child.emplace(std::pair<QString,QString>("End Point", (cli.second).getEndPoint()));
       child.emplace(std::pair<QString,QString>("Status", (cli.second).isActive()));

       rv.emplace(std::pair<int, std::map<QString, QString> >(id++, std::move(child)));
    }

    return rv;
}


//wrapper implementation
CServerManager::SCliWrap::SCliWrap(unsigned int id, unsigned int port)
{
    m_dockName = std::move(QString(DOCKER_NAME).arg(QString(std::to_string(id).c_str())));
    m_dockRm = std::move(QString(DOCKER_RM).arg(m_dockName));
    m_dockCmd = std::move(QString(DOCKER_CMD).arg(QString(std::to_string(port).c_str())).arg(m_dockName).arg(m_dockName));
    m_endPoint = std::move(QString("http://172.17.0.1:%1").arg(std::to_string(port).c_str()));
    m_cli.reset(new CJasonHttpClient(m_endPoint));
}



bool CServerManager::SCliWrap::run()
{
    bool rv(true);

    QString dockStr(std::move(this->toString()));

    qDebug() << "starting :" << dockStr;

    //build docker
    QString buildDocker(DOCKER_BUILD);
    //create a string with the correct docker build info.
    runDockerCmd(std::move(buildDocker.arg(DOCKER_IMG).arg(DOCKER_LOCK)));

    //remove old docker instance
    remove();

    //run deamon
    QString arguments(std::move(m_dockCmd + " -d=true " + DOCKER_IMG + " " + RUN_DEMON));
    runDockerCmd(std::move(arguments));

    m_isActive = true;

    return rv;
}

bool CServerManager::SCliWrap::remove()
{
    bool rv(true);

    //remove old docker instance
    QString rmArgs = QString(DOCKER_RM).arg(m_dockName);
    runDockerCmd(std::move(rmArgs));

    m_isActive = false;

    return rv;
}

QJsonRpcMessage CServerManager::SCliWrap::sendMsg(QString cmd, QString args)
{
    QJsonRpcMessage rv;

    QStringList arguments = args.split(" ");

    QJsonRpcMessage sendMsg = std::move(m_cli->prepareMessage(std::move(cmd), std::move(arguments)));

    rv = std::move(m_cli->sendAndWaitForResp(std::move(sendMsg)));

    return rv;
}

void CServerManager::SCliWrap::runDockerCmd(const QString& args)
{
    QStringList arguments(std::move(args.split(" ")));

    QProcess proc;

    qDebug() << "Running command: " << DOCKER << "args:" + args;

    proc.start(DOCKER, std::move(arguments));

    if (!proc.waitForFinished())
    {
        qDebug() << "Error couldnt start finish command!";
    }

    qDebug() << "run finished with: " << proc.readAllStandardOutput() << proc.readAllStandardError();
}

QString CServerManager::SCliWrap::toString()
{
    QString rv;

    rv = "Name: " + getName() + " End Point: " + getEndPoint() + "Status: " + (m_isActive ? "Active" : "Deactivated");

    return rv;
}
