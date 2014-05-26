#include "SourceLocation.hpp"

namespace csa{

SourceLocation::SourceLocation(const std::string &file, unsigned int line, unsigned int column, unsigned int offset)
    : m_file(file)
    , m_line(line)
    , m_column(column)
    , m_offset(offset){
}

SourceLocation::SourceLocation(const char *file, unsigned int line, unsigned int column, unsigned int offset)
    : m_file(file)
    , m_line(line)
    , m_column(column)
    , m_offset(offset){
}

SourceLocation::SourceLocation(const CXFile &file, unsigned int line, unsigned int column, unsigned int offset)
    : m_line(line)
    , m_column(column)
    , m_offset(offset){

    CXString fileName = clang_getFileName(file);
    m_file.assign(clang_getCString(fileName));
    clang_disposeString(fileName);
}

SourceLocation::SourceLocation(const CXSourceLocation &location){
    assign(location);
}

SourceLocation::~SourceLocation(){
}

void SourceLocation::assign(const CXSourceLocation &location){
    CXFile file;
    clang_getSpellingLocation(location, &file, &m_line, &m_column, &m_offset);

    CXString fileName = clang_getFileName(file);
    if (fileName.data != 0)
        m_file.assign(clang_getCString(fileName));
    clang_disposeString(fileName);
}

void SourceLocation::assign(const SourceLocation &other){
    m_file   = other.m_file;
    m_line   = other.m_line;
    m_column = other.m_column;
    m_offset = other.m_offset;
}

void SourceLocation::assign(unsigned int offset, const CXTranslationUnit &translationUnit){
    CXFile file = clang_getFile(translationUnit, m_file.c_str());
    if ( file != 0 ){
        assign(clang_getLocationForOffset(translationUnit, file, offset));
    }
}

const std::string& SourceLocation::filePath() const{
    return m_file;
}

}// namespace
