#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QPushButton>
#include <QProcess>
#include <QMessageBox>
#include <QMouseEvent>
#include <QMovie>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainWindow::setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    // Gets installed packages
    searchInstalledPackages();

    checkIfVcpkgaIsInstalled();

    QLabel* gifLabel = new QLabel(this); // Create a QLabel
    QMovie* gifMovie = new QMovie(":/spinner.gif"); // Load the GIF

    gifLabel->setMovie(gifMovie); // Set the movie on the QLabel
    gifMovie->start(); // Start playing the GIF

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(gifLabel);
    ui->loading->setLayout(layout);
    ui->loading->setVisible(true);

    QLabel* gifLabel2 = new QLabel(this); // Create a QLabel
    QMovie* gifMovie2 = new QMovie(":/spinner.gif"); // Load the GIF

    gifLabel2->setMovie(gifMovie2); // Set the movie on the QLabel
    gifMovie2->start(); // Start playing the GIF

    QVBoxLayout* layout2 = new QVBoxLayout;
    layout2->addWidget(gifLabel2);
    ui->loading_2->setLayout(layout2);
    ui->loading_2->setVisible(false);

    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setShowGrid(false);

    ui->searchTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->searchTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->searchTable->setShowGrid(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::checkIfVcpkgaIsInstalled(){
    QDir pathDir("C:/Development/vcpkg");
    if (pathDir.exists())
    {
        ui->stackedWidget->setCurrentIndex(0);
        return true;
    }else{
        ui->stackedWidget->setCurrentIndex(1);
        return false;
    }
}

void MainWindow::searchInstalledPackages(){
    // Create a QProcess instance to run the vcpkg list command
    QProcess process;
    process.start("C:/Development/vcpkg/vcpkg.exe", QStringList() << "list");

    // Wait for the process to finish
    process.waitForFinished();

    // Read the output of the process
    QByteArray output = process.readAllStandardOutput();
    QString outputString = QString::fromUtf8(output);

    // Split the output by line
    QStringList lines = outputString.split("\n", Qt::SkipEmptyParts);

    ui->tableWidget->setRowCount(lines.size()); // Set the correct number of rows
    ui->tableWidget->setColumnWidth(1, 100);
    int row = 0;
    // Populate the QTableWidget with package information
    for (const QString& line : lines) {
        QStringList parts = line.split(' ');
        if (parts.size() >= 2) {
            QString packageName = parts[0];

            QTableWidgetItem* nameItem = new QTableWidgetItem(packageName);

            ui->tableWidget->setItem(row, 0, nameItem);
            //ui->tableWidget->setItem(row, 1, versionItem);

            // Create a QPushButton for the "REMOVE" action
            QPushButton* button = new QPushButton("REMOVE");
            button->setStyleSheet("QPushButton { background-color: rgb(41, 98, 255); font: 900 9pt 'Arial Black'; color: rgb(255, 255, 255); border: 1px; border-radius: 10px; border-style: outset;} QPushButton::hover { background-color: rgb(33, 78, 203); font: 900 9pt 'Arial Black'; color: rgb(255, 255, 255); border: 0px; } QPushButton::focus:pressed { background-color: rgb(38, 72, 184); font: 900 9pt 'Arial Black'; color: rgb(255, 255, 255); border: 0px; }");


            // Set a property to identify the button with the package name
            button->setProperty("btnDeleteId", packageName);

            // Connect the button's click signal to a lambda function
            connect(button, &QPushButton::clicked, [this, button]() {
                QString packageName = button->property("btnDeleteId").toString();
                on_btn_remove(packageName);
            });



            // Add the button to the second column of the row
            ui->tableWidget->setCellWidget(row, 1,  button);
        }

        ++row;
    }
}

void MainWindow::on_btn_remove(QString packageName){

    QMessageBox msgBox;
    msgBox.setWindowFlags(Qt::FramelessWindowHint);

    msgBox.setStyleSheet("QMessageBox{background-color: rgba(33, 33, 33, 100);} QMessageBox QLabel {color: white; font: 900 10pt 'Arial Black';}");
    msgBox.setText("¿Are you shure you want to delete " + packageName + "? ");

        // Add custom buttons
        QPushButton *cancelButton = msgBox.addButton(" Cancel ", QMessageBox::ActionRole);
    QPushButton *yesButton = msgBox.addButton(" Yes ", QMessageBox::ActionRole);

    yesButton->setStyleSheet("QPushButton {"
                             "background-color: transparent;"
                             "font: 900 10pt 'Arial Black';"
                             "color: rgb(255, 255, 255);"
                             "border: 1px;"
                             "border-color: rgb(255, 255, 255);"
                             "border-style: outset;"
                             "border-radius: 10px;"
                             "}"

                             "QPushButton::hover{ "
                             "  background-color: rgb(252, 196, 25);"
                             "font: 900 10pt 'Arial Black';"
                             "color: rgb(255, 255, 255);"
                             " border: 0px;"

                             "}"

                             "QPushButton::focus:pressed{ "
                             "background-color: rgb(252, 72, 25);"
                             "font: 900 10pt 'Arial Black';"
                             "color: rgb(255, 255, 255);"
                             "border: 0px;"
                             "}");

    cancelButton->setStyleSheet("QPushButton {"
                                "background-color: transparent;"
                                "font: 900 10pt 'Arial Black';"
                                "color: rgb(255, 255, 255);"
                                "border: 1px;"
                                "border-color: rgb(255, 255, 255);"
                                "border-style: outset;"
                                "border-radius: 10px;"
                                "}"

                                "QPushButton::hover{ "
                                "  background-color: rgb(252, 196, 25);"
                                "font: 900 10pt 'Arial Black';"
                                "color: rgb(255, 255, 255);"
                                " border: 0px;"
                                "}"

                                "QPushButton::focus:pressed{ "
                                "background-color: rgb(252, 72, 25);"
                                "font: 900 10pt 'Arial Black';"
                                "color: rgb(255, 255, 255);"
                                "border: 0px;"
                                "}");

    msgBox.exec();

    if (msgBox.clickedButton() == yesButton)
    {
        QProcess vcpkgProcess;
        vcpkgProcess.start("C:/Development/vcpkg/vcpkg.exe", QStringList() << "remove" << packageName << "--recurse");

        vcpkgProcess.waitForFinished();

        if (vcpkgProcess.exitCode() == 0) {
            qDebug() << "Package removed successfully!";

            // Set the status bar style
            statusBar()->setStyleSheet("color: #ffffff; background-color: #388E3C; font-size: 13px;");

            statusBar()->show();

            // Create a QTimer to hide the message after 3 seconds
            QTimer::singleShot(3000, statusBar(), [this]() {
                statusBar()->clearMessage(); // Clear the message after 3 seconds
                statusBar()->hide(); // Hide the status bar
            });


            // Show the status message
            std::string package = packageName.toStdString(); // Replace this with the actual package name
            QString message = tr("Package ") + QString::fromStdString(package) + tr(" removed successfully!");
            statusBar()->showMessage(message);


            searchInstalledPackages();
        } else {
            qDebug() << "Failed to remove package. Error code:" << vcpkgProcess.exitCode();
            qDebug() << "Error output:" << vcpkgProcess.readAllStandardError();
        }
    }
    else if (msgBox.clickedButton() == cancelButton)
    {
        qDebug() << "NO";
    }
}

void MainWindow::on_btnInstall_clicked()
{
    ui->btnInstall->setVisible(false);
    ui->loading_2->setVisible(true);
    ui->label->setText("This will take a few seconds, please wait.");

    // Connect a slot to the QTimer's timeout signal
    QObject::connect(&installTimer, &QTimer::timeout, [&]() {
        // This code will be executed after the timer expires (3 seconds in this case)
        // Check if the folder exists; if not, create it
        QDir dir("C:/Development");
        if (!dir.exists()) {
            if (dir.mkpath(".")) {
                qDebug() << "Folder created successfully!";
            } else {
                qDebug() << "Failed to create folder.";
            }
        } else {
            qDebug() << "Folder already exists.";
        }

        // Change the current directory to C:/Development/
        QDir::setCurrent("C:/Development");

        // Run the git clone command
        QProcess gitProcess;
        gitProcess.start("git", QStringList() << "clone" << "https://github.com/Microsoft/vcpkg.git");

        if (gitProcess.waitForFinished()) {
            qDebug() << "Git clone completed successfully!";
            runVcpkgbat();
        } else {
            qDebug() << "Failed to run 'git clone'. Error code:" << gitProcess.exitCode();
            qDebug() << "Error output:" << gitProcess.readAllStandardError();
        }

        installTimer.stop();
    });

    // Start the timer with a 3-second interval
    installTimer.start(3000); // 3000 milliseconds = 3 seconds
}

void MainWindow::runVcpkgbat()
{
    // Run the git clone command
    QProcess batProcess;
    batProcess.start("C:/Development/vcpkg/bootstrap-vcpkg.bat", QStringList());

    if (batProcess.waitForFinished()) {
        qDebug() << "Bat run completed successfully!";
        ui->btnInstall->setDisabled(false);
        ui->label->setText("This will take a few seconds, please wait.");
        ui->stackedWidget->setCurrentIndex(0);

        // Set the status bar style
        statusBar()->setStyleSheet("color: #ffffff; background-color: #388E3C; font-size: 13px;");

        statusBar()->show();

        // Create a QTimer to hide the message after 3 seconds
        QTimer::singleShot(4000, statusBar(), [this]() {
            statusBar()->clearMessage(); // Clear the message after 3 seconds
            statusBar()->hide(); // Hide the status bar
        });

        QString message = tr("vcpkg installed successfully!");
        statusBar()->showMessage(message);

        // Gets installed packages
        searchInstalledPackages();
    } else {
        qDebug() << "Failed to run 'bootstrap-vcpkg.bat'. Error code:" << batProcess.exitCode();
        qDebug() << "Error output:" << batProcess.readAllStandardError();
    }
}

void MainWindow::on_btnMin_clicked()
{
    this->showMinimized();
}


void MainWindow::on_btnClose_clicked()
{
    this->close();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    startPos = event->pos();
    QWidget::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint delta = event->pos() - startPos;
    QWidget *w = window();
    if (w)
        w->move(w->pos() + delta);
    QWidget::mouseMoveEvent(event);
}

void MainWindow::on_btnBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_btnAdd_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_btnSearch_clicked()
{
    // Create a QProcess instance to run the vcpkg search command
    QProcess process;
    process.start("C:/Development/vcpkg/vcpkg.exe", QStringList() << "search" << ui->searchText->text());

    // Wait for the process to finish
    process.waitForFinished();

    // Read the output of the process
    QByteArray output = process.readAllStandardOutput();
    QString outputString = QString::fromUtf8(output);

    // Split the output by line
    QStringList lines = outputString.split("\n", Qt::SkipEmptyParts);

    ui->searchTable->setRowCount(lines.size() - 2); // Set the correct number of rows
    ui->searchTable->setColumnWidth(1, 100);

    int row = 0;
    // Populate the QTableWidget with package information
    for (const QString& line : lines) {
        QStringList parts = line.split(' ');
        if (parts.size() >= 2) {
            QString packageName = parts[0];

            QTableWidgetItem* nameItem = new QTableWidgetItem(packageName);

            ui->searchTable->setItem(row, 0, nameItem);

            // Create a QPushButton for the "REMOVE" action
            QPushButton* button = new QPushButton("ADD");
            button->setStyleSheet("QPushButton { background-color: rgb(41, 98, 255); font: 900 9pt 'Arial Black'; color: rgb(255, 255, 255); border: 1px; border-radius: 10px; border-style: outset; } QPushButton::hover { background-color: rgb(33, 78, 203); font: 900 9pt 'Arial Black'; color: rgb(255, 255, 255); border: 0px; } QPushButton::focus:pressed { background-color: rgb(38, 72, 184); font: 900 9pt 'Arial Black'; color: rgb(255, 255, 255); border: 0px; }");


            // Set a property to identify the button with the package name
            button->setProperty("btnSearchId", packageName);

            // Connect the button's click signal to a lambda function
            connect(button, &QPushButton::clicked, [this, button]() {
                QString packageName = button->property("btnSearchId").toString();
                on_btnAdd(packageName);
            });

            // Add the button to the second column of the row
            ui->searchTable->setCellWidget(row, 1,  button);

        }


        ++row;
    }
}

/*void MainWindow::on_btnAdd(QString packageName){

    // Run the vcpkg install command
    QProcess addProcess;
    addProcess.start("C:/Development/vcpkgtest/vcpkg/vcpkg.exe", QStringList() << "install" << packageName);

    if (addProcess.waitForFinished()) {
        qDebug() << "vcpkg install completed successfully!";
        runVcpkgbat();
    } else {
        qDebug() << "Failed to run 'vcpkg install'. Error code:" << addProcess.exitCode();
        qDebug() << "Error output:" << addProcess.readAllStandardError();
    }
}*/

void MainWindow::on_btnAdd(QString packageName) {
    ui->installingLabel->setText("Installing " + packageName +" please wait.");
    ui->stackedWidget->setCurrentIndex(3);

    // Run the vcpkg install command
    addProcess.start("C:/Development/vcpkg/vcpkg.exe", QStringList() << "install" << packageName);

    // Connect the finished signal to a slot in MainWindow
    connect(&addProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onProcessAddFinished(int, QProcess::ExitStatus)));

    // Optionally, you can also connect the errorOccurred signal to handle errors
    connect(&addProcess, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(onProcessError(QProcess::ProcessError)));

}

