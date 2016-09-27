#include "qcsacompletionset.h"
#include "qcsaconsole.h"
#include <QVariantMap>

#include <QDebug>

namespace csa{

QCSACompletionItem::QCSACompletionItem(int type, QString name, QString usage, QString description)
    : m_type(type)
    , m_name(name)
    , m_usage(usage)
    , m_description(description)
{
}

QCSACompletionItem::~QCSACompletionItem(){
}

QCSACompletionSet::QCSACompletionSet(QObject *parent)
    : QObject(parent)
{
}

QCSACompletionSet::~QCSACompletionSet(){
    for ( QList<QCSACompletionItem*>::iterator it = m_items.begin(); it != m_items.end(); ++it ){
        delete *it;
    }
    m_items.clear();
}

void QCSACompletionSet::registerItem(QCSACompletionItem *item){
    int position = 0;
    QList<QCSACompletionItem*>::iterator it = m_items.begin();
    while( it != m_items.end() ){
        QCSACompletionItem* current = *it;
        if ( item->name() < current->name() ){
            m_items.insert(position, item);
            break;
        }
        ++it;
        ++position;
    }
    if ( it == m_items.end() )
        m_items.append(item);
}

void QCSACompletionSet::registerItem(int type, const QString &name, const QString &usage, const QString &description){
    registerItem(new QCSACompletionItem(type, name, usage, description));
}

QString QCSACompletionSet::getDescription(const QString &name, QCSACompletionItem::Type type){
    for ( QList<QCSACompletionItem*>::iterator it = m_items.begin(); it != m_items.end(); ++it ){
        if ( (*it)->name().mid(0, (*it)->name().indexOf('(')) == name && (*it)->type() & type)
            return (*it)->description();
    }
    return "";
}

QString QCSACompletionSet::getCompletionContext(const QString &text, QCSACompletionItem::Type *type) const{

    bool isEscapeFlag      = false;
    bool isQuoteFlag       = false;
    bool isDoubleQuoteFlag = false;
    bool isGlobalFlag      = true;

    int  lastPunctuationPosition = 0;

    int position = 0;

    QString::const_iterator it = text.begin();
    while (it != text.end()){
        const QChar& c = *it;
        switch(c.toLatin1()){
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
        ++it;
    }

    if (type)
        *type = isGlobalFlag ? QCSACompletionItem::Global : QCSACompletionItem::Object;

    QString context = text.mid(lastPunctuationPosition);
    if ( isDoubleQuoteFlag || isQuoteFlag )
        return QString("-");

    return context;
}

QList<QCSACompletionItem *> QCSACompletionSet::getCompletionItems(
        const QString &context, QCSACompletionItem::Type type)
{
    QList<QCSACompletionItem*> items;
    if ( context == "-" )
        return items;

    QString contextToLower = context.toLower();

    for ( QList<QCSACompletionItem*>::iterator it = m_items.begin(); it != m_items.end(); ++it ){
        QCSACompletionItem* ci = *it;
        if ( (context == "" || ci->name().toLower().indexOf(contextToLower) == 0) && type & ci->type() ){
            items.append(ci);
        }
    }

    return items;
}

void QCSACompletionSet::initDefaultCompletions(){
    registerItem(QCSACompletionItem::Global, "quit()", "quit()", "");
    registerItem(QCSACompletionItem::Global, "console()", "console.log()", "");
    registerItem(QCSACompletionItem::Global, "cl", "console.log()", "");
    registerItem(QCSACompletionItem::Global, "nodes(selector)", "nodes('*')", "");
    registerItem(QCSACompletionItem::Global, "createFile(file)", "createFile('name')", "");
    registerItem(QCSACompletionItem::Global, "reparse(file)", "reparse('file')", "");
    registerItem(QCSACompletionItem::Global, "parse(file)", "parse('file')", "");
    registerItem(QCSACompletionItem::Global, "makePath(path)", "makePath('path')", "");
    registerItem(QCSACompletionItem::Global, "codebase", "codebase", "");

    registerItem(QCSACompletionItem::Object, "children()", "codebase", "");
    registerItem(QCSACompletionItem::Object, "find(selector, typeName)", "find(\'*\')", "");
    registerItem(QCSACompletionItem::Object, "remove()", "remove()", "");
}

void QCSACompletionSet::registerPlugin(const QString &name, const QVariantMap &pluginData){
    QCSACompletionItem* newItem = new QCSACompletionItem(
        QCSACompletionItem::Object,
        name,
        pluginData.contains("usage") ? pluginData["usage"].toString() : "",
        pluginData.contains("description") ? pluginData["description"].toString() : ""
    );

    registerItem(newItem);
}

}
