
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
