#include "chatbot.h"
#include "ui_chatbot.h"
#include <QMessageBox>
#include <QSqlError>
#include "connection.h"

chatbot::chatbot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chatbot)
{
    ui->setupUi(this);

    // Initialisation de l'interface utilisateur
    chatArea = new QTextEdit(this);
    chatArea->setReadOnly(true); // Désactiver l'édition du texte
    inputBox = new QLineEdit(this);
    inputBox->setMinimumHeight(35);
    sendButton = new QPushButton("Send", this);

    // Layout vertical
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chatArea);
    layout->addWidget(inputBox);
    layout->addWidget(sendButton);
    setLayout(layout);

    // Connecter le bouton Send à la fonction onSendButtonClicked
    connect(sendButton, &QPushButton::clicked, this, &chatbot::onSendButtonClicked);

    chatArea->append("Chatbot: Hello! How can I help you?");
    chatArea->append("<br>");
}

chatbot::~chatbot()
{
    Connection::getDatabase().close(); // Ne pas laisser la connexion ouverte
    delete ui;
}

void chatbot::onSendButtonClicked()
{
    QString query = inputBox->text().trimmed();

    // Ajouter la question en rouge
    chatArea->append("<p style='color: red; font-weight: bold;'>User: " + query + "</p>");

    QString response = getResponseFromDatabase(query);

    // Ajouter la réponse en vert avec un espace après
    chatArea->append("<p style='color: green;'>Chatbot: " + response + "</p>");
    chatArea->append("<br>"); // Ajoute une ligne vide pour espacer les échanges

    inputBox->clear();
}

QString chatbot::getResponseFromDatabase(const QString &query)
{
    QSqlQuery sqlQuery(Connection::getDatabase()); // Utiliser la connexion partagée

    // Requête SQL pour récupérer la réponse
    sqlQuery.prepare("SELECT response FROM CHATBOT WHERE LOWER(question) = LOWER(:query)");
    sqlQuery.bindValue(":query", query);

    if (!sqlQuery.exec()) {
        return "Failed to execute query: " + sqlQuery.lastError().text();
    }

    if (sqlQuery.next()) {
        return sqlQuery.value(0).toString();  // Récupérer la réponse
    } else {
        return "I don't understand the question. Can you reformulate?";
    }
}
