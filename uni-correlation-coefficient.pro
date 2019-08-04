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
           src/settings/settings.hpp \ 
           \
           \ # Various interface elements
           src/blocks/ChartView/ChartView.hpp \
           src/blocks/FileBlock/FileBlock.hpp \
           src/blocks/HoverButton/HoverButton.hpp \
           src/blocks/UploadForm/UploadForm.hpp \
           \
           \ # Dialog windows
           src/dialog/Chart/Chart.hpp \
           src/dialog/Coefficients/Coefficients.hpp \
           src/dialog/TableDialog/TableDialog.hpp \
           src/dialog/TableDialog/FloatItem/FloatItem.hpp \
           src/dialog/WebLoader/WebLoader.hpp \ 
           \
           \ # Work with user files
           src/FileUpload/FileUpload.hpp \
           \
           \ # Class for working with tables
           src/Table/Table.hpp

SOURCES += src/main.cpp \
           \
           \ # Work with setting file
           src/settings/settings.cpp \ 
           src/settings/settingsCheck/settingsCheck.cpp \
           src/settings/webResCheck/webResCheck.cpp \
           \
           \ # Various interface elements
           src/blocks/ChartView/ChartView.cpp \
           src/blocks/FileBlock/FileBlock.cpp \
           src/blocks/HoverButton/HoverButton.cpp \
           src/blocks/UploadForm/UploadForm.cpp \
           \
           \ # Dialog windows
           src/dialog/Chart/Chart.cpp \
           src/dialog/Coefficients/Coefficients.cpp \
           src/dialog/Coefficients/createWindow/createWindow.cpp \
           src/dialog/Coefficients/Exchange/Exchange.cpp \
           src/dialog/Coefficients/Kendall/Kendall.cpp \
           src/dialog/Coefficients/Spearman/Spearman.cpp \
           src/dialog/TableDialog/TableDialog.cpp \
           src/dialog/WebLoader/WebLoader.cpp \
           src/dialog/WebLoader/fileUploading/fileUploading.cpp \
           \
           \ # Work with user files
           src/FileUpload/FileUpload.cpp \
           src/FileUpload/createMainWindow/createMainWindow.cpp \
           src/FileUpload/fileParsing/fileParsing.cpp \
           \
           \ # Class for working with tables
           src/Table/Table.cpp