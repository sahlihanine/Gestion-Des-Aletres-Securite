#ifndef DEPARTMENT_MANAGER_H
#define DEPARTMENT_MANAGER_H

#include "mainwindow.h"
#include "department_list.h"  // Inclut la classe department_list
#include <QDialog>  // Assurez-vous que QDialog est inclus proprement

namespace Ui {
class department_manager;
}

class department_manager : public QDialog
{
    Q_OBJECT

public:
    explicit department_manager(QWidget *parent = nullptr);  // Utilisez QWidget pour le parent
    ~department_manager();

private slots:
    void on_pushButton_clicked();   // Slot pour ouvrir MainWindow
    void on_pushButton_2_clicked(); // Slot pour ouvrir department_list
    void on_pb_ticket_clicked();
    void on_pb_employer_clicked();

private:
    Ui::department_manager *ui;
    MainWindow *mainwindow_page;        // Instance de MainWindow
    department_list *department_list_page; // Instance de department_list
};

#endif // DEPARTMENT_MANAGER_H
