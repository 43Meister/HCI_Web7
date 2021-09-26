
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
#include <random>

namespace Ui {
class CDemo;
struct SLoggingTask;
}

static const qint32 DONT_CARE(-1);

//uniformity random class .
class CUnfiformRandomReal
{
public:

    CUnfiformRandomReal():
        m_mt(m_rd()),
        m_uniforDist(std::make_unique<std::uniform_real_distribution<qreal> >(0, 0))
    {}

    CUnfiformRandomReal(const qreal& lowLim, const qreal& upLim):
        m_mt(m_rd()),
        m_uniforDist(std::make_unique<std::uniform_real_distribution<qreal> >(lowLim, upLim))
    {}

    inline void setNewLimits(const qreal& lowLim, const qreal& upLim)
    {
        m_uniforDist = std::make_unique<std::uniform_real_distribution<qreal> >(lowLim, upLim);
    }

    inline qreal getNumber()
    {
        return (*(m_uniforDist.get()))(m_mt);
    }

private:
    std::random_device m_rd;
    std::mt19937 m_mt;
    std::unique_ptr<std::uniform_real_distribution<qreal> > m_uniforDist;

};

//uniformity random class .
class CUnfiformRandomInt
{
public:

    CUnfiformRandomInt():
        m_mt(m_rd()),
        m_uniforDist(std::make_unique<std::uniform_int_distribution<quint32> >(0, 0))
    {}

    CUnfiformRandomInt(const quint32& lowLim, const quint32& upLim):
        m_mt(m_rd()),
        m_uniforDist(std::make_unique<std::uniform_int_distribution<quint32> >(lowLim, upLim))
    {}

    inline void setNewLimits(const quint32& lowLim, const quint32& upLim)
    {
        m_uniforDist = std::make_unique<std::uniform_int_distribution<quint32> >(lowLim, upLim);
    }

    inline quint32 getNumber()
    {
        return (*(m_uniforDist.get()))(m_mt);
    }

private:
    std::random_device m_rd;
    std::mt19937 m_mt;
    std::unique_ptr<std::uniform_int_distribution<quint32> > m_uniforDist;

};

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

    static qreal getAmount(qreal& currBalance)
    {
        CUnfiformRandomReal rd(0, currBalance);
        return rd.getNumber();
    }

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
            std::string errMsg("");
            static constexpr quint32 ONE{1};
            auto num = operCount.fetch_add(ONE);

             LOGGER_HELPER(INFO, errMsg, "performed: [", num, "] operations");
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

        void minerMain(qint32 a_miner, QVector<quint32> a_players, std::promise<void>& p);

        void printBalanceOfAllPlayers();

    private:
        std::atomic<quint32> operCount;
        std::atomic_flag stopMe = ATOMIC_FLAG_INIT;
        qint32 miner;
        QVector<quint32> players;
        quint32 currMaxOpers;
        std::mutex mut;
        CUnfiformRandomReal m_realRD;


        void distCash();

        void init(qint32 a_miner, QVector<quint32> a_players);

        inline void setCount()
        {
            std::string errMsg("");
            static const qint32 MAX_OPERS(50);

            static CUnfiformRandomInt countGenerator(0, MAX_OPERS);

            currMaxOpers = countGenerator.getNumber();
            LOGGER_HELPER(INFO, errMsg, "changed the number of operations needed for mining to: [", currMaxOpers, "]");
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

        void playerMain(TMinerSptr a_miner, QVector<quint32> a_players, std::future<void>& f);

    private:

        QVector<quint32> players;
        TMinerSptr miner = nullptr;
        std::atomic_flag stopMe = ATOMIC_FLAG_INIT;
        qint32 vectSize;

        void init(TMinerSptr a_miner, QVector<quint32> a_players);

        inline qint32 peekPlayer(qint32 currIndex)
        {
            static const qint32 PLAYER_SIZE{players.size() - 1};
            static CUnfiformRandomInt playerPeeker(0, PLAYER_SIZE);
            qint32 player = playerPeeker.getNumber();

            if ((currIndex != DONT_CARE) && (currIndex == player))
            {
                if (player > 0)
                {
                    --player;
                }
                else
                {
                    ++player;
                }
            }

            return player;
        }

    };


private:

    std::shared_ptr<SMiner> m_miner;