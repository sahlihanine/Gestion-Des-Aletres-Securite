#include "register_window.h"
#include "login_ib.h"
#include "interfaceticket.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegExpValidator>
#include <QDateTime>

RegisterWindow::RegisterWindow(QWidget *parent) : QWidget(parent)
{
    QLabel *titleLabel = new QLabel("<h2 style='color:#222222;'>Register Employee</h2>", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *idLabel = new QLabel("ID :", this);
    idInput = new QLineEdit(this);

    QLabel *cinLabel = new QLabel("CIN :", this);
    cinInput = new QLineEdit(this);

    QLabel *nameLabel = new QLabel("Nom :", this);
    nameInput = new QLineEdit(this);

    QLabel *emailLabel = new QLabel("Email :", this);
    emailInput = new QLineEdit(this);

    QLabel *posteLabel = new QLabel("Poste :", this);
    posteInput = new QLineEdit(this);

    QLabel *departementLabel = new QLabel("ID Departement :", this);
    departementInput = new QLineEdit(this);

    saveButton = new QPushButton("Save", this);
    returnButton = new QPushButton("Return", this);

    QVBoxLayout *formLayout = new QVBoxLayout();
    formLayout->addWidget(idLabel);
    formLayout->addWidget(idInput);
    formLayout->addWidget(cinLabel);
    formLayout->addWidget(cinInput);
    formLayout->addWidget(nameLabel);
    formLayout->addWidget(nameInput);
    formLayout->addWidget(emailLabel);
    formLayout->addWidget(emailInput);
    formLayout->addWidget(posteLabel);
    formLayout->addWidget(posteInput);
    formLayout->addWidget(departementLabel);
    formLayout->addWidget(departementInput);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(saveButton);
    buttonsLayout->addWidget(returnButton);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
    setFixedSize(400, 500);
    setWindowTitle("Register Employee");

    connect(saveButton, &QPushButton::clicked, this, &RegisterWindow::saveDetails);
    connect(returnButton, &QPushButton::clicked, this, &RegisterWindow::goBackToLogin);
}

void RegisterWindow::saveDetails()
{
    QString id = idInput->text().trimmed();
    QString cin = cinInput->text().trimmed();
    QString name = nameInput->text().trimmed();
    QString email = emailInput->text().trimmed();
    QString poste = posteInput->text().trimmed();
    QString departement = departementInput->text().trimmed();

    if (id.isEmpty() || cin.isEmpty() || name.isEmpty() || email.isEmpty() || poste.isEmpty() || departement.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis.");
        return;
    }

    QRegExp cinPattern("^[0-9]{8}$");
    if (!cinPattern.exactMatch(cin)) {
        QMessageBox::warning(this, "Erreur", "Le CIN doit contenir exactement 8 chiffres.");
        return;
    }

    QStringList nameParts = name.split(" ");
    if (nameParts.size() < 2) {
        QMessageBox::warning(this, "Erreur", "Le nom doit contenir à la fois un prénom et un nom.");
        return;
    }

    QRegExp emailPattern("^[a-zA-Z0-9._%+-]+@gmail.com$");
    if (!emailPattern.exactMatch(email)) {
        QMessageBox::warning(this, "Erreur", "L'email doit être un adresse Gmail valide.");
        return;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM EMPLOYER1 WHERE ID_EMPLOYER = :id OR email = :email OR cin = :cin");
    checkQuery.bindValue(":id", id);
    checkQuery.bindValue(":email", email);
    checkQuery.bindValue(":cin", cin);
    if (checkQuery.exec() && checkQuery.next()) {
        QMessageBox::warning(this, "Erreur", "L'ID, le CIN ou l'email existe déjà.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYER1 (ID_EMPLOYER, cin, nom, email, poste, id_departement) "
                  "VALUES (:id, :cin, :name, :email, :poste, :departement)");
    query.bindValue(":id", id);
    query.bindValue(":cin", cin);
    query.bindValue(":name", name);
    query.bindValue(":email", email);
    query.bindValue(":poste", poste);
    query.bindValue(":departement", departement);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "L'employé a été enregistré avec succès.");
        idInput->clear();
        cinInput->clear();
        nameInput->clear();
        emailInput->clear();
        posteInput->clear();
        departementInput->clear();

        // Ajout dans l'historique
        QSqlQuery historiqueQuery;
        QString dateAction = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
        historiqueQuery.prepare("INSERT INTO historiquei (ID_HISTORIQUE, ACTION, DATE_ACTION, ID_EMPLOYER, DETAILS) "
                                "VALUES (HISTORIQUE_SEQ.NEXTVAL, :action, TO_TIMESTAMP(:date_action, 'YYYY-MM-DD HH24:MI:SS.FF3'), :id_employer, :details)");
        historiqueQuery.bindValue(":action", "Add");
        historiqueQuery.bindValue(":date_action", dateAction);
        historiqueQuery.bindValue(":id_employer", id);
        historiqueQuery.bindValue(":details", QString("Name: %1, Position: %2").arg(name).arg(poste));

        if (!historiqueQuery.exec()) {
            qDebug() << "Erreur lors de l'ajout à l'historique:" << historiqueQuery.lastError().text();
        }
        this->close();
        interfaceTicket t;
        t.exec();

    } else {
        qDebug() << "Erreur lors de l'insertion des données :" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Impossible d'enregistrer l'employé.");
    }
}

void RegisterWindow::goBackToLogin()
{
    Login_ib *loginWindow = new Login_ib();
    loginWindow->show();
    this->close();
}
