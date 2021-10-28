#ifndef ITASKLISTDELEGATE_H
#define ITASKLISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QMouseEvent>

class ITaskListDelegate : public QStyledItemDelegate
{
    // +--------------------+
    // | X Description      |
    // |   DatetimeStart    |
    // |   DatetimeEnd      |
    // |   Result           |
    // +--------------------+
    enum Line {
        Description,
        DatetimeStart,
        DatetimeEnd,
        Result,

        Count,

        First = Description,
    };

    int margin(const QStyleOptionViewItem& option) const {
        return option.fontMetrics.lineSpacing() / 10;
    }
    QRect abortButtonArea(const QStyleOptionViewItem& option) const {
        int m = margin(option);
        return QRect(option.rect.x() + m * 2,
                     option.rect.y() + m * 2,
                     option.fontMetrics.lineSpacing(),
                     option.fontMetrics.lineSpacing());
    }

public:
    explicit ITaskListDelegate(QObject* parent = nullptr);

protected:
    bool editorEvent(QEvent* event,
                     QAbstractItemModel* model,
                     const QStyleOptionViewItem& option,
                     const QModelIndex& index) override;

    // painting
    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;

    virtual QSize sizeHint(const QStyleOptionViewItem& option,
                           const QModelIndex& index) const override;
};

#endif // ITASKLISTDELEGATE_H
