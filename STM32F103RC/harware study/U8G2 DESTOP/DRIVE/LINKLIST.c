#include "LINKLIST.h"

MenuItem* createMenuItem(char *name, void (*action)(void))//生成菜单项
{
    MenuItem *newItem = (MenuItem *)malloc(sizeof(MenuItem));//分配内存 newItem指向分配的内存 sizeof(MenuItem)设定内存大小
    if (newItem == NULL)
    {
        return NULL; // 内存分配失败
    }
    newItem->name = name;
    newItem->action = action;
    newItem->next = NULL;
    newItem->prev = NULL;
    return newItem;
}
MenuItem* appendMenuItem(MenuItem *head, MenuItem *newItem)//添加菜单项
{
    if (newItem == NULL) return head;//如果newItem为空则返回head
    if (head == NULL) return newItem;//如果head为空则返回newItem
    MenuItem *p = head;//指向head
    while (p->next)//找到最后一个空节点 next = NULL
    { 
        p = p->next;//指向next
    }
    p->next = newItem;//连接新节点
    newItem->prev = p;//新节点链接上一个节点
    return head;
}
MenuItem* SlideMenuItemList(MenuItem *head, int k) // 向右滑动 k 位
{
    if (head == NULL || head->next == NULL || k == 0) return head;

    // 计算链表长度并找到尾节点
    MenuItem *tail = head;
    int n = 1;
    while (tail->next) { tail = tail->next; n++; }

    k = k % n;
    if (k == 0) return head;

    int steps = n - k - 1;
    MenuItem *newTail = head;
    for (int i = 0; i < steps; i++) newTail = newTail->next;

    MenuItem *newHead = newTail->next;

    newTail->next = NULL;
    newHead->prev = NULL;

    tail->next = head;
    head->prev = tail;

    return newHead;
}










