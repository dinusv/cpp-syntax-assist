
# Copy js configuration file to release path

Release:DESTDIR = release/../release
Debug:DESTDIR   = debug/../debug

CONFIGURATION_FILE      = $$PWD/configuration/configuration.js
CONFIGURATION_FILE_DEST = $$DESTDIR

win32:CONFIGURATION_FILE      ~= s,/,\\,g
win32:CONFIGURATION_FILE_DEST ~= s,/,\\,g
QMAKE_POST_LINK = $$QMAKE_COPY \"$$CONFIGURATION_FILE\" \"$$CONFIGURATION_FILE_DEST\" $$escape_expand(\\n\\t)

# Enable javascript and console output

QT     += script
CONFIG += console

# Installation path
# target.path =

include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

include(csatree/csatree.pri)
include(ast/ast.pri)
include(codebase/codebase.pri)
include(3rdparty/libclang.pro)

SOURCES += main.cpp

OTHER_FILES += \
	configuration/configuration.js \
	qml/main.qml \
	img/logo.png

RESOURCES += \
	resources.qrc

win32{
	RC_FILE = img/win/csaicon.rc
}
