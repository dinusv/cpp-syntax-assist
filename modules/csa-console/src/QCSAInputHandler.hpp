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
    void completeFunction(const char* editBuffer, std::vector<std::string>& completions);

private:
    QCSAInputHandler();
    ~QCSAInputHandler();

    QCSAInputHandler(const QCSAInputHandler&);
    QCSAInputHandler& operator =(const QCSAInputHandler&);

    csa::QCSAPluginCollection* m_pluginCollection;
    csa::QCSAPluginLoader*     m_scriptEngine;

};

#endif // QCSAINPUTREADER_H
