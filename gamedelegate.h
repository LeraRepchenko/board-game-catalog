#ifndef GAMEDELEGATE_H
#define GAMEDELEGATE_H

#include <QStyledItemDelegate>

class GameDelegate : public QStyledItemDelegate
{
public:
    explicit GameDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
};

#endif // GAMEDELEGATE_H