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
        height = 16;
        width = 30;
        minenum = 99;
    }
    else
        user_defined_dialog_set();
    this->setGeometry(350,200,width*CELL_SIZE,height*CELL_SIZE+INTERVAL+TOPHEIGHT);
    this->setFixedSize(this->size());
    allcell.resize(height*width);
    minerandomset();
    num_filled();
    mytime = 0;
    timer = new QTimer(this);
    connect(this->timer,&QTimer::timeout,this,&gameplay::timeadd);
    timer->start(1000);
    mine_left_num = minenum;
    start = false;
    end = 0;
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
    QPixmap image_initcell(":/new/prefix1/image/unclip.png");
    /*QPixmap image0(":/new/prefix1/image/0.png");
    QPixmap image1(":/new/prefix1/image/1.png");
    QPixmap image2(":/new/prefix1/image/2.png");
    QPixmap image3(":/new/prefix1/image/3.png");
    QPixmap image4(":/new/prefix1/image/4.png");
    QPixmap image5(":/new/prefix1/image/5.png");
    QPixmap image6(":/new/prefix1/image/6.png");
    QPixmap image7(":/new/prefix1/image/7.png");
    QPixmap image8(":/new/prefix1/image/8.png");*/
    QPixmap image_good(":/new/prefix1/image/good.png");
    QPixmap image_bad(":/new/prefix1/image/bad.png");
    QPixmap image_great(":/new/prefix1/image/great.png");
    QPixmap image_bomb(":/new/prefix1/image/bomb.png");
    QPixmap image_flag(":/new/prefix1/image/red flag.png");
    QPixmap image_question(":/new/prefix1/image/question mark.png");
    QPainter painter(this);

    int firsttime = mytime/100;
    int secondtime = (mytime%100)/10;
    int thirdtime = mytime%10;
    QString firsttimestr = QString::number(firsttime);
    QString secondtimestr = QString::number(secondtime);
    QString thirdtimestr = QString::number(thirdtime);
    QString firsttimestrimage = ":/new/prefix1/image/t"+firsttimestr+".png";
    QString secondtimestrimage = ":/new/prefix1/image/t"+secondtimestr+".png";
    QString thirdtimestrimage = ":/new/prefix1/image/t"+thirdtimestr+".png";
    QPixmap firsttimepix(firsttimestrimage);
    QPixmap secondtimepix(secondtimestrimage);
    QPixmap thirdtimepix(thirdtimestrimage);
    painter.drawPixmap((width/2-4)*CELL_SIZE/2,INTERVAL+2,CELL_SIZE,TOPHEIGHT-4,firsttimepix);
    painter.drawPixmap((width/2-2)*CELL_SIZE/2,INTERVAL+2,CELL_SIZE,TOPHEIGHT-4,secondtimepix);
    painter.drawPixmap((width/2)*CELL_SIZE/2,INTERVAL+2,CELL_SIZE,TOPHEIGHT-4,thirdtimepix);

    int firstminenum = mine_left_num/100;
    int secondminenum = (mine_left_num%100)/10;
    int thirdminenum = mine_left_num%10;
    QString firstminenumstr = QString::number(firstminenum);
    QString secondminenumstr = QString::number(secondminenum);
    QString thirdminenumstr = QString::number(thirdminenum);
    QString firstminenumstrimage = ":/new/prefix1/image/t"+firstminenumstr+".png";
    QString secondminenumstrimage = ":/new/prefix1/image/t"+secondminenumstr+".png";
    QString thirdminenumstrimage = ":/new/prefix1/image/t"+thirdminenumstr+".png";
    QPixmap firstminenumpix(firstminenumstrimage);
    QPixmap secondminenumpix(secondminenumstrimage);
    QPixmap thirdminenumpix(thirdminenumstrimage);
    painter.drawPixmap((width*3/2-2)*CELL_SIZE/2,INTERVAL+2,CELL_SIZE,TOPHEIGHT-4,firstminenumpix);
    painter.drawPixmap((width*3/2)*CELL_SIZE/2,INTERVAL+2,CELL_SIZE,TOPHEIGHT-4,secondminenumpix);
    painter.drawPixmap((width*3/2+2)*CELL_SIZE/2,INTERVAL+2,CELL_SIZE,TOPHEIGHT-4,thirdminenumpix);

    for(int i = 0; i < allcell.size();++i)
    {
        int row = i/width;
        int column = i%width;
        if(allcell[i].get_state() == 0)
            painter.drawPixmap(column*CELL_SIZE,row*CELL_SIZE+INTERVAL+TOPHEIGHT,CELL_SIZE,CELL_SIZE,image_initcell);
        else if(allcell[i].get_state() == 1)
        {
            int kind = allcell[i].get_kind();
            if(kind == 0)
            {
                int num = allcell[i].get_num();
                QString numstr = QString::number(num);
                QString numstrimage = ":/new/prefix1/image/"+numstr+".png";
                QPixmap numpix(numstrimage);
                painter.drawPixmap(column*CELL_SIZE,row*CELL_SIZE+INTERVAL+TOPHEIGHT,CELL_SIZE,CELL_SIZE,numpix);
            }
            else
            {
                painter.drawPixmap(column*CELL_SIZE,row*CELL_SIZE+INTERVAL+TOPHEIGHT,CELL_SIZE,CELL_SIZE,image_bomb);
            }
        }
        else if(allcell[i].get_state() == 2)
        {
            painter.drawPixmap(column*CELL_SIZE,row*CELL_SIZE+INTERVAL+TOPHEIGHT,CELL_SIZE,CELL_SIZE,image_flag);
        }
        else if(allcell[i].get_state() == 3)
        {
            painter.drawPixmap(column*CELL_SIZE,row*CELL_SIZE+INTERVAL+TOPHEIGHT,CELL_SIZE,CELL_SIZE,image_question);
        }
    }
    if(end == 0)
        painter.drawPixmap((width-1)*CELL_SIZE/2,INTERVAL+TOPHEIGHT/5,CELL_SIZE,CELL_SIZE,image_good);
    else if(end == 1)
        painter.drawPixmap((width-1)*CELL_SIZE/2,INTERVAL+TOPHEIGHT/5,CELL_SIZE,CELL_SIZE,image_great);
    else if(end == 2)
    {
        for(int i = 0; i < allcell.size(); ++i)
        {
            int row = i/width;
            int column = i%width;
            if(allcell[i].get_kind() == 1)
                painter.drawPixmap(column*CELL_SIZE,row*CELL_SIZE+INTERVAL+TOPHEIGHT,CELL_SIZE,CELL_SIZE,image_bomb);
        }
    }
}

void gameplay::mousePressEvent(QMouseEvent *)
{

}

void gameplay::timeadd()
{
    mytime++;
    update();
}
