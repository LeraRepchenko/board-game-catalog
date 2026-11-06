#include "gamedelegate.h"
#include <QPainter>
#include <QPalette>
#include <QStyle>

GameDelegate::GameDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void GameDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{

    painter->save();


    QRect r = option.rect;
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(r, option.palette.brush(QPalette::Highlight));
    } else {

        painter->fillRect(r, index.row() % 2 == 0 ? QColor("#f9f9f9") : QColor("#ffffff"));
    }


    painter->setPen(option.state & QStyle::State_Selected ? Qt::white : Qt::black);
    QFont font = painter->font();
    font.setBold(true);
    painter->setFont(font);


    QString text = index.data(Qt::DisplayRole).toString();


    painter->drawText(r.adjusted(10, 0, -10, 0), Qt::AlignVCenter | Qt::AlignLeft, text);

    // 4. Рисуем декоративную линию снизу
    painter->setPen(QPen(QColor(0, 0, 0, 30), 1));
    painter->drawLine(r.bottomLeft(), r.bottomRight());


    painter->restore();
}