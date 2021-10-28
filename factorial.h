#ifndef FACTORIAL_H
#define FACTORIAL_H

#include "itask.h"

class FactorialTask : public ITask
{
    qint64 m_fac = 1;

public:
    explicit FactorialTask(qint64 n) : ITask(), m_fac(n)
    {
        setDesc(tr("Факториал(%1)").arg(n));
    }

    void run() override {
        setDatetimeStarted(QDateTime::currentDateTime());
        setStatus(Status::Running);

        qint64 res = 1;
        for (qint64 i = 1; i <= m_fac; i++) {
            setProgress(static_cast<double>(i) / m_fac * 100.);
            res *= i;

            if (isCanceled()) {
                setDatetimeEnded(QDateTime::currentDateTime());
                setResult(res);
                setStatus(Status::Canceled);
                emit finished(Status::Canceled,res);
                return;
            }
            QThread::msleep(1e3);
        }

        setDatetimeEnded(QDateTime::currentDateTime());
        setResult(res);
        setStatus(Status::Finished);
        emit finished(Status::Finished,res);
    }
};

class DoubleFactorialTask : public ITask
{
    qint64 m_fac = 1;

public:
    explicit DoubleFactorialTask(qint64 n) : ITask(), m_fac(n)
    {
        setDesc(tr("Двойной Факториал(%1)").arg(n));
    }

    void run() override {
        setDatetimeStarted(QDateTime::currentDateTime());
        setStatus(Status::Running);

        qint64 res = 1;
        for (qint64 i = 2 - m_fac % 2; i <= m_fac; i += 2) {
            setProgress(static_cast<double>(i) / m_fac * 100.);
            res *= i;

            if (isCanceled()) {
                setDatetimeEnded(QDateTime::currentDateTime());
                setResult(res);
                setStatus(Status::Canceled);
                emit finished(Status::Canceled,res);
                return;
            }
            QThread::msleep(1e3);
        }

        setDatetimeEnded(QDateTime::currentDateTime());
        setResult(res);
        setStatus(Status::Finished);
        emit finished(Status::Finished,res);
    }
};

#endif // FACTORIAL_H
