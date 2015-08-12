# Copy tests to release path
# --------------------------

TEST_DATA_DEPLOY_FROM = $$PWD/data

win32:CONFIG(debug, debug|release): TEST_DATA_DEPLOY_TO = $$OUT_PWD/debug/data
else:win32:CONFIG(release, debug|release): TEST_DATA_DEPLOY_TO = $$OUT_PWD/release/data
else:unix: TEST_DATA_DEPLOY_TO = $$OUT_PWD

win32:TEST_DATA_DEPLOY_TO ~= s,/,\\,g
win32:TEST_DATA_DEPLOY_FROM ~= s,/,\\,g

testdatacopy.commands = $(COPY_DIR) \"$$TEST_DATA_DEPLOY_FROM\" \"$$TEST_DATA_DEPLOY_TO\"
first.depends = $(first) testdatacopy
export(first.depends)
export(testdatacopy.commands)
QMAKE_EXTRA_TARGETS += first testdatacopy


# Enable javascript and console output
# ------------------------------------

QT     += qml quick script testlib
CONFIG += console testcase

## Configure Dependencies
## ----------------------

INCLUDEPATH += $$PWD/../modules/csa/astnodes
INCLUDEPATH += $$PWD/../modules/csa/codebase
INCLUDEPATH += $$PWD/../modules/csa/script
DEPENDPATH  += $$PWD/../modules/csa/astnodes
DEPENDPATH  += $$PWD/../modules/csa/codebase
DEPENDPATH  += $$PWD/../modules/csa/script

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../modules/csa/release/ -lcsa
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../modules/csa/debug/ -lcsa
else:unix: LIBS += -L$$OUT_PWD/../modules/application/ -lcsa

include($$PWD/src/tests.pri)

TARGET   = test_csa
TEMPLATE = app

OTHER_FILES += \
    data/parsing/unknowntype.test \
    data/parsing/unknowntype.expect \
    data/parsing/parserplugin.js \
    data/insertion/insertion.js \
    data/insertion/insertion.test \
    data/insertion/insertion.expect
