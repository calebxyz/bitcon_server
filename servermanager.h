#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QString>
#include <memory>
#include <algorithm>
#include "jasonHttpclient.h"

namespace NBitcoinServer
{
    class serverManager;
}

class CServerManager : public QObject
{
    Q_OBJECT
public:


    //static constans
    static const char* ACTIVE;
    static const char* DEACTIVE;
    static const char* RESP_SEPERATOR;

    using TStringPair = std::pair<QString, QString>;

    using TStringMap = std::map<QString, QString>;

    using TServTable = std::map<int , TStringMap>;

    static CServerManager& getReference();

    void registerClient();

    void deleteClient(uint id);

    QStringList toString();

    void startClient(const uint id);

    void startAll();

    void deleteAll();

    TServTable getTableData();

    TStringMap sendMsg(int idx, QString cmd, QString args, QByteArray* rawJason = nullptr);

    inline void setAddress(const uint32_t ind, const QString& add)
    {
        auto cli = m_cliMap.find(ind);

        //cli.second->setAddress(std::move(add));
    }

    //delete copy ctor and operator=
    CServerManager(CServerManager&) = delete;
    CServerManager operator=(CServerManager&) = delete;

signals:

public slots:

private:

    //static member that will hold the ports for clients
    static unsigned int s_port;
    //static member that represents docker ids
    static unsigned int s_id;

    explicit CServerManager(QObject *parent = 0);

    struct SCliWrap
    {
    public:
        SCliWrap(unsigned int port, unsigned int id);
        bool run();
        bool remove();
        TStringMap sendMsg(QString cmd, QString args, QByteArray* rawJason = nullptr);


        QString toString();

        inline QString getName()
        {
            return m_dockName;
        }

        inline QString getEndPoint()
        {
            return m_endPoint;
        }

        inline QString isActive()
        {
            return m_isActive ? ACTIVE : DEACTIVE;
        }

        inline void setAddress(const QString& add)
        {
            m_myAddress = std::move(add);
        }


    private:
        QString m_dockName;
        QString m_dockCmd;
        QString m_dockRm;
        QString m_demonRun;
        QString m_endPoint;
        bool m_isActive = false; //c++11 init style
        QString m_myAddress;
        std::unique_ptr<CJasonHttpClient> m_cli;

        void runDockerCmd(const QString& args);
        TStringMap parse(QJsonRpcMessage msg);
    };

    template <typename TFunc>
    void forEach(TFunc func)
    {
        //std::for
        std::for_each(m_cliMap.begin(), m_cliMap.end(), func);
    }

    //using this pair for map  easy access
    using TCliPair = std::pair<const uint,SCliWrap>;

    std::map<const uint, SCliWrap> m_cliMap;

};

#endif // SERVERMANAGER_H
