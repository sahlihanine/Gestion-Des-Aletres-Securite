#ifndef DEPARTMENT_LIST_H
#define DEPARTMENT_LIST_H
#include <QLineEdit>

//#if QT_VERSION >= 0x050000
//#include <QtWidgets/QDialog>
//#else
#include <QtWidgets/QDialog>
//#endif

namespace Ui {
class department_list;
}

class department_list : public QDialog
{
    Q_OBJECT

public:
    explicit department_list(QWidget *parent = nullptr);
    ~department_list();

private:
    Ui::department_list *ui;
    void removeRow(int row);
    void onCheckBoxToggled(bool checked);
    QLineEdit *filterLineEdit; // Zone de recherche
    void filterTable(const QString &filterText); // Nouvelle fonction pour filtrer
    void exportToPDF(); // Nouvelle fonction pour exporter en PDF
    void printTable();           // Nouvelle méthode pour imprimer
    void captureImage();
    void showHistory();  // Nouvelle méthode pour afficher l'historique

};

#endif // DEPARTMENT_LIST_H
