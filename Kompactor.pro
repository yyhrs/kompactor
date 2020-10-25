#-------------------------------------------------
#
# Project created by Shrademn
#
#-------------------------------------------------

QT += core gui widgets concurrent

TARGET = Kompactor
TEMPLATE = app

CONFIG += c++17
RC_FILE = details.rc

DESTDIR = $$PWD/bin

SOURCES += main.cpp \
    Assets/ChunkFile.cpp \
    Assets/Files.cpp \
	Assets/Models.cpp \
	Kompactor.cpp \
	MainWindow.cpp

HEADERS += \
    Assets/ChunkFile.h \
	Assets/Files.h \
    Assets/Models.h \
    General/ExactTypes.h \
    General/Exceptions.h \
    General/GameTypes.h \
	General/Objects.h \
	General/Utils.h \
	Kompactor.hpp \
	MainWindow.hpp \
	general/3DTypes.h \
	general/ExactTypes.h


FORMS += \
    MainWindow.ui

RESOURCES += resources.qrc

DISTFILES += details.rc

include(core/Core.pri)
include(widget/Widget.pri)

win32:CONFIG(release, debug|release) {
	DEPLOY_COMMAND = $$(QTDIR)\bin\windeployqt
	DEPLOY_TARGET = $$shell_quote($$shell_path($${DESTDIR}/$${TARGET}.exe))
	QMAKE_POST_LINK = $${DEPLOY_COMMAND} $${DEPLOY_TARGET}
}

