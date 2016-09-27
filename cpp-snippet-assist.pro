TEMPLATE = subdirs
CONFIG  += ordered

SUBDIRS += $$PWD/modules/modules.pro
SUBDIRS += $$PWD/tests/tests.pro

OTHER_FILES += \
    $$PWD/plugins/addclass.js \
    $$PWD/plugins/wrapnamespace.js \
    $$PWD/plugins/preventcopy.js \
    $$PWD/plugins/addincludeguard.js \
    $$PWD/plugins/addnamespace.js \
    $$PWD/plugins/resolution.js \
    $$PWD/plugins/adddestructor.js \
    $$PWD/plugins/setsingleton.js \
    $$PWD/plugins/addsetter.js \
    $$PWD/plugins/addgetter.js \
    $$PWD/plugins/addproperty.js \
    $$PWD/plugins/addconstructor.js


OTHER_FILES += \
    $$PWD/config/default.json
