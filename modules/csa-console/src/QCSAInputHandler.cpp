#include "QCSAInputHandler.hpp"
#include "QCSAPluginLoader.hpp"
#include "QCSACompletionSet.hpp"
#include "QCSAConsole.hpp"

#include "linenoise.hpp"
#include <iostream>

#include <QJSValue>
#include <QDebug>

using namespace csa;


QCSAInputHandler::QCSAInputHandler(){
    linenoise::SetCompletionCallback(&QCSAInputHandler::completeFunctionDelegate);
    linenoise::SetHistoryMaxLen(20);
}

QCSAInputHandler::~QCSAInputHandler(){
}

QCSAInputHandler &QCSAInputHandler::getInstance(){
    static QCSAInputHandler instance;
    return instance;
}

void QCSAInputHandler::initPluginHandlers(
        csa::QCSAPluginLoader *scriptEngine,
        csa::QCSACompletionSet *pluginCollection)
{
    m_scriptEngine  = scriptEngine;
    m_completionSet = pluginCollection;

    initCompletionList();
}

void QCSAInputHandler::initCompletionList(){
    m_completionSet->initDefaultCompletions();
}

int QCSAInputHandler::inputLoop(){

    bool loadLastHistory = false;

    while (true){
        size_t suggestionPosition = 0;
        std::string line = linenoise::Readline("csa> ", loadLastHistory, &suggestionPosition);

        if ( suggestionPosition ){
            loadLastHistory = true;
            if ( suggestionPosition > 0 ? line[suggestionPosition - 1] == '?' : false){
                QString contextSubline = QString::fromStdString(line.substr(0, suggestionPosition - 1));

                QCSACompletionItem::Type completionType;
                QString context = m_completionSet->getCompletionContext(contextSubline, &completionType);
                if ( context != "" ){
                    std::string::iterator it = line.begin() +  suggestionPosition;
                    while ( it != line.end() ){
                        if ( isalnum(*it) || *it == '$' || *it == '_')
                            context += QChar(*it);
                        else
                            break;
                        ++it;
                    }
                }
                QString description = m_completionSet->getDescription(context, completionType);
                QCSAConsole::log(QCSAConsole::General, context + "\n" + description);

                line = line.substr(0, suggestionPosition - 1) + line.substr(suggestionPosition);

            } else {
                QCSACompletionItem::Type completionType;
                QString subline = QString::fromStdString(line.substr(0, suggestionPosition));
                QString context = m_completionSet->getCompletionContext(subline, &completionType);

                QList<QCSACompletionItem*> items = m_completionSet->getCompletionItems(context, completionType);

                for ( QList<QCSACompletionItem*>::iterator it = items.begin(); it != items.end(); ++it ){
                    QCSAConsole::log(QCSAConsole::General, (*it)->name());
                }
            }
            linenoise::AddHistory(line.c_str());

        } else {
            QString strLine = QString::fromStdString(line).trimmed();
            if (strLine == "quit()")
                break;

            if ( m_scriptEngine ){
                QJSValue result;
                if ( m_scriptEngine->execute(strLine, result) ){
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
    getInstance().completeFunction(editBuffer, completions);
}

void QCSAInputHandler::completeFunction(const char *editBuffer, std::vector<std::string> &completions){

    QCSACompletionItem::Type completionType;
    QString editBufferStr = editBuffer;
    QString context = m_completionSet->getCompletionContext(editBufferStr, &completionType);
    QString preffix = editBufferStr.mid(0, editBufferStr.length() - context.length());

    QList<QCSACompletionItem*> items = m_completionSet->getCompletionItems(context, completionType);

    for ( QList<QCSACompletionItem*>::iterator it = items.begin(); it != items.end(); ++it ){
        int bracketIndex = (*it)->usage().indexOf('(');
        if ( bracketIndex > -1 )
            ++bracketIndex;
        completions.push_back( (preffix + (*it)->usage().mid(0, bracketIndex)).toStdString() );
    }
}
