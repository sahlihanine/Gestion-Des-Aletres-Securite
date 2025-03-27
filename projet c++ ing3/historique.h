#ifndef HISTORIQUE_H
#define HISTORIQUE_H

#include <QDialog>

namespace Ui {
class historique;
}

class historique : public QDialog
{
    Q_OBJECT

public:
    explicit historique(QWidget *parent = nullptr);
    ~historique();

private slots:
    void on_button_retour_clicked();

private:
    Ui::historique *ui;
    void retourMainWindow();
};

#endif // HISTORIQUE_H
