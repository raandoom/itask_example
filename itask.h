#ifndef ITASK_H
#define ITASK_H

#include <QThread>
#include <QRunnable>
#include <QMetaType>
#include <QDateTime>

class ITask : public QObject, private QRunnable
{
    Q_OBJECT

    QString m_desc;
    QDateTime m_start_dt;
    QDateTime m_end_dt;

public:
    enum class Status {
        Prepared,
        Running,
        Finished,
        Canceled
    };
    Q_ENUM(ITask::Status);

    void cancel();
    bool isCanceled() const;

    QString desc() const;

    int progress()  const;
    Status status() const;
    qint64 result() const;

    QDateTime datetimeStarted() const;
    QDateTime datetimeEnded() const;

    friend class IThreadPool;

protected:
    explicit ITask();
    virtual ~ITask();

    void setDesc(const QString& desc);

    void setProgress(int percent);
    void setStatus(Status s);
    void setResult(qint64 res);

    void setDatetimeStarted(const QDateTime& dt);
    void setDatetimeEnded(const QDateTime& dt);

private:
    QAtomicInteger<int>     m_progress;
    QAtomicInteger<int>     m_status;
    QAtomicInt              m_cancel = 0;
    QAtomicInteger<qint64>  m_res;

signals:
    void progressChanged(int progress);
    void finished(ITask::Status s, qint64 res);
};

#endif // ITASK_H
