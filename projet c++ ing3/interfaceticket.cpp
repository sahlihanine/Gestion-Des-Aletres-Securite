#include "interfaceticket.h"
#include "ui_interfaceticket.h"
#include "ticket.h"
#include "connection.h"
#include "historique.h"
#include "chatbot.h"
#include "ticket_history.h"
#include "listtickets.h"
#include "profil.h"

#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
//#include <QSoundEffect>

interfaceTicket::interfaceTicket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::interfaceTicket)
{
    ui->setupUi(this);
}

interfaceTicket::~interfaceTicket()
{
    delete ui;
}

void interfaceTicket::on_pb_ticket_clicked()
{
    ticket t;
    t.setIdEmployer(idEmployer);
    t.setIdDepartement(idDepartement);
    t.exec();
}

void interfaceTicket::on_pb_tickets_list_clicked()
{
    listTickets h;
    h.exec();
}


void interfaceTicket::on_pb_chatbot_clicked()
{
    chatbot c;
    c.exec();
}

void interfaceTicket::on_pb_tickets_history_clicked()
{
    // Requête pour récupérer les informations depuis la table TICKET
    QSqlQuery query;
    query.prepare("SELECT ID_TICKET, ID_EMPLOYER, ID_DEPARTEMENT, TYPE, TITLE, DESCRIPTION, CREATION_DATE, STATUE "
                  "FROM TICKET "
                  "LEFT JOIN HISTORIQUE ON TICKET.ID_TICKET = HISTORIQUE.ID_T "
                  "WHERE HISTORIQUE.ID_T IS NULL");


    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Échec de la récupération des données : " + query.lastError().text());
        return;
    }

    // Remplir la table HISTORIQUE avec les données récupérées
    QSqlQuery insertQuery;
    while (query.next()) {
        insertQuery.prepare("INSERT INTO HISTORIQUE (ACTION, ID_T, ID_E, ID_D, TP, TL, DESCR, C_DATE, ST) "
                            "VALUES (:action, :idTicket, :idEmployer, :idDepartement, :type, :titre, :description, :creationDate, :statue)");
        insertQuery.bindValue(":action", "ADDED");
        insertQuery.bindValue(":idTicket", query.value("ID_TICKET"));
        insertQuery.bindValue(":idEmployer", query.value("ID_EMPLOYER"));
        insertQuery.bindValue(":idDepartement", query.value("ID_DEPARTEMENT"));
        insertQuery.bindValue(":type", query.value("TYPE"));
        insertQuery.bindValue(":titre", query.value("TITLE"));
        insertQuery.bindValue(":description", query.value("DESCRIPTION"));
        insertQuery.bindValue(":creationDate", query.value("CREATION_DATE"));
        insertQuery.bindValue(":statue", query.value("STATUE"));

        if (!insertQuery.exec()) {
            QMessageBox::critical(this, "Erreur", "Échec de l'insertion : " + insertQuery.lastError().text());
        }
    }

    ticket_history th;
    th.exec();
}

void interfaceTicket::on_pb_profil_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT ID_EMPLOYER,CIN, NOM, POSTE, EMAIL, ID_DEPARTEMENT FROM EMPLOYER1 WHERE ID_EMPLOYER = :idEmployer");
    query.bindValue(":idEmployer", QString::number(idEmployer));

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to fetch employee data: " + query.lastError().text());
        return;
    }
    else
    {
        if (query.next()) {
            QString cin = query.value("CIN").toString();
            QString nom = query.value("NOM").toString();
            QString poste = query.value("POSTE").toString();
            QString email = query.value("EMAIL").toString();
            QString idEmployer = QString::number(query.value(0).toInt());
            QString idDepartement = QString::number(query.value("ID_DEPARTEMENT").toInt());

        profil p;
        p.set_Profil_details(idEmployer, idDepartement, cin, nom, poste,email);
        p.exec();
        }
        else {QMessageBox::critical(this, "Database Error", "Failed");}
    }

}



