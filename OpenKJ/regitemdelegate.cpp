/*
 * Copyright (c) 2013-2019 Thomas Isaac Lightburn
 *
 *
 * This file is part of OpenKJ.
 *
 * OpenKJ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "regitemdelegate.h"

#include <QSqlQuery>
#include "settings.h"

extern Settings *settings;

RegItemDelegate::RegItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}


void RegItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize sbSize(QFontMetrics(settings->applicationFont()).height(), QFontMetrics(settings->applicationFont()).height());

    int topPad = (option.rect.height() - sbSize.height()) / 2;
    int leftPad = (option.rect.width() - sbSize.width()) / 2;
    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());
    if (index.column() == 2)
    {
        QSqlQuery query;
        query.exec("SELECT COUNT(*) FROM regularsongs WHERE regsingerid == " + index.sibling(index.row(), 0).data().toString());
        if (query.first())
        {
            painter->save();
            if (option.state & QStyle::State_Selected)
            {
                painter->setPen(option.palette.highlightedText().color());
                painter->fillRect(option.rect, option.palette.highlight());
            }
            painter->drawText(option.rect, Qt::TextSingleLine | Qt::AlignVCenter | Qt::AlignCenter, query.value(0).toString());
            painter->restore();

        }
        return;
    }
    if (index.column() == 3)
    {
        painter->drawImage(QRect(option.rect.x() + leftPad,option.rect.y() + topPad, sbSize.width(), sbSize.height()), QImage(":/icons/Icons/list-add-user-small.png").scaled(sbSize));
        return;
    }
    if (index.column() == 4)
    {
        painter->drawImage(QRect(option.rect.x() + leftPad,option.rect.y() + topPad, sbSize.width(), sbSize.height()), QImage(":/icons/Icons/edit-delete.png").scaled(sbSize));
        return;
    }
    painter->save();
    if (option.state & QStyle::State_Selected)
        painter->setPen(option.palette.highlightedText().color());
    painter->drawText(option.rect, Qt::TextSingleLine | Qt::AlignVCenter, " " + index.data().toString());
    painter->restore();
}
