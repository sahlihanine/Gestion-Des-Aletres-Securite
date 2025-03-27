#include "afficher.h"
#include "ui_afficher.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QPushButton>
#include <QMessageBox>
#include <QDateTime>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QPdfWriter>
#include <QMessageBox>
#include <QPainter>
#include <QAbstractItemModel>
#include <QPrinter>
#include <QTextDocument>
#include <QPrintDialog>


afficher::afficher(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::afficher) {
    ui->setupUi(this);
    remplirTableWidget();
    ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
    connect(ui->button_rechercher, &QPushButton::clicked, this, &afficher::rechercherParCIN);
    connect(ui->button_reset, &QPushButton::clicked, this, &afficher::reinitialiserTableau);
    connect(ui->button_retour, &QPushButton::clicked, this, &afficher::retourMainWindow);
}

afficher::~afficher() {
    delete ui;
}

void afficher::remplirTableWidget() {
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setHorizontalHeaderLabels({"ID", "CIN", "Name", "Email", "Position", "Modify", "Delete"});

    QSqlQuery query("SELECT id_employer, cin, nom, email, poste FROM employer1");
    if (!query.isActive()) {
        QMessageBox::critical(this, "Error", "Failed to execute the query: " + query.lastError().text());
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));

        QPushButton *btnModifier = new QPushButton("Modify");
        ui->tableWidget->setCellWidget(row, 5, btnModifier);
        connect(btnModifier, &QPushButton::clicked, this, [this, row]() {
            onModifierClicked(row);
        });

        QPushButton *btnSupprimer = new QPushButton("Delete");
        ui->tableWidget->setCellWidget(row, 6, btnSupprimer);
        connect(btnSupprimer, &QPushButton::clicked, this, [this, row]() {
            onSupprimerClicked(row);
        });

        row++;
    }

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        ui->tableWidget->setRowHeight(i, 40); // Par exemple, 30 pixels
    }
}

void afficher::onModifierClicked(int row) {
    QString id = ui->tableWidget->item(row, 0)->text();
    QString cin = ui->tableWidget->item(row, 1)->text();
    QString nom = ui->tableWidget->item(row, 2)->text();
    QString email = ui->tableWidget->item(row, 3)->text();
    QString poste = ui->tableWidget->item(row, 4)->text();

    QSqlQuery query;
    query.prepare("UPDATE employer1 SET cin = :cin, nom = :nom, email = :email, poste = :poste WHERE id_employer = :id_employer");
    query.bindValue(":id_employer", id);
    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":email", email);
    query.bindValue(":poste", poste);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Error while modifying the employee: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "Success", "Employee modified successfully!");

        // Ajouter l'action dans l'historique
        QSqlQuery historiqueQuery;
        historiqueQuery.prepare("INSERT INTO historiquei (ACTION, DATE_ACTION, ID_EMPLOYER, DETAILS) "
                                "VALUES (:action, CURRENT_TIMESTAMP, :id_employer, :details)");
        historiqueQuery.bindValue(":action", "Modify");
        historiqueQuery.bindValue(":id_employer", id);
        historiqueQuery.bindValue(":details", "CIN: " + cin + ", Nom: " + nom + ", Email: " + email + ", Poste: " + poste);
        if (!historiqueQuery.exec()) {
            QMessageBox::critical(this, "Error", "Error while saving to the history: " + historiqueQuery.lastError().text());
        }
    }
}

void afficher::onSupprimerClicked(int row) {
    QString id = ui->tableWidget->item(row, 0)->text();
    QString cin = ui->tableWidget->item(row, 1)->text();
    QString nom = ui->tableWidget->item(row, 2)->text();
    QString email = ui->tableWidget->item(row, 3)->text();
    QString poste = ui->tableWidget->item(row, 4)->text();

    QSqlQuery query;
    query.prepare("DELETE FROM employer1 WHERE id_employer = :id_employer");
    query.bindValue(":id_employer", id);
    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":email", email);
    query.bindValue(":poste", poste);


    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Error while deleting the employee: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "Success", "Employee deleted successfully!");
        ui->tableWidget->removeRow(row);

        // Ajouter l'action dans l'historique
        QSqlQuery historiqueQuery;
        historiqueQuery.prepare("INSERT INTO historiquei (ACTION, DATE_ACTION, ID_EMPLOYER, DETAILS) "
                                "VALUES (:action, CURRENT_TIMESTAMP, :id_employer, :details)");
        historiqueQuery.bindValue(":action", "Delete");
        historiqueQuery.bindValue(":id_employer", id);
        historiqueQuery.bindValue(":details", "CIN: " + cin + ", Nom: " + nom + ", Email: " + email + ", Poste: " + poste);
        if (!historiqueQuery.exec()) {
            QMessageBox::critical(this, "Error", "Error while saving to the history: " + historiqueQuery.lastError().text());
        }
    }
}
void afficher::rechercherParCIN()
{
    QString cinRecherche = ui->lineEdit_recherche->text(); // Récupérer le texte saisi dans la barre de recherche

    if (cinRecherche.isEmpty()) {
        // Si le champ de recherche est vide, réafficher toutes les lignes
        for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
            ui->tableWidget->setRowHidden(i, false); // Toutes les lignes deviennent visibles
        }
        return;
    }

    bool trouve = false; // Indicateur pour vérifier si un employé est trouvé

    // Parcourir toutes les lignes du tableau
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        // Supposons que la colonne CIN est la 1ère colonne (indice 1)
        QString cin = ui->tableWidget->item(i, 1)->text(); // Obtenir le texte de la colonne CIN

        // Vérifier si le CIN correspond au texte recherché
        if (cin == cinRecherche) {
            ui->tableWidget->setRowHidden(i, false); // Afficher la ligne correspondante
            trouve = true;
        } else {
            ui->tableWidget->setRowHidden(i, true); // Masquer les lignes qui ne correspondent pas
        }
    }

    if (!trouve) {
        QMessageBox::information(this, "Research", "No employee found with this CIN.");
    }
    if (cinRecherche.isEmpty()) {
        reinitialiserTableau(); // Appelle directement la réinitialisation
        return;
    }
}

