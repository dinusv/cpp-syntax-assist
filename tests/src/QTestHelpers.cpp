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


#include "QTestHelpers.hpp"

#include <QJSValue>
#include <QJSValueIterator>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

namespace helpers{

QJsonValue jsonFromScriptValue(const QJSValue& val){
    if ( val.isArray() ){
            QJSValueIterator it(val);

            QJsonArray arr;
            while( it.hasNext() ){
                it.next();

                QJsonValue itJsonValue = jsonFromScriptValue(it.value());
                if( !itJsonValue.isNull() )
                    arr.append(itJsonValue);
            }

            return QJsonValue(arr);
    } else if ( val.isObject() ){
        QJSValueIterator it(val);

        QJsonObject obj;
        while( it.hasNext() ){
            it.next();
            QJsonValue itJsonValue = jsonFromScriptValue(it.value());
            if( !itJsonValue.isNull() )
                obj[it.name()] = itJsonValue;
        }

        return QJsonValue(obj);
    } else  if ( val.isString() ){
        return QJsonValue(val.toString());
    } else if ( val.isBool() ){
        return QJsonValue(val.toString());
    } else if ( val.isNumber() ){
        return QJsonValue(val.toNumber() );
    }
    return QJsonValue();
}

bool compareJsonValues(const QJsonValue& val1, const QJsonValue& val2){
    if ( val1.type() != val2.type() )
        return false;

    if ( val1.isObject() ){
        QJsonObject val1Ob = val1.toObject();
        QJsonObject val2Ob = val2.toObject();

        if ( val1Ob.size() != val2Ob.size() )
            return false;

        for ( QJsonObject::const_iterator it = val1Ob.begin(); it != val1Ob.end(); ++it ){
            if ( !val2Ob.contains(it.key()) )
                return false;

            if ( !compareJsonValues(it.value(), val2Ob[it.key()]) )
                return false;
        }

    } else if ( val1.isArray() ){
        QJsonArray val1Arr = val1.toArray();
        QJsonArray val2Arr = val2.toArray();

        if ( val1Arr.size() != val2Arr.size() )
            return false;

        for ( int i = 0; i < val1Arr.size(); ++i ){
            if ( !compareJsonValues(val1Arr.at(i), val2Arr.at(i) ) )
                return false;
        }

    } else {
        return (val1 == val2);
    }

    return true;
}

QJsonObject parseJsonFile(const QString& filePath, bool* parseOk){
    QFile file(filePath);
    if ( !file.open(QIODevice::ReadOnly ) ){
        if ( parseOk )
            *parseOk = false;
        return QJsonObject();
    }

    QByteArray fileData = file.readAll();
    QJsonDocument fileJsonData = QJsonDocument::fromJson(fileData);

    if( parseOk )
        *parseOk = true;
    return fileJsonData.object();
}

QSharedPointer<csa::QCodeBase> createCodeBaseFromFile(const QString& filePath){
    const char* args[] = {"-c", "-x", "c++"};
    return QSharedPointer<csa::QCodeBase>(new csa::QCodeBase(args, 3, QStringList() << filePath, ""));
}

}
