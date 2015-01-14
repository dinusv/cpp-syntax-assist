/****************************************************************************
**
** Copyright (C) 2014 Dinu SV.
** (contact: mail@dinusv.com)
** This file is part of C++ Snippet Assist application.
**
** GNU General Public License Usage
** 
** This file may be used under the terms of the GNU General Public License 
** version 3.0 as published by the Free Software Foundation and appearing 
** in the file LICENSE.GPL included in the packaging of this file.  Please 
** review the following information to ensure the GNU General Public License 
** version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/


#ifndef CODEBASE_HPP
#define CODEBASE_HPP

#include <QObject>
#include <QtScript>

namespace csa{
    class TokenClassifier;
    class SourceLocation;
    namespace ast{
        class QASTNode;
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

    csa::ast::QASTNode* selected();

    void reparse();
    void propagateUserCursor(int offset, const QString& file);
    void propagateUserCursor(int line, int column, const QString& file);
    void propagateUserCursor(const csa::SourceLocation& location);

public slots:
    bool select(const QString& typeString, const QString& name);
    void insert(const QScriptValue& value);

    QString nodeIdentifier() const;

private:
    csa::ast::QASTNode* m_root;
    csa::ast::QASTNode* m_current;

    SyntaxTreeModel*      m_treeModel;
    csa::TokenClassifier* m_classifier;
 };

inline csa::ast::QASTNode *CodeBase::selected(){
    return m_current;
}

#endif // CODEBASE_HPP
