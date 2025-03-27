#ifndef REGISTER_WINDOW_H
#define REGISTER_WINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

class RegisterWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    //~RegisterWindow();

private slots:
    void saveDetails();
    void goBackToLogin();

private:
    QLineEdit *idInput, *cinInput, *nameInput, *emailInput, *posteInput, *departementInput;
    QPushButton *saveButton, *returnButton;
};

#endif // REGISTER_WINDOW_H
