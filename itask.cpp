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
    m_cancel.store(1);
}

bool ITask::isCanceled() const
{
    return m_cancel;
}

QString ITask::desc() const
{
    return m_desc;
}

int ITask::progress() const
{
    return m_progress.load();
}

ITask::Status ITask::status() const
{
    return static_cast<Status>(m_status.load());
}

qint64 ITask::result() const
{
    return m_res.load();
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

void ITask::setProgress(int percent)
{
    m_progress.store(percent);
    emit progressChanged(percent);
}

void ITask::setStatus(Status s)
{
    m_status.store(static_cast<int>(s));
}

void ITask::setResult(qint64 res)
{
    m_res.store(res);
}

void ITask::setDatetimeStarted(const QDateTime& dt)
{
    m_start_dt = dt;
}

void ITask::setDatetimeEnded(const QDateTime& dt)
{
    m_end_dt = dt;
}
