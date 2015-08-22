
win32{
    INCLUDEPATH += $$PWD/include
    INCLUDEPATH += $$PWD/include/clang-c

    LIBS += -L$$PWD/lib -llibclang

    FILE = $$PWD/lib/libclang.dll
    DDIR = $${DLLDESTDIR}

    win32:FILE ~= s,/,\\,g
    win32:DDIR ~= s,/,\\,g

    QMAKE_POST_LINK += $$QMAKE_COPY \"$$FILE\" \"$$DDIR\" $$escape_expand(\\n\\t)

    export(QMAKE_POST_LINK)
}

unix{
    LLVM_CONFIG_INCLUDE_DIR = $$system(llvm-config --includedir)
    LLVM_CONFIG_LIB_DIR     = $$system(llvm-config --libdir)

    INCLUDEPATH += $$LLVM_CONFIG_INCLUDE_DIR
    LIBS        += -L$$LLVM_CONFIG_LIB_DIR -lclang
}
