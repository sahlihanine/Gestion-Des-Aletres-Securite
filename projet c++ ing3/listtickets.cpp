#include "listtickets.h"
#include "ui_listtickets.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>

#include "modefier.h"
#include "details.h"


listTickets::listTickets(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::listTickets)
{
    ui->setupUi(this);

    // Remplir le QTableWidget avec les données
    remplirTableWidget();

    ui->searchLineEdit->setPlaceholderText("Rechercher");

    // Connecter la barre de recherche
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &listTickets::filterTable);

    ui->ticket_comboBox->addItem("Title");
    ui->ticket_comboBox->addItem("Id");
    ui->ticket_comboBox->addItem("Date");
    ui->ticket_comboBox->addItem("Status");
    ui->ticket_comboBox->addItem("Type");
}

listTickets::~listTickets()
{
    delete ui;
}


void listTickets::remplirTableWidget()
{
    // Configurer le tableau
    ui->ticket_tableWidget->setColumnCount(8);
    ui->ticket_tableWidget->setHorizontalHeaderLabels({"ID", "TITLE", "TYPE", "Status", "DATE", "DETAILS", "DELETE", "MODIFY", "QR CODE"});

    // Supprimer la numérotation des lignes
    ui->ticket_tableWidget->verticalHeader()->setVisible(false);

    // Charger les données de la base
    QSqlQuery query("SELECT ID_TICKET, TITLE, TYPE, STATUE, CREATION_DATE FROM TICKET");

    int row = 0;
    while (query.next()) {
        // Ajouter une nouvelle ligne dans le tableau
        ui->ticket_tableWidget->insertRow(row);

        // Remplir les colonnes
        ui->ticket_tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString())); // ID
        ui->ticket_tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString())); // Titre

        // Ajouter l'élément TYPE avec un carré coloré à gauche
        QTableWidgetItem *typeItem = new QTableWidgetItem(query.value(2).toString());
        QString type = query.value(2).toString().toLower(); // Pour éviter les erreurs de casse

        QColor typeColor;
        if (type == "low") {
            typeColor = Qt::green;
        } else if (type == "medium") {
            typeColor = Qt::yellow;
        } else if (type == "critical") {
            typeColor = Qt::red;
        }

        QPixmap typePixmap(10, 10); // Taille du carré pour TYPE
        typePixmap.fill(typeColor);
        typeItem->setIcon(QIcon(typePixmap)); // Ajouter l'icône à l'élément TYPE

        ui->ticket_tableWidget->setItem(row, 2, typeItem);

        // Ajouter l'élément STATUS avec un carré coloré à gauche
        QTableWidgetItem *statusItem = new QTableWidgetItem(query.value(3).toString());
        QString status = query.value(3).toString().toLower(); // Pour éviter les erreurs de casse

        QColor statusColor;
        if (status == "pending") {
            statusColor = Qt::blue;
        } else if (status == "in progress") {
            statusColor = Qt::yellow;
        } else if (status == "solved") {
            statusColor = Qt::green;
        }

        QPixmap statusPixmap(10, 10); // Taille du carré pour STATUS
        statusPixmap.fill(statusColor);
        statusItem->setIcon(QIcon(statusPixmap)); // Ajouter l'icône à l'élément STATUS

        ui->ticket_tableWidget->setItem(row, 3, statusItem);

        // Ajouter la date
        ui->ticket_tableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString())); // Date

        // Ajouter les boutons
        QPushButton *btnDetails = new QPushButton("DETAILS");
        btnDetails->setProperty("row", row);
        ui->ticket_tableWidget->setCellWidget(row, 5, btnDetails);
        connect(btnDetails, &QPushButton::clicked, this, &listTickets::onDetailsClicked);

        QPushButton *btnSupprimer = new QPushButton("DELETE");
        ui->ticket_tableWidget->setCellWidget(row, 6, btnSupprimer);
        connect(btnSupprimer, &QPushButton::clicked, this, &listTickets::onSupprimerClicked);

        QPushButton *btnModifier = new QPushButton("MODIFY");
        btnModifier->setProperty("row", row); // Associe l'indice de la ligne au bouton
        ui->ticket_tableWidget->setCellWidget(row, 7, btnModifier);
        connect(btnModifier, &QPushButton::clicked, this, &listTickets::onModifierClicked);

        row++;
    }

    // Configurer la taille des colonnes pour qu'elles aient toutes la même largeur
    int columnCount = ui->ticket_tableWidget->columnCount();
    int fixedWidth = ui->ticket_tableWidget->width() / columnCount; // Largeur totale divisée par le nombre de colonnes

    ui->ticket_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->ticket_tableWidget->horizontalHeader()->setDefaultSectionSize(fixedWidth);

    // Ajuster les colonnes si nécessaire
    for (int col = 0; col < columnCount; ++col) {
        ui->ticket_tableWidget->setColumnWidth(col, fixedWidth);
    }
}


