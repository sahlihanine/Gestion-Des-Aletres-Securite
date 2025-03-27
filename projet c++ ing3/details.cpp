#include "details.h"
#include "ui_details.h"

#include <QFileDialog>
#include <QSqlQuery>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPixmap>
#include <QWidget>
#include <QDebug>

details::details(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::details)
{
    ui->setupUi(this);
    // Connexion du bouton screenshot à la fonction captureScreenshot
    connect(ui->screenshotButton, &QPushButton::clicked, this, &details::captureScreenshot);

}

details::~details()
{
    delete ui;
}

void details::setTicketDataDetails(const QString &id, const QString &id_dep, const QString &id_emp, const QString &titre, const QString &description, const QString &type, const QString &statue, const QString &date)
{
    ticketId = id;

    ui->ID_Ticket->setText(id);
    ui->ID_Dep->setText(id_dep);
    ui->ID_Emp->setText(id_emp);
    ui->Desc->setText(description);
    ui->Statue->setText(statue);
    ui->Titre->setText(titre);
    ui->Type->setText(type);
    ui->Date->setText(date);

    // Assurez-vous de relancer l'affichage pour que les données apparaissent correctement
    update();
}


void details::on_pb_print_clicked()
{
    // 1. Vérification de la taille et affichage
    ui->widgetConteneur->resize(800, 600); // Taille minimale
    ui->widgetConteneur->show();
    QApplication::processEvents(); // Rafraîchissement de l'interface

    // 2. Capture temporaire du widget
    QPixmap pixmap = ui->widgetConteneur->grab();
    if (pixmap.isNull()) {
        qDebug() << "Erreur : Impossible de capturer l'écran du widget.";
        return;
    }

    // 3. Configuration de l'imprimante
    QPrinter printer;
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Rejected) {
        return; // Annulation
    }

    // 4. Impression avec QPainter
    QPainter painter(&printer);
    QRect rect = painter.viewport();
    QSize size = pixmap.size();
    size.scale(rect.size(), Qt::KeepAspectRatio);

    painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
    painter.setWindow(pixmap.rect());
    painter.drawPixmap(0, 0, pixmap);

    if (painter.end()) {
        qDebug() << "Impression réussie.";
    } else {
        qDebug() << "Erreur lors de l'impression.";
    }
}



void details::captureScreenshot() {
    QPixmap pixmap = this->grab(); // Capturer toute la fenêtre

    QString fileName = QFileDialog::getSaveFileName(this, tr("Enregistrer la capture"), QDir::homePath(), tr("Images (*.png *.jpg)"));
    if (!fileName.isEmpty()) {
        pixmap.save(fileName); // Enregistrer la capture d'écran
        qDebug() << "Capture enregistrée : " << fileName;
    }
}



