#include "stm32f10x.h"
#include "u8g2.h"
#include "RCC.h"
#include "u8g2_user.h"
#include "LINKLIST.h"
#include "USART.h"
#include <stdio.h>
typedef enum {
    KeyNull  = 0,
    KeyDown  = 1,
    KeyUp    = 2,
    KeyEnter = 3
} KeyState;
u8g2_t u8g2;
MenuItem *menuHead = NULL;//指向MenuItem结构体的指针
void menuAction1(void)
{

}
void menuAction2(void)
{

}
void menuAction3(void)
{

}
void menuConfig(MenuItem **menuHead)
{
    *menuHead = appendMenuItem(*menuHead, createMenuItem("Setting", menuAction1));//节点1
    *menuHead = appendMenuItem(*menuHead, createMenuItem("Display", menuAction2));//节点2
    *menuHead = appendMenuItem(*menuHead, createMenuItem("Cleaning", menuAction3));//节点3
}
int main()
{
    RCC_init();
    USART1_init();
    u8g2_init(&u8g2,1);//U8G2初始化 硬件I2C+DMA
    menuConfig(&menuHead);
    while (1)
    {
        uint8_t menuListIndex = 0, Arrow = 1;
        KeyState key = KeyNull;
        MenuItem *current = menuHead;
        u8g2_ClearBuffer(&u8g2);
        for(menuListIndex = 1; current != NULL && menuListIndex *10 < 60; menuListIndex++, current = current->next)//遍历链表显示菜单项
        {
            // 使用 u8g2 显示节点的 name
            u8g2_DrawStr(&u8g2, 34, menuListIndex * 10, current->name);
            u8g2_DrawStr(&u8g2, 0, Arrow * 10, ">"); // 显示箭头
            if (menuListIndex == Arrow && key == KeyEnter)//如果选中了某一项并按下确认键
            {
                if (current->action != NULL)
                {
                    current->action(); // 执行对应的操作
                }
            }
            if (Arrow == 60 && key == KeyDown)//链表下滑
            {
                SlideMenuItemList(menuHead, 1);
            }
            if (Arrow == 0 && key == KeyUp)//链表上滑
            {
                SlideMenuItemList(menuHead, -1);
            }
        }
        u8g2_SendBuffer(&u8g2);
        Systick_ms(1000); // 延时 1 秒
    }
    
}

