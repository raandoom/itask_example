#include "itask.h"

ITask::ITask() :
    QObject(),
    QRunnable()
{
    qRegisterMetaType<ITask::Status>("ITask::Status");
    setAutoDelete(false);
}

ITask::~ITask()
{
    cancel();
}

void ITask::cancel()
{
    m_cancel.storeRelaxed(1);
}

bool ITask::isCanceled() const
{
    return m_cancel;
}

QString ITask::desc() const
{
    return m_desc;
}

ITask::Status ITask::status() const
{
    return static_cast<Status>(m_status.loadRelaxed());
}

qint64 ITask::result() const
{
    return m_res.loadRelaxed();
}

QDateTime ITask::datetimeStarted() const
{
    return m_start_dt;
}

QDateTime ITask::datetimeEnded() const
{
    return m_end_dt;
}

void ITask::setDesc(const QString& desc)
{
    m_desc = desc;
}

void ITask::setStatus(Status s)
{
    m_status.storeRelaxed(static_cast<int>(s));
}

void ITask::setResult(qint64 res)
{
    m_res.storeRelaxed(res);
}

void ITask::setDatetimeStarted(const QDateTime& dt)
{
    m_start_dt = dt;
}

void ITask::setDatetimeEnded(const QDateTime& dt)
{
    m_end_dt = dt;
}
