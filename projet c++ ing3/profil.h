#ifndef PROFIL_H
#define PROFIL_H

#include <QDialog>

namespace Ui {
class profil;
}

class profil : public QDialog
{
    Q_OBJECT

public:
    explicit profil(QWidget *parent = nullptr);
    ~profil();
    void set_Profil_details(const QString &idEmployer, const QString &idDepartement, const QString &cin,const QString &nom, const QString &poste,const QString &email);

private:
    Ui::profil *ui;
};

#endif // PROFIL_H












