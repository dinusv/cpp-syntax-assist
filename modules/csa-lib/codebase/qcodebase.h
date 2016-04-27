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

#include "qcsaglobal.h"
#include "qastnode.h"
#include "qastfile.h"

namespace csa{

class QTokenClassifier;
class QSourceLocation;

class QCodebasePrivate;
class Q_CSA_EXPORT QCodebase : public QObject{

    Q_OBJECT

public:
    explicit QCodebase(
        const char* const* translationUnitArgs,
        int                translationUnitNumArgs,
        const QStringList& entries = QStringList(),
        const QString&     searchDir = "",
        QObject*           parent = 0);
    ~QCodebase();

    void propagateUserCursor(int offset, const QString& file);
    void propagateUserCursor(int line, int column, const QString& file);
    void propagateUserCursor(const csa::QSourceLocation& location);

    void setHeaderSearchPattern(const QStringList& pattern);
    void setSourceSearchPattern(const QStringList& pattern);

    const QList<csa::ast::QASTFile*>& astFiles() const;

public slots:    
    bool select(const QString &searchData, const QString &type = "");
    bool selectNode(csa::ast::QASTNode* node);
    csa::ast::QASTNode* selectedNode();

    // Parsing
    // -------

    void parsePath(const QString& path);
    csa::ast::QASTFile* parseFile(const QString& file);
    csa::ast::QASTFile* reparseFile(csa::ast::QASTFile* file);

    // Location
    // --------

    csa::QSourceLocation* createLocation(const QString &file, unsigned int lineOrOffset, unsigned int column = 0);

    // Files
    // -----

    void setProjectDir(const QString& path);
    QList<QObject*> find(const QString& searchData, const QString& type = "");
    csa::ast::QASTFile* findFile(const QString& fileName);
    csa::ast::QASTFile* findSource(const QString& headerFile);
    csa::ast::QASTFile* findHeader(const QString& sourceFile);
    QList<QObject*> files() const;

    // File Creation and Removal
    // -------------------------

    csa::ast::QASTFile* createFile(const QString& filePath);
    bool removeFile(csa::ast::QASTFile* file);
    bool makePath(const QString& path);

    // Save
    // ----

    void save();

signals:
    void fileAdded(csa::ast::QASTFile* file);
    void fileAboutToBeReparsed(csa::ast::QASTFile* file);
    void fileReparsed(csa::ast::QASTFile* file);
    void nodeSelected(csa::ast::QASTNode* node);

private:
    // prevent copy
    QCodebase(const QCodebase& other);
    QCodebase& operator=(const QCodebase& other);

    // d ptr
    QCodebasePrivate* const d_ptr;
    Q_DECLARE_PRIVATE(QCodebase)


    csa::QTokenClassifier* classifierForFile(const QString& file);
    void reparseIndex(int index);

private:
    QList<ast::QASTFile*>  m_files;
    QString                m_projectDir;
    QStringList            m_headerSearchPatterns;
    QStringList            m_sourceSearchPatterns;

    csa::ast::QASTFile*    m_root;
    csa::ast::QASTNode*    m_current;

    int                    m_translationUnitNumArgs;
    char**                 m_translationUnitArgs;

    QList<csa::QTokenClassifier*> m_classifiers;
};


inline QList<QObject*> QCodebase::files() const{
    return QObject::children();
}

inline csa::ast::QASTNode* QCodebase::selectedNode(){
    return m_current;
}

} // namespace

#endif // QCODEBASE_HPP
