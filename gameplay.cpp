#include "gameplay.h"
#include "ui_gameplay.h"

gameplay::gameplay(bool lang,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gameplay)
{
    ui->setupUi(this);
    language = lang;
    pattern = 0;
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
    this->setWindowIcon(QIcon(":/new/prefix1/image/title.png"));
    if(language == false)
        this->setWindowTitle("Minesweeper");
    else
        this->setWindowTitle("扫雷");
    mode_sel = new mode_select(language,this);
    user_def = new user_defined_input(language,this);
    menubar = new QMenuBar(this);
    QMenu* menu1 = new QMenu("Option",this);

    QAction* mode_select_ac = new QAction("mode",this);
    QAction* music_volume = new QAction("volumn",this);
    QAction* tips = new QAction("tips",this);
    QAction* regame = new QAction("restart",this);

    if(language == true)
    {
        menu1->setTitle("选项");
        mode_select_ac->setText("模式选择");
        music_volume->setText("音量调节");
        tips->setText("建议");
        regame->setText("重新开始");
    }

    connect(mode_select_ac,&QAction::triggered,this,&gameplay::dia_init);
    connect(music_volume,&QAction::triggered,this,&gameplay::musicset);
    connect(tips,&QAction::triggered,this,&gameplay::providetips);
    connect(regame,&QAction::triggered,this,&gameplay::restartgame);

    menu1->addAction(mode_select_ac);
    menu1->addAction(music_volume);
    menu1->addAction(tips);
    menu1->addAction(regame);
    menubar->addMenu(menu1);

    timer = new QTimer(this);
    connect(this->timer,&QTimer::timeout,this,&gameplay::timeadd);

    QString runpath = QCoreApplication::applicationDirPath();
    QString runpath1 =  runpath + "/bgm/City of the sky.mp3";
    QString runpath2 =  runpath + "/bgm/The wizard of oz.mp3";
    QString runpath3 =  runpath + "/bgm/Spirited away.mp3";

    musiclist = new QMediaPlaylist(this);
    music = new QMediaPlayer(this);

    musiclist->addMedia(QUrl(runpath1));
    musiclist->addMedia(QUrl(runpath2));
    musiclist->addMedia(QUrl(runpath3));

    music->setPlaylist(musiclist);
    music->setVolume(80);
    music->play();

    if(mode_sel->exec() == QDialog::Accepted)
    {
        pattern = mode_sel->getpat();
    }
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
    //this->setGeometry(700,300,width*CELL_SIZE,height*CELL_SIZE+INTERVAL+TOPHEIGHT);
    this->setFixedSize(width*CELL_SIZE,height*CELL_SIZE+INTERVAL+TOPHEIGHT);
    allcell.resize(height*width);
    restartgame();
}

gameplay::~gameplay()
{
    delete ui;
}

void gameplay::musicset()
{
    QDialog dia(this);
    dia.setFixedSize(400,200);
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    dia.setWindowFlags(flags);
    if(language == false)
        dia.setWindowTitle("Volume");
    else
        dia.setWindowTitle("音量调节");

    QPushButton isok(&dia);
    QPushButton isno(&dia);
    if(language == false)
    {
        isok.setText("ok");
        isno.setText("cancel");
    }
    else
    {
        isok.setText("确定");
        isno.setText("取消");
    }
    isok.move(80,150);
    isno.move(230,150);
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
    isok.setFixedSize(isok.size());
    isno.setFixedSize(isno.size());
    isok.setStyleSheet(qss0);
    isno.setStyleSheet(qss0);

    QSpinBox spinbox(&dia);
    spinbox.setMinimum(0);
    spinbox.setMaximum(100);
    spinbox.setSingleStep(1);

    QSlider slider(&dia);
    slider.setOrientation(Qt::Horizontal);
    slider.setMinimum(0);
    slider.setMaximum(100);
    slider.setSingleStep(1);
    spinbox.move(50,50);
    slider.move(150,50);
    slider.setFixedWidth(200);
    QString qss1 = "QSlider::groove:horizontal {\
            border: 1px solid #4A708B;\
            background: #C0C0C0;\
            height: 5px;\
            border-radius: 1px;\
            padding-left:-1px;\
            padding-right:-1px;\
            }\
            QSlider::sub-page:Horizontal \
            {\
                background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(231,80,229, 255), stop:1 rgba(7,208,255, 255));\
                height:4px;\
            }\
            QSlider::add-page:Horizontal\
                 {     \
                    background-color: rgb(87, 97, 106);\
                    height:4px;\
                 }\
            QSlider::handle:horizontal\
            {\
                background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,\
                stop:0.6 #45ADED, stop:0.778409 rgba(255, 255, 255, 255));\
                width: 11px;\
                margin-top: -3px;\
                margin-bottom: -3px;\
                border-radius: 5px;\
            }";
    slider.setStyleSheet(qss1);
    connect(&spinbox, SIGNAL(valueChanged(int)), &slider, SLOT(setValue(int)));
    connect(&slider, SIGNAL(valueChanged(int)), &spinbox, SLOT(setValue(int)));
    spinbox.setValue(music->volume());
    connect(&isok,&QPushButton::clicked,&dia,&QDialog::accept);
    connect(&isno,&QPushButton::clicked,&dia,&QDialog::close);

    if(dia.exec() == QDialog::Accepted)
    {
        music->setVolume(slider.value());
    }
}

