#include "interfaceemployer.h"
#include "ui_interfaceemployer.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QTableView>
#include <QSqlTableModel>
#include <QItemSelectionModel>
#include <QDateTime>
#include <QDebug>
#include <QDialog>
#include "afficher.h"
#include "historique.h"

InterfaceEmployer::InterfaceEmployer(QWidget *parent) :
    QDialog(parent),  // Héritage de QMainWindow avec un QWidget* parent
    ui(new Ui::InterfaceEmployer)  // Initialisation de l'interface utilisateur
{
    ui->setupUi(this);  // Configure l'objet 'ui'

    // Connexion à la base de données
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Projet");
    db.setUserName("Hanin");
    db.setPassword("tekup");

    if (!db.open()) {
        QMessageBox::critical(this, "Erreur", "Impossible de se connecter à la base de données.");
        return;
    }

    // Initialisation du modèle pour la table employer1
    model = new QSqlTableModel(this, db);
    model->setTable("employer1");
    model->select();

    // Connexion des signaux et des slots
    connect(ui->addButton, &QPushButton::clicked, this, &InterfaceEmployer::addEmployer);
    connect(ui->updateButton, &QPushButton::clicked, this, &InterfaceEmployer::updateEmployer);
    connect(ui->deleteButton, &QPushButton::clicked, this, &InterfaceEmployer::deleteEmployer);
    connect(ui->showButton, &QPushButton::clicked, this, &InterfaceEmployer::showEmployers);
    connect(ui->clearButton, &QPushButton::clicked, this, &InterfaceEmployer::clearFields);
    connect(ui->historiqueButton, &QPushButton::clicked, this, &InterfaceEmployer::on_historiqueButton_clicked);



}
InterfaceEmployer::~InterfaceEmployer()
{
    delete ui;
}

void InterfaceEmployer::addEmployer()
{
    QString id = ui->idLineEdit->text();  // ID de l'employé
    QString cin = ui->cinLineEdit->text();  // CIN de l'employé
    QString nom = ui->nomLineEdit->text();  // Nom de l'employé
    QString email = ui->emailLineEdit->text();  // Email de l'employé
    QString poste = ui->posteLineEdit->text();  // Poste de l'employé
    QString deptId = ui->deptLineEdit->text();  // Département de l'employé

    // Vérifiez si tous les champs sont remplis
    if (id.isEmpty() || cin.isEmpty() || nom.isEmpty() || email.isEmpty() || poste.isEmpty() || deptId.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields must be filled.");
        return;
    }

    // Contrôle de saisie : Vérifiez si l'ID existe déjà
    QSqlQuery checkIdQuery;
    checkIdQuery.prepare("SELECT COUNT(*) FROM employer1 WHERE id_employer = :id_employer");
    checkIdQuery.bindValue(":id_employer", id.toInt());
    if (!checkIdQuery.exec()) {
        QMessageBox::critical(this, "Error", "Error while verifying the ID: " + checkIdQuery.lastError().text());
        return;
    }
    checkIdQuery.next();
    if (checkIdQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Error", "The ID already exists. Please enter another one.");
        return;
    }

    // Contrôle de saisie : Vérifiez que le CIN est unique
    QSqlQuery checkCinQuery;
    checkCinQuery.prepare("SELECT COUNT(*) FROM employer1 WHERE cin = :cin");
    checkCinQuery.bindValue(":cin", cin);
    if (!checkCinQuery.exec()) {
        QMessageBox::critical(this, "Error", "Error while verifying the CIN: " + checkCinQuery.lastError().text());
        return;
    }
    checkCinQuery.next();
    if (checkCinQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Error", "The CIN already exists. Please enter another one.");
        return;
    }

    // Contrôle de saisie : Vérifiez que l'email est unique
    QSqlQuery checkEmailQuery;
    checkEmailQuery.prepare("SELECT COUNT(*) FROM employer1 WHERE email = :email");
    checkEmailQuery.bindValue(":email", email);
    if (!checkEmailQuery.exec()) {
        QMessageBox::critical(this, "Error", "Error while verifying the Email: " + checkEmailQuery.lastError().text());
        return;
    }
    checkEmailQuery.next();
    if (checkEmailQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Error", "The Email already exists. Please enter another one.");
        return;
    }

    // Contrôle de saisie : Vérifiez que le CIN contient exactement 8 chiffres
    if (!cin.contains(QRegularExpression("^[0-9]{8}$"))) {
        QMessageBox::warning(this, "Error", "The CIN must contain exactly 8 digits.");
        return;
    }

    // Contrôle de saisie : Vérifiez que le nom contient au moins deux mots
    if (!nom.contains(QRegularExpression("^\\S+\\s+\\S+$"))) {
        QMessageBox::warning(this, "Error", "The name must contain at least a first name and a last name.");
        return;
    }

    // Contrôle de saisie : Vérifiez que l'email se termine par @gmail.com
    if (!email.endsWith("@gmail.com")) {
        QMessageBox::warning(this, "Error", "The email must end with '@gmail.com'.");
        return;
    }

    // Insérer l'employé dans la table employer1
    QSqlQuery query;
    query.prepare("INSERT INTO employer1 (id_employer, cin, nom, email, poste, id_departement) "
                  "VALUES (:id_employer, :cin, :nom, :email, :poste, :id_departement)");
    query.bindValue(":id_employer", id.toInt());
    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":email", email);
    query.bindValue(":poste", poste);
    query.bindValue(":id_departement", deptId.toInt());

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Error while adding the employee: " + query.lastError().text());
        return;
    }

    // Ajouter une entrée dans l'historique pour l'ajout de l'employé
    logHistorique("Add", id, "Name: " + nom + ", Position: " + poste);
    QMessageBox::information(this, "Success", "Employee added successfully!");
    model->select();

}
void InterfaceEmployer::logHistorique(const QString &action, const QString &idEmployer, const QString &details)
{
    QString dateAction = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");

    QSqlQuery query;
    query.prepare("INSERT INTO historiquei (ID_HISTORIQUE, ACTION, DATE_ACTION, ID_EMPLOYER, DETAILS) "
                  "VALUES (HISTORIQUE_SEQ.NEXTVAL, :action, TO_TIMESTAMP(:date_action, 'YYYY-MM-DD HH24:MI:SS.FF3'), :id_employer, :details)");
    query.bindValue(":action", action);
    query.bindValue(":date_action", dateAction);
    query.bindValue(":id_employer", idEmployer.toInt());
    query.bindValue(":details", details);

    if (!query.exec()) {
        qDebug() << "Error while adding to the history:" << query.lastError().text();
        QMessageBox::critical(this,"Error", "Error while adding to the history: " + query.lastError().text());
    } else {
        qDebug() << "Successful insertion into the history.";
    }
}


