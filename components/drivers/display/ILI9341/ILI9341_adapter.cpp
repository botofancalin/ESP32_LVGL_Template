// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/* C Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* RTOS Includes */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"

/* SPI Includes */
#include "ILI9341.h"
#include "iot_lcd.h"

/* System Includes */
#include "esp_log.h"

/* uGFX Config Include */
#include "sdkconfig.h"

class CEspLcdAdapter : public CEspLcd
{
public:
    const uint16_t *pFrameBuffer = NULL;
    CEspLcdAdapter(lcd_conf_t *lcd_conf, int height = LCD_TFTHEIGHT, int width = LCD_TFTWIDTH, bool dma_en = true, int dma_word_size = 1024, int dma_chan = 1) : CEspLcd(lcd_conf, height, width, dma_en, dma_word_size, dma_chan)
    {
        /* Code here*/
    }
    void inline writeCmd(uint8_t cmd)
    {
        transmitCmd(cmd);
    }
    void inline writeData(uint16_t data)
    {
        transmitData(data);
    }
    void inline writeData(uint8_t data)
    {
        transmitData(data);
    }
    void inline writeData(uint16_t data, int point_num)
    {
        transmitData(data, point_num);
    }
    void inline writeCmdData(uint8_t cmd, uint32_t data)
    {
        transmitCmdData(cmd, data);
    }
    void inline writeData(uint8_t *data, uint16_t length)
    {
        transmitData(data, length);
    }
    void inline readData()
    {
        /* Code here*/
    }
};

static CEspLcdAdapter *lcd_obj = NULL;

void board_lcd_write_cmd(uint8_t cmd)
{
    lcd_obj->writeCmd(cmd);
}

void board_lcd_write_data(uint16_t data)
{
    lcd_obj->writeData(data);
}

void board_lcd_write_data_byte(uint8_t data)
{
    lcd_obj->writeData(data);
}

void board_lcd_write_data_byte_repeat(uint16_t data, int point_num)
{
    lcd_obj->writeData(data, point_num);
}

void board_lcd_write_cmddata(uint8_t cmd, uint32_t data)
{
    lcd_obj->writeCmdData(cmd, data);
}

/* lvgl include */
#include "lvgl_disp_config.h"

/*Write the internal buffer (VDB) to the display. 'lv_flush_ready()' has to be called when finished*/
void ex_disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t *color_p)
{
    lcd_obj->drawBitmap((int16_t)x1, (int16_t)y1, (const uint16_t *)color_p, (int16_t)(x2 - x1 + 1), (int16_t)(y2 - y1 + 1));
    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_flush_ready();
}

/*Fill an area with a color on the display*/
void ex_disp_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color)
{
    lcd_obj->fillRect((int16_t)x1, (int16_t)y1, (int16_t)(x2 - x1 + 1), (int16_t)(y2 - y1 + 1), (uint16_t)color.full);
}

/*Write pixel map (e.g. image) to the display*/
void ex_disp_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t *color_p)
{
    lcd_obj->drawBitmap((int16_t)x1, (int16_t)y1, (const uint16_t *)color_p, (int16_t)(x2 - x1 + 1), (int16_t)(y2 - y1 + 1));
}

lv_disp_drv_t lvgl_lcd_display_init()
{
    /*Initialize LCD*/
    lcd_conf_t lcd_pins = {
        .lcd_model = LCD_MOD_AUTO_DET,
        .pin_num_miso = ILI9341_MISO,
        .pin_num_mosi = ILI9341_MOSI,
        .pin_num_clk = ILI9341_CLK,
        .pin_num_cs = ILI9341_CS,
        .pin_num_dc = ILI9341_DC,
        .pin_num_rst = -1,
        .pin_num_bckl = ILI9341_BCKL,
        .clk_freq = SPI_FREQ * 1000 * 1000,
        .rst_active_level = 0,
        .bckl_active_level = ILI9341_BCKL_ON,
        .spi_host = (spi_host_device_t)LVGL_LCD_SPI,
        .init_spi_bus = true,
    };

    if (lcd_obj == NULL) {
        lcd_obj = new CEspLcdAdapter(&lcd_pins, LV_VER_RES, LV_HOR_RES);
    }

    lv_disp_drv_t disp_drv;      /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/

#ifdef CONFIG_LVGL_DISP_ROTATE_0
    board_lcd_write_cmd(ILI9341_MEMACCESS_REG);
    board_lcd_write_data_byte(0x80 | 0x08);
    ESP_LOGI("lvgl_example", "CONFIG_LVGL_DISP_ROTATE_0");
#elif defined(CONFIG_LVGL_DISP_ROTATE_90)
    board_lcd_write_cmd(ILI9341_MEMACCESS_REG);
    board_lcd_write_data_byte(0x20 | 0x08);
    ESP_LOGI("lvgl_example", "CONFIG_LVGL_DISP_ROTATE_90");
#elif defined(CONFIG_LVGL_DISP_ROTATE_180)
    board_lcd_write_cmd(ILI9341_MEMACCESS_REG);
    board_lcd_write_data_byte(0x40 | 0x08);
    ESP_LOGI("lvgl_example", "CONFIG_LVGL_DISP_ROTATE_180");
#elif defined(CONFIG_LVGL_DISP_ROTATE_270)
    board_lcd_write_cmd(ILI9341_MEMACCESS_REG);
    board_lcd_write_data_byte(0xE0 | 0x08);
    ESP_LOGI("lvgl_example", "CONFIG_LVGL_DISP_ROTATE_270");
#endif

    /* Set up the functions to access to your display */
    if (LV_VDB_SIZE != 0) {
        disp_drv.disp_flush = ex_disp_flush; /*Used in buffered mode (LV_VDB_SIZE != 0  in lv_conf.h)*/
    } else if (LV_VDB_SIZE == 0) {
        disp_drv.disp_fill = ex_disp_fill; /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/
        disp_drv.disp_map = ex_disp_map;   /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/
    }

    /* Finally register the driver */
    lv_disp_drv_register(&disp_drv);
    return disp_drv;
}