void MainWindow::onProcessAddFinished(int exitCode, QProcess::ExitStatus exitStatus) {

    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        qDebug() << "vcpkg install completed successfully!";

        // Gets installed packages
        searchInstalledPackages();

        ui->stackedWidget->setCurrentIndex(0);

        // Set the status bar style
        statusBar()->setStyleSheet("color: #ffffff; background-color: #388E3C; font-size: 13px;");

        statusBar()->show();

        // Create a QTimer to hide the message after 3 seconds
        QTimer::singleShot(3000, statusBar(), [this]() {
            statusBar()->clearMessage(); // Clear the message after 3 seconds
            statusBar()->hide(); // Hide the status bar
        });


        QString message = tr("Package added successfully!");
        statusBar()->showMessage(message);

    } else {
        qDebug() << "Failed to run 'vcpkg install'. Error code:" << exitCode;
        qDebug() << "Error output:" << addProcess.readAllStandardError();
    }

    // Disconnect the finished signal when done
    disconnect(&addProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onProcessFinished(int, QProcess::ExitStatus)));
}

// Slot to handle process finished
void MainWindow::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {

    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        qDebug() << "vcpkg install completed successfully!";

        runVcpkgbat();
    } else {
        qDebug() << "Failed to run 'vcpkg install'. Error code:" << exitCode;
        qDebug() << "Error output:" << addProcess.readAllStandardError();
    }

    // Disconnect the finished signal when done
    disconnect(&addProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onProcessFinished(int, QProcess::ExitStatus)));
}

// Slot to handle process error (optional)
void MainWindow::onProcessError(QProcess::ProcessError error) {
    qDebug() << "Error occurred while running 'vcpkg install'. Error code:" << error;
}
