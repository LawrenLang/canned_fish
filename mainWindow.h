#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <bits/stdc++.h>
#include <qfiledialog.h>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void openfile();

public slots:
    void on_inputButton_clicked();
    void on_outputButton_clicked();
    void on_turnButton_clicked();
    void on_inputClear_clicked();
    void on_outputClear_clicked();
    void on_help_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
