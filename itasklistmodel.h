#ifndef ITASKLISTMODEL_H
#define ITASKLISTMODEL_H

#include "itask.h"

#include <QMap>
#include <QSharedPointer>
#include <QAbstractListModel>

class ITaskListModel : public QAbstractListModel
{
    Q_OBJECT

    QMap<ITask*,QSharedPointer<ITask>> m_data;

public:
    enum DataRole {
        Description = Qt::UserRole + 1,
        Status,
        Result,
        DatetimeStart,
        DatetimeEnd,
    };

    explicit ITaskListModel(QObject* parent = nullptr);

    void addTask(const QSharedPointer<ITask>& ptr);
    void removeTask(const QSharedPointer<ITask>& ptr);

    QSharedPointer<ITask> task(const QModelIndex& index);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
};

#endif // ITASKLISTMODEL_H
