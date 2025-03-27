#ifndef LOGIN_IB_H
#define LOGIN_IB_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QGuiApplication>
#include <QScreen>
#include "register_window.h" // Inclure la classe RegisterWindow

class Login_ib : public QWidget
{
    Q_OBJECT

public:
    explicit Login_ib(QWidget *parent = nullptr);

private slots:
    void registerUser();
    void loginUser();

private:
    QLineEdit *cinInput, *nameInput, *emailInput;
    QPushButton *loginButton, *registerButton;

    void centerWindow(QWidget *window);
};

#endif // LOGIN_IB_H
