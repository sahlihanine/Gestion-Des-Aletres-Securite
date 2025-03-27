#include "page_principale.h"
#include "register_window.h"
#include "login_ib.h"
#include <QDebug>

page_principale::page_principale(QDialog *parent)
    : QDialog(parent),
      ui(new Ui::page_principale)
{
    ui->setupUi(this); // 'this' est maintenant de type QWidget*
    login_admin = new login(this); // Passez 'this' comme parent si nécessaire
    connect(ui->pushButton_2, &QPushButton::clicked, this, &page_principale::on_pushButton_2_clicked);

}

page_principale::~page_principale()
{
    delete ui;
    delete login_admin; // Assurez-vous de libérer la mémoire correctement
}

void page_principale::on_pushButton_Admin_clicked()
{
  //  login_admin->show(); // Utilisez 'show()' pour afficher la fenêtre login
}

void page_principale::on_pushButton_clicked()
{
    login_admin->show(); // Utilisez 'show()' pour afficher la fenêtre login

}

void page_principale::on_pushButton_2_clicked()
{
    // Assurez-vous qu'une seule instance est créée
    static Login_ib *loginWindow = nullptr;

    if (!loginWindow) {
        loginWindow = new Login_ib();
    }

    loginWindow->show();
}

