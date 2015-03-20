
# Copy js configuration file to release path

Release:DESTDIR = release/../release
Debug:DESTDIR   = debug/../debug

#CONFIGURATION_FILE      = $$PWD/configuration/configuration.js
#CONFIGURATION_FILE_DEST = $$DESTDIR

#win32:CONFIGURATION_FILE      ~= s,/,\\,g
#win32:CONFIGURATION_FILE_DEST ~= s,/,\\,g
#QMAKE_POST_LINK = $$QMAKE_COPY \"$$CONFIGURATION_FILE\" \"$$CONFIGURATION_FILE_DEST\" $$escape_expand(\\n\\t)

win32:CONFIG(debug, debug|release): PLUGIN_DEPLOY = $$OUT_PWD/debug/plugins
else:win32:CONFIG(release, debug|release): PLUGIN_DEPLOY = $$OUT_PWD/release/plugins
else:unix: DEPLOY_PATH = $$OUT_PWD

plugincopy.commands = $(COPY_DIR) \"$$PWD/plugins\" \"$$PLUGIN_DEPLOY\"
first.depends = $(first) plugincopy
export(first.depends)
export(plugincopy.commands)
QMAKE_EXTRA_TARGETS += first plugincopy

# Enable javascript and console output

QT     += script
CONFIG += console

# Installation path
# target.path =

include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

include(3rdparty/libclang.pro)
include(loader/loader.pri)
include(ast/ast.pri)
include(codebase/codebase.pri)

OTHER_FILES += \
    qml/main.qml \
    img/logo.png \
    plugins/destructor.js \
    plugins/namespace.js \
    plugins/preventcopy.js \
    plugins/property.js

RESOURCES += \
    resources.qrc

win32{
    RC_FILE = img/win/csaicon.rc
}
