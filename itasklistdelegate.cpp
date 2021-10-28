#include "itasklistdelegate.h"
#include "itasklistmodel.h"

ITaskListDelegate::ITaskListDelegate(QObject* parent) :
    QStyledItemDelegate(parent)
{

}

bool ITaskListDelegate::editorEvent(QEvent* event,
                                    QAbstractItemModel* model,
                                    const QStyleOptionViewItem& option,
                                    const QModelIndex& index)
{
    ITask::Status status =
            index.data(ITaskListModel::DataRole::Status).value<ITask::Status>();
    if(event->type() == QEvent::MouseButtonRelease &&
            status == ITask::Status::Running)
    {
        QMouseEvent* me = static_cast<QMouseEvent*>(event);
        const QRect& abortRect = abortButtonArea(option);
        if (abortRect.contains(me->pos())) {
            ITaskListModel* im = qobject_cast<ITaskListModel*>(model);
            const QSharedPointer<ITask>& task = im->task(index);
            if (task) {
                task->cancel();
            }
        }
    }

    return QStyledItemDelegate::editorEvent(event,model,option,index);
}

void ITaskListDelegate::paint(QPainter* painter,
                              const QStyleOptionViewItem& option,
                              const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter,option,index);

    painter->save();

    int lsp = option.fontMetrics.lineSpacing();
    int mrg = margin(option);

    painter->save();
    painter->setPen(QPen(Qt::lightGray,2));
    painter->drawRect(option.rect);

    int progressHeight = 4;
    painter->drawLine(option.rect.left(),
                      option.rect.top() + progressHeight / 2,
                      option.rect.right(),
                      option.rect.top() + progressHeight / 2);

    int progress =
            index.data(ITaskListModel::DataRole::Progress).toInt();

    painter->setPen(QPen(Qt::green,progressHeight));
    painter->drawLine(option.rect.left(),
                      option.rect.top() + progressHeight / 2,
                      option.rect.left() + option.rect.width() * static_cast<double>(progress) / 100.,
                      option.rect.top() + progressHeight / 2);

    ITask::Status status =
            index.data(ITaskListModel::DataRole::Status).value<ITask::Status>();

    if (status == ITask::Status::Running) {
        const QRect& abortRect = abortButtonArea(option);

        painter->setPen(QPen(Qt::red,3));
        painter->drawLine(abortRect.topLeft().x() + mrg * 2,
                          abortRect.topLeft().y() + mrg * 2,
                          abortRect.bottomRight().x() - mrg * 2,
                          abortRect.bottomRight().y() - mrg * 2);
        painter->drawLine(abortRect.topRight().x() - mrg * 2,
                          abortRect.topRight().y() + mrg * 2,
                          abortRect.bottomLeft().x() + mrg * 2,
                          abortRect.bottomLeft().y() - mrg * 2);
    }

    painter->restore();

    QString text;
    for (int i = Line::First; i < Line::Count; i++) {
        switch (i) {
        case Line::Description  :
            text = index.data(ITaskListModel::DataRole::Description).toString();
            break;
        case Line::DatetimeStart:
        {
            text = tr("Начало: ");
            const QVariant& dtv = index.data(ITaskListModel::DataRole::DatetimeStart);
            if (!dtv.isNull()) {
                text.append(dtv.toDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz"));
            }
            break;
        }
        case Line::DatetimeEnd:
        {
            text = tr("Конец: ");
            const QVariant& dtv = index.data(ITaskListModel::DataRole::DatetimeEnd);
            if (!dtv.isNull()) {
                text.append(dtv.toDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz"));
            }
            break;
        }
        case Line::Result       :
            switch (status)
            {
            case ITask::Status::Running :
                text = tr("Выполняется");
                break;
            case ITask::Status::Finished :
                text = tr("Завершено: %1").
                        arg(index.data(ITaskListModel::DataRole::Result).toLongLong());
                break;
            case ITask::Status::Canceled :
                text = tr("Отменено");
                break;
            default:
                break;
            }
            break;
        default:
            continue;
        }

        painter->drawText(option.rect.x() + mrg * 4 + lsp,
                          option.rect.y() + lsp * i,
                          option.rect.width() - mrg * 2,
                          lsp,
                          0,
                          text);
    }

    painter->restore();
}

QSize ITaskListDelegate::sizeHint(const QStyleOptionViewItem& option,
                                  const QModelIndex& index) const
{
    Q_UNUSED(index)
    return QSize(option.rect.width(),
                 option.fontMetrics.lineSpacing() * Line::Count + margin(option) * 2);
}
