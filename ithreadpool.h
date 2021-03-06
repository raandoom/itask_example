#ifndef ITHREADPOOL_H
#define ITHREADPOOL_H

#include "itask.h"

#include <QMap>
#include <QThreadPool>
#include <QSharedPointer>

class IThreadPool : public QObject
{
    Q_OBJECT

    QThreadPool* m_pool = nullptr;
    QMap<ITask*,QSharedPointer<ITask>> m_tasks;

public:
    explicit IThreadPool(size_t poolSize) : QObject() {
        m_pool = QThreadPool::globalInstance();
        m_pool->setMaxThreadCount(poolSize);
    }
    ~IThreadPool() {
        for (auto i : m_tasks) {
            i->cancel();
        }
        m_tasks.clear();
    };

    template<typename T, typename ...Args>
    QSharedPointer<ITask> createTask(Args... a) {
        QSharedPointer<ITask> task = QSharedPointer<T>::create(a...);
        m_tasks.insert(task.data(),task);

        connect(task.data(),&ITask::finished,this,[this]() {
            ITask* task = qobject_cast<ITask*>(QObject::sender());
            emit taskEnded(m_tasks.take(task));
        });
        connect(task.data(),&ITask::progressChanged,this,[this](int percent) {
            ITask* task = qobject_cast<ITask*>(QObject::sender());
            emit taskProgress(m_tasks.value(task),percent);
        });

        m_pool->start(task.data());
        return task;
    }

    bool waitForDone(int msecs = -1) {
        return m_pool->waitForDone(msecs);
    }

signals:
    void taskEnded(const QSharedPointer<ITask>& task);
    void taskProgress(const QSharedPointer<ITask>& task, int percent);
};

#endif // ITHREADPOOL_H
