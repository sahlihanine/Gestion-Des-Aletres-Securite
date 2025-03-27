#ifndef TICKET_H
#define TICKET_H

#include <QDialog>
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QButtonGroup>

namespace Ui {
class ticket;
}

class ticket : public QDialog
{
    Q_OBJECT

public:
    explicit ticket(QWidget *parent = nullptr);
    ~ticket();
    void setIdEmployer(int id){idEmployer=id;};
    void setIdDepartement(int idD){idDepartement=idD;};
    int getIdEmployer(){return idEmployer;};
    int getIdDepartement(){return idDepartement;};


private slots:
    void on_pb_add_clicked();

private:
    Ui::ticket *ui;
    int idTicket;
    QString type;
    QString titre;
    QString statue;
    QString description;
    int idEmployer;
    int idDepartement;
    QButtonGroup *typeGroup;
    bool isValid;
};

#endif // TICKET_H
