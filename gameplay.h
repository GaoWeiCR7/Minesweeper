#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#define CELL_SIZE 30
#define INTERVAL 25
#define TOPHEIGHT 50
#define BIAS 1


#include <QDialog>
#include "user_defined_input.h"
#include <QToolBar>
#include <QMenuBar>
#include <QLabel>
#include "cell.h"
#include <QVector>
#include <ctime>
#include <QTimer>
#include <QString>
#include <QMouseEvent>
#include "mode_select.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QMessageBox>

namespace Ui {
class gameplay;
}

class gameplay : public QDialog
{
    Q_OBJECT

public:
    explicit gameplay(bool lang,QWidget *parent = nullptr);
    ~gameplay();
    void user_defined_dialog_set();
    void dia_init();
    void cell_init();
    void minerandomset();
    void num_filled();

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

    void timeadd();
    int cal_num(int x, int y);//计算鼠标点击的cell
    void restartgame();//重新开始本轮游戏
    void openothers(int row, int col);

    void musicset();
    void providetips();

private:
    Ui::gameplay *ui;
    mode_select* mode_sel;
    user_defined_input* user_def;
    bool language; //0: English   1:中文
    int pattern; // 0:easy  1:normal  2:hard  3: user-defined
    int height;//高度
    int width;//宽度
    int minenum;//雷数
    int mine_left_num;//剩下的雷数

    int mytime;//时间
    QTimer *timer;

    bool leftpress;
    bool rightpress;
    bool doublepress;
    bool moveout;

    int startnum;//鼠标点击的cell

    bool gamestart;//游戏是否开始，点击完第一个格子后游戏开始，同时计时
    int gameend;//游戏是否结束，0：游戏未结束  1：succeed  2：failed

    QMenuBar* menubar;
    QVector<cell> allcell;

    QMediaPlayer* music;
    QMediaPlaylist* musiclist;
};



#endif // GAMEPLAY_H
