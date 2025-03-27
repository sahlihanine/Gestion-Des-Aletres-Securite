#include "department_manager.h"
#include "ui_department_manager.h"
#include "ticket_history.h"

#include<QTableWidget>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include "interfaceemployer.h"



department_manager::department_manager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::department_manager)
{
    ui->setupUi(this);

    mainwindow_page = new MainWindow();             // Instanciation de MainWindow
    department_list_page = new department_list();   // Instanciation de department_list
}

department_manager::~department_manager()
{
    delete ui;
    delete mainwindow_page;        // Libère la mémoire de MainWindow
    delete department_list_page;   // Libère la mémoire de department_list
}

// Slot pour ouvrir MainWindow
void department_manager::on_pushButton_clicked()
{
    mainwindow_page->show();
}

// Slot pour ouvrir department_list
void department_manager::on_pushButton_2_clicked()
{
    departement dpt;
    department_list_page->exec();  // Ouvre department_list en mode modal
}

void department_manager::on_pb_ticket_clicked()
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

void department_manager::on_pb_employer_clicked()
{
    // Créez une instance de l'interface InterfaceEmployer
    InterfaceEmployer *employerWindow = new InterfaceEmployer(this);

    // Affichez la fenêtre
    employerWindow->show();
}
