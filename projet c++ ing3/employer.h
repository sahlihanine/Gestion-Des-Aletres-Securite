#ifndef EMPLOYER_H
#define EMPLOYER_H

#include <QString>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

class Employer {
public:
    int id_employer;
    QString nom;
    QString email;
    QString poste;
    int id_departement;

    Employer(int id = 0, const QString& nom = "", const QString& email = "", const QString& poste = "", int dept = 0)
        : id_employer(id), nom(nom), email(email), poste(poste), id_departement(dept) {}

    bool create();
    bool update();
    bool remove();
    static QList<Employer> getAll();
    static Employer getById(int id);
};

#endif // EMPLOYER_H
