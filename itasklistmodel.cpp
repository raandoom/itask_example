#include "itasklistmodel.h"

ITaskListModel::ITaskListModel(QObject* parent) :
    QAbstractListModel(parent)
{
}

void ITaskListModel::addTask(const QSharedPointer<ITask>& ptr)
{
    beginResetModel();
    m_data.insert(ptr.data(),ptr);
    endResetModel();

    emit dataChanged(QModelIndex(),QModelIndex());
}
void ITaskListModel::removeTask(const QSharedPointer<ITask>& ptr)
{
    beginResetModel();
    m_data.remove(ptr.data());
    endResetModel();

    emit dataChanged(QModelIndex(),QModelIndex());
}

QSharedPointer<ITask> ITaskListModel::task(const QModelIndex& index)
{
    if (!index.isValid())
        return nullptr;

    return m_data.values().at(index.row());
}

int ITaskListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_data.size();
}

QVariant ITaskListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    QSharedPointer<ITask> ptr = m_data.values().at(index.row());

    switch (role) {
    case DataRole::Description  : return ptr->desc();
    case DataRole::Status       : return static_cast<int>(ptr->status());
    case DataRole::Result       : return ptr->result();
    case DataRole::DatetimeStart: return ptr->datetimeStarted();
    case DataRole::DatetimeEnd  : return ptr->datetimeEnded();
    case DataRole::Progress     : return ptr->progress();
    default:
        break;
    }

    return QVariant();
}