void InterfaceEmployer::on_historiqueButton_clicked()
{
    // Assurez-vous qu'une seule instance de la fenêtre historique est créée
    static historique *historiqueWindow = nullptr;

    if (!historiqueWindow) {
        historiqueWindow = new historique(this); // Crée l'instance si elle n'existe pas
    }

    historiqueWindow->show(); // Affiche la fenêtre
    historiqueWindow->raise(); // Met la fenêtre au premier plan
    historiqueWindow->activateWindow(); // Active la fenêtre
}


void InterfaceEmployer::updateEmployer()
{
    QString id = ui->idLineEdit->text();
    QString cin = ui->cinLineEdit->text();
    QString nom = ui->nomLineEdit->text();
    QString email = ui->emailLineEdit->text();
    QString poste = ui->posteLineEdit->text();
    QString deptId = ui->deptLineEdit->text();

    if (id.isEmpty() || cin.isEmpty() || nom.isEmpty() || email.isEmpty() || poste.isEmpty() || deptId.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields must be filled.");
        return;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM employer1 WHERE id_employer = :id_employer");
    checkQuery.bindValue(":id_employer", id.toInt());

    if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() == 0) {
        QMessageBox::warning(this, "Error", "No employee exists with this ID.");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE employer1 SET cin = :cin, nom = :nom, email = :email, poste = :poste, "
                  "id_departement = :id_departement WHERE id_employer = :id_employer");
    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":email", email);
    query.bindValue(":poste", poste);
    query.bindValue(":id_departement", deptId.toInt());
    query.bindValue(":id_employer", id.toInt());

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Error during the update: " + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Success", "Employee updated successfully!");
    model->select();

    qDebug() << "Ajout dans l'historique pour modification.";
    logHistorique("Modify", id,
                  "Name: " + nom + ", Email: " + email + ", Position: " + poste + ", Department: " + deptId);

}


void InterfaceEmployer::deleteEmployer()
{
    QString id = ui->idLineEdit->text();

    if (id.isEmpty()) {
        QMessageBox::warning(this, "Error", "Employee ID is required.");
        return;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM employer1 WHERE id_employer = :id_employer");
    checkQuery.bindValue(":id_employer", id.toInt());

    if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() == 0) {
        QMessageBox::warning(this, "Error", "No employee exists with this ID.");
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM employer1 WHERE id_employer = :id_employer");
    query.bindValue(":id_employer", id.toInt());

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Error during deletion: " + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Success", "Employee deleted successfully!");
    model->select();

    qDebug() << "Ajout dans l'historique pour suppression.";
    //logHistorique("Delete", id, "Employee with ID: " + id + " Deleted.");
}


void InterfaceEmployer::showEmployers()
{
    model->select(); // Actualiser la vue
}
void InterfaceEmployer::clearFields()
{
    ui->idLineEdit->clear();
    ui->cinLineEdit->clear();
    ui->nomLineEdit->clear();
    ui->emailLineEdit->clear();
    ui->posteLineEdit->clear();
    ui->deptLineEdit->clear();
}

void InterfaceEmployer::onSelectionChanged(const QItemSelection &selected)
{
    QModelIndexList selectedIndexes = selected.indexes();
    if (!selectedIndexes.isEmpty()) {
        // Récupérer l'index de la ligne sélectionnée
        int row = selectedIndexes.first().row();

        // Remplir les QLineEdits avec les informations de l'employé sélectionné
        QString id = model->data(model->index(row, 0)).toString();
        QString cin = model->data(model->index(row, 1)).toString(); // Récupérer le CIN
        QString nom = model->data(model->index(row, 2)).toString();
        QString email = model->data(model->index(row, 3)).toString();
        QString poste = model->data(model->index(row, 4)).toString();
        QString dept = model->data(model->index(row, 5)).toString();

        ui->idLineEdit->setText(id);
        ui->cinLineEdit->setText(cin); // Afficher le CIN
        ui->nomLineEdit->setText(nom);
        ui->emailLineEdit->setText(email);
        ui->posteLineEdit->setText(poste);
        ui->deptLineEdit->setText(dept);
    }
}

void InterfaceEmployer::on_showButton_clicked() {
    afficher *a = new afficher(this); // Créer une nouvelle instance de la classe afficher
    a->exec(); // Afficher la fenêtre en mode modal
}

void InterfaceEmployer::on_updateButton_clicked()
{
    afficher *a = new afficher(this); // Créer une nouvelle instance de la classe afficher
    a->exec();
}

void InterfaceEmployer::on_deleteButton_clicked()
{
    afficher *a = new afficher(this); // Créer une nouvelle instance de la classe afficher
    a->exec();
}

