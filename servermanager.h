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

    using TServTable = std::map<int ,std::map<QString, QString> >;

    static CServerManager& getReference();

    void registerClient();

    void deleteClient(uint id);

    QStringList toString();

    void startClient(const uint id);

    void startAll();

    void deleteAll();

    TServTable getTableData();

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
        QJsonRpcMessage sendMsg(QString cmd, QString args);
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
            return m_isActive ? "Active" : "Deactive";
        }


    private:
        QString m_dockName;
        QString m_dockCmd;
        QString m_dockRm;
        QString m_demonRun;
        QString m_endPoint;
        bool m_isActive = false; //c++11 init style
        std::unique_ptr<CJasonHttpClient> m_cli;

        void runDockerCmd(const QString& args);
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
