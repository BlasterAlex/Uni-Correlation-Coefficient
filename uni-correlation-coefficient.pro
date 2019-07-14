TEMPLATE = app
TARGET = uni-correlation-coefficient
INCLUDEPATH += .

DEFINES += QT_DEPRECATED_WARNINGS

QT += core gui widgets charts

MOC_DIR     = build
OBJECTS_DIR = build
RC_FILE     = data/images/icons/icon.rc

# Input
HEADERS += \ # Work with setting file
           src/qt/settings/settings.hpp \ 
           \
           \ # Various interface elements
           src/qt/blocks/HoverButton/HoverButton.hpp \
           src/qt/blocks/UploadForm/UploadForm.hpp \
           src/qt/blocks/FileBlock/FileBlock.hpp \
           src/qt/blocks/TableDialog/TableDialog.hpp \
           src/qt/blocks/TableDialog/FloatItem/FloatItem.hpp \
           \
           \ # Correlation graph
           src/qt/blocks/Chart/Chart.hpp \
           src/qt/blocks/Chart/ChartView/ChartView.hpp \
           \
           \ # Work with user files
           src/qt/files/FileUpload/FileUpload.hpp \
           src/qt/files/fileParsing/fileParsing.hpp \
           \
           \ # Class for working with tables
           src/qt/Table/Table.hpp \
           \
           \ # Correlation coefficients
           src/qt/Coefficients/Coefficients.hpp

SOURCES += src/qt/main.cpp \
           \ # Work with setting file
           src/qt/settings/settings.cpp \
           \
           \ # Various interface elements
           src/qt/blocks/HoverButton/HoverButton.cpp \
           src/qt/blocks/UploadForm/UploadForm.cpp \
           src/qt/blocks/FileBlock/FileBlock.cpp \
           src/qt/blocks/TableDialog/TableDialog.cpp \
           \
           \ # Correlation graph
           src/qt/blocks/Chart/Chart.cpp \
           src/qt/blocks/Chart/ChartView/ChartView.cpp \
           \
           \ # Work with user files
           src/qt/files/FileUpload/FileUpload.cpp \
           src/qt/files/fileParsing/fileParsing.cpp \
           \
           \ # Class for working with tables
           src/qt/Table/Table.cpp \
           \
           \ # Correlation coefficients
           src/qt/Coefficients/Coefficients.cpp \
           src/qt/Coefficients/createWindow/createWindow.cpp\
           src/qt/Coefficients/Spearman/Spearman.cpp \
           src/qt/Coefficients/Kendall/Kendall.cpp \
           src/qt/Coefficients/Exchange/Exchange.cpp