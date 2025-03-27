#include "departement.h"
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidget>
departement::departement()
{
departement_name="";

}
departement::departement(QString departement_name)
{
  this->departement_name=departement_name;
}
QString departement::get_departement_name(){return  departement_name;}

bool departement::ajouter(QString name,int ID)
{
    // Étape 1 : Création de l'objet QSqlDatabase
      QSqlDatabase db = QSqlDatabase::addDatabase("QODBC"); // Pour MySQL

      // Étape 2 : Configuration de la connexion
      db.setHostName("localhost");       // Nom ou adresse du serveur
      db.setDatabaseName("Projet"); // Nom de la base de données
      db.setUserName("Hanin");     // Nom d'utilisateur
      db.setPassword("tekup");    // Mot de passe

      // Étape 3 : Ouverture de la connexion
      if (!db.open()) {
          qDebug() << "Erreur de connexion à la base de données :"
                   << db.lastError().text();
          return -1;
      } else {
          qDebug() << "Connexion réussie à la base de données !";
          QSqlQuery query;
          query.prepare("INSERT INTO DEPARTMENT (name,ID) VALUES (:name ,:ID)");

              // Lier les variables aux paramètres de la requête
              query.bindValue(":name", name); // Lier le paramètre :nom à la variable nom
              query.bindValue(":ID", ID); // Lier le paramètre :nom à la variable nom

              if (query.exec()) {
                      qDebug() << "department a été inséré avec succès!";
                      return true;  // Succès
                  } else {
                      qDebug() << "Erreur lors de l'insertion : " << query.lastError().text();
                      return false;  // Erreur
                  }
}




}



void afficher(QTableWidget *tableWidget)
{
    // Configuration de la connexion à la base de données
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");  // ou QSQLITE, QPSQL, selon votre SGBD
    db.setHostName("localhost");
    db.setDatabaseName("Projet");  // Nom de la base de données
    db.setUserName("Hanin");      // Utilisateur
    db.setPassword("tekup");      // Mot de passe

    // Ouverture de la connexion à la base de données
    if (!db.open()) {
        qDebug() << "Erreur de connexion à la base de données :" << db.lastError().text();
        return;
    }

    // Création de la requête SQL
    QSqlQuery query;
    query.prepare("SELECT * FROM DEPARTMENT");

    // Exécution de la requête
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();
        return;
    }

    // Vider le QTableWidget avant de le remplir avec de nouvelles données
    tableWidget->clearContents();
    tableWidget->setRowCount(0);

    // Récupérer le nombre de colonnes
    //int columnCount = query.record().count();

    // Configurer le nombre de colonnes dans le QTableWidget
    tableWidget->setColumnCount(1);

    // Récupérer les en-têtes de colonnes à partir des noms des champs de la requête
    for (int i = 0; i < 1; ++i) {
        tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(1));
    }

    // Remplir le QTableWidget avec les résultats de la requête
    int rowCount = 0;
    while (query.next()) {
        tableWidget->insertRow(rowCount);  // Ajouter une ligne dans le QTableWidget

        // Remplir chaque cellule de la ligne avec les données
        for (int i = 0; i < 1; ++i) {
            QString data = query.value(i).toString();  // Récupérer la valeur de la cellule
            tableWidget->setItem(rowCount, i, new QTableWidgetItem(data));
        }
        rowCount++;
    }

    // Ajuster la taille des colonnes pour s'adapter au contenu
    tableWidget->resizeColumnsToContents();
}

