#include "employer.h"
#include "connection.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

bool Employer::create() {
    QSqlQuery query;
    query.prepare("INSERT INTO employer (id_employer, nom, email, poste, id_departement) "
                  "VALUES (:id_employer, :nom, :email, :poste, :id_departement)");

    query.bindValue(":id_employer", id_employer);
    query.bindValue(":nom", nom);
    query.bindValue(":email", email);
    query.bindValue(":poste", poste);
    query.bindValue(":id_departement", id_departement);

    if (query.exec()) {
        return true;
    } else {
        qWarning() << "Erreur lors de l'insertion de l'employé:" << query.lastError().text();
        return false;
    }
}

bool Employer::update() {
    QSqlQuery query;
    query.prepare("UPDATE employer SET nom = :nom, email = :email, poste = :poste, id_departement = :id_departement "
                  "WHERE id_employer = :id_employer");

    query.bindValue(":id_employer", id_employer);
    query.bindValue(":nom", nom);
    query.bindValue(":email", email);
    query.bindValue(":poste", poste);
    query.bindValue(":id_departement", id_departement);

    if (query.exec()) {
        return true;
    } else {
        qWarning() << "Erreur lors de la mise à jour de l'employé:" << query.lastError().text();
        return false;
    }
}

bool Employer::remove() {
    QSqlQuery query;
    query.prepare("DELETE FROM employer WHERE id_employer = :id_employer");
    query.bindValue(":id_employer", id_employer);

    if (query.exec()) {
        return true;
    } else {
        qWarning() << "Erreur lors de la suppression de l'employé:" << query.lastError().text();
        return false;
    }
}

QList<Employer> Employer::getAll() {
    QList<Employer> employers;
    QSqlQuery query("SELECT * FROM employer");

    while (query.next()) {
        Employer employer;
        employer.id_employer = query.value("id_employer").toInt();
        employer.nom = query.value("nom").toString();
        employer.email = query.value("email").toString();
        employer.poste = query.value("poste").toString();
        employer.id_departement = query.value("id_departement").toInt();
        employers.append(employer);
    }

    return employers;
}

Employer Employer::getById(int id) {
    QSqlQuery query;
    query.prepare("SELECT * FROM employer WHERE id_employer = :id_employer");
    query.bindValue(":id_employer", id);

    if (query.exec() && query.next()) {
        Employer employer;
        employer.id_employer = query.value("id_employer").toInt();
        employer.nom = query.value("nom").toString();
        employer.email = query.value("email").toString();
        employer.poste = query.value("poste").toString();
        employer.id_departement = query.value("id_departement").toInt();
        return employer;
    }

    return Employer(); // Retourne un employé vide en cas d'absence
}
