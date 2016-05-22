#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QMenuBar>
#include <QTimer>
#include <QMessageBox>
#include <math.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentAction(NoAct),
    isEmpty(true)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);

    bar1 = new QLabel("版本号1.0    ");
    bar2 = new QLabel("    作者：赵磊");
    statusBar()->addWidget(bar1);
    statusBar()->addWidget(bar2,1);
    ui->labelDisplay->setText("别不服，这计算器很牛");

    connect(ui->btnClear,       SIGNAL(clicked(bool)),this, SLOT(Clear()));
    connect(ui->btnNegativity,  SIGNAL(clicked(bool)),this, SLOT(negationClicked()));
    connect(ui->btnPercentage,  SIGNAL(clicked(bool)),this, SLOT(percentClicked()));
    connect(ui->btnDecimalPoint,SIGNAL(clicked(bool)),this, SLOT(decimalPointCliecked()));
    connect(ui->btnEquit,       SIGNAL(clicked(bool)),this, SLOT(evaluationCliecked()));

    connect(ui->btnAdd, SIGNAL(clicked(bool)),  this,   SLOT(actionChanged()));
    connect(ui->btnSub, SIGNAL(clicked(bool)),  this,   SLOT(actionChanged()));
    connect(ui->btnMult,SIGNAL(clicked(bool)),  this,   SLOT(actionChanged()));
    connect(ui->btnDiv, SIGNAL(clicked(bool)),  this,   SLOT(actionChanged()));

    connect(ui->btnNum0,SIGNAL(clicked(bool)),  this,   SLOT(inputNumber()));
    connect(ui->btnNum1,SIGNAL(clicked(bool)),  this,   SLOT(inputNumber()));
    connect(ui->btnNum2,SIGNAL(clicked(bool)),  this,   SLOT(inputNumber()));
    connect(ui->btnNum3,SIGNAL(clicked(bool)),  this,   SLOT(inputNumber()));
    connect(ui->btnNum4,SIGNAL(clicked(bool)),  this,   SLOT(inputNumber()));
    connect(ui->btnNum5,SIGNAL(clicked(bool)),  this,   SLOT(inputNumber()));
    connect(ui->btnNum6,SIGNAL(clicked(bool)),  this,   SLOT(inputNumber()));
    connect(ui->btnNum7,SIGNAL(clicked(bool)),  this,   SLOT(inputNumber()));
    connect(ui->btnNum8,SIGNAL(clicked(bool)),  this,   SLOT(inputNumber()));
    connect(ui->btnNum9,SIGNAL(clicked(bool)),  this,   SLOT(inputNumber()));


    connect(ui->actionAbout_QT,     SIGNAL(triggered(bool)),qApp,SLOT(aboutQt()));
    connect(ui->actionAbout_Calc,   SIGNAL(triggered(bool)),this,SLOT(aboutApp()));
    connect(ui->actionAbout_Jolly,  SIGNAL(triggered(bool)),this,SLOT(aboutJolly()));

    connect(ui->actUnitSelect,      SIGNAL(triggered(bool)),this,SLOT(unitSelect()));
}


void MainWindow::Clear()
{
    if(ui->btnClear->text() == "AC"){
        currentAction = NoAct;
        currentNumber = "0";
        lastNumber = "0";
        isEmpty = true;
    }
    else if(ui->btnClear->text() == "C"){
        currentNumber = "0";
        isEmpty = true;
        ui->btnClear->setText("AC");
    }
    displayNumber();
}

void MainWindow::actionChanged()
{
    QPushButton *currentButton = qobject_cast<QPushButton*>(this->sender());
    if(currentButton){
        isEmpty = false;
        if(currentButton == ui->btnAdd){
            currentAction = AddFlag;
            statusBar()->showMessage("加",3000);
        }
        else if(currentButton == ui->btnSub){
            currentAction = SubFlag;
            statusBar()->showMessage("减",3000);
        }
        else if(currentButton == ui->btnMult){
            currentAction = MultFlag;
            statusBar()->showMessage("乘",3000);
        }
        else if(currentButton == ui->btnDiv){
            currentAction = DivFlag;
            statusBar()->showMessage("除",3000);
        }
        else{
            currentAction = NoAct;
        }
    }
    displayNumber();
}

