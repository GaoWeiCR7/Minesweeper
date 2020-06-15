#ifndef MODE_SELECT_H
#define MODE_SELECT_H

#include <QDialog>
#include "gameplay.h"

namespace Ui {
class mode_select;
}

class mode_select : public QDialog
{
    Q_OBJECT

public:
    explicit mode_select(bool lang,QWidget *parent = nullptr);
    ~mode_select();

private slots:
    void on_easy_clicked();

    void on_normal_clicked();

    void on_hard_clicked();

    void on_userdefined_clicked();

private:
    Ui::mode_select *ui;
    gameplay* gamepl;
    bool language;
};

#endif // MODE_SELECT_H
