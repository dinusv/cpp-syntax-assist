/****************************************************************************
**
** Copyright (C) 2014-2015 Dinu SV.
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


#ifndef QTESTHELPERS_HPP
#define QTESTHELPERS_HPP

#include <QJsonValue>
#include <QSharedPointer>
#include "QCodeBase.hpp"

class QJSValue;

namespace helpers{

    QJsonValue jsonFromScriptValue(const QJSValue& val);
    bool compareJsonValues(const QJsonValue& val1, const QJsonValue& val2);
    QJsonObject parseJsonFile(const QString& filePath, bool* parseOk = 0);

    QSharedPointer<csa::QCodeBase> createCodeBaseFromFile(
            const QString& filePath,
            csa::QCodeBaseObserver* observer = 0
    );

}// namespace

#endif // QTESTHELPERS_HPP
