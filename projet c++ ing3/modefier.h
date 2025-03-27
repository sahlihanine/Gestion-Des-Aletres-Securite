#ifndef MODEFIER_H
#define MODEFIER_H

#include <QDialog>
#include <QButtonGroup>

namespace Ui {
class modefier;
}

class modefier : public QDialog
{
    Q_OBJECT

public:
    explicit modefier(QWidget *parent = nullptr);
    ~modefier();

    void setTicketData(const QString &titre, const QString &description, const QString &type, const QString &statue, const QString &id);


private slots:
    void on_pb_valider_clicked();

    void on_pb_cancel_clicked();

private:
    Ui::modefier *ui;
    QString ticketId;  // Pour garder une trace de l'ID du ticket
    QButtonGroup *typeGroup;
    QButtonGroup *statusGroup;

signals:
    void ticketModified(const QString &ticketId, const QString &newTitre, const QString &newDescription, const QString &newType);

};

#endif // MODEFIER_H
