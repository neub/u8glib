/*

  >>> PLEASE UNCOMMENT DISPLAY TYPE IN THE INIT PROCEDURE <<<

*/

#include "u8g_linux.h"

#include <stdio.h>
#include <stdlib.h>

/*========================================================================*/
/*
  The following delay procedures must be implemented for u8glib

  void u8g_Delay(uint16_t val)		Delay by "val" milliseconds
  void u8g_MicroDelay(void)		Delay be one microsecond
  void u8g_10MicroDelay(void)	Delay by 10 microseconds

*/
void u8g_Delay(uint16_t val)
{
  usleep(1000UL*(uint32_t)val);
}

void u8g_MicroDelay(void)
{
  usleep(1);
}

void u8g_10MicroDelay(void)
{
  usleep(10);
}

void u8g_linux_Init(u8g_t *u8g, int argc, const char *argv[])
{
  uint8_t ok = 255;
  uint8_t i2c_bus;

  if (argc <= 1) {
    fprintf(stderr, "Usage: %s I2CBUS\n", argv[0]);
    exit(1);
  }

  i2c_bus = atoi(argv[1]);

  /*
    Please uncomment one of the displays below
    Notes:
      - "2x": high speed version, which uses more RAM
  */

  // ok = u8g_InitI2C(u8g, &u8g_dev_ssd1306_128x32_2x_i2c, i2c_bus);
  // ok = u8g_InitI2C(u8g, &u8g_dev_ssd1306_128x32_i2c, i2c_bus);
  // ok = u8g_InitI2C(u8g, &u8g_dev_ssd1306_128x64_2x_i2c, i2c_bus);
  // ok = u8g_InitI2C(u8g, &u8g_dev_ssd1306_128x64_i2c, i2c_bus);
  // ok = u8g_InitI2C(u8g, &u8g_dev_ssd1306_64x48_2x_i2c, i2c_bus);
  // ok = u8g_InitI2C(u8g, &u8g_dev_ssd1306_64x48_i2c, i2c_bus);
  // ok = u8g_InitI2C(u8g, &u8g_dev_ssd1306_adafruit_128x64_i2c, i2c_bus);
  // ok = u8g_InitI2C(u8g, &u8g_dev_ssd1309_128x64_i2c, i2c_bus);
  // ok = u8g_InitI2C(u8g, &u8g_dev_ssd1327_96x96_2x_gr_i2c, i2c_bus);
  // ok = u8g_InitI2C(u8g, &u8g_dev_ssd1327_96x96_gr_i2c, i2c_bus);

  /* flip screen, if required */
  // u8g_SetRot180(u8g);

  if (ok == 255) {
    fprintf(stderr, "No display driver selected (see src/linux/common/init.c)\n");
    exit(2);
  }

  if (!ok)
    exit(3);
}
