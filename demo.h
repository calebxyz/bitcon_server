#ifndef DEMO_H
#define DEMO_H

#include <QDialog>
#include "logable.h"
#include "servermanager.h"
#include <atomic>
#include <mutex>
#include <future>
#include <thread>
#include <QFile>
#include <QTextStream>

namespace Ui {
class CDemo;
struct SLoggingTask;
}

//container calls for the logging operation we want it to have slots
//so it will inharite the QObject
struct SLoggingTask : public QObject
{
    Q_OBJECT

  public:

    SLoggingTask():
        m_runLogger(false)
    {}

    void mainFunc(QTextStream& stream);

    inline void stop()
    {
        m_runLogger = false;
    }

    inline void start()
    {
        m_runLogger = true;
    }

signals:
    void loggerTrigger(QString str);


private:
    bool m_runLogger;
};

class CDemo : public QDialog, CLogable
{
    Q_OBJECT

public:

    explicit CDemo(QWidget *parent = 0);
    void show();
    ~CDemo();

    static qreal getAmount(qreal& currBalance);

public slots:
    void drawLogger(QString str);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::CDemo *ui;
    CServerManager& m_serverManager;

    struct SMiner : CLogable
    {

        SMiner() :
            CLogable("MinerLogger")
        {}

        inline void add()
        {
            static constexpr quint32 ONE{1};
            operCount.fetch_add(ONE);
            //std::atomic_fetch_add(&operCount, &ONE);
        }

        inline void reset()
        {
            operCount.store(0);
        }

        inline void stop()
        {
            std::string errMsg("");
            LOGGER_HELPER(INFO, errMsg, "Stoping Miner");
            stopMe.test_and_set();
        }

        void minerMain(qint32 a_miner, QVector<quint32> a_players, std::promise<void> p);

    private:
        std::atomic<quint32> operCount;
        std::atomic_flag stopMe = ATOMIC_FLAG_INIT;
        qint32 miner;
        QVector<quint32> players;
        quint32 currMaxOpers;
        std::mutex mut;


        void distCash();

        void init(qint32 a_miner, QVector<quint32> a_players);

        inline void setCount()
        {
            static const qint32 MAX_OPERS(100);

            currMaxOpers = std::rand() % MAX_OPERS;
        }

    };

    using TMinerSptr = std::shared_ptr<SMiner>;

    struct SPlayer : public CLogable
    {

        SPlayer():
            CLogable("PlayersLogger")
        {}

        inline void stop()
        {
            std::string errMsg("");
            LOGGER_HELPER(INFO, errMsg, "Stoping Players");
            stopMe.test_and_set();
        }

        void playerMain(TMinerSptr a_miner, QVector<quint32> a_players, std::future<void> f);

    private:

        QVector<quint32> players;
        TMinerSptr miner = nullptr;
        std::atomic_flag stopMe = ATOMIC_FLAG_INIT;
        qint32 vectSize;

        void init(TMinerSptr a_miner, QVector<quint32> a_players);

        inline qint32 peekPlayer()
        {
            auto player = (std::rand() % vectSize);
            return player < vectSize ? player : player - 1;
        }

    };


private:

    std::shared_ptr<SMiner> m_miner;
    std::shared_ptr<SPlayer> m_player;
    std::shared_ptr<SLoggingTask> m_logReader;
    std::unique_ptr<std::thread> m_minerTask;
    std::unique_ptr<std::thread> m_playerTask;
    std::unique_ptr<std::thread> m_logReaderTask;
    QFile m_logFile;
    QTextStream m_stream;


};

#endif // DEMO_H
