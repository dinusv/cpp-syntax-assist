#include "QCSAInputHandler.hpp"
#include "QCSAPluginLoader.hpp"
#include "QCSAPluginCollection.hpp"
#include "QCSAConsole.hpp"

#include "linenoise.hpp"
#include <iostream>

#include <QJSValue>
#include <QDebug>

using namespace csa;

class QCSACompletionItemPrivate{

public:
    enum Type{
        Object,
        Global
    };

    QCSACompletionItemPrivate(
            QCSACompletionItemPrivate::Type ptype,
            const std::string& pname,
            const std::string& pusage)
        : type(ptype)
        , name(pname)
        , usage(pusage)
    {
    }

    static QCSACompletionItemPrivate* createGlobal(const std::string& pname, const std::string& pusage);
    static QCSACompletionItemPrivate* createForObject(const std::string& pname, const std::string& pusage);

    Type        type;
    std::string name;
    std::string usage;

    bool match(const char* data, Type ptype);
};

inline QCSACompletionItemPrivate *QCSACompletionItemPrivate::createGlobal(
        const std::string &pname,
        const std::string &pusage)
{
    return new QCSACompletionItemPrivate(QCSACompletionItemPrivate::Global, pname, pusage);
}

inline QCSACompletionItemPrivate *QCSACompletionItemPrivate::createForObject(
        const std::string &pname,
        const std::string &pusage)
{
    return new QCSACompletionItemPrivate(QCSACompletionItemPrivate::Object, pname, pusage);
}

inline bool QCSACompletionItemPrivate::match(const char *data, QCSACompletionItemPrivate::Type ptype){
    if ( ptype == type && name.find(data) == 0 )
        return true;

    return false;
}

QCSAInputHandler::QCSAInputHandler(){
    linenoise::SetCompletionCallback(&QCSAInputHandler::completeFunctionDelegate);
    linenoise::SetHistoryMaxLen(20);
    initCompletionList();
}

QCSAInputHandler::~QCSAInputHandler(){
    for(QList<QCSACompletionItemPrivate*>::iterator it = m_completionList.begin(); it != m_completionList.end(); ++it){
        delete *it;
    }
    m_completionList.clear();
}

QCSAInputHandler &QCSAInputHandler::getInstance(){
    static QCSAInputHandler instance;
    return instance;
}

void QCSAInputHandler::initPluginHandlers(
        csa::QCSAPluginLoader *scriptEngine,
        csa::QCSAPluginCollection *pluginCollection)
{
    m_scriptEngine     = scriptEngine;
    m_pluginCollection = pluginCollection;
}

void QCSAInputHandler::initCompletionList(){
    m_completionList.append(QCSACompletionItemPrivate::createGlobal("quit()", "quit()"));
    m_completionList.append(QCSACompletionItemPrivate::createGlobal("console", "console"));
    m_completionList.append(QCSACompletionItemPrivate::createGlobal("cl", "console.log("));
    m_completionList.append(QCSACompletionItemPrivate::createGlobal("nodes(selector)", "nodes("));
    m_completionList.append(QCSACompletionItemPrivate::createGlobal("createFile(file)", "createFile(\'"));
    m_completionList.append(QCSACompletionItemPrivate::createGlobal("reparse(file)", "reparse(\'"));
    m_completionList.append(QCSACompletionItemPrivate::createGlobal("parse(file)", "parse(\'"));
    m_completionList.append(QCSACompletionItemPrivate::createGlobal("makePath(path)", "makePath(\'"));
    m_completionList.append(QCSACompletionItemPrivate::createGlobal("codeBase", "codeBase"));

    m_completionList.append(QCSACompletionItemPrivate::createForObject("children()", "children()"));
    m_completionList.append(QCSACompletionItemPrivate::createForObject("find(selector, typename)", "find(\'"));
    m_completionList.append(QCSACompletionItemPrivate::createForObject("remove()", "remove()"));
}

