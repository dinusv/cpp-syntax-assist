
win32{
    INCLUDEPATH += $$PWD/include
    INCLUDEPATH += $$PWD/include/clang-c

    LIBS += -L$$PWD/lib -llibclang
}

unix{
    LLVM_CONFIG_INCLUDE_DIR = $$system(llvm-config --includedir)
    LLVM_CONFIG_LIB_DIR     = $$system(llvm-config --libdir)

    INCLUDEPATH += $$LLVM_CONFIG_INCLUDE_DIR
    LIBS        += -L$$LLVM_CONFIG_LIB_DIR -lclang
}
