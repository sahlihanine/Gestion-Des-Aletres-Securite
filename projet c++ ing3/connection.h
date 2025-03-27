#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>

#include <QtCore/QObject>
#include <QtCore/QSharedDataPointer>
#include <QtCore/qglobal.h>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#else
#include <QtDeclarative/QDeclarativeItem>
#include <QtGui/QMainWindow>
#include <QtGui/QWidget>
#endif



class Connection
{
private:
    static QSqlDatabase db;
public:
    Connection();
    ~Connection();
    bool ouvrirConnction();
    void fermerConnction();

    static QSqlDatabase& getDatabase(); // Retourne une référence à la base de données

    Q_DISABLE_COPY(Connection) // Empêche la copie accidentelle

};

#endif // CONNEXION_H

