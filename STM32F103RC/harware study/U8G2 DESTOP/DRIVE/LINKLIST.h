#ifndef __LINKLIST_H
#define __LINKLIST_H

#include "stm32f10x.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct MenuItem
{
    char *name;//菜单名称
    struct MenuItem *next;//下一个链表
    struct MenuItem *prev;//上一个链表
    void (*action)(void);//执行参数
} MenuItem;

MenuItem* createMenuItem(char *name, void (*action)(void));//生成菜单项
MenuItem* appendMenuItem(MenuItem *head, MenuItem *newItem);//添加菜单项
MenuItem* SlideMenuItemList(MenuItem *head, int k); // 向右滑动 k 位

#endif // __LINKLIST_H




