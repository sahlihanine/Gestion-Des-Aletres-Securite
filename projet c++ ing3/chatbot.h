#ifndef CHATBOT_H
#define CHATBOT_H

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlQuery>

namespace Ui {
class chatbot;
}

class chatbot : public QDialog
{
    Q_OBJECT

public:
    explicit chatbot(QWidget *parent = nullptr);
    ~chatbot();

private slots:
    void onSendButtonClicked();

private:
    Ui::chatbot *ui;

    QTextEdit *chatArea;  // Zone de chat
    QLineEdit *inputBox;  // Champ de saisie
    QPushButton *sendButton;  // Bouton pour envoyer les messages

    QString getResponseFromDatabase(const QString &query);  // Fonction pour obtenir une réponse depuis la base de données
};

#endif // CHATBOT_H
