#QT += core gui network

INCLUDEPATH += $$PWD/../QSsh/include/ssh

QSSH_ROOT = $$PWD/../QSsh/lib
message($$QSSH_ROOT)

win32:CONFIG(release, debug|release): LIBS += -L$${QSSH_ROOT} -lQSsh
else:win32:CONFIG(debug, debug|release): LIBS += -L$${QSSH_ROOT} -lQSsh
