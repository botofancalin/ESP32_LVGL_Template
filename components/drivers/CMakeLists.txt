set(COMPONENT_ADD_INCLUDEDIRS . "include" ./param/include ./display/ILI9341/include ./input/touch/XPT2046/include)

# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES driver spi_flash)
set(COMPONENT_PRIV_REQUIRES freertos lvgl_gui)

set(COMPONENT_SRCS ./param/param.c ./display/ILI9341/spi_lcd.c ./input/touch/XPT2046/iot_xpt.c ./display/ILI9341/iot_lcd.cpp ./display/ILI9341/ILI9341_adapter.cpp ./input/touch/XPT2046/xpt2046_obj.cpp ./input/touch/XPT2046/XPT2046_adapter.cpp)

register_component()
