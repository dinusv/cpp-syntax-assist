#include "qjsonconvert.h"
#include <QJSValueIterator>
#include <QJsonArray>
#include <QJSEngine>

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

QJSValue QJsonConvert::scriptValueFromJson(QJSEngine* engine, const QJsonValue& jsonval){
    if ( jsonval.isArray() ){
        return scriptValueFromJsonArray(engine, jsonval.toArray());
    } else if ( jsonval.isObject() ){
        return scriptValueFromJsonObject(engine, jsonval.toObject());
    } else if ( jsonval.isBool() ){
        return QJSValue(jsonval.toBool());
    } else if ( jsonval.isDouble() ){
        return QJSValue(jsonval.toDouble());
    } else if ( jsonval.isString() ){
        return QJSValue(jsonval.toString());
    } else if ( jsonval.isNull() ){
        return QJSValue(QJSValue::NullValue);
    } else
        return QJSValue(QJSValue::UndefinedValue);
}

QJSValue QJsonConvert::scriptValueFromJsonDoc(QJSEngine* engine, const QJsonDocument& jsondoc){
    if (jsondoc.isObject()) {
        return scriptValueFromJsonObject(engine, jsondoc.object());
    } else if (jsondoc.isArray()) {
        return scriptValueFromJsonArray(engine, jsondoc.array());
    } else if(jsondoc.isNull()){
        return QJSValue(QJSValue::NullValue);
    } else {
        return QJSValue();
    }
}

QJSValue QJsonConvert::scriptValueFromJsonObject(QJSEngine* engine, const QJsonObject& jsonobject){
    QJSValue val = engine->newObject();
    for ( QJsonObject::ConstIterator it = jsonobject.constBegin(); it != jsonobject.constEnd(); ++it ){
        val.setProperty(it.key(), scriptValueFromJson(engine, it.value()));
    }
    return val;
}

QJSValue QJsonConvert::scriptValueFromJsonArray(QJSEngine* engine, const QJsonArray& jsonarray){
    QJSValue val = engine->newArray(jsonarray.size());
    int i = 0;
    for ( QJsonArray::ConstIterator it = jsonarray.constBegin(); it != jsonarray.constEnd(); ++it ){
        val.setProperty(i++, scriptValueFromJson(engine, *it));
    }
    return val;
}


QJsonConvert::QJsonConvert(){
}

}// namespace
