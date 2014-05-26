#include "AnnotatedTokenSet.hpp"
#include <sstream>
#include <iostream>

namespace csa{

AnnotatedTokenSet::AnnotatedTokenSet(const CXCursor &cursor, const CXTranslationUnit &tu)
    : m_cursor(cursor)
    , m_translationUnit(tu){
}

void AnnotatedTokenSet::append(const CXToken &token){
    m_tokens.push_back(token);
}

bool AnnotatedTokenSet::operator ==(const AnnotatedTokenSet &other){
    return clang_equalCursors(m_cursor, other.cursor());
}

bool AnnotatedTokenSet::operator !=(const AnnotatedTokenSet &other){
    return !clang_equalCursors(m_cursor, other.cursor());
}

const CXTranslationUnit &AnnotatedTokenSet::translationUnit(){
    return m_translationUnit;
}

void AnnotatedTokenSet::dump(std::string &str){
    CXString displayName = clang_getCursorDisplayName(m_cursor);
    str.append("Cursor :");
    str.append(clang_getCString(displayName));
    str.append("\n Tokens :");
    clang_disposeString(displayName);

    for ( AnnotatedTokenSet::Iterator it = begin(); it != end(); ++it ){
        CXString tokenString           = clang_getTokenSpelling(m_translationUnit, *it);
        CXSourceLocation tokenLocation = clang_getTokenLocation(m_translationUnit, *it);
        unsigned int column, line;
        clang_getSpellingLocation(tokenLocation, 0, &line, &column, 0);
        std::stringstream stream;
        stream << " (" << line << "," << column << ") \"";
        str.append(stream.str() + clang_getCString(tokenString) + "\"");
        clang_disposeString(tokenString);
    }
    str.append("\n");
}

}// namespace
