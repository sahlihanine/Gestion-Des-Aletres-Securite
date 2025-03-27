#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "departement.h"
#include <QMainWindow>
#include<QDialog>

namespace Ui {
class MainWindow;
}

class MainWindow :public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget  *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_ajouter_clicked();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
