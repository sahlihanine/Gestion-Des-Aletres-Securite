#include "department_list.h"
#include "ui_department_list.h"
#include <QVBoxLayout>
#include <QCheckBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileDialog>
#include <QPrinter>
#include <QPainter>
#include <QPushButton>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>
#include <QPrintDialog> // Nécessaire pour imprimer

department_list::department_list(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::department_list)
{
    ui->setupUi(this);

    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    this->setLayout(mainLayout);

    // Barre de recherche
    filterLineEdit = new QLineEdit(this);
    filterLineEdit->setPlaceholderText("Filtrer par nom...");
    mainLayout->addWidget(filterLineEdit);

    // Table pour afficher les départements
    ui->tableWidget->setColumnCount(2);
    QStringList headers;
    headers << "Nom du Département" << "Supprimer";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    mainLayout->addWidget(ui->tableWidget);

    // Boutons
    QPushButton *exportButton = new QPushButton("Exporter en PDF", this);
    QPushButton *printButton = new QPushButton("Imprimer", this);
    QPushButton *captureButton = new QPushButton("Capturer l'image", this);

    // Ajouter les boutons au layout
    mainLayout->addWidget(exportButton);
    mainLayout->addWidget(printButton);
    mainLayout->addWidget(captureButton);

    // Connexions
    connect(exportButton, &QPushButton::clicked, this, &department_list::exportToPDF);
    connect(printButton, &QPushButton::clicked, this, &department_list::printTable);
    connect(captureButton, &QPushButton::clicked, this, &department_list::captureImage);
    connect(filterLineEdit, &QLineEdit::textChanged, this, &department_list::filterTable);

    // Configuration de la base de données
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("localhost");
    db.setDatabaseName("projet");
    db.setUserName("Hanin");
    db.setPassword("tekup");

    if (!db.open()) {
        qDebug() << "Erreur de connexion à la base de données :" << db.lastError().text();
        return;
    }

    // Charger les données depuis la base de données
    QSqlQuery query;
    query.prepare("SELECT * FROM DEPARTMENT");

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();
        return;
    }

    int rowCount = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(rowCount);
        QString data = query.value(0).toString();
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(data));

        QCheckBox *checkBox = new QCheckBox();
        connect(checkBox, &QCheckBox::toggled, this, [=](bool checked) {
            if (checked) removeRow(rowCount);
        });
        ui->tableWidget->setCellWidget(rowCount, 1, checkBox);
        rowCount++;
    }

    ui->tableWidget->resizeColumnsToContents();
}

// Fonction pour exporter la table en PDF
void department_list::exportToPDF()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QPainter painter;
    if (!painter.begin(&printer)) {
        qDebug() << "Erreur lors de la préparation du PDF";
        return;
    }

    // Titre
    painter.setFont(QFont("Arial", 16));
    painter.drawText(100, 100, "Liste des Départements");

    // Positions pour le tableau
    int x = 100;
    int y = 200;
    int rowHeight = 30;

    // En-tête
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(x, y, "Nom du Département");

    // Contenu du tableau
    painter.setFont(QFont("Arial", 10));
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        y += rowHeight;
        QTableWidgetItem *item = ui->tableWidget->item(i, 0);
        if (item) {
            painter.drawText(x, y, item->text());
        }
    }

    painter.end();
    qDebug() << "PDF exporté avec succès : " << fileName;
}

// Fonction pour imprimer la table
void department_list::printTable()
{
    QPrinter printer;
    QPrintDialog printDialog(&printer, this);

    if (printDialog.exec() != QDialog::Accepted) {
        return; // Si l'utilisateur annule
    }

    QPainter painter(&printer);
    if (!painter.isActive()) {
        qDebug() << "Erreur : Imprimante non disponible.";
        return;
    }

    // Titre
    painter.setFont(QFont("Arial", 16));
    painter.drawText(100, 100, "Liste des Départements");

    // Positions pour le tableau
    int x = 100;
    int y = 200;
    int rowHeight = 30;

    painter.setFont(QFont("Arial", 10, QFont::Normal));
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        QTableWidgetItem *item = ui->tableWidget->item(i, 0);
        if (item) {
            painter.drawText(x, y, item->text());
            y += rowHeight;
        }
    }

    painter.end();
    qDebug() << "Impression terminée avec succès.";
}

// Fonction pour capturer une image de la fenêtre
void department_list::captureImage()
{
    QPixmap pixmap = this->grab(); // Capture toute la fenêtre
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer l'image", "", "PNG Files (*.png);;JPEG Files (*.jpg)");

    if (!fileName.isEmpty()) {
        if (pixmap.save(fileName)) {
            qDebug() << "Capture enregistrée avec succès : " << fileName;
        } else {
            qDebug() << "Erreur lors de l'enregistrement de la capture.";
        }
    }
}

// Fonction pour filtrer les données dans la table
void department_list::filterTable(const QString &filterText)
{
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        QTableWidgetItem *item = ui->tableWidget->item(i, 0);
        bool match = item && item->text().contains(filterText, Qt::CaseInsensitive);
        ui->tableWidget->setRowHidden(i, !match);
    }
}

// Fonction pour supprimer une ligne de la table
void department_list::removeRow(int row)
{
    if (row < 0 || row >= ui->tableWidget->rowCount()) return;

    QTableWidgetItem *item = ui->tableWidget->item(row, 0);
    if (item) {
        QString name = item->text();

        QSqlQuery query;
        query.prepare("DELETE FROM DEPARTMENT WHERE name = :name");
        query.bindValue(":name", name);

        if (query.exec()) {
            qDebug() << "Ligne supprimée : " << name;
            ui->tableWidget->removeRow(row);
        } else {
            qDebug() << "Erreur lors de la suppression :" << query.lastError().text();
        }
    }
}

department_list::~department_list()
{
    delete ui;
}
