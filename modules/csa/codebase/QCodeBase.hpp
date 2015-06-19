/****************************************************************************
**
** Copyright (C) 2014-2015 Dinu SV.
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

class QCodeBasePrivate;
class Q_CSA_EXPORT QCodeBase : public QObject{

    Q_OBJECT

public:
    explicit QCodeBase(
            const char* const* translationUnitArgs,
            int                translationUnitNumArgs,
            const QStringList& files,
            const QString&     searchDir = "",
            QCodeBaseObserver* observer = 0,
            QObject*           parent = 0);
    ~QCodeBase();

    csa::ast::QASTNode* selected();

    void propagateUserCursor(int offset, const QString& file);
    void propagateUserCursor(int line, int column, const QString& file);
    void propagateUserCursor(const csa::QSourceLocation& location);

    csa::ast::QASTFile* loadFile(const QString& file);

    void updateTreeModel();

    void setHeaderSearchPattern(const QStringList& pattern);
    void setSourceSearchPattern(const QStringList& pattern);

public slots:
    QList<csa::ast::QASTFile*> files();
    csa::ast::QASTFile* findFile(const QString& fileName);
    csa::ast::QASTNode* cursorNode();

    csa::QSourceLocation* createLocation(const QString &file, unsigned int offset);
    csa::QSourceLocation* createLocation(const QString &file, unsigned int line, unsigned int column);

    void save();
    bool select(const QString &typeString, const QString &name);

    csa::ast::QASTFile* findSource(const QString& headerFile);
    csa::ast::QASTFile* findHeader(const QString& sourceFile);

private:
    // prevent copy
    QCodeBase(const QCodeBase& other);
    QCodeBase& operator=(const QCodeBase& other);

    QCodeBasePrivate* const d_ptr;
    Q_DECLARE_PRIVATE(QCodeBase)


    csa::QTokenClassifier* classifierForFile(const QString& file);

    QList<ast::QASTFile*>  m_files;
    QString                m_searchDir;
    QStringList            m_headerSearchPatterns;
    QStringList            m_sourceSearchPatterns;

    csa::ast::QASTFile*    m_root;
    csa::ast::QASTNode*    m_current;

    QCodeBaseObserver*     m_observer;

    int                    m_translationUnitNumArgs;
    char**                 m_translationUnitArgs;

    QList<csa::QTokenClassifier*> m_classifiers;
};

inline csa::ast::QASTNode *QCodeBase::selected(){
    return m_current;
}

} // namespace

#endif // QCODEBASE_HPP
