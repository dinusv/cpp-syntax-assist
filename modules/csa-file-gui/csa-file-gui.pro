# Copy js plugins to release path
# -------------------------------

PLUGIN_DEPLOY_FROM = $$PWD/../../plugins

win32:CONFIG(debug, debug|release): PLUGIN_DEPLOY_TO = $$OUT_PWD/../build/debug/plugins
else:win32:CONFIG(release, debug|release): PLUGIN_DEPLOY_TO = $$OUT_PWD/../build/release/plugins
else:unix: PLUGIN_DEPLOY_TO = $$OUT_PWD/../build

win32:PLUGIN_DEPLOY_TO ~= s,/,\\,g
win32:PLUGIN_DEPLOY_FROM ~= s,/,\\,g

plugincopy.commands = $(COPY_DIR) \"$$PLUGIN_DEPLOY_FROM\" \"$$PLUGIN_DEPLOY_TO\"
first.depends = $(first) plugincopy
export(first.depends)
export(plugincopy.commands)

QMAKE_EXTRA_TARGETS += first plugincopy


# Enable javascript and console output
# ------------------------------------

QT     += qml quick script
CONFIG += console

# Configure Dependencies
# ----------------------

INCLUDEPATH += $$PWD/../csa/astnodes
INCLUDEPATH += $$PWD/../csa/codebase
INCLUDEPATH += $$PWD/../csa/script
DEPENDPATH  += $$PWD/../csa/astnodes
DEPENDPATH  += $$PWD/../csa/codebase
DEPENDPATH  += $$PWD/../csa/script

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../csa/release/ -lcsa
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../csa/debug/ -lcsa
else:unix: LIBS += -L$$OUT_PWD/../build/ -lcsa

# Add source files
# ----------------

include($$PWD/src/csa-file-gui.pri)
include($$PWD/../../3rdparty/libclang.pro)

# Configure name and resources
# ----------------------------

RESOURCES += \
    $$PWD/resources/resources.qrc

TARGET   = csa-file-gui
TEMPLATE = app

win32{
    RC_FILE = $$PWD/img/win/csaicon.rc
}

# Deployment
# ----------

win32:CONFIG(debug, debug|release): PLUGIN_DEPLOY_TO = $$OUT_PWD/../build/debug
else:win32:CONFIG(release, debug|release): PLUGIN_DEPLOY_TO = $$OUT_PWD/release/plugins
else:unix: PLUGIN_DEPLOY_TO = $$OUT_PWD


win32:CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/../build/release
else:win32:CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/../build/debug
else:unix: DESTDIR = $$OUT_PWD/../build


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

