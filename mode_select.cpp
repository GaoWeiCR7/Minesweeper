#include "mode_select.h"
#include "ui_mode_select.h"

mode_select::mode_select(bool lang,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mode_select)
{
    ui->setupUi(this);
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
    pattern = 0;
    if(false == lang)
    {
        this->setWindowTitle("mode");
        ui->easy->setText("easy");
        ui->normal->setText("normal");
        ui->hard->setText("hard");
        ui->userdefined->setText("user-defined");
    }
    else
    {
        this->setWindowTitle("模式");
        ui->easy->setText("简单");
        ui->normal->setText("普通");
        ui->hard->setText("困难");
        ui->userdefined->setText("自定义");
    }
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
    ui->easy->setStyleSheet(qss0);
    ui->normal->setStyleSheet(qss0);
    ui->hard->setStyleSheet(qss0);
    ui->userdefined->setStyleSheet(qss0);
    this->setFixedSize(this->size());
}

void mode_select::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(rect(),QPixmap(":/new/prefix1/image/mainpic.png"));
}

mode_select::~mode_select()
{
    delete ui;
}

void mode_select::on_easy_clicked()
{
    pattern = 0;
    accept();
}

void mode_select::on_normal_clicked()
{
    pattern = 1;
    accept();
}

void mode_select::on_hard_clicked()
{
    pattern = 2;
    accept();
}

void mode_select::on_userdefined_clicked()
{
    pattern = 3;
    accept();
}

int mode_select::getpat()
{
    return pattern;
}
