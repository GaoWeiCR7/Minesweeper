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
    cell_init();
    minerandomset();
    num_filled();
    mytime = 0;
    timer = new QTimer(this);
    connect(this->timer,&QTimer::timeout,this,&gameplay::timeadd);
    mine_left_num = minenum;
    gamestart = false;
    gameend = 0;
    leftpress = false;
    rightpress = false;
    doublepress = false;
    moveout = false;
    startnum = -1;
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

void gameplay::cell_init()
{
    for(int i = 0; i < allcell.size(); ++i)
    {
        allcell[i].change_state(0);
        allcell[i].set_kind(0);
        allcell[i].set_num(0);
        allcell[i].setpress(false);
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
    QPixmap image_good(":/new/prefix1/image/good.png");
    QPixmap image_bad(":/new/prefix1/image/bad.png");
    QPixmap image_great(":/new/prefix1/image/great.png");
    QPixmap image_bomb(":/new/prefix1/image/bomb.png");
    QPixmap image_flag(":/new/prefix1/image/red flag.png");
    QPixmap image_question(":/new/prefix1/image/question mark.png");
    QPixmap image_errorbomb(":/new/prefix1/image/errorbomb.png");
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
    painter.drawPixmap(width*CELL_SIZE*3/4-CELL_SIZE,INTERVAL+2,CELL_SIZE,TOPHEIGHT-4,firstminenumpix);
    painter.drawPixmap(width*CELL_SIZE*3/4,INTERVAL+2,CELL_SIZE,TOPHEIGHT-4,secondminenumpix);
    painter.drawPixmap(width*CELL_SIZE*3/4+CELL_SIZE,INTERVAL+2,CELL_SIZE,TOPHEIGHT-4,thirdminenumpix);

    for(int i = 0; i < allcell.size();++i)
    {
        int row = i/width;
        int column = i%width;
        if(allcell[i].get_state() == 0)
        {
            if(allcell[i].getpress() == false)
                painter.drawPixmap(column*CELL_SIZE,row*CELL_SIZE+INTERVAL+TOPHEIGHT,CELL_SIZE,CELL_SIZE,image_initcell);
            else
                painter.drawPixmap(column*CELL_SIZE+BIAS,row*CELL_SIZE+INTERVAL+TOPHEIGHT+BIAS,CELL_SIZE,CELL_SIZE,image_initcell);
        }
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
    if(gameend == 0)
        painter.drawPixmap((width-1)*CELL_SIZE/2,INTERVAL+TOPHEIGHT/5,CELL_SIZE,CELL_SIZE,image_good);
    else if(gameend == 1)
        painter.drawPixmap((width-1)*CELL_SIZE/2,INTERVAL+TOPHEIGHT/5,CELL_SIZE,CELL_SIZE,image_great);
    else if(gameend == 2)
    {
        for(int i = 0; i < allcell.size(); ++i)
        {
            int row = i/width;
            int column = i%width;
            if(allcell[i].get_state() == 2&&allcell[i].get_kind()!=1)
                painter.drawPixmap(column*CELL_SIZE,row*CELL_SIZE+INTERVAL+TOPHEIGHT,CELL_SIZE,CELL_SIZE,image_errorbomb);
            if(allcell[i].get_state() == 3&&allcell[i].get_kind()!=1)
                painter.drawPixmap(column*CELL_SIZE,row*CELL_SIZE+INTERVAL+TOPHEIGHT,CELL_SIZE,CELL_SIZE,image_initcell);
            if(allcell[i].get_kind() == 1)
                painter.drawPixmap(column*CELL_SIZE,row*CELL_SIZE+INTERVAL+TOPHEIGHT,CELL_SIZE,CELL_SIZE,image_bomb);
        }
        painter.drawPixmap((width-1)*CELL_SIZE/2,INTERVAL+TOPHEIGHT/5,CELL_SIZE,CELL_SIZE,image_bad);
    }
}

void gameplay::mousePressEvent(QMouseEvent *event)
{
    int startx = event->x();
    int starty = event->y();
    startnum = cal_num(startx,starty);
    if(event->buttons() == Qt::LeftButton)
    {
        if(startnum >= 0)
            allcell[startnum].setpress(true);
        leftpress = true;
        rightpress = false;
        doublepress = false;
    }
    else if(event->buttons() == (Qt::LeftButton|Qt::RightButton))
    {
        if(startnum >= 0)
        {
            allcell[startnum].setpress(true);
            int row = startnum/width;
            int col = startnum%width;
            if(row > 0) allcell[width*(row-1)+col].setpress(true);
            if(row < height-1) allcell[width*(row+1)+col].setpress(true);
            if(col > 0) allcell[width*row+col-1].setpress(true);
            if(col < width-1) allcell[width*row+col+1].setpress(true);
            if(row>0&&col>0) allcell[width*(row-1)+col-1].setpress(true);
            if(row>0&&col<width-1) allcell[width*(row-1)+col+1].setpress(true);
            if(row<height-1&&col>0) allcell[width*(row+1)+col-1].setpress(true);
            if(row<height-1&&col<width-1) allcell[width*(row+1)+col+1].setpress(true);
        }
        leftpress = false;
        rightpress = false;
        doublepress = true;
    }
    else if(event->buttons() == Qt::RightButton)
    {
        if(startnum >= 0)
            allcell[startnum].setpress(true);
        leftpress = false;
        rightpress = true;
        doublepress = false;
    }
    update();
}

void gameplay::mouseMoveEvent(QMouseEvent *event)
{
    int movex = event->x();
    int movey = event->y();
    int movenum = cal_num(movex,movey);
    if(movenum!=startnum)
    {
        moveout = true;
        for(int i = 0; i < allcell.size(); ++i)
            allcell[i].setpress(false);
    }
    update();
}

void gameplay::mouseReleaseEvent(QMouseEvent *event)
{
    int endx = event->x();
    int endy = event->y();
    int endnum = cal_num(endx,endy);
    int row = endnum/width;
    int col = endnum%width;
    for(int i = 0; i < allcell.size(); ++i)
        allcell[i].setpress(false);
    if(endnum!=startnum)
        moveout = true;
    if(moveout == true)
    {
        moveout = false;
        return;
    }
    if(endnum == -5)//点击表情，重新游戏
        restartgame();
    if(endnum >= 0)//点击cell
    {
        if(gameend == 0)//游戏没有结束
        {
            if(gamestart == false)
            {
                gamestart = true;
                timer->start(1000);
            }
            if(leftpress == true)
            {
                if(allcell[endnum].get_state() == 0)//未被挖开
                {
                    if(allcell[endnum].get_num() == 0&&allcell[endnum].get_kind() == 0)//区域性打开
                        openothers(row,col);
                    allcell[endnum].change_state(1);
                }
            }
            else if(rightpress == true)
            {
                if(allcell[endnum].get_state() == 0)//未被挖开
                {
                    allcell[endnum].change_state(2);
                    mine_left_num--;
                }
                else if(allcell[endnum].get_state() == 2)//被标识红旗
                {
                    allcell[endnum].change_state(3);
                    mine_left_num++;
                }
                else if(allcell[endnum].get_state() == 3)//被标识为问号
                    allcell[endnum].change_state(0);
            }
            else if(doublepress == true)
            {
                if(allcell[endnum].get_state() == 1)//挖开
                {
                    int flagcount = 0;
                    if(row > 0)
                        if(allcell[width*(row-1)+col].get_state() == 2)
                            flagcount++;
                    if(row < height-1)
                        if(allcell[width*(row+1)+col].get_state() == 2)
                            flagcount++;
                    if(col > 0)
                        if(allcell[width*row+col-1].get_state() == 2)
                            flagcount++;
                    if(col < width-1)
                        if(allcell[width*row+col+1].get_state() == 2)
                            flagcount++;
                    if(row>0&&col>0)
                        if(allcell[width*(row-1)+col-1].get_state() == 2)
                            flagcount++;
                    if(row>0&&col<width-1)
                        if(allcell[width*(row-1)+col+1].get_state() == 2)
                            flagcount++;
                    if(row<height-1&&col>0)
                        if(allcell[width*(row+1)+col-1].get_state() == 2)
                            flagcount++;
                    if(row<height-1&&col<width-1)
                        if(allcell[width*(row+1)+col+1].get_state() == 2)
                            flagcount++;
                    if(flagcount == allcell[endnum].get_num())
                    {
                        if(row > 0)
                            if(allcell[width*(row-1)+col].get_state() == 0)
                            {
                                if(allcell[width*(row-1)+col].get_num() == 0&&allcell[width*(row-1)+col].get_kind() == 0)//区域性打开
                                    openothers(row-1,col);
                                allcell[width*(row-1)+col].change_state(1);
                            }
                        if(row < height-1)
                            if(allcell[width*(row+1)+col].get_state() == 0)
                            {
                                if(allcell[width*(row+1)+col].get_num() == 0&&allcell[width*(row+1)+col].get_kind() == 0)//区域性打开
                                    openothers(row+1,col);
                                allcell[width*(row+1)+col].change_state(1);
                            }
                        if(col > 0)
                            if(allcell[width*row+col-1].get_state() == 0)
                            {
                                if(allcell[width*row+col-1].get_num() == 0&&allcell[width*row+col-1].get_kind() == 0)//区域性打开
                                    openothers(row,col-1);
                                allcell[width*row+col-1].change_state(1);
                            }
                        if(col < width-1)
                            if(allcell[width*row+col+1].get_state() == 0)
                            {
                                if(allcell[width*row+col+1].get_num() == 0&&allcell[width*row+col+1].get_kind() == 0)//区域性打开
                                    openothers(row,col+1);
                                allcell[width*row+col+1].change_state(1);
                            }
                        if(row>0&&col>0)
                            if(allcell[width*(row-1)+col-1].get_state() == 0)
                            {
                                if(allcell[width*(row-1)+col-1].get_num() == 0&&allcell[width*(row-1)+col-1].get_kind() == 0)//区域性打开
                                    openothers(row-1,col-1);
                                allcell[width*(row-1)+col-1].change_state(1);
                            }
                        if(row>0&&col<width-1)
                            if(allcell[width*(row-1)+col+1].get_state() == 0)
                            {
                                if(allcell[width*(row-1)+col+1].get_num() == 0&&allcell[width*(row-1)+col+1].get_kind() == 0)//区域性打开
                                    openothers(row-1,col+1);
                                allcell[width*(row-1)+col+1].change_state(1);
                            }
                        if(row<height-1&&col>0)
                            if(allcell[width*(row+1)+col-1].get_state() == 0)
                            {
                                if(allcell[width*(row+1)+col-1].get_num() == 0&&allcell[width*(row+1)+col-1].get_kind() == 0)//区域性打开
                                    openothers(row+1,col-1);
                                allcell[width*(row+1)+col-1].change_state(1);
                            }
                        if(row<height-1&&col<width-1)
                            if(allcell[width*(row+1)+col+1].get_state() == 0)
                            {
                                if(allcell[width*(row+1)+col+1].get_num() == 0&&allcell[width*(row+1)+col+1].get_kind() == 0)//区域性打开
                                    openothers(row+1,col+1);
                                allcell[width*(row+1)+col+1].change_state(1);
                            }
                    }
                }
            }
        }
        gameend = 1;
        for(int i = 0; i < allcell.size(); ++i)
        {
            if(allcell[i].get_state() == 0&&allcell[i].get_kind() == 0)//有未被挖开的数字cell
                gameend = 0;
            else if(allcell[i].get_state() == 1&&allcell[i].get_kind() == 1)//挖开雷
            {
                gameend = 2;
                timer->stop();
                break;
            }
        }
        if(gameend == 1)
            timer->stop();
    }
    update();
}

void gameplay::openothers(int row, int col)
{
    int num = width*row+col;
    if(allcell[num].get_state() == 0)
    {
        allcell[num].change_state(1);
        if(allcell[num].get_num() == 0&&allcell[num].get_kind() == 0)
        {
            if(row > 0) openothers(row-1,col);
            if(row < height-1) openothers(row+1,col);
            if(col > 0) openothers(row,col-1);
            if(col < width-1) openothers(row,col+1);
            if(row>0&&col>0) openothers(row-1,col-1);
            if(row>0&&col<width-1) openothers(row-1,col+1);
            if(row<height-1&&col>0) openothers(row+1,col-1);
            if(row<height-1&&col<width-1) openothers(row+1,col+1);
        }
    }
}

void gameplay::timeadd()
{
    mytime++;
    update();
}

int gameplay::cal_num(int x, int y)
{
    if(x>=(width-1)*CELL_SIZE/2&&x<=(width-1)*CELL_SIZE/2+CELL_SIZE
            &&y>=INTERVAL+TOPHEIGHT/5&&y<=INTERVAL+TOPHEIGHT/5+CELL_SIZE)
        return -5;
    y = y - INTERVAL - TOPHEIGHT;
    if(y < 0)
        return -1;
    x = x/CELL_SIZE;
    y = y/CELL_SIZE;
    return y*width+x;
}

void gameplay::restartgame()
{
    cell_init();
    minerandomset();
    num_filled();
    timer->stop();
    mytime = 0;
    mine_left_num = minenum;
    gamestart = false;
    gameend = 0;
    leftpress = false;
    rightpress = false;
    doublepress = false;
    moveout = false;
    startnum = -1;
}
