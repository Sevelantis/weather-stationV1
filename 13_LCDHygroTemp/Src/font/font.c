/*
 7-17-2011
 Spark Fun Electronics 2011
 Nathan Seidle

 This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 This code writes a series of images and text to the Nokia 5110 84x48 graphic LCD:
 http://www.sparkfun.com/products/10168

 Do not drive the backlight with 5V. It will smoke. However, the backlight on the LCD seems to be
 happy with direct drive from the 3.3V regulator.
 Although the PCD8544 controller datasheet recommends 3.3V, the graphic Nokia 5110 LCD can run at 3.3V or 5V.
 No resistors needed on the signal lines.

 You will need 5 signal lines to connect to the LCD, 3.3 or 5V for power, 3.3V for LED backlight, and 1 for ground.
 */

#include "font.h"

//This table contains the hex values that represent pixels
//for a font that is 5 pixels wide and 8 pixels high
const uint8_t font5x8[][5] = {
  {0x00, 0x00, 0x00, 0x00, 0x00} // 20
  ,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
  ,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
  ,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
  ,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
  ,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
  ,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
  ,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
  ,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
  ,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
  ,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
  ,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
  ,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
  ,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
  ,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
  ,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
  ,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
  ,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
  ,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
  ,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
  ,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
  ,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
  ,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
  ,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
  ,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
  ,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
  ,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
  ,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
  ,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
  ,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
  ,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
  ,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
  ,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
  ,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
  ,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
  ,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
  ,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
  ,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
  ,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
  ,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
  ,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
  ,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
  ,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
  ,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
  ,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
  ,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
  ,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
  ,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
  ,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
  ,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
  ,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
  ,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
  ,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
  ,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
  ,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
  ,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
  ,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
  ,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
  ,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
  ,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
  ,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c
  ,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
  ,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
  ,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
  ,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
  ,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
  ,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
  ,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
  ,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
  ,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
  ,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
  ,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
  ,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
  ,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
  ,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
  ,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
  ,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
  ,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
  ,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
  ,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
  ,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
  ,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
  ,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
  ,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
  ,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
  ,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
  ,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
  ,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
  ,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
  ,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
  ,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
  ,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
  ,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
  ,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
  ,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ~
  ,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f DEL
};

