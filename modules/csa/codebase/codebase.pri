INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/QCSAGlobal.hpp \
    $$PWD/QCodeBase.hpp \
    $$PWD/QASTVisitor.hpp \
    $$PWD/QAnnotatedTokenSet.hpp \
    $$PWD/QSourceLocation.hpp \
    $$PWD/QTokenClassifier.hpp \
    $$PWD/QAnnotatedToken.hpp \
    $$PWD/QCodeBaseObserver.hpp \
    $$PWD/QASTSearch.hpp \
    $$PWD/QSourceLocation_p.hpp

SOURCES += \
    $$PWD/QCodeBase.cpp \
    $$PWD/QASTVisitor.cpp \
    $$PWD/QAnnotatedTokenSet.cpp \
    $$PWD/QSourceLocation.cpp \
    $$PWD/QTokenClassifier.cpp \
    $$PWD/QCodeBaseObserver.cpp \
    $$PWD/QAnnotatedToken.cpp \
    $$PWD/QASTSearch.cpp
