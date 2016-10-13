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
#include <QTemporaryFile>

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
    , m_translationUnitFullArgs(0)
    , m_translationUnitArgsDirty(false)
    , m_includeCodeDirty(false)
    , m_includeFile()
{
    update(require(configName));
}

QCodebaseConfig::~QCodebaseConfig(){
    freeTranslationUnitArgs();
}

void QCodebaseConfig::translationUnitArgsUpdate() const{
    if ( m_includeCodeDirty ){
        if ( !m_includeFile.open() )
            QCSAConsole::logError("Failed to append include code due to inability to open include file.");
        m_includeFile.write(m_includeCode.toUtf8());
        m_includeFile.close();
        m_includeCodeDirty = false;
        m_translationUnitArgsDirty = true;
    }
    if ( m_translationUnitArgsDirty ){
        freeTranslationUnitArgs();

        m_translationUnitNumArgs = m_translationUnitArgs.size();
        m_translationUnitNumArgs += m_includeCode.isEmpty() ? 0 : 2;

        m_translationUnitFullArgs = new char*[m_translationUnitNumArgs];
        for ( int i = 0; i < m_translationUnitArgs.size(); ++i ){
            QByteArray arg = m_translationUnitArgs[i].toUtf8();
            m_translationUnitFullArgs[i] = new char[arg.length() + 1];
            strcpy(m_translationUnitFullArgs[i], arg.constData());
            m_translationUnitFullArgs[i][arg.length()] = '\0';
        }

        if ( !m_includeCode.isEmpty() ){
            const char* includeStr = "-include";
            int currentIndex = m_translationUnitArgs.size();
            m_translationUnitFullArgs[currentIndex] = new char[9];
            strcpy(m_translationUnitFullArgs[currentIndex], includeStr);
            m_translationUnitFullArgs[currentIndex][8] = '\0';

            ++currentIndex;
            QByteArray includeFile = m_includeFile.fileName().toUtf8();
            m_translationUnitFullArgs[currentIndex] = new char[includeFile.length() + 1];
            strcpy(m_translationUnitFullArgs[currentIndex], includeFile.constData());
            m_translationUnitFullArgs[currentIndex][includeFile.length()] = '\0';
        }

        m_translationUnitArgsDirty = false;
    }
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
    for ( int i = 0; i < m_translationUnitArgs.size(); ++i )
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
    m_translationUnitArgs.clear();
    QJSValueIterator it(val);
    while( it.hasNext() ){
        it.next();
        if ( it.name() != "length" )
            m_translationUnitArgs.append(it.value().toString());
    }
    m_translationUnitArgsDirty = true;
}

void QCodebaseConfig::freeTranslationUnitArgs() const{
    if ( m_translationUnitFullArgs ){
        for ( int i = 0; i < m_translationUnitNumArgs; ++i ){
            delete [] m_translationUnitFullArgs[i];
        }
        delete[] m_translationUnitFullArgs;
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
    if ( val.hasProperty("includeCode") ){
        m_includeCodeDirty = true;
        m_includeCode = val.property("includeCode").toString();
    }

    emit dataChanged();
}

}// namespace
