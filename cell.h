#ifndef CELL_H
#define CELL_H


class cell
{
public:
    cell();
    cell(int n, int k);
    void change_state(int changed_state);
    void set_num(int n);
    void numadd();
    void set_kind(int k);
    void setpress(bool p);
    bool getpress();
    int get_state();
    int get_num();
    int get_kind();
private:
    int state;//0:未挖开    1:挖开    2:被标识红旗    3:被标识为问号
    int num;//挖开后显示的数字，若不是数字则默认为0
    int kind;//0:数字   1:雷
    bool press;//判断是否正在被点击
};

#endif // CELL_H