int QCSAInputHandler::inputLoop(){

    bool loadLastHistory = false;

    while (true){
        size_t suggestionPosition = 0;
        std::string line = linenoise::Readline("csa> ", loadLastHistory, &suggestionPosition);

        if ( suggestionPosition ){
            std::string subline = line.substr(0, suggestionPosition);
            std::vector<std::string> completions;
            completeFunction(subline.c_str(), completions);
            for ( std::vector<std::string>::iterator it = completions.begin(); it != completions.end(); ++it ){
                QCSAConsole::log(QCSAConsole::General, QString(it->c_str()));
            }
        } else {
            if (line == "quit()")
                break;

            if ( m_scriptEngine ){
                QJSValue result;
                if ( m_scriptEngine->execute(QString::fromStdString(line), result) ){
                    loadLastHistory = false;
                    QCSAConsole::log(QCSAConsole::General, result.toString());
                } else {
                    loadLastHistory = true;
                }
            } else {
                QCSAConsole::logError("Fatal error: Failed to initialize script engine.");
                return -1;
            }
            linenoise::AddHistory(line.c_str());
        }
    }
    return 0;
}

void QCSAInputHandler::completeFunctionDelegate(const char *editBuffer, std::vector<std::string> &completions){
    getInstance().completeFunction(editBuffer, completions, false);
}

void QCSAInputHandler::completeFunction(
        const char *editBuffer,
        std::vector<std::string> &completions,
        bool nameCompletions)
{

    bool isEscapeFlag      = false;
    bool isQuoteFlag       = false;
    bool isDoubleQuoteFlag = false;
    bool isGlobalFlag      = true;

    int  lastPunctuationPosition = 0;

    int position = 0;
    const char* c = editBuffer;
    while (*c){
        switch(*c){
        case '\\':
            if ( isQuoteFlag || isDoubleQuoteFlag ){
                isEscapeFlag = !isEscapeFlag;
            }
            break;
        case '\'':
            if ( !isEscapeFlag && isQuoteFlag ){
                isQuoteFlag = false;
            } else if ( !isQuoteFlag ){
                isQuoteFlag = true;
            }
            isEscapeFlag = false;
            break;
        case '\"':
            if ( !isEscapeFlag && isDoubleQuoteFlag ){
                isDoubleQuoteFlag = false;
            } else if ( !isDoubleQuoteFlag ){
                isDoubleQuoteFlag = true;
            }
            isEscapeFlag = false;
            break;
        case '(':
        case ')':
        case '{':
        case '}':
        case ';':
        case ',':
        case '+':
        case '-':
        case '*':
        case ' ':
        case '/':
            if ( !isQuoteFlag && !isDoubleQuoteFlag ){
                lastPunctuationPosition = position + 1;
                isGlobalFlag = true;
            }
            break;
        case '.':
            if ( !isQuoteFlag && !isDoubleQuoteFlag ){
                lastPunctuationPosition = position + 1;
                isGlobalFlag = false;
            }
            break;
        }

        ++position;
        ++c;
    }

    const char* editBufferCut = &editBuffer[lastPunctuationPosition];
    if ( strlen(editBufferCut) > 0 && !isQuoteFlag && !isDoubleQuoteFlag ){

        // Search local completions

        for ( QList<QCSACompletionItemPrivate*>::iterator it = m_completionList.begin();
              it != m_completionList.end();++it
        ){
            if ( (*it)->match(
                     editBufferCut,
                    isGlobalFlag ? QCSACompletionItemPrivate::Global : QCSACompletionItemPrivate::Object) )
            {
                completions.push_back(nameCompletions ? (*it)->name : (*it)->usage);
            }
        }

        // Search plugin completions

        m_pluginCollection->filter(QString(editBufferCut));
        for ( int i = 0; i < m_pluginCollection->totalRows(); ++i ){
            completions.push_back(
                nameCompletions ? m_pluginCollection->name(i).toStdString() : m_pluginCollection->usage(i).toStdString()
            );
        }
    }
}
