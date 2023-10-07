#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qprocess.h"
#include "qtimer.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QPoint startPos;
    QProcess addProcess;
    QTimer installTimer;

private slots:
    bool checkIfVcpkgaIsInstalled();
    void searchInstalledPackages();
    void on_btn_remove(QString packageName);
    void on_btnAdd(QString packageName);
    void on_btnInstall_clicked();
    void on_btnMin_clicked();
    void on_btnClose_clicked();
    void runVcpkgbat();
    void on_btnBack_clicked();

    void on_btnAdd_clicked();

    void on_btnSearch_clicked();
    void onProcessAddFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

};
#endif // MAINWINDOW_H
