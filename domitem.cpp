﻿/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtXml>

#include "domitem.h"

DomItem::DomItem(QDomNode &node, int row, DomItem *parent)
{
    domNode = node;
    // Record the item's location within its parent.
    rowNumber = row;
    parentItem = parent;

    // Add childItems here
    for(int i=0; i<node.childNodes().count(); i++)
    {
        QDomNode childNode = node.childNodes().item(i);
        DomItem *childItem = new DomItem(childNode, i, this);
        childItems.insert(i,childItem);
    }
}

DomItem::~DomItem()
{
//    QHash<int,DomItem*>::iterator it;
//    for (it = childItems.begin(); it != childItems.end(); ++it)
//       delete it.value();
    qDeleteAll(childItems);
}

QDomNode DomItem::node() const
{
    return domNode;
}

DomItem *DomItem::parent()
{
    return parentItem;
}

DomItem *DomItem::child(int i)
{
       qDebug()<<i<<'\t'<<domNode.nodeName();
       qDebug()<<domNode.childNodes().count();
       for(int j=0; j<domNode.childNodes().count(); j++)
           qDebug()<<domNode.childNodes().at(j).nodeName();
       for(int j=0; j<childItems.size(); j++)
           qDebug()<<childItems.value(j)->node().nodeName();
/*    if (childItems.contains(i))
        return childItems[i];

    if (i >= 0 && i < domNode.childNodes().count()) {
        QDomNode childNode = domNode.childNodes().item(i);
        DomItem *childItem = new DomItem(childNode, i, this);
        childItems[i] = childItem;
        return childItem;
    }
    return 0;*/
//    if(childItems.contains(childItems.value(i)))
//    {
//        qDebug()<<childItems.value(i)->node().toElement().tagName();
        return childItems.value(i);
//    }

/*    if (i >= 0 && i < domNode.childNodes().count()) {
        QDomNode childNode = domNode.childNodes().item(i);
        {
            DomItem *childItem = new DomItem(childNode, i, this);
            childItems.insert(i, childItem);
            return childItem;
        }
    }
    return 0;*/
}

int DomItem::row()
{
    return rowNumber;
}

int DomItem::childNum()
{
    return childItems.size();
}

bool DomItem::insertChildren(int position, DomItem item)
{
    if (position < 0 || position > childItems.size())
        return false;

//    DomItem *item = new DomItem(node, position, this);
    item.parentItem = this;
    item.rowNumber = position;

    qDebug()<<childItems.size();
    childItems.insert(position, &item);
    qDebug()<<childItems.size();

    return true;
}

bool DomItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);
//        childItems.remove(position);

    return true;
}
