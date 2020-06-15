#ifndef USER_DEFINED_INPUT_H
#define USER_DEFINED_INPUT_H
#define cout qDebug() << "[" <<__FILE__ <<":"<<__FUNCTION__<<":"<<__LINE__ <<"]"

#include <QDialog>
#include <QIntValidator>
#include <QDebug>


namespace Ui {
class user_defined_input;
}

class user_defined_input : public QDialog
{
    Q_OBJECT

public:
    explicit user_defined_input(bool lang,QWidget *parent = nullptr);
    ~user_defined_input();
    int get_height();
    int get_width();
    int get_minenum();
private slots:
    void on_ok_clicked();

private:
    Ui::user_defined_input *ui;
    int height;
    int width;
    int minenum;
};

#endif // USER_DEFINED_INPUT_H
