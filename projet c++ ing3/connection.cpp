#include "connection.h"
#include <QDebug>
#include <QSqlError>

// Initialisation du membre statique
QSqlDatabase Connection::db = QSqlDatabase::addDatabase("QODBC");

Connection::Connection() {}

Connection::~Connection()
{
    fermerConnction(); // Fermer la connexion lors de la destruction de l'objet
}

bool Connection::ouvrirConnction()
{
    if (!db.isOpen()) {
        db.setDatabaseName("Projet");
        db.setUserName("Hanin");  // Nom de l'utilisateur
        db.setPassword("tekup"); // Mot de passe

        if (db.open()) {
            qDebug() << "Connexion à la base de données réussie.";
            return true;
        } else {
            qDebug() << "Erreur d'ouverture de la base de données: " << db.lastError().text();
            return false;
        }
    }
    return true; // Si déjà ouvert
}

void Connection::fermerConnction()
{
    if (db.isOpen()) {
        db.close();
        qDebug() << "Connexion à la base de données fermée.";
    }
}

QSqlDatabase& Connection::getDatabase()
{
    return db;
}
