INCLUDEPATH += $$PWD

HEADERS += \
	$$PWD/Visitor.hpp \
	$$PWD/CSANode.hpp \
	$$PWD/CSANamespace.hpp \
	$$PWD/CSAFileRoot.hpp \
	$$PWD/CSAClass.hpp \
	$$PWD/CSAAccessSpecifier.hpp \
	$$PWD/CSAField.hpp \
    csatree/SourceLocation.hpp \
    csatree/AnnotatedTokenSet.hpp \
    csatree/TokenClassifier.hpp \
    csatree/CSAConstructor.hpp \
    csatree/CSADestructor.hpp \
    csatree/CSAMethod.hpp

SOURCES += \
	$$PWD/Visitor.cpp \
	$$PWD/CSANode.cpp \
	$$PWD/CSANamespace.cpp \
	$$PWD/CSAFileRoot.cpp \
	$$PWD/CSAClass.cpp \
	$$PWD/CSAAccessSpecifier.cpp \
	$$PWD/CSAField.cpp \
    csatree/SourceLocation.cpp \
    csatree/AnnotatedTokenSet.cpp \
    csatree/TokenClassifier.cpp \
    csatree/CSAConstructor.cpp \
    csatree/CSADestructor.cpp \
    csatree/CSAMethod.cpp

