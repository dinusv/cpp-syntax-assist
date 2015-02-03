/****************************************************************************
**
** Copyright (C) 2014 Dinu SV.
** (contact: mail@dinusv.com)
** This file is part of C++ Snippet Assist application.
**
** GNU General Public License Usage
** 
** This file may be used under the terms of the GNU General Public License 
** version 3.0 as published by the Free Software Foundation and appearing 
** in the file LICENSE.GPL included in the packaging of this file.  Please 
** review the following information to ensure the GNU General Public License 
** version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/


#ifndef QCONFIGUREDENGINE_HPP
#define QCONFIGUREDENGINE_HPP

#include <QQuickItem>

class QScriptEngine;

namespace csa{

class QCodeBase;

class QConfiguredEngine : public QQuickItem{

    Q_OBJECT

public:
    explicit QConfiguredEngine(QQuickItem *parent = 0);
    QConfiguredEngine(QScriptEngine* internal, QQuickItem *parent = 0);

public slots:
    bool execute(const QString& jsCode);

private:
    QScriptEngine*  m_engine;
};

}

#endif // QCONFIGUREDENGINE_HPP
