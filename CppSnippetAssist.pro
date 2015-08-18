TEMPLATE = subdirs
CONFIG  += ordered

SUBDIRS += $$PWD/modules/modules.pro
SUBDIRS += $$PWD/tests/tests.pro

OTHER_FILES += \
    $$PWD/plugins/adddestructor.js \
    $$PWD/plugins/wrapnamespace.js \
    $$PWD/plugins/preventcopy.js \
    $$PWD/plugins/addproperty.js

