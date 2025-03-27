#include "login_ib.h"
#include "interfaceticket.h"
#include <QMessageBox>


Login_ib::Login_ib(QWidget *parent) : QWidget(parent)
{
    QLabel *titleLabel = new QLabel("<h2 style='color:#222222;'>Login & Register</h2>", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    // Ajout du champ CIN
    QLabel *cinLabel = new QLabel("CIN :", this);
    cinInput = new QLineEdit(this);

    // Champ Nom
    QLabel *nameLabel = new QLabel("Nom :", this);
    nameInput = new QLineEdit(this);

    // Champ Email
    QLabel *emailLabel = new QLabel("Email :", this);
    emailInput = new QLineEdit(this);

    // Séparateur
    QFrame *separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);

    // Boutons Login et Register
    loginButton = new QPushButton("Login", this);
    registerButton = new QPushButton("Register", this);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(loginButton);
    buttonsLayout->addWidget(registerButton);

    QLabel *registerNowLabel = new QLabel("<a href='#'>Vous n'avez pas de compte ? Enregistrez-vous maintenant !</a>", this);
    registerNowLabel->setProperty("link", true);
    registerNowLabel->setAlignment(Qt::AlignCenter);
    registerNowLabel->setOpenExternalLinks(false);

    // Layout du formulaire
    QVBoxLayout *formLayout = new QVBoxLayout();

    formLayout->addWidget(cinLabel);  // Ajouter le label CIN
    formLayout->addWidget(cinInput);  // Ajouter le champ CIN
    formLayout->addWidget(nameLabel);
    formLayout->addWidget(nameInput);
    formLayout->addWidget(emailLabel);
    formLayout->addWidget(emailInput);
    formLayout->addWidget(separator);
    formLayout->addLayout(buttonsLayout);
    formLayout->addWidget(registerNowLabel);

    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(formLayout);

    setLayout(mainLayout);
    setFixedSize(400, 380);  // Augmenter la taille pour tenir compte du champ CIN
    setWindowTitle("Login & Register");

    // Centrer la fenêtre
    centerWindow(this);

    connect(registerButton, &QPushButton::clicked, this, &Login_ib::registerUser);
    connect(loginButton, &QPushButton::clicked, this, &Login_ib::loginUser);

    connect(registerNowLabel, &QLabel::linkActivated, this, &Login_ib::registerUser);
}
void Login_ib::loginUser()
{
    // Récupérer les valeurs des champs
    QString cinValue = cinInput->text();
    QString nameValue = nameInput->text();
    QString emailValue = emailInput->text();

    if (cinValue.isEmpty() || nameValue.isEmpty() || emailValue.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both CIN and Email.");
        return;
    }

    // Vérifier si l'employé existe dans la base de données
    QSqlQuery query;
    query.prepare("SELECT ID_EMPLOYER, ID_DEPARTEMENT FROM employer1 WHERE cin = :cin AND email = :email");
    query.bindValue(":cin", cinValue);
    query.bindValue(":email", emailValue);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to query the database: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        // Récupérer les valeurs de ID_EMPLOYER et ID_DEPARTEMENT
        int idEmployer = query.value("ID_EMPLOYER").toInt();
        int idDepartement = query.value("ID_DEPARTEMENT").toInt();

        QMessageBox::information(this, "Login Successful",
                                 QString("Welcome back! Your Employer ID is %1 and Department ID is %2.")
                                 .arg(idEmployer)
                                 .arg(idDepartement));

        interfaceTicket it;
        it.setIdEmployer(idEmployer);
        it.setIdDepartement(idDepartement);
        it.exec();
        this->close();
    } else {
        QMessageBox::warning(this, "Login Failed", "The employee does not exist in the database.");
    }
}


void Login_ib::registerUser()
{
    // Créer et afficher la fenêtre RegisterWindow
    RegisterWindow *registerWindow = new RegisterWindow();
    registerWindow->show();

    // Fermer la fenêtre actuelle
    this->close();
}

void Login_ib::centerWindow(QWidget *window)
{
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - window->width()) / 2;
    int y = (screenGeometry.height() - window->height()) / 2;
    window->move(x, y);
}
