#include "gameplay.h"
#include "ui_gameplay.h"

gameplay::gameplay(bool lang, int pat,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gameplay)
{
    ui->setupUi(this);
    language = lang;
    pattern = pat;
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
    this->setWindowTitle("Minesweeper");
    menubar = new QMenuBar(this);
    QMenu* menu1 = new QMenu("Option",this);

    QAction* mode_select = new QAction("mode",this);
    QAction* music_volume = new QAction("volumn",this);
    QAction* tips = new QAction("tips",this);

    menu1->addAction(mode_select);
    menu1->addAction(music_volume);
    menu1->addAction(tips);
    menubar->addMenu(menu1);

    dia_init();
}

gameplay::~gameplay()
{
    delete ui;
}

void gameplay::dia_init()
{
    if(pattern == 0)
    {
        height = 9;
        width = 9;
        minenum = 10;
    }
    else if(pattern == 1)
    {
        height = 16;
        width = 16;
        minenum = 40;
    }
    else if (pattern == 2)
    {
        height = 30;
        width = 16;
        minenum = 99;
    }
    else
        user_defined_dialog_set();
    this->setGeometry(350,200,height*CELL_SIZE,width*CELL_SIZE);
    allcell.resize(height*width);


}

void gameplay::user_defined_dialog_set()
{
    user_def = new user_defined_input(language,this);
    if(user_def->exec() == QDialog::Accepted)
    {
        height = user_def->get_height();
        width = user_def->get_width();
        minenum = user_def->get_minenum();
    }
}

void gameplay::minerandomset()
{
    qsrand(time(NULL));
    int cur_mine_num = 0;
    int mod = height*width;
    while(cur_mine_num < minenum)
    {
        int k = qrand()%mod;
        if(allcell[k].get_kind()!=1)//雷格不能多次产生雷
        {
            allcell[k].set_kind(1);
            cur_mine_num++;
        }
    }
}

void gameplay::num_filled()
{
    for(int i = 0; i < allcell.size();++i)
    {
        if(allcell[i].get_kind()==1)//以雷为中心将所有相邻的格子的num加1
        {
            int row = i/width;
            int column = i%width;
            if(row > 0) allcell[width*(row-1)+column].numadd();
            if(row < height-1) allcell[width*(row+1)+column].numadd();
            if(column > 0)allcell[width*row+column-1].numadd();
            if(column < width-1) allcell[width*row+column+1].numadd();
            if(row>0&&column>0) allcell[width*(row-1)+column-1].numadd();
            if(row>0&&column<width-1) allcell[width*(row-1)+column+1].numadd();
            if(row<height-1&&column>0) allcell[width*(row+1)+column-1].numadd();
            if(row<height-1&&column<width-1) allcell[width*(row+1)+column+1].numadd();
        }
    }
}

void gameplay::paintEvent(QPaintEvent *)
{

}
