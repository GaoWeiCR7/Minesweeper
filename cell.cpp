#include "cell.h"

cell::cell()
{
    state = 0;
    num = 0;
    kind = 0;
}


cell::cell(int n, int k)
{
    state = 0;
    num = n;
    kind = k;
    press = false;
}

void cell::change_state(int changed_state)
{
    state = changed_state;
}

void cell::set_num(int n)
{
    num = n;
}

void cell::set_kind(int k)
{
    kind = k;
}

int cell::get_state()
{
    return state;
}
int cell::get_num()
{
    return num;
}
int cell::get_kind()
{
    return kind;
}

void cell::numadd()
{
    num++;
}

void cell::setpress(bool p)
{
    press = p;
}

bool cell::getpress()
{
    return press;
}
