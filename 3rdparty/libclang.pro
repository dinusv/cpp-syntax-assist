INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/include/clang-c

win32{
	LIBS        += -L$$PWD/lib -llibclang
}
