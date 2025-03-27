#ifndef INTERFACEEMPLOYER_H
#define INTERFACEEMPLOYER_H

#include <QMainWindow>  // Utilisation de QMainWindow, pas QDialog
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QItemSelectionModel>
#include "ui_interfaceemployer.h"  // Inclure le fichier .ui généré

namespace Ui {
class InterfaceEmployer;
}

class InterfaceEmployer : public QDialog {  // Héritage de QMainWindow
    Q_OBJECT

public:
    explicit InterfaceEmployer(QWidget *parent = nullptr);
    ~InterfaceEmployer();

private slots:
    void addEmployer();
    void updateEmployer();
    void deleteEmployer();
    void showEmployers();
    void clearFields();
    void onSelectionChanged(const QItemSelection &selected);
    void logHistorique(const QString &action, const QString &idEmployer, const QString &details);
    void on_showButton_clicked();
    void on_updateButton_clicked();
    void on_deleteButton_clicked();
    void on_historiqueButton_clicked();

private:
    Ui::InterfaceEmployer *ui;
    QSqlDatabase db;
    QSqlTableModel *model;
};

#endif // INTERFACEEMPLOYER_H
