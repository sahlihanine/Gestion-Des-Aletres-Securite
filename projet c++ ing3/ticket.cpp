#include "ticket.h"
#include "ui_ticket.h"
#include "connection.h"
#include <QButtonGroup>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

ticket::ticket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ticket)
{
    ui->setupUi(this);
    // Regrouper les boutons radio pour gérer le type
    typeGroup = new QButtonGroup(this);
    typeGroup->addButton(ui->radioButton_type1, 1); // "Low"
    typeGroup->addButton(ui->radioButton_type2, 2); // "Medium"
    typeGroup->addButton(ui->radioButton_type3, 3); // "Critical"
}

ticket::~ticket()
{
    delete ui;
}

void ticket::on_pb_add_clicked()
{
    // Réinitialiser les labels
    ui->error_label_type->clear();
    ui->error_label_title->clear();
    ui->error_label_description->clear();

    // Réinitialiser la validation
    bool isValid = true;

    // Récupérer les valeurs saisies dans l'interface
    QString titre = ui->lineEdit_titre->text();
    QString description = ui->textEdit_description->toPlainText();

    // Récupérer les valeurs de type dans l'interface
    int selectedId = typeGroup->checkedId();
    QString type;

    // Vérifier si aucun bouton radio n'est sélectionné
    if (selectedId == -1) {
        type = "";  // Aucun type sélectionné
    } else {
        switch (selectedId) {
        case 1:
            type = "Low";
            break;
        case 2:
            type = "Medium";
            break;
        case 3:
            type = "Critical";
            break;
        }
    }

    // Validation des champs
    if (titre.isEmpty())
    {
        ui->error_label_title->setStyleSheet("color: red;");
        ui->error_label_title->setText("The title is required.");
        isValid = false;
    }
    if (description.isEmpty())
    {
        ui->error_label_description->setStyleSheet("color: red;");
        ui->error_label_description->setText("The description is required.");
        isValid = false;
    }
    if (type.isEmpty())
    {
        ui->error_label_type->setStyleSheet("color: red;");
        ui->error_label_type->setText("The type is required.");
        isValid = false;
    }

    if (!isValid) {
        return; // Arrêter si un champ est invalide
    }

    // Préparation de la requête SQL
    QSqlQuery query;
    query.prepare("INSERT INTO ticket (ID_EMPLOYER, ID_DEPARTEMENT, TYPE, TITLE, DESCRIPTION, STATUE) "
                  "VALUES (:idEmployer, :idDepartement, :type, :titre, :description, :statue)");

    query.bindValue(":type", type);
    query.bindValue(":titre", titre);
    query.bindValue(":description", description);
    query.bindValue(":idEmployer", getIdEmployer());
    query.bindValue(":idDepartement", getIdDepartement());
    query.bindValue(":statue", "Pending");


    if (!query.exec())
    {
        QMessageBox::critical(this, "Failed!", "Can't add ticket !");
        accept(); // Fermer la fenêtre de modification
    }
    else
    {
        QMessageBox::information(this, "Succès", "Success to add ticket !");
        accept(); // Fermer la fenêtre de modification

        // Réinitialiser les champs de saisie
        ui->lineEdit_titre->clear();
        ui->textEdit_description->clear();

        typeGroup->setExclusive(false); // Désactiver l'exclusivité temporairement
        ui->radioButton_type1->setChecked(false);
        ui->radioButton_type2->setChecked(false);
        ui->radioButton_type3->setChecked(false);
        typeGroup->setExclusive(true); // Réactiver l'exclusivité
    }
}


