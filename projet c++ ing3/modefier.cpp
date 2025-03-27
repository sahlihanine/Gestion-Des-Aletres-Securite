#include "modefier.h"
#include "ui_modefier.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>


modefier::modefier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modefier)
{
    ui->setupUi(this);

    // Regrouper les boutons radio pour le type
    typeGroup = new QButtonGroup(this);
    typeGroup->addButton(ui->radioButton_type1);
    typeGroup->addButton(ui->radioButton_type2);
    typeGroup->addButton(ui->radioButton_type3);

    // Regrouper les boutons radio pour le statut
    statusGroup = new QButtonGroup(this);
    statusGroup->addButton(ui->radioButton_pending);
    statusGroup->addButton(ui->radioButton_progress);
    statusGroup->addButton(ui->radioButton_solved);
}

modefier::~modefier()
{
    delete ui;
}

void modefier::setTicketData(const QString &titre, const QString &description, const QString &type, const QString &statue, const QString &id)
{
    ticketId = id;

    ui->lineEdit_titre->setText(titre);
    ui->textEdit_description->setPlainText(description);

    if (type == "Low") {
        ui->radioButton_type1->setChecked(true);
    } else if (type == "Medium") {
        ui->radioButton_type2->setChecked(true);
    } else if (type == "Critical") {
        ui->radioButton_type3->setChecked(true);
    }

    if (statue == "Pending") {
        ui->radioButton_pending->setChecked(true);
    } else if (statue == "In progress") {
        ui->radioButton_progress->setChecked(true);
    } else if (statue == "Solved") {
        ui->radioButton_solved->setChecked(true);
    }
}

void modefier::on_pb_valider_clicked()
{
    // Afficher une boîte de dialogue de confirmation pour la modification
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Are you sure you want to modify the ticket?",
                                   QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        QMessageBox::information(this, "Cancelled", "The modification has been cancelled.");
        // Afficher une boîte de dialogue pour demander si l'utilisateur souhaite quitter
        QMessageBox::StandardButton exitReply;
        exitReply = QMessageBox::question(this, "Exit Modification", "Do you want to exit the modification window?",
                                          QMessageBox::Yes | QMessageBox::No);

        if (exitReply == QMessageBox::Yes) {
            reject(); // Ferme la fenêtre sans appliquer de modifications
        }
        return; // Ne pas exécuter la suite du code
    }

    // Récupérer les nouvelles valeurs des champs
    QString newTitre = ui->lineEdit_titre->text();
    QString newDescription = ui->textEdit_description->toPlainText();

    // Récupérer le type sélectionné
    QString newType;
    if (ui->radioButton_type1->isChecked()) {
        newType = "Low";
    } else if (ui->radioButton_type2->isChecked()) {
        newType = "Medium";
    } else if (ui->radioButton_type3->isChecked()) {
        newType = "Critical";
    }

    // Récupérer le statut sélectionné
    QString newStatue;
    if (ui->radioButton_pending->isChecked()) {
        newStatue = "Pending";
    } else if (ui->radioButton_progress->isChecked()) {
        newStatue = "In progress";
    } else if (ui->radioButton_solved->isChecked()) {
        newStatue = "Solved";
    }

    // Récupérer les anciennes valeurs pour les insérer dans HISTORIQUE
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT ID_EMPLOYER, ID_DEPARTEMENT, TITLE, DESCRIPTION, TYPE, STATUE, CREATION_DATE "
                        "FROM TICKET WHERE ID_TICKET = :id");
    selectQuery.bindValue(":id", ticketId);

    if (!selectQuery.exec() || !selectQuery.next()) {
        QMessageBox::critical(this, "Error", "Failed to retrieve ticket details: " + selectQuery.lastError().text());
        return;
    }


    // Insérer l'historique de modification
    QSqlQuery insertQuery;
    insertQuery.prepare(
        "INSERT INTO HISTORIQUE (ACTION, ID_T, ID_E, ID_D, TP, TL, DESCR, C_DATE, ST) "
        "VALUES (:action, :idTicket, :idEmployer, :idDepartement, :type, :titre, :description, :creationDate, :statue)"
    );

    insertQuery.bindValue(":action", "MODIFIED");
    insertQuery.bindValue(":idTicket", ticketId);
    insertQuery.bindValue(":idEmployer", selectQuery.value("ID_EMPLOYER"));
    insertQuery.bindValue(":idDepartement", selectQuery.value("ID_DEPARTEMENT"));
    insertQuery.bindValue(":type", newType);
    insertQuery.bindValue(":titre", newTitre);
    insertQuery.bindValue(":description", newDescription);
    insertQuery.bindValue(":creationDate", QDateTime::currentDateTime());
    insertQuery.bindValue(":statue", newStatue);

    if (!insertQuery.exec()) {
        QMessageBox::critical(this, "Error", "Failed to insert into HISTORIQUE: " + insertQuery.lastError().text());
        return;
    }

    // Mettre à jour la base de données
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE TICKET SET TITLE = :titre, DESCRIPTION = :description, TYPE = :type, STATUE = :statue WHERE ID_TICKET = :id");
    updateQuery.bindValue(":titre", newTitre);
    updateQuery.bindValue(":description", newDescription);
    updateQuery.bindValue(":type", newType);
    updateQuery.bindValue(":statue", newStatue);
    updateQuery.bindValue(":id", ticketId);

    if (updateQuery.exec()) {
        QMessageBox::information(this, "Success", "The ticket has been successfully modified!");
        // Signaler que la modification a été effectuée
        emit ticketModified(ticketId, newTitre, newStatue, newType); // Envoyer l'ID et les nouvelles données
        accept(); // Fermer la fenêtre de modification
    } else {
        QMessageBox::critical(this, "Error", "Failed to modify the ticket: " + updateQuery.lastError().text());
    }
}


void modefier::on_pb_cancel_clicked()
{
    accept();
}
