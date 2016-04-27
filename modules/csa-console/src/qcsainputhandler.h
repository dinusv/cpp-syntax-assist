#ifndef QCSAINPUTREADER_H
#define QCSAINPUTREADER_H

#include <vector>
#include <string>
#include <QList>

namespace csa{
    class QCSACompletionSet;
    class QCSAEngine;
}

class QCSACompletionItemPrivate;
class QCSAInputHandler{

public:
    static QCSAInputHandler& getInstance();
    void initPluginHandlers(csa::QCSAEngine* scriptEngine, csa::QCSACompletionSet *pluginCollection);
    int inputLoop();

    static void completeFunctionDelegate(const char* editBuffer, std::vector<std::string>& completions);
    void completeFunction(const char* editBuffer, std::vector<std::string>& completions);

private:
    QCSAInputHandler();
    QCSAInputHandler(const QCSAInputHandler&);
    QCSAInputHandler& operator =(const QCSAInputHandler&);
    ~QCSAInputHandler();

    void initCompletionList();

    csa::QCSACompletionSet* m_completionSet;
    csa::QCSAEngine*        m_scriptEngine;

};

#endif // QCSAINPUTREADER_H
