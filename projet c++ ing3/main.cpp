
#include "page_principale.h"

#include <QtCore/qglobal.h>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif
#include <QSqlDatabase>
#include <QDebug>
#include <QFile>

#include<QSqlQuery>
#include <QCoreApplication>
#include <QtSql/QSqlError>


int main(int argc, char *argv[])
{
 //QCoreApplication a(argc, argv);
    QApplication a(argc, argv);

    // Charger le fichier de style QSS
    QFile file(":/style.qss"); // Chemin relatif si le fichier est ajouté aux ressources
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = file.readAll();
        a.setStyleSheet(styleSheet); // Appliquer le style à l'application
        file.close();
    } else {
        qWarning("Impossible de charger le fichier style.qss");}

    page_principale w;
    w.show();
    qDebug() << QSqlDatabase::drivers();
    return a.exec();
}




























