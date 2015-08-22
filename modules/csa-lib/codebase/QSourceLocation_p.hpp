#ifndef QSOURCELOCATION_P_HPP
#define QSOURCELOCATION_P_HPP

#include "clang-c/Index.h"
#include "QSourceLocation.hpp"
#include <QString>

namespace csa{

inline QSourceLocation createSourceLocation(
        const QString& filePath
){
    return QSourceLocation(filePath, 1, 1, 0, 0);
}

inline QSourceLocation createSourceLocation(
        const CXSourceLocation& location
){
    CXFile file;
    unsigned int line, column, offset;
    clang_getSpellingLocation(location, &file, &line, &column, &offset);

    CXString fileName = clang_getFileName(file);

    if (fileName.data != 0){
        QSourceLocation dstLocation(
                    QString(clang_getCString(fileName)).replace("//", "/"), line, column, offset);
        clang_disposeString(fileName);
        return dstLocation;
    }

    return createSourceLocation("");
}

inline QSourceLocation createSourceLocation(
        const QString& filePath,
        unsigned int offset,
        const CXTranslationUnit& translationUnit
){
    CXFile file = clang_getFile(translationUnit, filePath.toUtf8().constData());
    if ( file != 0 )
        return createSourceLocation(clang_getLocationForOffset(translationUnit, file, offset));

    return createSourceLocation(filePath);
}

inline QSourceLocation createSourceLocation(
        const QString& filePath,
        unsigned int line,
        unsigned int column,
        const CXTranslationUnit& translationUnit
){
    CXFile file = clang_getFile(translationUnit, filePath.toUtf8());
    if ( file != 0 )
        return createSourceLocation(clang_getLocation(translationUnit, file, line, column));

    return createSourceLocation(filePath);
}

}// namespace

#endif // QSOURCELOCATION_P_HPP
