#include "login.h"
#include "ui_login.h"
#include <QSqlDatabase>
#include <QDebug>
#include "department_manager.h"


login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    department = new department_manager();
}

login::~login()
{
    delete department;
    delete ui;
}

void login::on_pushButton_Login_clicked()
{
    //department->exec();
}


void login::on_pushButton_clicked()
{
   department->exec();
}