const uint8_t font8x14[256][16]={
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x00
		{0x00,0x07,0x80,0x08,0x40,0x15,0x40,0x14,0x40,0x14,0x40,0x15,0x80,0x08,0x00,0x07},	// 0x01
		{0x00,0x07,0x80,0x0F,0xC0,0x12,0xC0,0x17,0xC0,0x17,0xC0,0x12,0x80,0x0F,0x00,0x07},	// 0x02
		{0xF0,0x00,0xF8,0x03,0xF8,0x0F,0xF0,0x1F,0xF0,0x1F,0xF8,0x0F,0xF8,0x03,0xF0,0x00},	// 0x03
		{0x80,0x00,0xC0,0x01,0xE0,0x03,0xF0,0x07,0xF8,0x1F,0xE0,0x07,0xC0,0x01,0x80,0x00},	// 0x04
		{0x80,0x01,0xC0,0x03,0xF0,0x03,0xF8,0x1F,0xF8,0x1D,0xF0,0x03,0xC0,0x03,0x80,0x01},	// 0x05
		{0x00,0x03,0x80,0x07,0xC0,0x07,0xF0,0x1F,0xF8,0x1B,0xE0,0x07,0xC0,0x07,0x80,0x03},	// 0x06
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x07
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x08
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x09
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x0A
		{0x00,0x07,0x80,0x08,0x40,0x10,0x40,0x10,0x50,0x10,0xE8,0x08,0x18,0x07,0x30,0x00},	// 0x0B
		{0xE0,0x00,0x10,0x09,0x08,0x0A,0x08,0x1E,0x08,0x0A,0x10,0x09,0xE0,0x00,0x00,0x00},	// 0x0C
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x0D
		{0x00,0x60,0x00,0x70,0x00,0x70,0xE0,0x3F,0x30,0x18,0x10,0x18,0xF8,0x0F,0x00,0x00},	// 0x0E
		{0x00,0x01,0xC0,0x07,0xC0,0x06,0x60,0x0C,0x40,0x04,0x40,0x04,0xC0,0x07,0x00,0x01},	// 0x0F
		{0xE0,0x1F,0xC0,0x0F,0xC0,0x0F,0x80,0x07,0x80,0x03,0x80,0x03,0x00,0x01,0x00,0x01},	// 0x10
		{0x00,0x01,0x00,0x01,0x80,0x03,0x80,0x03,0x80,0x07,0xC0,0x0F,0xC0,0x0F,0xE0,0x1F},	// 0x11
		{0x00,0x00,0x00,0x00,0x20,0x10,0x10,0x20,0xF8,0x7F,0x10,0x20,0x20,0x10,0x00,0x00},	// 0x12
		{0x00,0x00,0x00,0x00,0xF8,0x1B,0x00,0x00,0x00,0x00,0xF8,0x1B,0x00,0x00,0x00,0x00},	// 0x13
		{0x70,0x00,0xF8,0x00,0xF8,0x00,0xF8,0x7F,0x08,0x00,0x08,0x00,0xF8,0x7F,0x00,0x00},	// 0x14
		{0x00,0x00,0x00,0x00,0xF0,0x63,0x48,0x46,0xC8,0x44,0x88,0x4C,0x08,0x3B,0x00,0x00},	// 0x15
		{0x00,0x00,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x00,0x00},	// 0x16
		{0x00,0x00,0x00,0x00,0x10,0x88,0x08,0x90,0xFC,0xBF,0x08,0x90,0x10,0x88,0x00,0x00},	// 0x17
		{0x00,0x00,0x00,0x00,0x20,0x00,0x10,0x00,0xF8,0x7F,0x10,0x00,0x20,0x00,0x00,0x00},	// 0x18
		{0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x20,0xF8,0x7F,0x00,0x20,0x00,0x10,0x00,0x00},	// 0x19
		{0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x01,0x40,0x05,0x80,0x03,0x00,0x01,0x00,0x00},	// 0x1A
		{0x00,0x00,0x00,0x01,0x80,0x03,0x40,0x05,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x00},	// 0x1B
		{0x00,0x00,0xC0,0x1F,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x00},	// 0x1C
		{0x00,0x00,0x00,0x01,0x80,0x03,0x00,0x01,0x00,0x01,0x80,0x03,0x00,0x01,0x00,0x00},	// 0x1D
		{0x00,0x10,0x00,0x1C,0x00,0x1E,0x80,0x1F,0xE0,0x1F,0x80,0x1F,0x00,0x1C,0x00,0x10},	// 0x1E
		{0x20,0x00,0xE0,0x00,0xE0,0x01,0xE0,0x07,0xE0,0x1F,0xE0,0x07,0xE0,0x00,0x20,0x00},	// 0x1F
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x20
		{0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x1B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x21
		{0x00,0x00,0x00,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x3C,0x00,0x00,0x00,0x00,0x00},	// 0x22
		{0x00,0x02,0x40,0x1A,0x40,0x07,0xE0,0x02,0x58,0x1A,0x40,0x07,0xE0,0x02,0x58,0x02},	// 0x23
		{0x00,0x00,0x70,0x10,0x88,0x10,0xFC,0x3F,0x08,0x11,0x08,0x0E,0x00,0x00,0x00,0x00},	// 0x24
		{0x70,0x10,0x88,0x08,0x88,0x06,0x70,0x01,0x80,0x0E,0x60,0x11,0x10,0x11,0x08,0x0E},	// 0x25
		{0x00,0x0F,0x00,0x19,0xF0,0x10,0x88,0x11,0x48,0x16,0x30,0x1C,0x00,0x1C,0x00,0x13},	// 0x26
		{0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x27
		{0x00,0x00,0x00,0x00,0xC0,0x07,0x30,0x18,0x08,0x20,0x0C,0x60,0x04,0x40,0x00,0x00},	// 0x28
		{0x00,0x00,0x04,0x40,0x0C,0x60,0x08,0x20,0x30,0x18,0xC0,0x07,0x00,0x00,0x00,0x00},	// 0x29
		{0x00,0x00,0x20,0x00,0xE0,0x01,0x98,0x00,0x40,0x00,0xA0,0x01,0x20,0x00,0x00,0x00},	// 0x2A
		{0x00,0x02,0x00,0x02,0x00,0x02,0xC0,0x1F,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x00},	// 0x2B
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x58,0x00,0x38,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x2C
		{0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x00},	// 0x2D
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x2E
		{0x00,0x00,0x00,0x40,0x00,0x38,0x00,0x07,0xC0,0x01,0x38,0x00,0x04,0x00,0x00,0x00},	// 0x2F
		{0x00,0x00,0xE0,0x07,0x10,0x08,0x08,0x10,0x08,0x10,0x10,0x08,0xE0,0x07,0x00,0x00},	// 0x30
		{0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0xF8,0x1F,0x00,0x10,0x00,0x10,0x00,0x10},	// 0x31
		{0x00,0x00,0x08,0x18,0x08,0x14,0x08,0x12,0x88,0x11,0x70,0x10,0x00,0x00,0x00,0x00},	// 0x32
		{0x00,0x00,0x00,0x00,0x08,0x10,0x88,0x10,0x88,0x10,0x88,0x10,0x70,0x0F,0x00,0x00},	// 0x33
		{0x00,0x00,0x00,0x03,0x80,0x02,0x60,0x02,0x10,0x02,0xF8,0x1F,0x00,0x02,0x00,0x00},	// 0x34
		{0x00,0x00,0x00,0x00,0xF8,0x10,0x88,0x10,0x88,0x10,0x08,0x11,0x08,0x0F,0x00,0x00},	// 0x35
		{0x00,0x00,0xE0,0x07,0x10,0x09,0x88,0x10,0x88,0x10,0x88,0x10,0x00,0x0F,0x00,0x00},	// 0x36
		{0x00,0x00,0x08,0x00,0x08,0x18,0x08,0x07,0x88,0x00,0x68,0x00,0x18,0x00,0x00,0x00},	// 0x37
		{0x00,0x00,0x70,0x0E,0x88,0x11,0x88,0x10,0x88,0x10,0x48,0x11,0x30,0x0E,0x00,0x00},	// 0x38
		{0x00,0x00,0xF0,0x00,0x08,0x11,0x08,0x11,0x08,0x11,0x90,0x08,0xE0,0x07,0x00,0x00},	// 0x39
		{0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x18,0xC0,0x18,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x3A
		{0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x58,0xC0,0x38,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x3B
		{0x00,0x00,0x00,0x02,0x00,0x02,0x00,0x05,0x80,0x08,0x80,0x08,0x40,0x10,0x00,0x00},	// 0x3C
		{0x00,0x00,0x80,0x04,0x80,0x04,0x80,0x04,0x80,0x04,0x80,0x04,0x80,0x04,0x00,0x00},	// 0x3D
		{0x00,0x00,0x40,0x10,0x80,0x08,0x80,0x08,0x00,0x05,0x00,0x02,0x00,0x02,0x00,0x00},	// 0x3E
		{0x00,0x00,0x18,0x00,0x08,0x00,0x08,0x1B,0x88,0x00,0x48,0x00,0x30,0x00,0x00,0x00},	// 0x3F
		{0xC0,0x07,0x30,0x0C,0x18,0x10,0xC8,0x17,0x28,0x14,0x28,0x1A,0xF0,0x07,0x00,0x04},	// 0x40
		{0x00,0x10,0x00,0x0E,0x80,0x05,0x60,0x04,0x30,0x04,0xC0,0x05,0x00,0x0E,0x00,0x10},	// 0x41
		{0x00,0x00,0xF0,0x1F,0x10,0x11,0x10,0x11,0x10,0x11,0x90,0x12,0x60,0x0C,0x00,0x00},	// 0x42
		{0xC0,0x07,0x20,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},	// 0x43
		{0xF0,0x1F,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x20,0x08,0xC0,0x07,0x00,0x00},	// 0x44
		{0x00,0x00,0xF0,0x1F,0x10,0x11,0x10,0x11,0x10,0x11,0x10,0x11,0x10,0x10,0x00,0x00},	// 0x45
		{0x00,0x00,0xF0,0x1F,0x10,0x01,0x10,0x01,0x10,0x01,0x10,0x01,0x10,0x00,0x00,0x00},	// 0x46
		{0xC0,0x07,0x20,0x08,0x10,0x10,0x10,0x10,0x10,0x12,0x10,0x12,0x10,0x1E,0x00,0x00},	// 0x47
		{0x00,0x00,0xF0,0x1F,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0xF0,0x1F,0x00,0x00},	// 0x48
		{0x00,0x00,0x10,0x10,0x10,0x10,0xF0,0x1F,0x10,0x10,0x10,0x10,0x00,0x00,0x00,0x00},	// 0x49
		{0x00,0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0xF0,0x0F,0x00,0x00,0x00,0x00},	// 0x4A
		{0x00,0x00,0xF0,0x1F,0x00,0x01,0x80,0x02,0x40,0x04,0x20,0x08,0x10,0x10,0x00,0x00},	// 0x4B
		{0x00,0x00,0xF0,0x1F,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x00},	// 0x4C
		{0xF0,0x1F,0x70,0x00,0x80,0x03,0x00,0x04,0x80,0x03,0x70,0x00,0xF0,0x1F,0x00,0x00},	// 0x4D
		{0x00,0x00,0xF0,0x1F,0x60,0x00,0x80,0x01,0x00,0x06,0x00,0x08,0xF0,0x1F,0x00,0x00},	// 0x4E
		{0xC0,0x07,0x20,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x20,0x08,0xC0,0x07,0x00,0x00},	// 0x4F
		{0x00,0x00,0xF0,0x1F,0x10,0x02,0x10,0x02,0x10,0x02,0x10,0x01,0xE0,0x01,0x00,0x00},	// 0x50
		{0xC0,0x07,0x20,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x20,0x28,0xC0,0x67,0x00,0x40},	// 0x51
		{0x00,0x00,0xF0,0x1F,0x10,0x01,0x10,0x01,0x10,0x07,0xE0,0x0C,0x00,0x10,0x00,0x00},	// 0x52
		{0x00,0x00,0xE0,0x10,0x90,0x10,0x10,0x11,0x10,0x11,0x10,0x12,0x10,0x0E,0x00,0x00},	// 0x53
		{0x10,0x00,0x10,0x00,0x10,0x00,0xF0,0x1F,0x10,0x00,0x10,0x00,0x10,0x00,0x00,0x00},	// 0x54
		{0x00,0x00,0xF0,0x0F,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0xF0,0x0F,0x00,0x00},	// 0x55
		{0x10,0x00,0xE0,0x00,0x00,0x07,0x00,0x18,0x00,0x06,0x80,0x01,0x60,0x00,0x10,0x00},	// 0x56
		{0xF0,0x00,0x00,0x0F,0x00,0x1C,0xC0,0x03,0x80,0x03,0x00,0x1C,0xC0,0x0F,0x70,0x00},	// 0x57
		{0x10,0x10,0x20,0x08,0x40,0x04,0x80,0x03,0x80,0x03,0x40,0x04,0x20,0x08,0x10,0x10},	// 0x58
		{0x10,0x00,0x60,0x00,0x80,0x00,0x00,0x1F,0x80,0x01,0x40,0x00,0x20,0x00,0x10,0x00},	// 0x59
		{0x10,0x18,0x10,0x14,0x10,0x12,0x10,0x11,0x90,0x10,0x50,0x10,0x30,0x10,0x00,0x00},	// 0x5A
		{0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x7F,0x04,0x40,0x04,0x40,0x04,0x40,0x00,0x00},	// 0x5B
		{0x00,0x00,0x04,0x00,0x38,0x00,0xC0,0x01,0x00,0x07,0x00,0x38,0x00,0x40,0x00,0x00},	// 0x5C
		{0x00,0x00,0x04,0x40,0x04,0x40,0x04,0x40,0xFC,0x7F,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x5D
		{0x00,0x00,0x00,0x04,0x00,0x03,0xE0,0x00,0x38,0x00,0xC0,0x01,0x00,0x06,0x00,0x00},	// 0x5E
		{0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20},	// 0x5F
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x08,0x00,0x00,0x00,0x00,0x00},	// 0x60
		{0x00,0x00,0x00,0x0C,0x40,0x12,0x40,0x12,0x40,0x12,0x40,0x12,0x80,0x1F,0x00,0x10},	// 0x61
		{0x00,0x00,0xFC,0x1F,0x80,0x08,0x40,0x10,0x40,0x10,0x40,0x10,0x80,0x0F,0x00,0x00},	// 0x62
		{0x00,0x00,0x00,0x07,0x80,0x08,0x40,0x10,0x40,0x10,0x40,0x10,0x40,0x10,0x00,0x00},	// 0x63
		{0x00,0x00,0x80,0x0F,0x40,0x10,0x40,0x10,0x40,0x10,0x80,0x08,0xFC,0x1F,0x00,0x00},	// 0x64
		{0x00,0x00,0x00,0x0F,0xC0,0x12,0x40,0x12,0x40,0x12,0x40,0x12,0x80,0x13,0x00,0x00},	// 0x65
		{0x00,0x00,0x40,0x00,0x40,0x00,0xF8,0x1F,0x44,0x00,0x44,0x00,0x44,0x00,0x00,0x00},	// 0x66
		{0x00,0x00,0x80,0x0F,0x40,0x90,0x40,0x90,0x40,0x90,0xC0,0x88,0xC0,0x7F,0x00,0x00},	// 0x67
		{0x00,0x00,0xFC,0x1F,0x80,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x80,0x1F,0x00,0x00},	// 0x68
		{0x00,0x00,0x40,0x00,0x40,0x00,0x4C,0x00,0xCC,0x1F,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x69
		{0x00,0x00,0x00,0x80,0x40,0x80,0x40,0x80,0x4C,0x80,0xCC,0x7F,0x00,0x00,0x00,0x00},	// 0x6A
		{0x00,0x00,0xFC,0x1F,0x00,0x02,0x00,0x05,0x80,0x04,0x40,0x08,0x00,0x10,0x00,0x00},	// 0x6B
		{0x00,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0xFC,0x1F,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x6C
		{0xC0,0x1F,0x80,0x00,0x40,0x00,0xC0,0x1F,0x80,0x00,0x40,0x00,0xC0,0x1F,0x00,0x00},	// 0x6D
		{0x00,0x00,0xC0,0x1F,0x80,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x80,0x1F,0x00,0x00},	// 0x6E
		{0x00,0x07,0x80,0x08,0x40,0x10,0x40,0x10,0x40,0x10,0x80,0x08,0x00,0x07,0x00,0x00},	// 0x6F
		{0x00,0x00,0xC0,0xFF,0x80,0x08,0x40,0x10,0x40,0x10,0x40,0x10,0x80,0x0F,0x00,0x00},	// 0x70
		{0x00,0x00,0x80,0x0F,0x40,0x10,0x40,0x10,0x40,0x10,0x80,0x08,0xC0,0xFF,0x00,0x00},	// 0x71
		{0x00,0x00,0xC0,0x1F,0x80,0x00,0x40,0x00,0x40,0x00,0xC0,0x00,0x00,0x00,0x00,0x00},	// 0x72
		{0x00,0x00,0x80,0x11,0x40,0x11,0x40,0x12,0x40,0x12,0x40,0x0C,0x00,0x00,0x00,0x00},	// 0x73
		{0x40,0x00,0x40,0x00,0xF0,0x0F,0x40,0x10,0x40,0x10,0x40,0x10,0x40,0x10,0x00,0x00},	// 0x74
		{0x00,0x00,0xC0,0x0F,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x08,0xC0,0x1F,0x00,0x00},	// 0x75
		{0x40,0x00,0x80,0x03,0x00,0x0C,0x00,0x10,0x00,0x0C,0x00,0x02,0x80,0x01,0x40,0x00},	// 0x76
		{0xC0,0x00,0x00,0x07,0x00,0x1C,0x00,0x03,0x80,0x03,0x00,0x1C,0x00,0x07,0xC0,0x00},	// 0x77
		{0x00,0x00,0x40,0x10,0x80,0x08,0x00,0x07,0x00,0x07,0x80,0x08,0x40,0x10,0x00,0x00},	// 0x78
		{0x40,0x80,0x80,0x81,0x00,0xC6,0x00,0x78,0x00,0x18,0x00,0x06,0x80,0x01,0x40,0x00},	// 0x79
		{0x40,0x10,0x40,0x18,0x40,0x14,0x40,0x12,0x40,0x11,0xC0,0x10,0x40,0x10,0x00,0x00},	// 0x7A
		{0x00,0x00,0x00,0x01,0x00,0x01,0xF8,0x3E,0x04,0x40,0x04,0x40,0x04,0x40,0x00,0x00},	// 0x7B
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x7F,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x7C
		{0x00,0x00,0x04,0x40,0x04,0x40,0x04,0x40,0xF8,0x3E,0x00,0x01,0x00,0x01,0x00,0x00},	// 0x7D
		{0x00,0x03,0x80,0x00,0x80,0x00,0x00,0x01,0x00,0x01,0x00,0x02,0x00,0x02,0x80,0x01},	// 0x7E
		{0x00,0x1F,0x80,0x10,0x40,0x10,0x60,0x10,0x40,0x10,0x80,0x10,0x00,0x1F,0x00,0x00},	// 0x7F
		{0xC0,0x07,0x20,0x08,0x10,0x10,0x10,0x10,0x10,0xB0,0x10,0xD0,0x10,0x10,0x00,0x00},	// 0x80
		{0x00,0x00,0xC0,0x0F,0x08,0x10,0x00,0x10,0x00,0x10,0x08,0x08,0xC0,0x1F,0x00,0x00},	// 0x81
		{0x00,0x00,0x00,0x0F,0xC0,0x12,0x40,0x12,0x48,0x12,0x44,0x12,0x80,0x13,0x00,0x00},	// 0x82
		{0x00,0x00,0x00,0x0C,0x48,0x12,0x44,0x12,0x44,0x12,0x48,0x12,0x80,0x1F,0x00,0x10},	// 0x83
		{0x00,0x00,0x00,0x0C,0x48,0x12,0x40,0x12,0x40,0x12,0x48,0x12,0x80,0x1F,0x00,0x10},	// 0x84
		{0x00,0x00,0x00,0x0C,0x40,0x12,0x44,0x12,0x48,0x12,0x40,0x12,0x80,0x1F,0x00,0x10},	// 0x85
		{0x00,0x00,0x00,0x0C,0x40,0x12,0x48,0x12,0x54,0x12,0x48,0x12,0x80,0x1F,0x00,0x10},	// 0x86
		{0x00,0x00,0x00,0x07,0x80,0x08,0x40,0xB0,0x40,0xD0,0x40,0x10,0x40,0x10,0x00,0x00},	// 0x87
		{0x00,0x00,0x00,0x0F,0xC8,0x12,0x44,0x12,0x44,0x12,0x48,0x12,0x80,0x13,0x00,0x00},	// 0x88
		{0x00,0x00,0x00,0x0F,0xC0,0x12,0x48,0x12,0x40,0x12,0x40,0x12,0x88,0x13,0x00,0x00},	// 0x89
		{0x00,0x00,0x00,0x0F,0xC0,0x12,0x44,0x12,0x48,0x12,0x40,0x12,0x80,0x13,0x00,0x00},	// 0x8A
		{0x00,0x00,0x40,0x00,0x48,0x00,0x40,0x00,0xC0,0x1F,0x08,0x00,0x00,0x00,0x00,0x00},	// 0x8B
		{0x00,0x00,0x40,0x00,0x48,0x00,0x44,0x00,0xC4,0x1F,0x08,0x00,0x00,0x00,0x00,0x00},	// 0x8C
		{0x00,0x00,0x40,0x00,0x40,0x00,0x44,0x00,0xC8,0x1F,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x8D
		{0x00,0x10,0x00,0x0E,0x84,0x05,0x60,0x04,0x30,0x04,0xC4,0x05,0x00,0x0E,0x00,0x10},	// 0x8E
		{0x00,0x18,0x00,0x0F,0xE8,0x04,0x34,0x04,0xE8,0x04,0x80,0x07,0x00,0x0C,0x00,0x10},	// 0x8F
		{0x00,0x00,0xF0,0x1F,0x10,0x11,0x10,0x11,0x18,0x11,0x14,0x11,0x10,0x10,0x00,0x00},	// 0x90
		{0x40,0x0C,0x40,0x12,0x40,0x12,0x80,0x0F,0x40,0x12,0x40,0x12,0x40,0x12,0x80,0x13},	// 0x91
		{0x00,0x10,0x00,0x0E,0x80,0x05,0x60,0x04,0xF0,0x1F,0x10,0x11,0x10,0x11,0x10,0x10},	// 0x92
		{0x00,0x07,0x80,0x08,0x48,0x10,0x44,0x10,0x44,0x10,0x88,0x08,0x00,0x07,0x00,0x00},	// 0x93
		{0x00,0x07,0x80,0x08,0x48,0x10,0x40,0x10,0x40,0x10,0x88,0x08,0x00,0x07,0x00,0x00},	// 0x94
		{0x00,0x07,0x80,0x08,0x44,0x10,0x48,0x10,0x40,0x10,0x80,0x08,0x00,0x07,0x00,0x00},	// 0x95
		{0x00,0x00,0xC0,0x0F,0x08,0x10,0x04,0x10,0x04,0x10,0x08,0x08,0xC0,0x1F,0x00,0x00},	// 0x96
		{0x00,0x00,0xC0,0x0F,0x04,0x10,0x08,0x10,0x00,0x10,0x00,0x08,0xC0,0x1F,0x00,0x00},	// 0x97
		{0x40,0x80,0x80,0x81,0x08,0xC6,0x00,0x78,0x00,0x18,0x08,0x06,0x80,0x01,0x40,0x00},	// 0x98
		{0xC0,0x07,0x20,0x08,0x14,0x10,0x10,0x10,0x10,0x10,0x24,0x08,0xC0,0x07,0x00,0x00},	// 0x99
		{0x00,0x00,0xF0,0x0F,0x04,0x10,0x00,0x10,0x00,0x10,0x04,0x10,0xF0,0x0F,0x00,0x00},	// 0x9A
		{0x80,0x17,0xC0,0x18,0x40,0x14,0x40,0x12,0x40,0x11,0xC0,0x08,0xC0,0x07,0x00,0x00},	// 0x9B
		{0x00,0x00,0x00,0x00,0x80,0x18,0xF0,0x17,0x88,0x10,0x08,0x10,0x08,0x10,0x00,0x00},	// 0x9C
		{0xC0,0x17,0x20,0x0C,0x10,0x12,0x10,0x11,0x90,0x10,0x60,0x08,0xD0,0x07,0x00,0x00},	// 0x9D
		{0x40,0x10,0x80,0x08,0x00,0x05,0x00,0x02,0x00,0x05,0x80,0x08,0x40,0x10,0x00,0x00},	// 0x9E
		{0x00,0x80,0x00,0x80,0x40,0x80,0xF8,0x7F,0x44,0x00,0x44,0x00,0x04,0x00,0x00,0x00},	// 0x9F
		{0x00,0x00,0x00,0x0C,0x40,0x12,0x40,0x12,0x48,0x12,0x44,0x12,0x80,0x1F,0x00,0x10},	// 0xA0
		{0x00,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0xC8,0x1F,0x04,0x00,0x00,0x00,0x00,0x00},	// 0xA1
		{0x00,0x07,0x80,0x08,0x40,0x10,0x40,0x10,0x48,0x10,0x84,0x08,0x00,0x07,0x00,0x00},	// 0xA2
		{0x00,0x00,0xC0,0x0F,0x00,0x10,0x00,0x10,0x08,0x10,0x04,0x08,0xC0,0x1F,0x00,0x00},	// 0xA3
		{0x00,0x00,0xC0,0x1F,0x88,0x00,0x44,0x00,0x48,0x00,0x48,0x00,0x84,0x1F,0x00,0x00},	// 0xA4
		{0x00,0x00,0xF0,0x1F,0x68,0x00,0x84,0x01,0x0C,0x06,0x08,0x08,0xF4,0x1F,0x00,0x00},	// 0xA5
		{0x00,0x00,0x00,0x00,0xC8,0x00,0xA8,0x00,0xA8,0x00,0xF8,0x00,0x80,0x00,0x00,0x00},	// 0xA6
		{0x00,0x00,0x70,0x00,0x88,0x00,0x88,0x00,0x88,0x00,0x70,0x00,0x00,0x00,0x00,0x00},	// 0xA7
		{0x00,0x00,0x00,0x60,0x00,0x90,0x00,0x88,0xC0,0x86,0x00,0x80,0x00,0xC0,0x00,0x00},	// 0xA8
		{0x00,0x00,0xF0,0x00,0x08,0x01,0x68,0x01,0x68,0x01,0x08,0x01,0xF0,0x00,0x00,0x00},	// 0xA9
		{0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x07,0x00,0x00},	// 0xAA
		{0x08,0x10,0xF8,0x0C,0x00,0x06,0x80,0x01,0x60,0x19,0x30,0x19,0x08,0x15,0x00,0x13},	// 0xAB
		{0x08,0x10,0xF8,0x0C,0x00,0x02,0x80,0x0D,0x60,0x0A,0x10,0x09,0x08,0x1F,0x00,0x08},	// 0xAC
		{0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xAD
		{0x00,0x00,0x00,0x03,0x80,0x04,0x40,0x08,0x00,0x03,0x80,0x04,0x40,0x08,0x00,0x00},	// 0xAE
		{0x00,0x00,0x40,0x08,0x80,0x04,0x00,0x03,0x40,0x08,0x80,0x04,0x00,0x03,0x00,0x00},	// 0xAF
		{0x24,0x25,0x00,0x00,0x00,0x00,0x24,0x25,0x00,0x00,0x00,0x00,0x24,0x25,0x00,0x00},	// 0xB0
		{0x48,0x92,0x24,0x25,0x00,0x00,0x48,0x92,0x24,0x25,0x00,0x00,0x48,0x92,0x24,0x25},	// 0xB1
		{0xFC,0xFF,0x24,0x25,0x24,0x25,0xFC,0xFF,0x24,0x25,0x24,0x25,0xFC,0xFF,0x24,0x25},	// 0xB2
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xB3
		{0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0xFC,0xFF,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xB4
		{0x00,0x10,0x00,0x0E,0x80,0x05,0x68,0x04,0x34,0x04,0xC0,0x05,0x00,0x0E,0x00,0x10},	// 0xB5
		{0x00,0x10,0x00,0x0E,0x88,0x05,0x64,0x04,0x34,0x04,0xC8,0x05,0x00,0x0E,0x00,0x10},	// 0xB6
		{0x00,0x10,0x00,0x0E,0x80,0x05,0x64,0x04,0x38,0x04,0xC0,0x05,0x00,0x0E,0x00,0x10},	// 0xB7
		{0xE0,0x07,0x10,0x08,0xC8,0x13,0x28,0x14,0x28,0x14,0x28,0x14,0x10,0x08,0xE0,0x07},	// 0xB8
		{0x80,0x02,0x80,0x02,0x80,0x02,0xFC,0xFE,0x00,0x00,0xFC,0xFF,0x00,0x00,0x00,0x00},	// 0xB9
		{0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x00,0x00,0xFC,0xFF,0x00,0x00,0x00,0x00},	// 0xBA
		{0x80,0x02,0x80,0x02,0x80,0x02,0x80,0xFE,0x80,0x00,0x80,0xFF,0x00,0x00,0x00,0x00},	// 0xBB
		{0x80,0x02,0x80,0x02,0x80,0x02,0xFC,0x02,0x00,0x02,0xFC,0x03,0x00,0x00,0x00,0x00},	// 0xBC
		{0x00,0x00,0xC0,0x03,0x20,0x04,0x10,0x08,0xF8,0x1F,0x10,0x08,0x10,0x08,0x00,0x00},	// 0xBD
		{0x08,0x00,0x10,0x05,0x60,0x05,0x80,0x1F,0x40,0x05,0x20,0x05,0x10,0x00,0x08,0x00},	// 0xBE
		{0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xBF
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x01,0x00,0x01,0x00,0x01,0x00,0x01},	// 0xC0
		{0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0xFC,0x01,0x00,0x01,0x00,0x01,0x00,0x01},	// 0xC1
		{0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0xFF,0x00,0x01,0x00,0x01,0x00,0x01},	// 0xC2
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x00,0x01,0x00,0x01,0x00,0x01},	// 0xC3
		{0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01},	// 0xC4
		{0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0xFC,0xFF,0x00,0x01,0x00,0x01,0x00,0x01},	// 0xC5
		{0x00,0x00,0x00,0x0C,0x48,0x12,0x44,0x12,0x48,0x12,0x48,0x12,0x84,0x1F,0x00,0x10},	// 0xC6
		{0x00,0x10,0x00,0x0E,0x88,0x05,0x64,0x04,0x3C,0x04,0xC8,0x05,0x04,0x0E,0x00,0x10},	// 0xC7
		{0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x03,0x00,0x02,0xFC,0x02,0x80,0x02,0x80,0x02},	// 0xC8
		{0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x80,0x00,0x80,0xFE,0x80,0x02,0x80,0x02},	// 0xC9
		{0x80,0x02,0x80,0x02,0x80,0x02,0xFC,0x02,0x00,0x02,0xFC,0x02,0x80,0x02,0x80,0x02},	// 0xCA
		{0x80,0x02,0x80,0x02,0x80,0x02,0x80,0xFE,0x80,0x00,0x80,0xFE,0x80,0x02,0x80,0x02},	// 0xCB
		{0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x00,0x00,0xFC,0xFE,0x80,0x02,0x80,0x02},	// 0xCC
		{0x80,0x02,0x80,0x02,0x80,0x02,0x80,0x02,0x80,0x02,0x80,0x02,0x80,0x02,0x80,0x02},	// 0xCD
		{0x80,0x02,0x80,0x02,0x80,0x02,0xFC,0xFE,0x00,0x00,0xFC,0xFE,0x80,0x02,0x80,0x02},	// 0xCE
		{0x10,0x08,0xE0,0x07,0x20,0x04,0x20,0x04,0x20,0x04,0xE0,0x07,0x10,0x08,0x00,0x00},	// 0xCF
		{0x04,0x07,0x94,0x08,0x5C,0x10,0x48,0x10,0x54,0x10,0xE0,0x08,0x80,0x07,0x00,0x00},	// 0xD0
		{0x00,0x01,0xF0,0x1F,0x10,0x11,0x10,0x11,0x10,0x10,0x10,0x10,0x20,0x08,0xC0,0x07},	// 0xD1
		{0x00,0x00,0xF0,0x1F,0x18,0x11,0x14,0x11,0x14,0x11,0x18,0x11,0x10,0x10,0x00,0x00},	// 0xD2
		{0x00,0x00,0xF0,0x1F,0x14,0x11,0x10,0x11,0x10,0x11,0x14,0x11,0x10,0x10,0x00,0x00},	// 0xD3
		{0x00,0x00,0xF0,0x1F,0x10,0x11,0x10,0x11,0x14,0x11,0x18,0x11,0x10,0x10,0x00,0x00},	// 0xD4
		{0x00,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0xC0,0x1F,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xD5
		{0x00,0x00,0x10,0x10,0x10,0x10,0xF0,0x1F,0x18,0x10,0x14,0x10,0x00,0x00,0x00,0x00},	// 0xD6
		{0x00,0x00,0x10,0x10,0x18,0x10,0xF4,0x1F,0x14,0x10,0x18,0x10,0x00,0x00,0x00,0x00},	// 0xD7
		{0x00,0x00,0x10,0x10,0x14,0x10,0xF0,0x1F,0x10,0x10,0x14,0x10,0x00,0x00,0x00,0x00},	// 0xD8
		{0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0xFC,0x01,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xD9
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x01,0x00,0x01,0x00,0x01},	// 0xDA
		{0xFC,0xFF,0xFC,0xFF,0xFC,0xFF,0xFC,0xFF,0xFC,0xFF,0xFC,0xFF,0xFC,0xFF,0xFC,0xFF},	// 0xDB
		{0x00,0xFE,0x00,0xFE,0x00,0xFE,0x00,0xFE,0x00,0xFE,0x00,0xFE,0x00,0xFE,0x00,0xFE},	// 0xDC
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x7C,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xDD
		{0x00,0x00,0x10,0x10,0x10,0x10,0xF4,0x1F,0x18,0x10,0x10,0x10,0x00,0x00,0x00,0x00},	// 0xDE
		{0xFC,0x01,0xFC,0x01,0xFC,0x01,0xFC,0x01,0xFC,0x01,0xFC,0x01,0xFC,0x01,0xFC,0x01},	// 0xDF
		{0xC0,0x07,0x20,0x08,0x10,0x10,0x10,0x10,0x18,0x10,0x24,0x08,0xC0,0x07,0x00,0x00},	// 0xE0
		{0x00,0x00,0xF8,0x1F,0x04,0x00,0x04,0x00,0xC4,0x10,0x38,0x11,0x00,0x12,0x00,0x0C},	// 0xE1
		{0xC0,0x07,0x20,0x08,0x18,0x10,0x14,0x10,0x14,0x10,0x28,0x08,0xC0,0x07,0x00,0x00},	// 0xE2
		{0xC0,0x07,0x20,0x08,0x10,0x10,0x14,0x10,0x18,0x10,0x20,0x08,0xC0,0x07,0x00,0x00},	// 0xE3
		{0x00,0x07,0x80,0x08,0x48,0x10,0x44,0x10,0x48,0x10,0x88,0x08,0x04,0x07,0x00,0x00},	// 0xE4
		{0xC0,0x07,0x20,0x08,0x18,0x10,0x14,0x10,0x1C,0x10,0x28,0x08,0xC4,0x07,0x00,0x00},	// 0xE5
		{0x00,0x00,0xC0,0xFF,0x00,0x08,0x00,0x10,0x00,0x10,0x00,0x08,0xC0,0x1F,0x00,0x00},	// 0xE6
		{0x00,0x00,0xFC,0xFF,0x80,0x08,0x40,0x10,0x40,0x10,0x40,0x10,0x80,0x0F,0x00,0x00},	// 0xE7
		{0x00,0x00,0xF0,0x1F,0x40,0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x80,0x03,0x00,0x00},	// 0xE8
		{0x00,0x00,0xF0,0x0F,0x00,0x10,0x00,0x10,0x08,0x10,0x04,0x10,0xF0,0x0F,0x00,0x00},	// 0xE9
		{0x00,0x00,0xF0,0x0F,0x08,0x10,0x04,0x10,0x04,0x10,0x08,0x10,0xF0,0x0F,0x00,0x00},	// 0xEA
		{0x00,0x00,0xF0,0x0F,0x00,0x10,0x00,0x10,0x04,0x10,0x08,0x10,0xF0,0x0F,0x00,0x00},	// 0xEB
		{0x40,0x80,0x80,0x81,0x00,0xC6,0x00,0x78,0x08,0x18,0x04,0x06,0x80,0x01,0x40,0x00},	// 0xEC
		{0x10,0x00,0x60,0x00,0x80,0x00,0x00,0x1F,0x88,0x01,0x44,0x00,0x20,0x00,0x10,0x00},	// 0xED
		{0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00},	// 0xEE
		{0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xEF
		{0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x00},	// 0xF0
		{0x00,0x11,0x00,0x11,0x00,0x11,0xC0,0x17,0x00,0x11,0x00,0x11,0x00,0x11,0x00,0x00},	// 0xF1
		{0x00,0xA0,0x00,0xA0,0x00,0xA0,0x00,0xA0,0x00,0xA0,0x00,0xA0,0x00,0xA0,0x00,0xA0},	// 0xF2
		{0xA8,0x10,0xA8,0x08,0xD8,0x06,0x00,0x0D,0x80,0x0A,0x60,0x09,0x10,0x1F,0x08,0x08},	// 0xF3
		{0x70,0x00,0xF8,0x00,0xF8,0x00,0xF8,0x7F,0x08,0x00,0x08,0x00,0xF8,0x7F,0x00,0x00},	// 0xF4
		{0x00,0x00,0x00,0x00,0xF0,0x63,0x48,0x46,0xC8,0x44,0x88,0x4C,0x08,0x3B,0x00,0x00},	// 0xF5
		{0x00,0x02,0x00,0x02,0x00,0x02,0xC0,0x1A,0xC0,0x1A,0x00,0x02,0x00,0x02,0x00,0x00},	// 0xF6
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xA0,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xF7
		{0x00,0x00,0x00,0x00,0x10,0x00,0x28,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xF8
		{0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00},	// 0xF9
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xFA
		{0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xFB
		{0x00,0x00,0x00,0x00,0x88,0x00,0xA8,0x00,0xA8,0x00,0xD8,0x00,0x00,0x00,0x00,0x00},	// 0xFC
		{0x00,0x00,0x00,0x00,0x88,0x00,0xC8,0x00,0xA8,0x00,0x98,0x00,0x00,0x00,0x00,0x00},	// 0xFD
		{0x00,0x00,0xC0,0x1F,0xC0,0x1F,0xC0,0x1F,0xC0,0x1F,0xC0,0x1F,0xC0,0x1F,0x00,0x00},	// 0xFE
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} 	// 0xFF
};