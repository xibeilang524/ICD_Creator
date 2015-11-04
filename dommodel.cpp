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

#include <QtGui>
#include <QtXml>

#include "domitem.h"
#include "dommodel.h"

DomModel::DomModel(QDomDocument document, QObject *parent)
    : QAbstractItemModel(parent), domDocument(document)
{
    rootItem = new DomItem(document, 0);
}

DomModel::~DomModel()
{
    delete rootItem;
}

int DomModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

QVariant DomModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    DomItem *item = static_cast<DomItem*>(index.internalPointer());

    QDomNode node = item->node();

    QStringList attributes;
    QDomNamedNodeMap attributeMap = node.attributes();

    QStringList text;

    switch (index.column()) {
        case 0:/*
            if(node.nodeType() == QDomNode::ElementNode)
                return node.nodeName();
            else if(node.nodeType() == QDomNode::CommentNode)
                return node.nodeValue();
            else if(node.nodeType() == QDomNode::ProcessingInstructionNode)
                return node.nodeName();
            else if(node.nodeType() == QDomNode::TextNode)
                return node.nodeValue();*/
        return node.nodeName();
/*
        case 1:
            for (int i = 0; i < attributeMap.count(); ++i) {
                QDomNode attribute = attributeMap.item(i);
                attributes << attribute.nodeName() + "=\""
                              +attribute.nodeValue() + "\"";
            }
            return attributes.join(" ");*/
        case 1:
            for (int i = 0; i < node.childNodes().count(); ++i) {
                if(node.childNodes().at(i).nodeType() == QDomNode::TextNode)
                    text << node.childNodes().at(i).nodeValue().split("\n").join(" ");
            }
            return text.join(" ");
        default:
            return QVariant();
    }
}

Qt::ItemFlags DomModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant DomModel::headerData(int section, Qt::Orientation orientation,
                              int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0:
                return tr("Name");
/*            case 1:
                return tr("Attributes");//*/
            case 1:
                return tr("Value");
            default:
                return QVariant();
        }
    }

    return QVariant();
}

QModelIndex DomModel::index(int row, int column, const QModelIndex &parent)
            const
{
//    qDebug()<<"Here is DomModel index"<<row<<" "<<column;
    if (!hasIndex(row, column, parent))
    {
//        qDebug()<<"there is no Index";
        return QModelIndex();
    }

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DomItem*>(parent.internalPointer());

//    qDebug()<<"The parentItem name is "<<parentItem->node().toElement().tagName();

    DomItem *childItem = parentItem->child(row);
//    qDebug()<<"The childItem name is "<<childItem->node().toElement().tagName();
    if (childItem->node().nodeType() == QDomNode::ElementNode)//||childItem->node().nodeType() == QDomNode::TextNode)
//    if(childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex DomModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    DomItem *childItem = static_cast<DomItem*>(child.internalPointer());
    DomItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == rootItem )
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int DomModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DomItem*>(parent.internalPointer());

    int count = parentItem->node().childNodes().count();
    for(int i=0; i<parentItem->node().childNodes().count(); i++)
        if(parentItem->node().childNodes().at(i).nodeType() == QDomNode::TextNode)
            count--;

    return count;//parentItem->node().childNodes().count();
}

DomItem *DomModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        DomItem *item = static_cast<DomItem*>(index.internalPointer());
        if (item) return item;
    }
    return rootItem;
}

void DomModel::update()
{
    resetInternalData();
//    rootItem = new DomItem(domDocument, 0);
}

bool DomModel::insertRows(int position, DomItem item, const QModelIndex &parent)
{
    DomItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position);
    success = parentItem->insertChildren(position, item);

//    DomItem *childItem = parentItem->child(position);
//    if (childItem->node().nodeType() == QDomNode::ElementNode)//||childItem->node().nodeType() == QDomNode::TextNode)
//        createIndex(position, 0, childItem);

    endInsertRows();

    return success;
}

bool DomModel::removeRows(int row, int count, const QModelIndex &parent)
{
    qDebug()<<"in RemoveRows";
    DomItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, row, row + count - 1);
    success = parentItem->removeChildren(row, count);
    endRemoveRows();

    return success;
}

bool DomModel::moveUpRows(const QModelIndex &index)//, int sourceFirst, int sourceLast, const QModelIndex &destinationParent, int position)
{
    qDebug()<<"in MoveRows";
    bool success = true;
    QModelIndex parentIndex = index.parent();
    DomItem* item = getItem(index);
    int row = index.row();

    beginMoveRows(parentIndex, row, row, parentIndex, row-1);
    insertRows(row-1, *item, parentIndex);
    success = removeRows(row,1,parentIndex);

    endMoveRows();

    return success;
}
