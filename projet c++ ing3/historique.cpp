#include "historique.h"
#include "ui_historique.h"
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError> // Ajout pour éviter l'erreur
#include <QMessageBox>
#include <QDebug>
#include "interfaceemployer.h"

historique::historique(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::historique)
{
    ui->setupUi(this);

    // Vérification de la connexion existante à la base de données
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Erreur", "Connexion à la base de données non établie.");
        return;
    }

    // Configuration du modèle
    QSqlTableModel *model = new QSqlTableModel(this, db);
    model->setTable("historiquei");  // Associe le modèle à la table "historiquei"
    if (!model->select()) {
        qDebug() << "Erreur lors du chargement de l'historique :" << model->lastError();
    }

    // Connecter le modèle au QTableView
    ui->historiqueTableView->setModel(model);

    // Ajuster la taille des colonnes
    ui->historiqueTableView->resizeColumnsToContents();
    connect(ui->button_retour, &QPushButton::clicked, this, &historique::retourMainWindow);
}

historique::~historique()
{
    delete ui;
}
void historique::retourMainWindow()
{
    this->close(); // Ferme la fenêtre actuelle (historique)
}


void historique::on_button_retour_clicked()
{
    retourMainWindow();
}
