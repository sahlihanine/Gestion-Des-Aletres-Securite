#ifndef TICKET_HISTORY_H
#define TICKET_HISTORY_H

#include <QDialog>
#include <QPushButton>


namespace Ui {
class ticket_history;
}

class ticket_history : public QDialog
{
    Q_OBJECT

public:
    explicit ticket_history(QWidget *parent = nullptr);
    ~ticket_history();
    void remplirTableWidget();
    void filterTable(const QString &searchText);
    void onDetailsClicked();

private:
    Ui::ticket_history *ui;
};

#endif // TICKET_HISTORY_H