void listTickets::onSupprimerClicked()
{
    // Récupérer la ligne du bouton cliqué
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button)
        return;

    // Trouver la ligne correspondante en utilisant la position du bouton dans le tableau
    QModelIndex index = ui->ticket_tableWidget->indexAt(button->pos());
    if (!index.isValid())
        return;

    int row = index.row(); // Obtenir l'indice de la ligne

    // Récupérer l'ID du ticket à modifier
    QTableWidgetItem *idItem = ui->ticket_tableWidget->item(row, 0);
    if (!idItem)
        return;

    // Récupérer la valeur de l'ID sous forme de chaîne de caractères
    QString id = idItem->text();

    // Afficher une boîte de dialogue de confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Are you sure you want to delete this ticket ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        QSqlQuery query;

        // Récupérer les informations du ticket pour les sauvegarder dans l'historique
        query.prepare("SELECT ID_EMPLOYER, ID_DEPARTEMENT, TYPE, TITLE, DESCRIPTION, CREATION_DATE, STATUE FROM TICKET WHERE ID_TICKET = :id");
        query.bindValue(":id", id);

        if (query.exec() && query.next()) {

            // Insérer dans la table HISTORIQUE avec l'action "DELETED"
            QSqlQuery insertQuery;
            insertQuery.prepare(
                "INSERT INTO HISTORIQUE (ACTION, ID_T, ID_E, ID_D, TP, TL, DESCR, C_DATE, ST) "
                "VALUES (:action, :idTicket, :idEmployer, :idDepartement, :type, :titre, :description, :creationDate, :statue)"
            );

            insertQuery.bindValue(":action", "DELETED");
            insertQuery.bindValue(":idTicket", id);
            insertQuery.bindValue(":idEmployer", query.value("ID_EMPLOYER"));
            insertQuery.bindValue(":idDepartement", query.value("ID_DEPARTEMENT"));
            insertQuery.bindValue(":type", query.value("TYPE"));
            insertQuery.bindValue(":titre", query.value("TITLE"));
            insertQuery.bindValue(":description", query.value("DESCRIPTION"));
            //insertQuery.bindValue(":creationDate", query.value("CREATION_DATE"));
            insertQuery.bindValue(":creationDate", QDateTime::currentDateTime());
            insertQuery.bindValue(":statue", query.value("STATUE"));


            if (!insertQuery.exec()) {
                QMessageBox::critical(this, "Error", "Failed to insert into HISTORIQUE: " + insertQuery.lastError().text());
                return;
            }
        } else {
            QMessageBox::critical(this, "Error", "Failed to retrieve ticket details: " + query.lastError().text());
            return;
        }
        query.prepare("DELETE FROM TICKET WHERE ID_TICKET = :id");
        query.bindValue(":id", id);

        if (query.exec()) {
            ui->ticket_tableWidget->removeRow(row);
            QMessageBox::information(this, "Success", "Ticket deleted !");
        } else {
            QMessageBox::critical(this, "Error", "Échec de la suppression du ticket : " + query.lastError().text());
        }
    } else {
        QMessageBox::information(this, "Annulé", "The deletion has been canceled.");
    }

}


void listTickets::onModifierClicked()
{
    // Récupérer le bouton qui a été cliqué
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button)
        return;

    // Obtenir la ligne associée au bouton
    bool ok;
    int row = button->property("row").toInt(&ok);
    if (!ok)
        return;

    // Récupérer l'ID du ticket à modifier
    QTableWidgetItem *idItem = ui->ticket_tableWidget->item(row, 0);
    if (!idItem)
        return;

    // Récupérer la valeur de l'ID sous forme de chaîne de caractères
    QString id = idItem->text();

    // Récupérer toutes les informations depuis la base de données
    QSqlQuery query;
    query.prepare("SELECT TITLE, DESCRIPTION, TYPE, STATUE FROM TICKET WHERE ID_TICKET = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        // Extraire les données récupérées
        QString titre = query.value(0).toString();
        QString description = query.value(1).toString();
        QString type = query.value(2).toString();
        QString statue = query.value(3).toString();

        // Ouvrir la boîte de dialogue de modification
        modefier m;
        m.setTicketData(titre, description, type, statue, id);

        // Connecter le signal de modification
        connect(&m, &modefier::ticketModified, this, &listTickets::updateTicketInTable);
        m.exec();

    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la récupération des informations du ticket : " + query.lastError().text());
    }
}


