#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "departement.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget*parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
ui->setupUi(this);
ui->lineEdit_departement_name->setMaxLength(10);
ui->lineEdit_department_id->setMaxLength(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pb_ajouter_clicked()
{
   hide();
}
void MainWindow::on_pushButton_clicked()
{
    QString name= ui->lineEdit_departement_name->text();
    int ID= (ui->lineEdit_department_id->text()).toInt();

 departement e;
    bool test=e.ajouter(name,ID);
  if(test)
{

QMessageBox::information(nullptr, QObject::tr("Ajouter un étudiant"),
                  QObject::tr("Etudiant ajouté.\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);

}
  else
      QMessageBox::critical(nullptr, QObject::tr("Ajouter un étudiant"),
                  QObject::tr("Erreur !.\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);
}
