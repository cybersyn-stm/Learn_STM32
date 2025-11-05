/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
#include <stdio.h>

#include "oled.h"
#include "u8g2.h"
#include "u8x8.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
u8g2_t u8g2;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  oled_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint32_t frame_count = 0;
  uint32_t start_time = HAL_GetTick();
  uint32_t fps = 0;
  char fps_str[10];
  int x = 128; // 初始x坐标，屏幕宽度
  int str_width = 60; // "FPS: 0000"宽度，6x10字体每字符6像素，9字符=54

  // 进度条相关变量
  int progress = 0;
  int progress_max = 100;
  int bar_x = 50; // 进度条起始x
  int bar_y = 20; // 进度条y
  int bar_width = 50; // 进度条总宽度
  int bar_height = 10; // 进度条高度

  uint32_t mode_start_time = HAL_GetTick();
  int show_mode = 0; // 0: 旋转立方体，1: 数字时钟，2: 模拟时钟，3: 动态条形图，4: 字体字符图标演示，5: 正弦波，6: 菜单示例
  int bar_values[5] = {10, 30, 20, 40, 15};
  int bar_dir[5] = {1, -1, 1, -1, 1};
  int font_demo_idx = 0;
  uint32_t font_demo_last = 0;

  // 菜单动画状态变量（只声明一次，主循环内不要再static）
  static int menu_sel = 0;
  static int menu_sel_prev = 0;
  static uint32_t menu_anim_start = 0;
  static int animating = 0;
  static uint32_t last_switch_time = 0;

  while (1) {
    uint32_t current_time = HAL_GetTick();
    uint32_t elapsed_time = current_time - start_time;

    if (elapsed_time >= 1000) {  // 每秒更新一次帧率
      fps = frame_count * 1000 / elapsed_time;
      start_time = current_time;
      frame_count = 0;
      sprintf(fps_str, "FPS: %lu", fps);
    }

    // 2秒切换显示模式，若到菜单页则停留
    if (show_mode != 6 && current_time - mode_start_time >= 3000) {
      show_mode = (show_mode + 1) % 7;
      mode_start_time = current_time;
    }

    u8g2_ClearBuffer(&u8g2);
    // 每页只显示对应内容
    if (show_mode == 0) {
      // 旋转立方体页
      u8g2_SetFont(&u8g2, u8g2_font_6x13_me);
      u8g2_DrawStr(&u8g2, 48, 12, "3D Cube");
      // 旋转立方体（缩小）
      float cx = 64.0f;
      float cy = 34.0f;
      float cube_size = 26.0f;
      float half = cube_size / 2.0f;
      static float angle = 0.0f;
      angle += 0.07f;
      if (angle > 6.2832f) angle -= 6.2832f;
      float cube3d[8][3] = {
        {-half, -half, -half},
        { half, -half, -half},
        { half,  half, -half},
        {-half,  half, -half},
        {-half, -half,  half},
        { half, -half,  half},
        { half,  half,  half},
        {-half,  half,  half}
      };
      float sinA = sinf(angle);
      float cosA = cosf(angle);
      float sinB = sinf(angle*0.7f);
      float cosB = cosf(angle*0.7f);
      int cube2d[8][2];
      for (int i = 0; i < 8; i++) {
        float x = cube3d[i][0];
        float y = cube3d[i][1];
        float z = cube3d[i][2];
        float x1 = x * cosA - z * sinA;
        float z1 = x * sinA + z * cosA;
        float y1 = y * cosB - z1 * sinB;
        // float z2 = y * sinB + z1 * cosB; // 不用z2
        cube2d[i][0] = (int)(cx + x1);
        cube2d[i][1] = (int)(cy + y1);
      }
      const uint8_t edges[12][2] = {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
      };
      for (int i = 0; i < 12; i++) {
        u8g2_DrawLine(&u8g2,
          cube2d[edges[i][0]][0], cube2d[edges[i][0]][1],
          cube2d[edges[i][1]][0], cube2d[edges[i][1]][1]);
      }
        // 底部进度条
        int pb_x = 16, pb_y = 60, pb_w = 96, pb_h = 4;
        int pb_val = (current_time / 20) % 101; // 0~100
        int pb_fill = pb_val * pb_w / 100;
        u8g2_DrawFrame(&u8g2, pb_x, pb_y, pb_w, pb_h);
        u8g2_DrawBox(&u8g2, pb_x + 1, pb_y + 1, pb_fill - 2 > 0 ? pb_fill - 2 : 0, pb_h - 2);
        char pb_str[8];
        snprintf(pb_str, sizeof(pb_str), "%3d%%", pb_val);
        u8g2_SetFont(&u8g2, u8g2_font_6x13_me);
        u8g2_DrawStr(&u8g2, pb_x + pb_w + 4, pb_y + pb_h, pb_str);
    } else if (show_mode == 1) {
      // 数字时钟页
      u8g2_SetFont(&u8g2, u8g2_font_logisoso18_tr);
      uint32_t sec = (current_time / 1000) % 60;
      uint32_t min = (current_time / 60000) % 60;
      uint32_t hour = (current_time / 3600000) % 24;
      char time_str[16];
      snprintf(time_str, sizeof(time_str), "%02lu:%02lu:%02lu", hour, min, sec);
      int tw = u8g2_GetStrWidth(&u8g2, time_str);
      int th = u8g2_GetMaxCharHeight(&u8g2);
      int tx = (128 - tw) / 2;
      int ty = (64 + th) / 2 - 4;
      u8g2_DrawStr(&u8g2, tx, ty, time_str);
    } else if (show_mode == 2) {
      // 模拟时钟页
      float center_x = 64.0f;
      float center_y = 32.0f;
      float radius = 30.0f;
      u8g2_DrawCircle(&u8g2, (int)center_x, (int)center_y, (int)radius, U8G2_DRAW_ALL);
      for (int i = 0; i < 12; i++) {
        float angle = i * 6.2832f / 12.0f;
        float x1 = center_x + cosf(angle) * (radius - 3);
        float y1 = center_y + sinf(angle) * (radius - 3);
        float x2 = center_x + cosf(angle) * (radius - 0.5f);
        float y2 = center_y + sinf(angle) * (radius - 0.5f);
        u8g2_DrawLine(&u8g2, (int)x1, (int)y1, (int)x2, (int)y2);
      }
      uint32_t sec = (current_time / 1000) % 60;
      uint32_t min = (current_time / 60000) % 60;
      uint32_t hour = (current_time / 3600000) % 12;
      float hour_angle = ((hour + min / 60.0f) * 6.2832f / 12.0f) - 1.5708f;
      float hour_x = center_x + cosf(hour_angle) * (radius * 0.5f);
      float hour_y = center_y + sinf(hour_angle) * (radius * 0.5f);
      u8g2_DrawLine(&u8g2, (int)center_x, (int)center_y, (int)hour_x, (int)hour_y);
      float min_angle = ((min + sec / 60.0f) * 6.2832f / 60.0f) - 1.5708f;
      float min_x = center_x + cosf(min_angle) * (radius * 0.8f);
      float min_y = center_y + sinf(min_angle) * (radius * 0.8f);
      u8g2_DrawLine(&u8g2, (int)center_x, (int)center_y, (int)min_x, (int)min_y);
      float sec_angle = (sec * 6.2832f / 60.0f) - 1.5708f;
      float sec_x = center_x + cosf(sec_angle) * (radius * 0.95f);
      float sec_y = center_y + sinf(sec_angle) * (radius * 0.95f);
      u8g2_DrawLine(&u8g2, (int)center_x, (int)center_y, (int)sec_x, (int)sec_y);
    } else if (show_mode == 3) {
      // 动态条形图页
      u8g2_SetFont(&u8g2, u8g2_font_6x13_me);
      u8g2_DrawStr(&u8g2, 44, 12, "Bar Chart");
      // 动态条形图（缩小）
      int bar_w = 10, bar_h = 22, gap = 6;
      int total_width = 5 * bar_w + 4 * gap;
      int base_x = (128 - total_width) / 2;
      int base_y = 24;
      for (int i = 0; i < 5; i++) {
        int h = bar_values[i];
        u8g2_DrawFrame(&u8g2, base_x + i * (bar_w + gap), base_y, bar_w, bar_h);
        u8g2_DrawBox(&u8g2, base_x + i * (bar_w + gap) + 1, base_y + bar_h - h, bar_w - 2, h);
        // 动态变化
        bar_values[i] += bar_dir[i];
        if (bar_values[i] > bar_h - 2) bar_dir[i] = -1;
        if (bar_values[i] < 5) bar_dir[i] = 1;
      }
      // 简单标签
      u8g2_SetFont(&u8g2, u8g2_font_6x13_me);
      u8g2_DrawStr(&u8g2, 40, 64, "Bar Chart");
     
    } else if (show_mode == 4) {
      // 正弦波演示页
      u8g2_SetFont(&u8g2, u8g2_font_6x13_me);
      u8g2_DrawStr(&u8g2, 36, 12, "Sine Wave");
      // 波形参数
      float amp = 20.0f; // 振幅
      float freq = 2.0f; // 周期数（屏幕宽度内）
      float phase = (current_time % 2000) * 2 * 3.1416f / 2000.0f; // 动态相位
      int y_offset = 36; // 居中基线
      int prev_x = 0, prev_y = y_offset;
      for (int x = 0; x < 128; x++) {
        float t = (float)x / 128.0f * freq * 2.0f * 3.1416f + phase;
        int y = y_offset + (int)(amp * sinf(t));
        if (x > 0) {
          u8g2_DrawLine(&u8g2, prev_x, prev_y, x, y);
        }
        prev_x = x;
        prev_y = y;
      }
      // 坐标轴
      u8g2_DrawLine(&u8g2, 0, y_offset, 127, y_offset); // x轴
      u8g2_DrawLine(&u8g2, 8, y_offset - (int)amp, 8, y_offset + (int)amp); // y轴
      // 标签
      u8g2_DrawStr(&u8g2, 2, y_offset - (int)amp - 2, "+A");
      u8g2_DrawStr(&u8g2, 2, y_offset + (int)amp + 10, "-A");
    } else if (show_mode == 5) {
      // 小球反弹独立演示页
      u8g2_SetFont(&u8g2, u8g2_font_6x13_me);
      static float ball_x = 64.0f, ball_y = 36.0f;
      static float ball_vx = 1.2f, ball_vy = 0.8f;
      const float ball_r = 8.0f;
      float min_x = ball_r, max_x = 127 - ball_r;
      float min_y = ball_r, max_y = 63 - ball_r;
      ball_x += ball_vx;
      ball_y += ball_vy;
      if (ball_x < min_x) { ball_x = min_x; ball_vx = -ball_vx; }
      if (ball_x > max_x) { ball_x = max_x; ball_vx = -ball_vx; }
      if (ball_y < min_y) { ball_y = min_y; ball_vy = -ball_vy; }
      if (ball_y > max_y) { ball_y = max_y; ball_vy = -ball_vy; }
      u8g2_DrawDisc(&u8g2, (int)ball_x, (int)ball_y, (int)ball_r, U8G2_DRAW_ALL);
      // 边框全屏
      u8g2_DrawFrame(&u8g2, 0, 0, 128, 64);
    } else if (show_mode == 6) {
      // 菜单示例页
      // 菜单内容选中框自适应+滑动动画
      u8g2_SetFont(&u8g2, u8g2_font_6x13_me);
      const char *menu_items[] = {"Start", "Settings-123123123123", "About", "Info", "Help", "Tools", "Exit"};
      int menu_count = 7;
      int menu_x = 20, menu_y = 3, menu_gap = 1;
      int menu_h = 13; // 字体高度
      int menu_show = 4; // 最多显示4项
      // 动画参数
      const int anim_time = 200; // 动画时长ms
      const int hold_time = 400; // 每项停留ms
      static int y_from = 0, y_to = 0;
      // 计算菜单滚动窗口和选中框在窗口内的相对位置（支持平滑滑动，scroll_anim为float）
      int scroll = 0;
      int sel_in_win = 0;
      if (menu_sel < 1) {
        scroll = 0;
        sel_in_win = menu_sel;
      } else if (menu_sel > menu_count - menu_show) {
        scroll = menu_count - menu_show;
        sel_in_win = menu_sel - scroll;
      } else {
        scroll = menu_sel - 1;
        sel_in_win = 1;
      }
      static float scroll_from = 0, scroll_to = 0;
      static float scroll_anim_val = 0;
      int y_anim = menu_y + sel_in_win * (menu_h + menu_gap);
      float scroll_anim = (float)scroll;
      // 切换检测与动画启动（动画未结束绝不切换，动画结束后再计时）
      if (!animating && current_time - last_switch_time > hold_time) {
        menu_sel_prev = menu_sel;
        menu_sel = (menu_sel + 1) % menu_count;
        // 计算新scroll和sel_in_win
        int next_scroll = 0, next_sel_in_win = 0;
        if (menu_sel < 1) {
          next_scroll = 0;
          next_sel_in_win = menu_sel;
        } else if (menu_sel > menu_count - menu_show) {
          next_scroll = menu_count - menu_show;
          next_sel_in_win = menu_sel - next_scroll;
        } else {
          next_scroll = menu_sel - 1;
          next_sel_in_win = 1;
        }
        y_from = menu_y + sel_in_win * (menu_h + menu_gap);
        y_to = menu_y + next_sel_in_win * (menu_h + menu_gap);
        scroll_from = scroll_anim_val;
        scroll_to = (float)next_scroll;
        menu_anim_start = current_time;
        animating = 1;
      }
      // 菜单内容和选中框真正平滑滑动动画（菜单内容和选中框都float插值，Q弹保留）
      static int anim_phase = 0; // 0:菜单滑动 1:Q弹选中框
      static float ease_k = 0.0f;
      if (animating) {
        uint32_t t = current_time - menu_anim_start;
        if (anim_phase == 0) {
          // 菜单内容滑动阶段
          if (t < anim_time) {
            float k = t / (float)anim_time;
            ease_k = k < 0.5f ? 4*k*k*k : 1 - powf(-2*k+2, 3)/2;
            scroll_anim_val = scroll_from + (scroll_to - scroll_from) * ease_k;
            scroll_anim = scroll_anim_val;
            // 选中框反向滑动
            y_anim = y_from + (y_to - y_from) * (1.0f - ease_k);
          } else {
            // 菜单滑动结束，进入Q弹阶段
            scroll_anim_val = scroll_to;
            scroll_anim = scroll_to;
            menu_anim_start = current_time;
            anim_phase = 1;
          }
        } else if (anim_phase == 1) {
          // Q弹选中框阶段，菜单内容已到位，选中框Q弹到目标
          if (t < anim_time) {
            float k = t / (float)anim_time;
            float c4 = (2 * 3.1415926f) / 3.0f;
            float elastic_k = (k == 0.0f) ? 0.0f : (k == 1.0f) ? 1.0f : powf(2, -10 * k) * sinf((k * 10 - 0.75f) * c4) + 1.0f;
            scroll_anim = scroll_to;
            y_anim = y_from + (y_to - y_from) * elastic_k;
          } else {
            scroll_anim = scroll_to;
            y_anim = y_to;
            animating = 0;
            anim_phase = 0;
            last_switch_time = current_time;
          }
        }
      } else {
        scroll_anim = scroll_to;
        scroll_anim_val = scroll_to;
        // 选中框静止
        int sel_in_win_now = 0;
        if (menu_sel < 1) sel_in_win_now = menu_sel;
        else if (menu_sel > menu_count - menu_show) sel_in_win_now = menu_sel - (menu_count - menu_show);
        else sel_in_win_now = 1;
        y_anim = menu_y + sel_in_win_now * (menu_h + menu_gap);
        anim_phase = 0;
      }
      // 绘制菜单项（只绘制窗口内，支持float滑动）
      for (int i = 0; i < menu_count; i++) {
        float y = menu_y + (i - scroll_anim) * (menu_h + menu_gap);
        if (y + menu_h < menu_y || y > menu_y + (menu_h + menu_gap) * (menu_show - 1) + menu_h) continue;
        char num[3];
        snprintf(num, sizeof(num), "%d.", i + 1);
        u8g2_DrawStr(&u8g2, menu_x, (int)y + menu_h, num);
        u8g2_DrawStr(&u8g2, menu_x + 16, (int)y + menu_h, menu_items[i]);
      }
      // 绘制自适应选中框
      int sel_w = u8g2_GetStrWidth(&u8g2, menu_items[menu_sel]);
      u8g2_SetDrawColor(&u8g2, 1);
      u8g2_DrawBox(&u8g2, menu_x + 16 - 2, y_anim + 1, sel_w + 4, menu_h + 1);
      u8g2_SetDrawColor(&u8g2, 0);
      u8g2_DrawStr(&u8g2, menu_x + 16, y_anim + menu_h, menu_items[menu_sel]);
      u8g2_SetDrawColor(&u8g2, 1);
      // 外边框
      int menu_w = 16 + 2 + 60; // 16:序号+间隔, 2:选中框边距, 60:最大内容宽度预留
      u8g2_DrawFrame(&u8g2, menu_x - 2, menu_y - 2, menu_w + 8, (menu_h + menu_gap) * menu_show - menu_gap + 8);
    }

    u8g2_SendBuffer(&u8g2);

    frame_count++;
    x--;
    if (x < -str_width) x = 128;
    progress++;
    if (progress > progress_max) progress = 0;
    // HAL_Delay(10); // 可选
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
     line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
