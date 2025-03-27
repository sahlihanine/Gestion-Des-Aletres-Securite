QT       += core gui \
    quick
QT       +=  sql
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    afficher.cpp \
    chatbot.cpp \
    connection.cpp \
    departement.cpp \
    department_list.cpp \
    department_manager.cpp \
    details.cpp \
    employer.cpp \
    historique.cpp \
    interfaceemployer.cpp \
    interfaceticket.cpp \
    listtickets.cpp \
    login.cpp \
    login_ib.cpp \
    main.cpp \
    mainwindow.cpp \
    modefier.cpp \
    page_principale.cpp \
    profil.cpp \
    register_window.cpp \
    ticket.cpp \
    ticket_history.cpp

HEADERS += \
    afficher.h \
    chatbot.h \
    connection.h \
    departement.h \
    department_list.h \
    department_manager.h \
    details.h \
    employer.h \
    historique.h \
    interfaceemployer.h \
    interfaceticket.h \
    listtickets.h \
    login.h \
    login_ib.h \
    mainwindow.h \
    modefier.h \
    page_principale.h \
    profil.h \
    register_window.h \
    ticket.h \
    ticket_history.h

FORMS += \
    afficher.ui \
    chatbot.ui \
    department_list.ui \
    department_manager.ui \
    details.ui \
    historique.ui \
    interfaceemployer.ui \
    interfaceticket.ui \
    listtickets.ui \
    login.ui \
    mainwindow.ui \
    modefier.ui \
    page_principale.ui \
    profil.ui \
    ticket.ui \
    ticket_history.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    im2.qrc \
    img.qrc \
    style.qrc

DISTFILES += \
    style.qss
