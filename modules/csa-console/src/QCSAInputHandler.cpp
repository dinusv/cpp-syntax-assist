#include "QCSAInputHandler.hpp"
#include "QCSAPluginLoader.hpp"
#include "QCSAPluginCollection.hpp"
#include "QCSAConsole.hpp"

#include "linenoise.hpp"
#include <iostream>

#include <QJSValue>

using namespace csa;

class QCSACompletionItemPrivate{
public:

};


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
        csa::QCSAPluginCollection *pluginCollection)
{
    m_scriptEngine     = scriptEngine;
    m_pluginCollection = pluginCollection;
}

int QCSAInputHandler::inputLoop(){

    bool loadLastHistory = false;

    while (true){
        size_t suggestionPosition = 0;
        std::string line = linenoise::Readline("csa> ", loadLastHistory, &suggestionPosition);

        if (line == "quit()")
            break;

        if ( suggestionPosition ){

        } else {
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

//        std::cout <<  "echo: '" << line << "'" << std::endl;
//        std::cout <<  "pos:  "  << suggestionPosition << std::endl;

    }
    return 0;
}

void QCSAInputHandler::completeFunctionDelegate(const char *editBuffer, std::vector<std::string> &completions){
    getInstance().completeFunction(editBuffer, completions);
}

void QCSAInputHandler::completeFunction(const char *editBuffer, std::vector<std::string> &completions){
    const char* dotPosition = strrchr(editBuffer, '.');
    if (dotPosition == NULL)
        dotPosition = editBuffer;

    std::string searchFilter = dotPosition;
    size_t i = 0;
    while ( i < searchFilter.size() ){
        char& c = searchFilter[i];
        if ( !isalnum(c) && c != '_' && c != '$' )
            break;
        ++i;
    }

    if ( i < searchFilter.size() )
        searchFilter = searchFilter.substr(0, i);

    if ( std::string("quit").find(searchFilter) != std::string::npos && dotPosition == editBuffer){
        completions.push_back("quit()");

    } else if ( std::string("nodes").find(searchFilter) != std::string::npos && dotPosition == editBuffer ){
        completions.push_back("nodes('");
    }
}
