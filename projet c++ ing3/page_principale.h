#ifndef PAGE_PRINCIPALE_H
#define PAGE_PRINCIPALE_H

#include <QWidget>
#include "ui_page_principale.h"
#include "login.h"
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class page_principale; }
QT_END_NAMESPACE

class page_principale : public QDialog
{
    Q_OBJECT

public:
    explicit page_principale(QDialog *parent = nullptr); // Utilisation de explicit pour éviter des conversions implicites
    ~page_principale();

private slots:
    void on_pushButton_Admin_clicked(); // Assurez-vous que cette méthode correspond au nom de votre slot

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::page_principale *ui;
    login *login_admin;
};

#endif // PAGE_PRINCIPALE_H
