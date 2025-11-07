#include "stm32f10x.h"
#include "u8g2.h"
#include "RCC.h"
#include "u8g2_user.h"
#include "LINKLIST.h"
#include "USART.h"
#include <stdio.h>
u8g2_t u8g2;
MenuItem *menuHead = NULL;//指向MenuItem结构体的指针
void menuAction1(void)
{
    u8g2_SetFont(&u8g2, u8g2_font_7x13_te);//设置字体
    u8g2_DrawStr(&u8g2, 0, 10, "Menu Item 1");//显示字符串
    u8g2_SendBuffer(&u8g2);//发送缓冲区数据到
}
void menuAction2(void)
{
    u8g2_SetFont(&u8g2, u8g2_font_7x13_te);//设置字体
    u8g2_DrawStr(&u8g2, 0, 30, "Menu Item 2");//显示字符串
    u8g2_SendBuffer(&u8g2);//发送缓冲区数据到
}
void menuAction3(void)
{
    u8g2_SetFont(&u8g2, u8g2_font_7x13_te);//设置字体
    u8g2_DrawStr(&u8g2, 0, 50, "Menu Item 3");//显示字符串
    u8g2_SendBuffer(&u8g2);//发送缓冲区数据到
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
        uint8_t menuListIndex = 0;
        MenuItem *current = menuHead;
        u8g2_ClearBuffer(&u8g2);
        for (menuListIndex = 1; current != NULL; menuListIndex++, current = current->next)
        {
            // 使用 u8g2 显示节点的 name
            u8g2_DrawStr(&u8g2, 34, menuListIndex * 10, current->name);
        }
        u8g2_SendBuffer(&u8g2);
        // 向右滑动 1 位
        menuHead = SlideMenuItemList(menuHead, 1);
        Systick_ms(1000); // 延时 1 秒
    }
    
}