void MainWindow::inputNumber()
{
    QPushButton *currentButton = qobject_cast<QPushButton*>(this->sender());

    if(currentButton)
    {
        ui->btnClear->setText("C");
        if(!isEmpty)
        {
            lastNumber = currentNumber;
            currentNumber.clear();
            isEmpty = true;
        }
        if(currentNumber == "0")
        {
            currentNumber.clear();
        }


        if(currentButton == ui->btnNum0)
        {
            currentNumber.append("0");
            statusBar()->showMessage("输入0",2000);
        }
        if(currentButton == ui->btnNum1)
        {
            currentNumber.append("1");
            statusBar()->showMessage("输入1",2000);
        }
        if(currentButton == ui->btnNum2)
        {
            currentNumber.append("2");
            statusBar()->showMessage("输入2",2000);
        }
        if(currentButton == ui->btnNum3)
        {
            currentNumber.append("3");
            statusBar()->showMessage("输入3",2000);
        }
        if(currentButton == ui->btnNum4)
        {
            currentNumber.append("4");
            statusBar()->showMessage("输入4",2000);
        }
        if(currentButton == ui->btnNum5)
        {
            currentNumber.append("5");
            statusBar()->showMessage("输入5",2000);
        }
        if(currentButton == ui->btnNum6)
        {
            currentNumber.append("6");
            statusBar()->showMessage("输入6",2000);
        }
        if(currentButton == ui->btnNum7)
        {
            currentNumber.append("7");
            statusBar()->showMessage("输入7",2000);
        }
        if(currentButton == ui->btnNum8)
        {
            currentNumber.append("8");
            statusBar()->showMessage("输入8",2000);
        }
        if(currentButton == ui->btnNum9)
        {
            currentNumber.append("9");
            statusBar()->showMessage("输入9",2000);
        }
    }
    displayNumber();
}

void MainWindow::displayNumber()
{
    if(currentNumber == "250"){
        ui->labelDisplay->setText("250,就是你");
        return;
    }
    ui->labelDisplay->setText(currentNumber);
}

void MainWindow::negationClicked()
{
    if(!isEmpty  ||  (currentNumber.size() == 0)){
        currentNumber = "0";
        isEmpty = true;
    }
    else{
        if(currentNumber.at(0) == '-')
            currentNumber.remove(0,1);
        else
            currentNumber.push_front("-");
    }
    displayNumber();
    statusBar()->showMessage("符号转换",2000);
}

void MainWindow::percentClicked()
{
    if(!isEmpty){
        currentNumber = "0";
        isEmpty = true;
    }
    else{
        double dResult = currentNumber.toDouble() / 100;
        currentNumber = QString::number(dResult);
    }
    displayNumber();
    statusBar()->showMessage("百分比计算",2000);
}

void MainWindow::decimalPointCliecked()
{
    if(!isEmpty){
        currentNumber = "0.";
        isEmpty = true;
    }
    else if(currentNumber.indexOf(".") == -1){
        currentNumber.push_back(".");
    }
    displayNumber();
    statusBar()->showMessage(".",2000);
}

void MainWindow::evaluationCliecked()
{
    switch(currentAction)
    {
    case NoAct:
    {
        statusBar()->showMessage("请选择一个运算符",2000);
        return ;
        break;
    }
    case AddFlag:
    {
        currentNumber = QString::number(lastNumber.toDouble()+currentNumber.toDouble());
        break;
    }
    case SubFlag:
    {
        currentNumber = QString::number(lastNumber.toDouble()-currentNumber.toDouble());
        break;
    }
    case MultFlag:
    {
        currentNumber = QString::number(lastNumber.toDouble()*currentNumber.toDouble());
        break;
    }
    case DivFlag:
    {
        currentNumber = QString::number(lastNumber.toDouble()/currentNumber.toDouble());
        break;
    }
    default:
        break;
    }
    currentAction = NoAct;
    displayNumber();
    statusBar()->showMessage("结果",2000);
}

void MainWindow::aboutApp()
{
    QMessageBox::about(this,"关于计算器","开发者：赵磊\n技术支持请联系微信：903221074");
}

void MainWindow::aboutJolly()
{
    QMessageBox::about(this,"关于赵磊","开发者、学生，金融爱好者\n微信：903221074");
}

void MainWindow::unitSelect()
{
    selectDialog = new QDialog(this);
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QGroupBox *groupBox_2;
    QLabel *displayLabel;

    selectDialog->resize(400,150);
    buttonBox = new QDialogButtonBox(selectDialog);
    buttonBox->setGeometry(30,100,341,32);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

    groupBox = new QGroupBox(selectDialog);
    groupBox_2 = new QGroupBox(selectDialog);
    groupBox->setGeometry(10,10,180,80);
    groupBox_2->setGeometry(210,10,180,80);
    groupBox->setTitle("要转换的数值");
    groupBox_2->setTitle("转换后的单位");

    displayLabel = new QLabel(ui->labelDisplay->text(), groupBox);

    selectCombo = new QComboBox(groupBox_2);
    selectCombo->setGeometry(10,40,170,26);
    selectCombo->clear();
    selectCombo->insertItem(0,"米->厘米");
    selectCombo->insertItem(0,"米->平方");

    connect(buttonBox,  SIGNAL(rejected()),selectDialog,SLOT(deleteLater()));
    connect(buttonBox,  SIGNAL(accepted()),this,SLOT(unitCalc()));

    selectDialog->exec();
}

void MainWindow::unitCalc()
{
    switch (selectCombo->currentIndex()) {
    case 1:
        currentNumber = QString::number(currentNumber.toDouble() * 100);
        break;
    case 0:
        currentNumber = QString::number(pow(currentNumber.toDouble(),2));
        break;
    default:
        break;
    }
    selectDialog->deleteLater();
    displayNumber();
}

MainWindow::~MainWindow()
{
    delete ui;
}
