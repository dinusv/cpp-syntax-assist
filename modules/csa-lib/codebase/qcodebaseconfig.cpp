#include "qcodebaseconfig.h"
#include "qjsonconvert.h"
#include "qcsaconsole.h"

#include <QJSValue>
#include <QJSValueIterator>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace csa{

QCodebaseConfig::QCodebaseConfig(
        QJSEngine* engine,
        const QString& configPath,
        const QString& configName,
        QObject* parent)
    : QObject(parent)
    , m_engine(engine)
    , m_configPath(configPath)
    , m_translationUnitNumArgs(0)
    , m_translationUnitArgs(0)
{
    update(require(configName));
}

QCodebaseConfig::~QCodebaseConfig(){
    freeTranslationUnitArgs();
}

QJSValue QCodebaseConfig::require(const QString& config){
    QFileInfo relativeConfigInfo(config);
    if ( relativeConfigInfo.exists() && relativeConfigInfo.isFile() )
        return requireFile(config);

    QString extensionConfig = config.toLower().endsWith(".json") ? config : config + ".json";
    QFileInfo absConfigInfo(QDir::cleanPath(m_configPath + QDir::separator() + extensionConfig));
    if ( absConfigInfo.exists() && absConfigInfo.isFile() )
        return requireFile(absConfigInfo.filePath());

    return QJSValue();
}

void QCodebaseConfig::save(QString filePath){
    if ( filePath.isEmpty() )
        filePath = m_configFile;
    if ( !filePath.contains("/") && !filePath.contains("\\") ){
        if ( !filePath.toLower().endsWith(".json") )
            filePath = filePath + ".json";
        filePath = QDir::cleanPath(m_configPath + QDir::separator() + filePath);
    }
//    qDebug() << filePath;
    QFile file(filePath);
    if ( !file.open(QFile::WriteOnly) ){
        QCSAConsole::logError("Failed to open file for writing: " + filePath);
        return;
    }
    file.write(QJsonDocument(serializeData()).toJson(QJsonDocument::Indented));

    m_configPath = filePath;
}

QJsonObject QCodebaseConfig::serializeData(){
    QJsonObject root;

    // Translation Unit Args
    QJsonArray clangArgs;
    for ( int i = 0; i < m_translationUnitNumArgs; ++i )
        clangArgs.append(QJsonValue(m_translationUnitArgs[i]));
    root["clangArgs"] = clangArgs;


    QJsonArray headerFilePatterns;
    for ( int i = 0; i < m_headerFilePatterns.size(); ++i )
        headerFilePatterns.append(QJsonValue(m_headerFilePatterns[i]));
    root["headerFilePatterns"] = headerFilePatterns;

    QJsonArray sourceFilePatterns;
    for ( int i = 0; i < m_sourceFilePatterns.size(); ++i )
        sourceFilePatterns.append(QJsonValue(m_sourceFilePatterns[i]));
    root["sourceFilePatterns"] = sourceFilePatterns;

    QJsonArray includeFiles;
    for ( int i = 0; i < m_includeFiles.size(); ++i )
        includeFiles.append(QJsonValue(m_includeFiles[i]));
    root["includeFiles"] = includeFiles;
    root["includeCode"]  = m_includeCode;

    return root;
}

QJSValue QCodebaseConfig::requireFile(const QString& path){
    QFile file(path);
    if ( !file.open(QFile::ReadOnly) ){
        QCSAConsole::logError("Failed to open file: " + path);
        return QJSValue();
    }
    QByteArray data = file.readAll();
    file.close();

    QJsonParseError jsonerror;
    QJsonDocument jsondoc = QJsonDocument::fromJson(data, &jsonerror);
    if ( jsonerror.error != QJsonParseError::NoError ){
        QCSAConsole::logError("Failed to parse file: " + path + ". Error: " + jsonerror.errorString());
        return QJSValue();
    }
    return QJsonConvert::scriptValueFromJsonDoc(m_engine, jsondoc);
}

void QCodebaseConfig::parseTranslationUnitArgs(const QJSValue& val){
    m_translationUnitNumArgs = 0;
    QJSValueIterator countIt(val);
    while( countIt.hasNext() ){
        countIt.next();
        if ( countIt.name() != "length" )
            ++m_translationUnitNumArgs;
    }

    m_translationUnitArgs = new char*[m_translationUnitNumArgs];

    int i = 0;
    QJSValueIterator it(val);
    while( it.hasNext() ){
        it.next();
        if ( it.name() != "length" ){
            QByteArray arg = it.value().toString().toUtf8();
            m_translationUnitArgs[i] = new char[arg.length() + 1];
            strcpy(m_translationUnitArgs[i], arg.constData());
            m_translationUnitArgs[i][arg.length()] = '\0';
        }
        ++i;
    }
}

void QCodebaseConfig::freeTranslationUnitArgs(){
    if ( m_translationUnitArgs ){
        for ( int i = 0; i < m_translationUnitNumArgs; ++i ){
            delete [] m_translationUnitArgs[i];
        }
        delete[] m_translationUnitArgs;
    }
}

QStringList QCodebaseConfig::listFromJsArray(const QJSValue &array){
    QStringList base;

    QJSValueIterator it(array);
    while( it.hasNext() ){
        it.next();
        base << it.value().toString();
    }

    return base;
}

void QCodebaseConfig::update(const QJSValue& val){
    if ( val.hasProperty("clangArgs") )
        parseTranslationUnitArgs(val.property("clangArgs"));
    if ( val.hasProperty("headerFilePatterns") )
        m_headerFilePatterns = listFromJsArray(val.property("headerFilePatterns"));
    if ( val.hasProperty("sourceFilePatterns") )
        m_sourceFilePatterns = listFromJsArray(val.property("sourceFilePatterns"));
    if ( val.hasProperty("includeFiles") )
        m_includeFiles = listFromJsArray(val.property("includeFiles"));
    if ( val.hasProperty("includeCode") )
        m_includeCode = val.property("includeCode").toString();

    emit dataChanged();
}

}// namespace
