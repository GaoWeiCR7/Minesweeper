#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#define CELL_SIZE 20

#include <QDialog>
#include "user_defined_input.h"
#include <QToolBar>
#include <QMenuBar>
#include <QLabel>
#include "cell.h"
#include <QVector>
#include <ctime>

namespace Ui {
class gameplay;
}

class gameplay : public QDialog
{
    Q_OBJECT

public:
    explicit gameplay(bool lang,int pat,QWidget *parent = nullptr);
    ~gameplay();
    void user_defined_dialog_set();
    void dia_init();
    void minerandomset();
    void num_filled();
    void paintEvent(QPaintEvent *);

private:
    Ui::gameplay *ui;
    user_defined_input* user_def;
    bool language; //0: English   1:中文
    int pattern; // 0:easy  1:normal  2:hard  3: user-defined
    int height;//高度
    int width;//宽度
    int minenum;//雷数
    int mine_left_num;//剩下的雷数
    QMenuBar* menubar;
    QVector<cell> allcell;
};



#endif // GAMEPLAY_H
