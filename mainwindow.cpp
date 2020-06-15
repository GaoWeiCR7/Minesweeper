#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("language");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    mode_sel = new mode_select(false,this);
    this->close();
    mode_sel->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    mode_sel = new mode_select(true,this);
    this->close();
    mode_sel->show();
}
