#include "mode_select.h"
#include "ui_mode_select.h"

mode_select::mode_select(bool lang,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mode_select)
{
    ui->setupUi(this);
    language = lang;
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
    if(false == language)
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
}

mode_select::~mode_select()
{
    delete ui;
}

void mode_select::on_easy_clicked()
{
    gamepl = new gameplay(language,0,this);
    this->close();
    gamepl->show();
}

void mode_select::on_normal_clicked()
{
    gamepl = new gameplay(language,1,this);
    this->close();
    gamepl->show();
}

void mode_select::on_hard_clicked()
{
    gamepl = new gameplay(language,2,this);
    this->close();
    gamepl->show();
}

void mode_select::on_userdefined_clicked()
{
    gamepl = new gameplay(language,3,this);
    this->close();
    gamepl->show();
}
