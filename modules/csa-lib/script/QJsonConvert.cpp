#include "QJsonConvert.hpp"
#include <QJSValueIterator>
#include <QJsonArray>

namespace csa{


QJsonValue QJsonConvert::jsonFromScriptValue(const QJSValue& jsval){
    if ( jsval.isArray() ){
        QJSValueIterator it(jsval);

        QJsonArray arr;
        while( it.hasNext() ){
            it.next();

            QJsonValue itJsonValue = jsonFromScriptValue(it.value());
            if( !itJsonValue.isNull() )
                arr.append(itJsonValue);
        }

        return QJsonValue(arr);
    } else if ( jsval.isObject() ){
        QJSValueIterator it(jsval);

        QJsonObject obj;
        while( it.hasNext() ){
            it.next();
            QJsonValue itJsonValue = jsonFromScriptValue(it.value());
            if( !itJsonValue.isNull() )
                obj[it.name()] = itJsonValue;
        }
        return QJsonValue(obj);
    } else  if ( jsval.isString() ){
        return QJsonValue(jsval.toString());
    } else if ( jsval.isBool() ){
        return QJsonValue(jsval.toString());
    } else if ( jsval.isNumber() ){
        return QJsonValue(jsval.toNumber() );
    }
    return QJsonValue();
}

QJSValue QJsonConvert::scriptValueFromJson(const QJsonValue& jsonval){
//TODO
}


QJsonConvert::QJsonConvert(){
}

}// namespace
