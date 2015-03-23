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


#include "QTokenClassifier.hpp"
#include "QAnnotatedTokenSet.hpp"
#include <cstdio>

namespace csa{

QTokenClassifier::QTokenClassifier(const CXTranslationUnit &transUnit, const char *fileName)
    : m_file(fileName){

    initialize(transUnit, getFileRange(transUnit, fileName));
}

QTokenClassifier::~QTokenClassifier(){
    disposeTokenSets();
}

QAnnotatedTokenSet *QTokenClassifier::findTokenSet(const CXCursor &cursor){
    for ( QTokenClassifier::Iterator it = tokenSetBegin(); it != tokenSetEnd(); ++it ){
        if ( clang_equalCursors( (*it)->cursor(), cursor) )
            return *it;
    }
    return 0;
}

void QTokenClassifier::appendTokenSet(QAnnotatedTokenSet* tokenSet){
    m_tokenSets.push_back(tokenSet);
}

void QTokenClassifier::initialize(const CXTranslationUnit &transUnit, const CXSourceRange &range){
    m_translationUnit = transUnit;

    clang_tokenize(m_translationUnit, range, &m_tokens, &m_tokensCount );

    CXCursor* cursors  = new CXCursor[m_tokensCount];
    clang_annotateTokens(m_translationUnit, m_tokens, m_tokensCount, cursors);

    QAnnotatedTokenSet* lastAddedToken = 0;
    for( unsigned int i = 0; i < m_tokensCount; ++i ){

        bool found = false;
        if ( lastAddedToken ){
            if ( clang_equalCursors(lastAddedToken->cursor(), cursors[i]) ){
                lastAddedToken->append(m_tokens[i]);
                found = true;
            }
        }

        if ( !found ){
            for ( QTokenClassifier::Iterator it = tokenSetBegin(); it != tokenSetEnd(); ++it ){
                if ( clang_equalCursors( (*it)->cursor(), cursors[i] ) ){
                    (*it)->append(m_tokens[i]);
                    found = true;
                    break;
                }
            }
        }

        if ( !found ){
            QAnnotatedTokenSet* tokenSet = new QAnnotatedTokenSet(cursors[i], transUnit);
            tokenSet->append(m_tokens[i]);
            m_tokenSets.push_back(tokenSet);
            lastAddedToken = tokenSet;
        }
    }

    delete[] cursors;
}

void QTokenClassifier::disposeTokenSets(){
    for ( QTokenClassifier::Iterator it = tokenSetBegin(); it != tokenSetEnd(); ++it )
        delete *it;
    clang_disposeTokens(m_translationUnit, m_tokens, m_tokensCount);
    m_tokensCount = 0;
    m_tokenSets.clear();
}

void QTokenClassifier::reparse(){
    disposeTokenSets();
    initialize(m_translationUnit, getFileRange(m_translationUnit, m_file.c_str()));
}

void QTokenClassifier::dump(std::string &str){
    str.append("Token Classifier : \n\n");
    for ( QTokenClassifier::Iterator it = tokenSetBegin(); it != tokenSetEnd(); ++it ){
        (*it)->dump(str);
        str.append("\n");
    }
}

unsigned int QTokenClassifier::getFileSize(const char* file){
    FILE *fp = fopen(file, "r");
    if ( !fp )
        return 0;
    fseek(fp, 0, SEEK_END);
    unsigned int result = ftell(fp);
    fclose(fp);
    return result;
}

CXSourceRange QTokenClassifier::getFileRange(const CXTranslationUnit& tu, const char* file){
    CXFile clangFile = clang_getFile(tu, file);
    CXSourceLocation fStart = clang_getLocationForOffset(tu, clangFile, 0);
    CXSourceLocation fEnd   = clang_getLocationForOffset(tu, clangFile, getFileSize(file));
    return clang_getRange(fStart, fEnd);
}

}// namespace
