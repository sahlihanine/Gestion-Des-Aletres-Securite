#ifndef DETAILS_H
#define DETAILS_H

#include <QDialog>

namespace Ui {
class details;
}

class details : public QDialog
{
    Q_OBJECT

public:
    explicit details(QWidget *parent = nullptr);
    ~details();
    void setTicketDataDetails(const QString &id, const QString &id_dep, const QString &id_emp, const QString &titre, const QString &description, const QString &type, const QString &statue, const QString &date);
    void captureScreenshot();


private slots:
    void on_pb_print_clicked();

private:
    Ui::details *ui;
    QString ticketId;  // Pour garder une trace de l'ID du ticket
};

#endif // DETAILS_H