void gameplay::providetips()
{
    QDialog dia(this);
    //dia.setFixedSize(400,200);
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    dia.setWindowFlags(flags);
    if(language == false)
        dia.setWindowTitle("Tips");
    else
        dia.setWindowTitle("建议");
    QLabel label(&dia);

    //label.setFixedSize(dia.size());
    label.setStyleSheet("background: white;");
    if(language == true)
        label.setText("tip1:\n如果您认为某个方块可能藏有地雷，请右键单击它。这会在该方块上做一个旗标。（如果不确定，请再次右键单击标记为问号。）\n\
tip2:\n如果一行中有三个方块显示为2-3-2，您就会知道该行旁边可能排列着三个雷。如果一个方块显示为 8，则它周围的每个方块下面都有一个雷。\n\
tip3:\n如果不确定下一个单击位置，可以尝试清除某些未探测的区域。在未标记方块的中间单击比在可能有雷的区域单击要好一些。\n");
    else
        label.setText("tip1:\nIf you think a cell might be mined, right-click it. This will make a flag on the square. (If not, right-click again and mark as a question mark.)\n\
tip2:\nIf there are three cells in a row that appear as 2-3-2, you know that there might be three mines lined up next to the row. If a cell appears as 8, there is a mine under each cell around it.\n\
tip3:\nIf you are not sure where to place a click, you can try to clear some unexplored areas. It is better to click in the middle of an unmarked cell than in a cell that may be mined.\n");
    label.setAlignment(Qt::AlignTop);
    label.setWordWrap(true);
    label.setFont(QFont("宋体" , 10 ));
    label.setMargin(10);
    label.adjustSize();
    dia.setFixedSize(label.size());
    dia.exec();
}

void gameplay::dia_init()
{
    if(mode_sel->exec() == QDialog::Accepted)
    {
        pattern = mode_sel->getpat();
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
        //this->setGeometry(700,300,width*CELL_SIZE,height*CELL_SIZE+INTERVAL+TOPHEIGHT);
        this->setFixedSize(width*CELL_SIZE,height*CELL_SIZE+INTERVAL+TOPHEIGHT);
        allcell.resize(height*width);
        restartgame();
    }
}

void gameplay::user_defined_dialog_set()
{
    if(user_def->exec() == QDialog::Accepted)
    {
        height = user_def->get_height();
        width = user_def->get_width();
        minenum = user_def->get_minenum();
        if(height<9||width<9||minenum<10)
            QMessageBox::information(nullptr,"提示","自定义设置过于简单！");
        height = (height>9)?height:9;
        width = (width>9)?width:9;
        minenum = (minenum>10)?minenum:10;
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
    painter.drawPixmap(rect(),QPixmap(":/new/prefix1/image/mainpic.png"));

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
            if(allcell[i].getpress() == true&&gameend == 0)
                painter.drawPixmap(column*CELL_SIZE+BIAS,row*CELL_SIZE+INTERVAL+TOPHEIGHT+BIAS,CELL_SIZE-2*BIAS,CELL_SIZE-2*BIAS,image_initcell);
            else
                painter.drawPixmap(column*CELL_SIZE,row*CELL_SIZE+INTERVAL+TOPHEIGHT,CELL_SIZE,CELL_SIZE,image_initcell);
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
                if(language == false)
                    QMessageBox::about(nullptr,"Failure","You have not passed the game!");
                else
                    QMessageBox::about(nullptr,"失败","你没有通过游戏！");
                break;
            }
        }
        if(gameend == 1)
        {
            timer->stop();
            if(language == false)
                QMessageBox::about(nullptr,"Success","You have passed the game Successfully!");
            else
                QMessageBox::about(nullptr,"成功","你已经成功通过游戏！");
        }
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
    update();
}
