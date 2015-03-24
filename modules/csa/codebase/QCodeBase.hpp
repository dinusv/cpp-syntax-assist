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


#ifndef QCODEBASE_HPP
#define QCODEBASE_HPP

#include <QObject>
#include <QtScript>

#include "QCSAGlobal.hpp"
#include "QASTNode.hpp"
#include "QASTFile.hpp"

namespace csa{

class QTokenClassifier;
class QSourceLocation;
class QCodeBaseObserver;

class Q_CSA_EXPORT QCodeBase : public QObject{

    Q_OBJECT

public:
    explicit QCodeBase(
            const char* const* translationUnitArgs,
            int                translationUnitNumArgs,
            const QString&     file,
            QCodeBaseObserver* treeModel,
            QObject*           parent = 0);

    csa::ast::QASTNode* selected();

    void propagateUserCursor(int offset, const QString& file);
    void propagateUserCursor(int line, int column, const QString& file);
    void propagateUserCursor(const csa::QSourceLocation& location);

    void updateTreeModel();

public slots:
    QList<csa::ast::QASTNode*> files();
    csa::ast::QASTNode* cursorNode();

    csa::QSourceLocation* createLocation(const QString &file, unsigned int offset);
    csa::QSourceLocation* createLocation(const QString &file, unsigned int line, unsigned int column);

    void save();
    bool select(const QString &typeString, const QString &name);

private:

    QList<ast::QASTNode*>  m_files;

    csa::ast::QASTFile*    m_root;
    csa::ast::QASTNode*    m_current;

    QCodeBaseObserver*     m_observer;
    csa::QTokenClassifier* m_classifier;
};

inline csa::ast::QASTNode *QCodeBase::selected(){
    return m_current;
}

} // namespace

#endif // QCODEBASE_HPP
