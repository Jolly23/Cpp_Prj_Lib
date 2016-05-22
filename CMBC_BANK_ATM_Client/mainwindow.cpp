#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "systime.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    SysTime nowtime;
    ui->setupUi(this);
    ui->DisplayLabel->setText(nowtime.GetTime());
    connect(ui->ATM_Button,SIGNAL(clicked(bool)),this,SLOT(Mess()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Mess()
{
    qDebug("ATM");
}
