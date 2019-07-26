TEMPLATE = app
TARGET = uni-correlation-coefficient
INCLUDEPATH += .

CONFIG += c++14
DEFINES += QT_DEPRECATED_WARNINGS
QT += core gui widgets charts webengine webenginewidgets

MOC_DIR     = build
OBJECTS_DIR = build
RC_FILE     = data/images/icons/icon.rc

# Input
HEADERS += \ # Work with setting file
           src/qt/settings/settings.hpp \ 
           \
           \ # Various interface elements
           src/qt/blocks/ChartView/ChartView.hpp \
           src/qt/blocks/FileBlock/FileBlock.hpp \
           src/qt/blocks/HoverButton/HoverButton.hpp \
           src/qt/blocks/UploadForm/UploadForm.hpp \
           \
           \ # Dialog windows
           src/qt/dialog/Chart/Chart.hpp \
           src/qt/dialog/Coefficients/Coefficients.hpp \
           src/qt/dialog/TableDialog/TableDialog.hpp \
           src/qt/dialog/TableDialog/FloatItem/FloatItem.hpp \
           src/qt/dialog/WebLoader/WebLoader.hpp \ 
           \
           \ # Work with user files
           src/qt/FileUpload/FileUpload.hpp \
           \
           \ # Class for working with tables
           src/qt/Table/Table.hpp

SOURCES += src/qt/main.cpp \
           \
           \ # Work with setting file
           src/qt/settings/settings.cpp \ 
           src/qt/settings/settingsCheck/settingsCheck.cpp \
           src/qt/settings/webResCheck/webResCheck.cpp \
           \
           \ # Various interface elements
           src/qt/blocks/ChartView/ChartView.cpp \
           src/qt/blocks/FileBlock/FileBlock.cpp \
           src/qt/blocks/HoverButton/HoverButton.cpp \
           src/qt/blocks/UploadForm/UploadForm.cpp \
           \
           \ # Dialog windows
           src/qt/dialog/Chart/Chart.cpp \
           src/qt/dialog/Coefficients/Coefficients.cpp \
           src/qt/dialog/Coefficients/createWindow/createWindow.cpp \
           src/qt/dialog/Coefficients/Exchange/Exchange.cpp \
           src/qt/dialog/Coefficients/Kendall/Kendall.cpp \
           src/qt/dialog/Coefficients/Spearman/Spearman.cpp \
           src/qt/dialog/TableDialog/TableDialog.cpp \
           src/qt/dialog/WebLoader/WebLoader.cpp \
           src/qt/dialog/WebLoader/fileUploading/fileUploading.cpp \
           \
           \ # Work with user files
           src/qt/FileUpload/FileUpload.cpp \
           src/qt/FileUpload/createMainWindow/createMainWindow.cpp \
           src/qt/FileUpload/fileParsing/fileParsing.cpp \
           \
           \ # Class for working with tables
           src/qt/Table/Table.cpp