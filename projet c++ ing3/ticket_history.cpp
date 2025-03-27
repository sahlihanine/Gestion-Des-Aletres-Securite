#include "ticket_history.h"
#include "ui_ticket_history.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

#include "connection.h"
#include "details.h"

ticket_history::ticket_history(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ticket_history)
{
    ui->setupUi(this);

    // Remplir le QTableWidget avec les données
    remplirTableWidget();

    ui->searchLineEdit->setPlaceholderText("Rechercher");
    // Connecter la barre de recherche
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &ticket_history::filterTable);

    ui->ticket_comboBox->addItem("Title");
    ui->ticket_comboBox->addItem("Action");
    ui->ticket_comboBox->addItem("Id");
    ui->ticket_comboBox->addItem("Id departement");
    ui->ticket_comboBox->addItem("Id employer");
    ui->ticket_comboBox->addItem("Date");
}

ticket_history::~ticket_history()
{
    delete ui;
}

void ticket_history::remplirTableWidget()
{
    // Configurer le tableau
    ui->ticket_tableWidget->setColumnCount(10);
    ui->ticket_tableWidget->setHorizontalHeaderLabels({"ACTION", "ID", "ID_EMPLOYER", "ID_DEPARTEMENT", "TITLE", "TYPE", "STATUS", "DESCRIPTION", "DATE", "DETAILS"});

    // Supprimer la numérotation des lignes
    ui->ticket_tableWidget->verticalHeader()->setVisible(false);

    // Charger les données de la base
    QSqlQuery query("SELECT ACTION, ID_T, ID_E, ID_D, TL, TP, ST, DESCR, C_DATE FROM HISTORIQUE");

    int row = 0;
    while (query.next()) {
        // Ajouter une nouvelle ligne dans le tableau
        ui->ticket_tableWidget->insertRow(row);

        // Ajouter les données de base
        ui->ticket_tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->ticket_tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->ticket_tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->ticket_tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->ticket_tableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        ui->ticket_tableWidget->setItem(row, 7, new QTableWidgetItem(query.value(7).toString()));
        ui->ticket_tableWidget->setItem(row, 8, new QTableWidgetItem(query.value(8).toString()));

        // Ajouter l'élément TYPE avec un carré coloré à gauche
        QTableWidgetItem *typeItem = new QTableWidgetItem(query.value(5).toString());
        QString type = query.value(5).toString().toLower();

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

        ui->ticket_tableWidget->setItem(row, 5, typeItem);

        // Ajouter l'élément STATUS avec un carré coloré à gauche
        QTableWidgetItem *statusItem = new QTableWidgetItem(query.value(6).toString());
        QString status = query.value(6).toString().toLower(); // Pour éviter les erreurs de casse

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

        ui->ticket_tableWidget->setItem(row, 6, statusItem);

        // Ajouter les boutons
        QPushButton *btnDetails = new QPushButton("DETAILS");
        btnDetails->setProperty("row", row);
        ui->ticket_tableWidget->setCellWidget(row, 9, btnDetails);
        connect(btnDetails, &QPushButton::clicked, this, &ticket_history::onDetailsClicked);

        row++;
    }

    // Configurer la taille des colonnes pour qu'elles aient toutes la même largeur
    int columnCount = ui->ticket_tableWidget->columnCount();
    int fixedWidth = 125; // Largeur fixe en pixels pour chaque colonne (vous pouvez ajuster selon vos besoins)

    ui->ticket_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    for (int col = 0; col < columnCount; ++col) {
        ui->ticket_tableWidget->setColumnWidth(col, fixedWidth);
    }
}

void ticket_history::filterTable(const QString &searchText)
{
    int searchColumn = 0;
    QString selectedFilter = ui->ticket_comboBox->currentText();

    if (selectedFilter == "Id") {
        searchColumn = 1;
    } else if (selectedFilter == "Action") {
        searchColumn = 0;
    } else if (selectedFilter == "Id departement") {
        searchColumn = 3;
    } else if (selectedFilter == "Id employer") {
        searchColumn = 2;
    } else if (selectedFilter == "Title") {
        searchColumn = 4;
    } else {
        searchColumn = 8;
    }

    for (int i = 0; i < ui->ticket_tableWidget->rowCount(); ++i) {
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


void ticket_history::onDetailsClicked()
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

    // Récupérer les données de la ligne sélectionnée
    QString id = ui->ticket_tableWidget->item(row, 1)->text();
    QString id_employer = ui->ticket_tableWidget->item(row, 2)->text();
    QString id_departement = ui->ticket_tableWidget->item(row, 3)->text();
    QString title = ui->ticket_tableWidget->item(row, 4)->text();
    QString type = ui->ticket_tableWidget->item(row, 5)->text();
    QString status = ui->ticket_tableWidget->item(row, 6)->text();
    QString description = ui->ticket_tableWidget->item(row, 7)->text();
    QString date = ui->ticket_tableWidget->item(row, 8)->text();

    // Créer une instance de la fenêtre de détails et lui transmettre les données
    details d;
    d.setTicketDataDetails(id, id_departement, id_employer, title, description, type, status, date);
    d.exec();
}

