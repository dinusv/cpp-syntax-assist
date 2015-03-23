# Copy js plugins to release path
# -------------------------------

win32:CONFIG(debug, debug|release): PLUGIN_DEPLOY = $$OUT_PWD/debug/plugins
else:win32:CONFIG(release, debug|release): PLUGIN_DEPLOY = $$OUT_PWD/release/plugins
else:unix: DEPLOY_PATH = $$OUT_PWD

plugincopy.commands = $(COPY_DIR) \"$$PWD/../../plugins\" \"$$PLUGIN_DEPLOY\"
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
else:unix: LIBS += -L$$OUT_PWD/../application/ -llcvlib

# Add source files
# ----------------

include($$PWD/src/application.pri)
include($$PWD/../../3rdparty/libclang.pro)

# Configure name and resources
# ----------------------------

RESOURCES += \
    $$PWD/resources/resources.qrc

TARGET   = cppsnippetassist
TEMPLATE = app

win32{
    RC_FILE = $$PWD/img/win/csaicon.rc
}

# Deployment
# ----------

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
