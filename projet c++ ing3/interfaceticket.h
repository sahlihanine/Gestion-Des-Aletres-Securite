#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "ticket.h"
#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class interfaceTicket; }
QT_END_NAMESPACE

class interfaceTicket : public QDialog
{
    Q_OBJECT


public:
    interfaceTicket(QWidget *parent = nullptr);
    ~interfaceTicket();
    void setIdEmployer(int id){idEmployer=id;};
    void setIdDepartement(int idD){idDepartement=idD;};

private slots:
    void on_pb_ticket_clicked();
    //void on_pushButton_3_clicked();

    void on_pb_chatbot_clicked();

    void on_pb_tickets_list_clicked();

    void on_pb_tickets_history_clicked();

    void on_pb_profil_clicked();

private:
    Ui::interfaceTicket *ui;
    int idEmployer;
    int idDepartement;
};
#endif // MAINWINDOW_H
