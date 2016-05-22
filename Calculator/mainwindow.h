#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include <QDialog>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QGroupBox>

namespace Ui {
class MainWindow;
}

enum ActFlag{
    NoAct = 0x9ff,
    AddFlag,
    SubFlag,
    MultFlag,
    DivFlag
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    void showMessage(const char *message,int timeOut);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QDialog *selectDialog;
    QComboBox *selectCombo;
    QLabel *bar1;
    QLabel *bar2;

    ActFlag currentAction;
    QString lastNumber;
    QString currentNumber;
    bool isEmpty;

public slots:
    void Clear();
    void actionChanged();
    void inputNumber();
    void displayNumber();
    void negationClicked(); //正负转换按钮
    void percentClicked();
    void decimalPointCliecked();
    void evaluationCliecked();
    void unitCalc();

    void unitSelect();
    void aboutApp();
    void aboutJolly();
};

#endif // MAINWINDOW_H
