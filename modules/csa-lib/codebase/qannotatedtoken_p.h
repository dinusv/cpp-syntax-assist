#ifndef QANNOTATEDTOKEN_P_HPP
#define QANNOTATEDTOKEN_P_HPP

#include "clang-c/Index.h"

namespace csa{

class QCXTokenWrapper{

public:
    QCXTokenWrapper(const CXToken& arg) : token(arg){}
    QCXTokenWrapper(const QCXTokenWrapper& other) : token(other.token){}
    QCXTokenWrapper& operator = (const QCXTokenWrapper& other){ token = other.token; return *this; }

    CXToken token;
};

}// namespace

#endif // QANNOTATEDTOKEN_P_HPP
