#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("language");
    QString qss0="QPushButton {\
    background-color: #CCCC99;\
    border-style: outset;\
    border-width: 2px;\
    border-radius: 5px;\
    border-color: #8B7355;\
    font: bold 14px;\
    min-width:2em;\
    color:black;\
    padding: 5px;\
    }\
    QPushButton:hover{\
    background-color: #FFCCCC;\
    }\
    QPushButton:pressed {\
    background-color: #99CCCC;\
    border-style: inset;\
    }\
    QPushButton:!enabled{\
    background-color: rgb(100, 100, 100);\
    border-style: inset;\
    }";
    ui->pushButton->setStyleSheet(qss0);
    ui->pushButton_2->setStyleSheet(qss0);
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
    this->setFixedSize(this->size());
}

void MainWindow::paintEvent(QPaintEvent* )
{
    QPainter p(this);
    p.drawPixmap(rect(),QPixmap(":/new/prefix1/image/mainpic.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    this->hide();
    gamepl = new gameplay(false,this);
    gamepl->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    this->hide();
    gamepl = new gameplay(true,this);
    gamepl->show();
}
