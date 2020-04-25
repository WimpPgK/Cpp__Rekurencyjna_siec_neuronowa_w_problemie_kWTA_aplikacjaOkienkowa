#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QPrinter>
#include <QPrintDialog>
#include <QMessageBox>
#include "rnn.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool flagLoadFromFile = false;
    int* tab;

private slots:
    void on_actionSave_as_triggered();
    void on_actionLoad_triggered();
    void on_start_QPushButton_clicked();
    void on_AutoEpsilonCheckBox_stateChanged(int arg1);
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QString currentFile = "";

};
#endif // MAINWINDOW_H