void listTickets::updateTicketInTable(const QString &ticketId, const QString &newTitre, const QString &newStatue, const QString &newType)
{
    int rowCount = ui->ticket_tableWidget->rowCount();

    // Rechercher la ligne correspondant à l'ID du ticket
    for (int row = 0; row < rowCount; ++row) {
        QTableWidgetItem *idItem = ui->ticket_tableWidget->item(row, 0);
        if (idItem && idItem->text() == ticketId) {
            // Mettre à jour les données de la ligne correspondante
            ui->ticket_tableWidget->item(row, 1)->setText(newTitre); // Titre

            // TYPE : Mettre à jour l'icône et le texte
            QTableWidgetItem *typeItem = ui->ticket_tableWidget->item(row, 2);
            typeItem->setText(newType);

            QString type = newType.toLower(); // Conversion pour éviter les erreurs de casse
            QColor typeColor;
            if (type == "low") {
                typeColor = Qt::green;
            } else if (type == "medium") {
                typeColor = Qt::yellow;
            } else if (type == "critical") {
                typeColor = Qt::red;
            }

            QPixmap typePixmap(10, 10);
            typePixmap.fill(typeColor);
            typeItem->setIcon(QIcon(typePixmap)); // Mettre à jour l'icône pour TYPE

            // STATUS : Mettre à jour l'icône et le texte
            QTableWidgetItem *statusItem = ui->ticket_tableWidget->item(row, 3);
            statusItem->setText(newStatue);

            QString status = newStatue.toLower(); // Conversion pour éviter les erreurs de casse
            QColor statusColor;
            if (status == "pending") {
                statusColor = Qt::blue;
            } else if (status == "in progress") {
                statusColor = Qt::yellow;
            } else if (status == "solved") {
                statusColor = Qt::green;
            }

            QPixmap statusPixmap(10, 10);
            statusPixmap.fill(statusColor);
            statusItem->setIcon(QIcon(statusPixmap)); // Mettre à jour l'icône pour STATUS

            break;
        }
    }
}


void listTickets::onDetailsClicked()
{
    // Récupérer le bouton qui a été cliqué
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button)
        return;

    // Trouver la ligne correspondante en utilisant la position du bouton dans le tableau
    QModelIndex index = ui->ticket_tableWidget->indexAt(button->pos());
    if (!index.isValid())
        return;

    int row = index.row(); // Obtenir l'indice de la ligne

    // Récupérer l'ID du ticket à modifier
    QTableWidgetItem *idItem = ui->ticket_tableWidget->item(row, 0);
    if (!idItem)
        return;

    // Récupérer la valeur de l'ID sous forme de chaîne de caractères
    QString id = idItem->text();

    // Récupérer toutes les informations depuis la base de données
    QSqlQuery query;
    query.prepare("SELECT TITLE, ID_DEPARTEMENT, ID_EMPLOYER,  CREATION_DATE, DESCRIPTION, STATUE, TYPE FROM TICKET WHERE ID_TICKET = :id");
    query.bindValue(":id", id);


    if (query.exec() && query.next()) {
        // Extraire les données récupérées
        QString titre = query.value(0).toString();
        QString id_dep = query.value(1).toString();
        QString id_emp = query.value(2).toString();
        QString date = query.value(3).toString();
        QString description = query.value(4).toString();
        QString statue = query.value(5).toString();
        QString type = query.value(6).toString();

        details d;
        d.setTicketDataDetails(id,id_dep,id_emp,titre,description,type,statue,date);
        d.exec();
    }
    else {
        QMessageBox::critical(this, "Erreur", "Échec de la récupération des informations du ticket : " + query.lastError().text());
    }
}


void listTickets::filterTable(const QString &searchText)
{
    int searchColumn = 0; // Par défaut, rechercher dans la colonne ID
    QString selectedFilter = ui->ticket_comboBox->currentText();

    if (selectedFilter == "Id") {
        searchColumn = 0;
    } else if (selectedFilter == "Date") {
        searchColumn = 4;
    } else if (selectedFilter == "Status") {
        searchColumn = 3;
    } else if (selectedFilter == "Type") {
        searchColumn = 2;
    } else {
        searchColumn = 1;
    }

    for (int i = 0; i < ui->ticket_tableWidget->rowCount(); ++i)
    {
        bool match = false;
        QTableWidgetItem *item = ui->ticket_tableWidget->item(i, searchColumn);
        if (item) {
            QString itemText = item->text().toLower(); // Convertir en minuscule
            QString searchTextLower = searchText.toLower(); // Convertir aussi la recherche en minuscule
            if (itemText.startsWith(searchTextLower)) {
                match = true;
            }
        }
        ui->ticket_tableWidget->setRowHidden(i, !match);
    }

}