void afficher::reinitialiserTableau()
{
    // Rendre toutes les lignes du tableau visibles
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        ui->tableWidget->setRowHidden(i, false);
    }

    // Effacer le champ de recherche
    ui->lineEdit_recherche->clear();
}



void afficher::on_button_rechercher_clicked()
{
    rechercherParCIN(); // Appeler la fonction de recherche
}


void afficher::on_button_reset_clicked()
{
    reinitialiserTableau();
}
void afficher::retourMainWindow()
{
    this->close(); // Ferme la fenêtre actuelle (afficher)
}


void afficher::on_button_retour_clicked()
{
    retourMainWindow();
}

void afficher::on_btnTrier_clicked()
{
    int critere = ui->comboBoxTri->currentIndex(); // Récupérer le critère sélectionné
    switch (critere) {
    case 0: // Tri par ID
        ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);
        break;
    case 1: // Tri par Nom
        ui->tableWidget->sortByColumn(2, Qt::AscendingOrder);
        break;
    case 2: // Tri par CIN
        ui->tableWidget->sortByColumn(1, Qt::AscendingOrder);
        break;
    default:
        break;
    }
}

void afficher::on_btnExporterPDF_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPagedPaintDevice::A4);
    pdfWriter.setResolution(300);
    QPainter painter(&pdfWriter);

    if (!painter.isActive()) {
        QMessageBox::warning(this, "Error", "Unable to initialize the PDF.");
        return;
    }

    int margin = 50; // Marges des côtés
    int tableTop = 300; // Position du tableau (plus éloigné du titre)
    int rowHeight = 60; // Hauteur des lignes (plus grand)
    int colWidth = 400; // Largeur des colonnes (plus grand)
    int pageWidth = pdfWriter.width();

    // Titre centré
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.drawText(QRect(margin, margin, pageWidth - 2 * margin, rowHeight),
                     Qt::AlignCenter, "Employee List");

    // En-têtes avec style
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.setBrush(QColor(200, 200, 200)); // Couleur d'arrière-plan des en-têtes
    for (int col = 0; col < ui->tableWidget->columnCount() - 2; ++col) { // Exclure Modifier/Supprimer
        QRect cellRect(margin + col * colWidth, tableTop, colWidth, rowHeight);
        painter.drawRect(cellRect);
        painter.drawText(cellRect, Qt::AlignCenter, ui->tableWidget->horizontalHeaderItem(col)->text());
    }

    // Contenu des cellules
    painter.setFont(QFont("Arial", 12));
    painter.setBrush(Qt::NoBrush); // Enlever l'arrière-plan pour les cellules normales
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        for (int col = 0; col < ui->tableWidget->columnCount() - 2; ++col) { // Exclure Modifier/Supprimer
            QRect cellRect(margin + col * colWidth, tableTop + (row + 1) * rowHeight, colWidth, rowHeight);
            painter.drawRect(cellRect);
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            if (item) {
                painter.drawText(cellRect, Qt::AlignCenter, item->text());
            }
        }
    }

    // Pied de page
    painter.setFont(QFont("Arial", 10));
    painter.drawText(QRect(margin, pdfWriter.height() - margin - rowHeight, pageWidth - 2 * margin, rowHeight),
                     Qt::AlignCenter, "Généré automatiquement avec Qt");

    painter.end();
    QMessageBox::information(this, "Success", "Stylized PDF file generated successfully!");
}


void afficher::on_btnImprimer_clicked()
{
    QPrinter printer;
    QPrintDialog printDialog(&printer, this);

    if (printDialog.exec() == QDialog::Rejected) {
        // Si l'utilisateur annule l'impression
        return;
    }

    QTextDocument doc;
    QString htmlContent;

    htmlContent.append("<table border='1' cellspacing='0' cellpadding='4'>");
    htmlContent.append("<tr>");
    for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
        htmlContent.append("<th>" + ui->tableWidget->horizontalHeaderItem(col)->text() + "</th>");
    }
    htmlContent.append("</tr>");

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        htmlContent.append("<tr>");
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            QTableWidgetItem* item = ui->tableWidget->item(row, col);
            htmlContent.append("<td>" + (item ? item->text() : "") + "</td>");
        }
        htmlContent.append("</tr>");
    }

    htmlContent.append("</table>");

    doc.setHtml(htmlContent);
    doc.print(&printer);  // Imprime directement sur l'imprimante sélectionnée
}
