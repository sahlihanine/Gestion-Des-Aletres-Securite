#include "profil.h"
#include "ui_profil.h"

profil::profil(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::profil)
{
    ui->setupUi(this);
}

profil::~profil()
{
    delete ui;
}

void  profil::set_Profil_details(const QString &idEmployer, const QString &idDepartement, const QString &cin,const QString &nom, const QString &poste,const QString &email)
{
    // Mettre à jour les labels avec les informations récupérées
    ui->ID_Emp->setText(idEmployer);
    ui->ID_Dep->setText(idDepartement);
    ui->Poste->setText(poste);
    ui->Email->setText(email);
    ui->Nom->setText(nom);
    ui->CIN->setText(cin);
}
