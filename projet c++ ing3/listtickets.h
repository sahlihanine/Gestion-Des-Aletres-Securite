#ifndef LISTTICKETS_H
#define LISTTICKETS_H

#include <QDialog>
#include <QSqlQuery>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
//#include "modefier.h"

namespace Ui {
class listTickets;
}

class listTickets : public QDialog
{
    Q_OBJECT

public:
    explicit listTickets(QWidget *parent = nullptr);
    ~listTickets();
    void remplirTableWidget();
    void updateTicketInTable(const QString &ticketId, const QString &newTitre, const QString &newDescription, const QString &newType);

private slots:
    void onSupprimerClicked();
    void onModifierClicked();
    void onDetailsClicked();
    void filterTable(const QString &searchText);

private:
    Ui::listTickets *ui;
};


#endif // LISTTICKETS_H
