#include "TokenClassifier.hpp"
#include "AnnotatedTokenSet.hpp"
#include <cstdio>

namespace csa{

TokenClassifier::TokenClassifier(const CXTranslationUnit &transUnit, const char *fileName)
    : m_file(fileName){

    initialize(transUnit, getFileRange(transUnit, fileName));
}

TokenClassifier::~TokenClassifier(){
    clang_disposeTokens(m_translationUnit, m_tokens, m_tokensCount);
    for ( TokenClassifier::Iterator it = tokenSetBegin(); it != tokenSetEnd(); ++it )
        delete *it;
    m_tokenSets.clear();
}

AnnotatedTokenSet *TokenClassifier::findTokenSet(const CXCursor &cursor){
    for ( TokenClassifier::Iterator it = tokenSetBegin(); it != tokenSetEnd(); ++it ){
        if ( clang_equalCursors( (*it)->cursor(), cursor) )
            return *it;
    }
    return 0;
}

void TokenClassifier::initialize(const CXTranslationUnit &transUnit, const CXSourceRange &range){
    m_translationUnit = transUnit;

    clang_tokenize(m_translationUnit, range, &m_tokens, &m_tokensCount );

    CXCursor* cursors  = new CXCursor[m_tokensCount];
    clang_annotateTokens(m_translationUnit, m_tokens, m_tokensCount, cursors);

    AnnotatedTokenSet* lastAddedToken = 0;
    for( unsigned int i = 0; i < m_tokensCount; ++i ){
        bool found = false;
        if ( lastAddedToken ){
            if ( clang_equalCursors(lastAddedToken->cursor(), cursors[i]) ){
                lastAddedToken->append(m_tokens[i]);
                found = true;
            }
        }
        if ( !found ){
            for ( TokenClassifier::Iterator it = tokenSetBegin(); it != tokenSetEnd(); ++it ){
                if ( clang_equalCursors( (*it)->cursor(), cursors[i] ) ){
                    (*it)->append(m_tokens[i]);
                    found = true;
                    break;
                }
            }
        }

        if ( !found ){
            AnnotatedTokenSet* tokenSet = new AnnotatedTokenSet(cursors[i], transUnit);
            tokenSet->append(m_tokens[i]);
            m_tokenSets.push_back(tokenSet);
            lastAddedToken = tokenSet;
        }
    }

    delete[] cursors;
}

void TokenClassifier::reparse(){
    clang_disposeTokens(m_translationUnit, m_tokens, m_tokensCount);
    m_tokensCount = 0;
    initialize(m_translationUnit, getFileRange(m_translationUnit, m_file.c_str()));
}

void TokenClassifier::dump(std::string &str){
    str.append("Token Classifier : \n\n");
    for ( TokenClassifier::Iterator it = tokenSetBegin(); it != tokenSetEnd(); ++it ){
        (*it)->dump(str);
        str.append("\n");
    }
}

unsigned int TokenClassifier::getFileSize(const char* file){
    FILE *fp = fopen(file, "r");
    if ( !fp )
        return 0;
    fseek(fp, 0, SEEK_END);
    unsigned int result = ftell(fp);
    fclose(fp);
    return result;
}

CXSourceRange TokenClassifier::getFileRange(const CXTranslationUnit& tu, const char* file){
    CXFile clangFile = clang_getFile(tu, file);
    CXSourceLocation fStart = clang_getLocationForOffset(tu, clangFile, 0);
    CXSourceLocation fEnd   = clang_getLocationForOffset(tu, clangFile, getFileSize(file));
    return clang_getRange(fStart, fEnd);
}

}// namespace
