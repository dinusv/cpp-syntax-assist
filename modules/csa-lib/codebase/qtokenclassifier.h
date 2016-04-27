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


#ifndef QCURSORTOKENBASE_HPP
#define QCURSORTOKENBASE_HPP

#include "clang-c/Index.h"
#include "qcsaglobal.h"
#include <vector>
#include <string>

namespace csa{

class QAnnotatedTokenSet;
class Q_CSA_EXPORT QTokenClassifier{

public:
    typedef std::vector<QAnnotatedTokenSet*> TokenSetVector;
    typedef std::vector<QAnnotatedTokenSet*>::iterator Iterator;
    typedef std::vector<QAnnotatedTokenSet*>::const_iterator ConstIterator;

public:
    QTokenClassifier(const CXTranslationUnit& transUnit, const char* fileName );
    ~QTokenClassifier();

    QAnnotatedTokenSet* findTokenSet(const CXCursor& cursor);

    Iterator tokenSetBegin();
    Iterator tokenSetEnd();
    ConstIterator tokenSetBegin() const;
    ConstIterator tokenSetEnd() const;

    void appendTokenSet(QAnnotatedTokenSet* tokenSet);
    void reparse();

    const CXTranslationUnit& translationUnit() const;
    const std::string& file() const;

    void dump(std::string& str);

    static unsigned int  getFileSize(const char* file);
    static CXSourceRange getFileRange(const CXTranslationUnit& tu, const char* file);

private:
    void initializeTokens(const CXTranslationUnit& transUnit, const CXSourceRange& range);
    void disposeTokenSets();

    TokenSetVector    m_tokenSets;
    CXTranslationUnit m_translationUnit;
    CXToken*          m_tokens;
    unsigned int      m_tokensCount;

    std::string       m_file;

};

inline QTokenClassifier::Iterator QTokenClassifier::tokenSetBegin(){
    return m_tokenSets.begin();
}

inline QTokenClassifier::Iterator QTokenClassifier::tokenSetEnd(){
    return m_tokenSets.end();
}

inline QTokenClassifier::ConstIterator QTokenClassifier::tokenSetBegin() const{
    return m_tokenSets.begin();
}

inline QTokenClassifier::ConstIterator QTokenClassifier::tokenSetEnd() const{
    return m_tokenSets.end();
}

inline const CXTranslationUnit &QTokenClassifier::translationUnit() const{
    return m_translationUnit;
}

inline const std::string &QTokenClassifier::file() const{
    return m_file;
}

}// namespace

#endif // QCURSORTOKENBASE_HPP
