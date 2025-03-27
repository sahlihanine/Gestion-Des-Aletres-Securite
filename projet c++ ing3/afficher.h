#ifndef AFFICHER_H
#define AFFICHER_H

#include <QDialog>

namespace Ui {
class afficher;
}

class afficher : public QDialog
{
    Q_OBJECT

public:
    explicit afficher(QWidget *parent = nullptr);
    ~afficher();

private slots:
    void on_button_rechercher_clicked();

    void on_button_reset_clicked();

    void on_button_retour_clicked();

    void on_btnTrier_clicked();


    void on_btnExporterPDF_clicked();

    void on_btnImprimer_clicked();

private:
    Ui::afficher *ui;

    // Méthode pour remplir le tableau
    void remplirTableWidget();

    // Slots pour les boutons "Modifier" et "Supprimer"
    void onModifierClicked(int row);
    void onSupprimerClicked(int row);
    void rechercherParCIN();
    void reinitialiserTableau();
    void retourMainWindow();
};

#endif // AFFICHER_H
