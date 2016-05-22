#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QThread>
#include <QtNetwork>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->hide();
    ui->progressBar->setValue(0);
    ui->stuid->setText("2014131126");
    ui->isp_pwd->setText("Chanel-5");
    ui->urp_pwd->setText("123456");
    ui->test_time_slider->setRange(100, 3000);
    ui->test_time_spinBox->setRange(100, 3000);
    ui->process_slider->setRange(1, 20);
    ui->process_spinBox->setRange(1, 20);
    QObject::connect(ui->process_spinBox, SIGNAL(valueChanged(int)), ui->process_slider, SLOT(setValue(int)));
    QObject::connect(ui->process_slider, SIGNAL(valueChanged(int)), ui->process_spinBox, SLOT(setValue(int)));
    QObject::connect(ui->test_time_spinBox, SIGNAL(valueChanged(int)), ui->test_time_slider, SLOT(setValue(int)));
    QObject::connect(ui->test_time_slider, SIGNAL(valueChanged(int)), ui->test_time_spinBox, SLOT(setValue(int)));

    connect(ui->btnStartTest, SIGNAL(clicked(bool)),this, SLOT(StartTestClick()));
    connect(ui->do_button, SIGNAL(clicked(bool)),this, SLOT(IspInfo_API()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::StartTestClick()
{
    int sum_test_times = ui->test_time_spinBox->value();
    int test_times = sum_test_times;
    int test_process = ui->process_spinBox->value();
    ui->progressBar->show();
    //QThread *proc = new QThread;
    int progress;

    QString STANDARD = IspInfo_API();
    QString ret;
    int err_times = 0;
    while(test_times >= 0){
       // ui->do_button->clicked();
        ret = IspInfo_API();
        ui->textBrowser->setText(ret);
        if(ret != STANDARD)
            ++err_times;
        progress = (double)(sum_test_times - test_times) / sum_test_times *100;
        qDebug() << progress;
        ui->progressBar->setValue(progress);
        --test_times;
    }
    QString showResult = "错误" + QString(err_times);
    qDebug() << err_times;
    ui->textBrowser->setText(showResult);
}

QString MainWindow::IspInfo_API()
{
    QString stuid = ui->stuid->text();
    QString passwd= ui->isp_pwd->text();
    QString API_URL = "http://api.dlnu.software/api/urp/usr_binfo?uid=" + stuid + "&passwd=" + passwd;
    QUrl url(API_URL);
    QNetworkAccessManager manager;
    QEventLoop loop;
    qDebug() << "Reading code form " << url;
    //发出请求
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    //请求结束并下载完成后，退出子事件循环
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //开启子事件循环
    loop.exec();

    return reply->readAll();
}


