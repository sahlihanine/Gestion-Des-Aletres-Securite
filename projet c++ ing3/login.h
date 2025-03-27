#ifndef LOGIN_H
#define LOGIN_H

/*#if QT_VERSION >= 0x050000
#include <QtWidgets/QDialog>
#else
#include <QtGui/QDialog>
#endif
*/
#include <QtWidgets/QDialog>
#include "department_manager.h"
namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private slots:
    void on_pushButton_Login_clicked();

    void on_pushButton_clicked();

private:
   Ui::login *ui;
department_manager *department;
};

#endif // LOGIN_H

