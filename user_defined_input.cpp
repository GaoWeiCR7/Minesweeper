#include "user_defined_input.h"
#include "ui_user_defined_input.h"

user_defined_input::user_defined_input(bool lang,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::user_defined_input)
{
    ui->setupUi(this);
    QIntValidator* aIntValidator = new QIntValidator;
    aIntValidator->setRange(1,99);
    ui->inheight->setValidator(aIntValidator);
    ui->inwidth->setValidator(aIntValidator);
    ui->inminenum->setValidator(aIntValidator);
    if(false == lang)
    {
        ui->height->setText("height");
        ui->width->setText("width");
        ui->minenum->setText("minenum");
        ui->ok->setText("OK");
        ui->cancel->setText("cancel");
    }
    else
    {
        ui->height->setText("高度");
        ui->width->setText("宽度");
        ui->minenum->setText("雷数");
        ui->ok->setText("确定");
        ui->cancel->setText("取消");
    }
    height = 9;
    width = 9;
    minenum = 10;
}

user_defined_input::~user_defined_input()
{
    delete ui;
}

void user_defined_input::on_ok_clicked()
{
    height = ui->inheight->text().toInt()<50?ui->inheight->text().toInt():50;
    width = ui->inwidth->text().toInt()<50?ui->inwidth->text().toInt():50;
    minenum = ui->inminenum->text().toInt();
    accept();
}

int user_defined_input::get_height()
{
    return height;
}

int user_defined_input::get_width()
{
    return width;
}

int user_defined_input::get_minenum()
{
    return minenum;
}
