
#ifndef DEPARTEMENT_H
#define DEPARTEMENT_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableWidget>

class departement
{public:
    departement();
    departement(QString);
    QString get_departement_name();
    bool ajouter(QString name,int ID);
    bool afficher(QTableWidget *tableWidget);
    bool supprimer();
private:
    QString departement_name;

};

#endif // ETUDIANT_H
QString get_departement_name();

