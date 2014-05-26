#ifndef CODEBASE_HPP
#define CODEBASE_HPP

#include <QObject>
#include <QtScript>

namespace csa{
    class TokenClassifier;
    class SourceLocation;
    namespace ast{
        class CSANode;
    }
}

class SyntaxTreeModel;
class CodeBase : public QObject{

    Q_OBJECT

public:
    explicit CodeBase(
            const char* const* translationUnitArgs,
            int                translationUnitNumArgs,
            const QString&     file,
            SyntaxTreeModel*   treeModel,
            QObject*           parent = 0);

    csa::ast::CSANode* selected();

    void reparse();
    void propagateUserCursor(int offset, const QString& file);
    void propagateUserCursor(int line, int column, const QString& file);
    void propagateUserCursor(const csa::SourceLocation& location);

public slots:
    bool select(const QString& typeString, const QString& name);
    void insert(const QScriptValue& value);

    QString nodeIdentifier() const;

private:
    csa::ast::CSANode* m_root;
    csa::ast::CSANode* m_current;

    SyntaxTreeModel*      m_treeModel;
    csa::TokenClassifier* m_classifier;
 };

inline csa::ast::CSANode *CodeBase::selected(){
    return m_current;
}

#endif // CODEBASE_HPP
