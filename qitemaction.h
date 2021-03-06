﻿#ifndef QITEMACTION_H
#define QITEMACTION_H

#include <QAction>
#include "dommodel.h"
#include "domitem.h"

class QItemAction : public QAction
{
    Q_OBJECT
public:
    explicit QItemAction(QObject *parent = 0);
    explicit QItemAction(const QString &text, QObject *parent = 0);
    void setNode(QDomNode newNode);
    void setIndex(QModelIndex newIndex);
    void setCP(QPoint newCP);

signals:
//    void clicked(QDomNode);
    void clicked(const QModelIndex&);
public slots:
    void onTrigger();

private:
    QDomNode node;
    QModelIndex index;
    QPoint cp;

};

#endif // QITEMACTION_H
