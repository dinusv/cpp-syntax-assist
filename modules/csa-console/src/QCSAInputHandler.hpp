#ifndef QCSAINPUTREADER_H
#define QCSAINPUTREADER_H

#include <vector>
#include <string>
#include <QList>

namespace csa{
    class QCSACompletionSet;
    class QCSAPluginLoader;
}

class QCSACompletionItemPrivate;
class QCSAInputHandler{

public:
    static QCSAInputHandler& getInstance();
    void initPluginHandlers(csa::QCSAPluginLoader* scriptEngine, csa::QCSACompletionSet *pluginCollection);
    int inputLoop();

    static void completeFunctionDelegate(const char* editBuffer, std::vector<std::string>& completions);
    void completeFunction(const char* editBuffer, std::vector<std::string>& completions);

private:
    QCSAInputHandler();
    ~QCSAInputHandler();

    void initCompletionList();

    QCSAInputHandler(const QCSAInputHandler&);
    QCSAInputHandler& operator =(const QCSAInputHandler&);

    csa::QCSACompletionSet* m_completionSet;
    csa::QCSAPluginLoader*  m_scriptEngine;

};

#endif // QCSAINPUTREADER_H
