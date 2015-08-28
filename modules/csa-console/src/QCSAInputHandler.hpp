#ifndef QCSAINPUTREADER_H
#define QCSAINPUTREADER_H

#include <vector>
#include <string>
#include <QList>

namespace csa{
    class QCSAPluginCollection;
    class QCSAPluginLoader;
}

class QCSACompletionItemPrivate;
class QCSAInputHandler{

public:
    static QCSAInputHandler& getInstance();
    void initPluginHandlers(csa::QCSAPluginLoader* scriptEngine, csa::QCSAPluginCollection* pluginCollection);
    int inputLoop();

    static void completeFunctionDelegate(const char* editBuffer, std::vector<std::string>& completions);
    void completeFunction(const char* editBuffer, std::vector<std::string>& completions, bool nameCompletions = true);

private:
    QCSAInputHandler();
    ~QCSAInputHandler();

    void initCompletionList();

    QCSAInputHandler(const QCSAInputHandler&);
    QCSAInputHandler& operator =(const QCSAInputHandler&);

    csa::QCSAPluginCollection* m_pluginCollection;
    csa::QCSAPluginLoader*     m_scriptEngine;

    QList<QCSACompletionItemPrivate*> m_completionList;

};

#endif // QCSAINPUTREADER_H
