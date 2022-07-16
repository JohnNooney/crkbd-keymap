/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <stdio.h>
char wpm_str[10];

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_DEL,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,   MO(1),  KC_SPC,     KC_ENT,   MO(2), KC_RALT
                                      //`--------------------------'  `--------------------------'

  ),

  [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_HOME, KC_END,   KC_PGUP,KC_PGDN, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_ESC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT,   MO(3), KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

  [2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, XXXXXXX, XXXXXXX, KC_UP, XXXXXXX, XXXXXXX,                      KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,  KC_GRV,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, KC_LEFT, KC_DOWN, KC_RIGHT, XXXXXXX,                      KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,   MO(3),  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

  [3] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6,                              KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  )
};


// OLED STUFF STARTS HERE
// based on https://github.com/qmk/qmk_firmware/blob/master/keyboards/kyria/keymaps/j-inc/keymap.c

// In your rules.mk make sure you have:
// OLED_DRIVER_ENABLE = yes
// WPM_ENABLE = yes

#ifdef OLED_ENABLE
#define IDLE_FRAMES 5
#define TAP_FRAMES  2
#define FRAME_DURATION 200 // milliseconds
#define TAP_INTERVAL  FRAME_DURATION*2
#define PAWS_INTERVAL FRAME_DURATION*8

uint32_t tap_timer = 0;

// Run-length encoded animation frames
#ifndef LEFTCAT // Right aligned Bongocat
static unsigned char const idle0[] PROGMEM = {144,
	0x3a,0x00,0x01,0x80,0x02,0x40,0x04,0x20,0x05,0x10,0x02,0x08,0x03,0x04,0x84,0x08,
	0x30,0x40,0x80,0x2e,0x00,0x03,0x80,0x31,0x00,0x83,0x18,0x64,0x82,0x05,0x02,0x01,
	0x01,0x04,0x00,0x02,0x80,0x09,0x00,0x82,0x80,0x00,0x02,0x30,0x82,0x00,0xc0,0x02,
	0xc1,0x87,0xc2,0x04,0x08,0x10,0x20,0x40,0x80,0x03,0x00,0x04,0x80,0x04,0x40,0x04,
	0x20,0x04,0x10,0x05,0x08,0x05,0x04,0x04,0x02,0x04,0x01,0x33,0x00,0x84,0xc0,0x38,
	0x04,0x03,0x07,0x00,0x03,0x0c,0x88,0x0d,0x01,0x00,0x40,0xa0,0x21,0x22,0x12,0x03,
	0x11,0x01,0x09,0x04,0x08,0x02,0x04,0x02,0x08,0x05,0x10,0x82,0x11,0x0f,0x02,0x01,
	0x36,0x00,0x05,0x80,0x05,0x40,0x05,0x20,0x05,0x10,0x05,0x08,0x05,0x04,0x82,0x02,
	0x03,0x02,0x02,0x06,0x01,0x02,0x02,0x02,0x04,0x05,0x08,0x01,0x07,0x3d,0x00};
static unsigned char const idle1[] PROGMEM = {147,
	0x3a,0x00,0x01,0x80,0x02,0x40,0x04,0x20,0x04,0x10,0x02,0x08,0x01,0x04,0x03,0x02,
	0x84,0x04,0x38,0x40,0x80,0x2e,0x00,0x03,0x80,0x31,0x00,0x82,0x1c,0xe2,0x02,0x01,
	0x04,0x02,0x01,0x01,0x04,0x00,0x02,0x80,0x09,0x00,0x82,0x80,0x00,0x02,0x30,0x82,
	0x00,0xc0,0x02,0xc1,0x87,0xc2,0x04,0x08,0x10,0x20,0x40,0x80,0x03,0x00,0x04,0x80,
	0x04,0x40,0x04,0x20,0x04,0x10,0x05,0x08,0x05,0x04,0x04,0x02,0x04,0x01,0x33,0x00,
	0x84,0x80,0x70,0x0c,0x03,0x07,0x00,0x03,0x0c,0x88,0x0d,0x01,0x00,0x40,0xa0,0x21,
	0x22,0x12,0x03,0x11,0x01,0x09,0x04,0x08,0x02,0x04,0x02,0x08,0x05,0x10,0x82,0x11,
	0x0f,0x02,0x01,0x36,0x00,0x05,0x80,0x05,0x40,0x05,0x20,0x05,0x10,0x05,0x08,0x05,
	0x04,0x82,0x02,0x03,0x02,0x02,0x06,0x01,0x02,0x02,0x02,0x04,0x05,0x08,0x01,0x07,
	0x3d,0x00};
static unsigned char const idle2[] PROGMEM = {154,
	0x33,0x00,0x02,0x80,0x05,0x00,0x01,0x80,0x02,0x40,0x04,0x20,0x04,0x10,0x83,0x08,
	0x04,0x02,0x02,0x01,0x85,0x02,0x0c,0x30,0x40,0x80,0x2e,0x00,0x03,0x80,0x31,0x00,
	0x82,0x1e,0xe1,0x02,0x00,0x02,0x01,0x02,0x02,0x01,0x01,0x04,0x00,0x02,0x80,0x09,
	0x00,0x82,0x80,0x00,0x02,0x30,0x82,0x00,0xc0,0x02,0xc1,0x87,0xc2,0x04,0x08,0x10,
	0x20,0x40,0x80,0x03,0x00,0x04,0x80,0x04,0x40,0x04,0x20,0x04,0x10,0x05,0x08,0x05,
	0x04,0x04,0x02,0x04,0x01,0x33,0x00,0x84,0x80,0x70,0x0c,0x03,0x07,0x00,0x03,0x0c,
	0x88,0x0d,0x01,0x00,0x40,0xa0,0x21,0x22,0x12,0x03,0x11,0x01,0x09,0x04,0x08,0x02,
	0x04,0x02,0x08,0x05,0x10,0x82,0x11,0x0f,0x02,0x01,0x36,0x00,0x05,0x80,0x05,0x40,
	0x05,0x20,0x05,0x10,0x05,0x08,0x05,0x04,0x82,0x02,0x03,0x02,0x02,0x06,0x01,0x02,
	0x02,0x02,0x04,0x05,0x08,0x01,0x07,0x3d,0x00};
static unsigned char const idle3[] PROGMEM = {140,
	0x3b,0x00,0x02,0x80,0x04,0x40,0x04,0x20,0x83,0x10,0x08,0x04,0x02,0x02,0x84,0x04,
	0x18,0x60,0x80,0x2f,0x00,0x03,0x80,0x31,0x00,0x82,0x3c,0xc2,0x02,0x01,0x02,0x02,
	0x02,0x04,0x82,0x02,0x01,0x10,0x00,0x02,0x60,0x82,0x00,0x81,0x02,0x82,0x86,0x84,
	0x08,0x10,0x20,0x40,0x80,0x04,0x00,0x04,0x80,0x04,0x40,0x04,0x20,0x04,0x10,0x05,
	0x08,0x05,0x04,0x04,0x02,0x04,0x01,0x33,0x00,0x84,0x80,0x70,0x19,0x06,0x07,0x00,
	0x03,0x18,0x88,0x1b,0x03,0x00,0x40,0xa0,0x22,0x24,0x14,0x03,0x12,0x01,0x0b,0x04,
	0x08,0x02,0x05,0x02,0x09,0x05,0x10,0x82,0x11,0x0f,0x02,0x01,0x36,0x00,0x05,0x80,
	0x05,0x40,0x05,0x20,0x05,0x10,0x05,0x08,0x05,0x04,0x82,0x02,0x03,0x02,0x02,0x06,
	0x01,0x02,0x02,0x02,0x04,0x05,0x08,0x01,0x07,0x3d,0x00};
static unsigned char const *idle_anim[IDLE_FRAMES] = {
	idle0, idle0, idle1, idle2, idle3 };

static unsigned char const paws[] PROGMEM = {150,
	0x33,0x00,0x02,0x80,0x05,0x00,0x01,0x80,0x02,0x40,0x04,0x20,0x04,0x10,0x83,0x08,
	0x04,0x02,0x02,0x01,0x85,0x02,0x0c,0x30,0x40,0x80,0x2e,0x00,0x03,0x80,0x31,0x00,
	0x82,0x1e,0xe1,0x02,0x00,0x02,0x01,0x02,0x02,0x01,0x81,0x02,0x80,0x02,0x00,0x02,
	0x80,0x09,0x00,0x82,0x80,0x00,0x02,0x30,0x02,0x00,0x8a,0x01,0xe1,0x1a,0x06,0x09,
	0x31,0x35,0x01,0x8a,0x7c,0x02,0x00,0x04,0x80,0x04,0x40,0x04,0x20,0x04,0x10,0x05,
	0x08,0x05,0x04,0x04,0x02,0x04,0x01,0x33,0x00,0x84,0x80,0x70,0x0c,0x03,0x02,0x00,
	0x88,0x18,0x06,0x05,0x98,0x99,0x84,0xc3,0x7c,0x02,0x41,0x02,0x40,0x84,0x20,0x21,
	0x22,0x12,0x03,0x11,0x01,0x09,0x04,0x08,0x06,0x04,0x03,0x02,0x04,0x01,0x36,0x00,
	0x05,0x80,0x05,0x40,0x05,0x20,0x05,0x10,0x05,0x08,0x05,0x04,0x82,0x02,0x03,0x02,
	0x02,0x06,0x01,0x47,0x00};

static unsigned char const tap0[] PROGMEM = {180,
	0x33,0x00,0x02,0x80,0x05,0x00,0x01,0x80,0x02,0x40,0x04,0x20,0x04,0x10,0x83,0x08,
	0x04,0x02,0x02,0x01,0x85,0x02,0x0c,0x30,0x40,0x80,0x07,0x00,0x04,0xf8,0x05,0x00,
	0x02,0x80,0x1c,0x00,0x03,0x80,0x31,0x00,0x82,0x1e,0xe1,0x02,0x00,0x02,0x01,0x02,
	0x02,0x01,0x81,0x02,0x80,0x02,0x00,0x02,0x80,0x09,0x00,0x82,0x80,0x00,0x02,0x30,
	0x02,0x00,0x02,0x01,0x8e,0x02,0x04,0x08,0x10,0x20,0x43,0x87,0x07,0x01,0x00,0xb8,
	0xbc,0xbe,0x9f,0x02,0x5f,0x82,0x4f,0x4c,0x04,0x20,0x04,0x10,0x05,0x08,0x05,0x04,
	0x04,0x02,0x04,0x01,0x33,0x00,0x84,0x80,0x70,0x0c,0x03,0x02,0x00,0x88,0x18,0x06,
	0x05,0x98,0x99,0x84,0x43,0x7c,0x02,0x41,0x02,0x40,0x84,0x20,0x21,0x22,0x12,0x03,
	0x11,0x01,0x09,0x04,0x08,0x02,0x04,0x02,0x08,0x05,0x10,0x85,0x11,0x0f,0x01,0x3d,
	0x7c,0x05,0xfc,0x82,0x3c,0x0c,0x2e,0x00,0x05,0x80,0x05,0x40,0x05,0x20,0x05,0x10,
	0x05,0x08,0x05,0x04,0x82,0x02,0x03,0x02,0x02,0x06,0x01,0x24,0x00,0x01,0x01,0x02,
	0x03,0x20,0x00};
static unsigned char const tap1[] PROGMEM = {153,
	0x33,0x00,0x02,0x80,0x05,0x00,0x01,0x80,0x02,0x40,0x04,0x20,0x04,0x10,0x83,0x08,
	0x04,0x02,0x02,0x01,0x85,0x02,0x0c,0x30,0x40,0x80,0x2e,0x00,0x03,0x80,0x31,0x00,
	0x82,0x1e,0xe1,0x02,0x00,0x02,0x01,0x02,0x02,0x01,0x01,0x04,0x00,0x02,0x80,0x09,
	0x00,0x82,0x80,0x00,0x02,0x30,0x02,0x00,0x8a,0x01,0xe1,0x1a,0x06,0x09,0x31,0x35,
	0x01,0x8a,0x7c,0x02,0x00,0x04,0x80,0x04,0x40,0x04,0x20,0x04,0x10,0x05,0x08,0x05,
	0x04,0x04,0x02,0x04,0x01,0x33,0x00,0x84,0x80,0x70,0x0c,0x03,0x0a,0x00,0x02,0x01,
	0x86,0x00,0x40,0xa0,0x21,0x22,0x12,0x03,0x11,0x01,0x09,0x04,0x08,0x06,0x04,0x03,
	0x02,0x04,0x01,0x36,0x00,0x05,0x80,0x05,0x40,0x05,0x20,0x05,0x10,0x05,0x08,0x05,
	0x04,0x82,0x02,0x03,0x02,0x7a,0x04,0x79,0x82,0x39,0x31,0x02,0x02,0x02,0x04,0x03,
	0x08,0x02,0x88,0x82,0x87,0x80,0x3c,0x00};
static unsigned char const *tap_anim[TAP_FRAMES] = {
	tap0, tap1 };
#endif // #ifndef LEFTCAT

#ifndef RIGHTCAT // Left aligned Bongocat
static unsigned char const left_idle0[] PROGMEM = {142,
	0x03,0x80,0x2e,0x00,0x84,0x80,0x40,0x30,0x08,0x03,0x04,0x02,0x08,0x05,0x10,0x04,
	0x20,0x02,0x40,0x01,0x80,0x3d,0x00,0x04,0x01,0x04,0x02,0x05,0x04,0x05,0x08,0x04,
	0x10,0x04,0x20,0x04,0x40,0x04,0x80,0x03,0x00,0x87,0x80,0x40,0x20,0x10,0x08,0x04,
	0xc2,0x02,0xc1,0x82,0xc0,0x00,0x02,0x30,0x82,0x00,0x80,0x09,0x00,0x02,0x80,0x04,
	0x00,0x01,0x01,0x05,0x02,0x83,0x82,0x64,0x18,0x56,0x00,0x02,0x01,0x82,0x0f,0x11,
	0x05,0x10,0x02,0x08,0x02,0x04,0x04,0x08,0x01,0x09,0x03,0x11,0x88,0x12,0x22,0x21,
	0xa0,0x40,0x00,0x01,0x0d,0x03,0x0c,0x07,0x00,0x84,0x03,0x04,0x38,0xc0,0x6d,0x00,
	0x01,0x07,0x05,0x08,0x02,0x04,0x02,0x02,0x06,0x01,0x02,0x02,0x82,0x03,0x02,0x05,
	0x04,0x05,0x08,0x05,0x10,0x05,0x20,0x05,0x40,0x05,0x80,0x11,0x00};
static unsigned char const left_idle1[] PROGMEM = {145,
	0x03,0x80,0x2e,0x00,0x84,0x80,0x40,0x38,0x04,0x03,0x02,0x01,0x04,0x02,0x08,0x04,
	0x10,0x04,0x20,0x02,0x40,0x01,0x80,0x3d,0x00,0x04,0x01,0x04,0x02,0x05,0x04,0x05,
	0x08,0x04,0x10,0x04,0x20,0x04,0x40,0x04,0x80,0x03,0x00,0x87,0x80,0x40,0x20,0x10,
	0x08,0x04,0xc2,0x02,0xc1,0x82,0xc0,0x00,0x02,0x30,0x82,0x00,0x80,0x09,0x00,0x02,
	0x80,0x04,0x00,0x01,0x01,0x04,0x02,0x02,0x01,0x82,0xe2,0x1c,0x56,0x00,0x02,0x01,
	0x82,0x0f,0x11,0x05,0x10,0x02,0x08,0x02,0x04,0x04,0x08,0x01,0x09,0x03,0x11,0x88,
	0x12,0x22,0x21,0xa0,0x40,0x00,0x01,0x0d,0x03,0x0c,0x07,0x00,0x84,0x03,0x0c,0x70,
	0x80,0x6d,0x00,0x01,0x07,0x05,0x08,0x02,0x04,0x02,0x02,0x06,0x01,0x02,0x02,0x82,
	0x03,0x02,0x05,0x04,0x05,0x08,0x05,0x10,0x05,0x20,0x05,0x40,0x05,0x80,0x11,0x00};
static unsigned char const left_idle2[] PROGMEM = {152,
	0x03,0x80,0x2e,0x00,0x85,0x80,0x40,0x30,0x0c,0x02,0x02,0x01,0x83,0x02,0x04,0x08,
	0x04,0x10,0x04,0x20,0x02,0x40,0x01,0x80,0x05,0x00,0x02,0x80,0x36,0x00,0x04,0x01,
	0x04,0x02,0x05,0x04,0x05,0x08,0x04,0x10,0x04,0x20,0x04,0x40,0x04,0x80,0x03,0x00,
	0x87,0x80,0x40,0x20,0x10,0x08,0x04,0xc2,0x02,0xc1,0x82,0xc0,0x00,0x02,0x30,0x82,
	0x00,0x80,0x09,0x00,0x02,0x80,0x04,0x00,0x01,0x01,0x02,0x02,0x02,0x01,0x02,0x00,
	0x82,0xe1,0x1e,0x56,0x00,0x02,0x01,0x82,0x0f,0x11,0x05,0x10,0x02,0x08,0x02,0x04,
	0x04,0x08,0x01,0x09,0x03,0x11,0x88,0x12,0x22,0x21,0xa0,0x40,0x00,0x01,0x0d,0x03,
	0x0c,0x07,0x00,0x84,0x03,0x0c,0x70,0x80,0x6d,0x00,0x01,0x07,0x05,0x08,0x02,0x04,
	0x02,0x02,0x06,0x01,0x02,0x02,0x82,0x03,0x02,0x05,0x04,0x05,0x08,0x05,0x10,0x05,
	0x20,0x05,0x40,0x05,0x80,0x11,0x00};
static unsigned char const left_idle3[] PROGMEM = {138,
	0x03,0x80,0x2f,0x00,0x84,0x80,0x60,0x18,0x04,0x02,0x02,0x83,0x04,0x08,0x10,0x04,
	0x20,0x04,0x40,0x02,0x80,0x3e,0x00,0x04,0x01,0x04,0x02,0x05,0x04,0x05,0x08,0x04,
	0x10,0x04,0x20,0x04,0x40,0x04,0x80,0x04,0x00,0x86,0x80,0x40,0x20,0x10,0x08,0x84,
	0x02,0x82,0x82,0x81,0x00,0x02,0x60,0x10,0x00,0x82,0x01,0x02,0x02,0x04,0x02,0x02,
	0x02,0x01,0x82,0xc2,0x3c,0x56,0x00,0x02,0x01,0x82,0x0f,0x11,0x05,0x10,0x02,0x09,
	0x02,0x05,0x04,0x08,0x01,0x0b,0x03,0x12,0x88,0x14,0x24,0x22,0xa0,0x40,0x00,0x03,
	0x1b,0x03,0x18,0x07,0x00,0x84,0x06,0x19,0x70,0x80,0x6d,0x00,0x01,0x07,0x05,0x08,
	0x02,0x04,0x02,0x02,0x06,0x01,0x02,0x02,0x82,0x03,0x02,0x05,0x04,0x05,0x08,0x05,
	0x10,0x05,0x20,0x05,0x40,0x05,0x80,0x11,0x00};
static unsigned char const *left_idle_anim[IDLE_FRAMES] = {
	left_idle0, left_idle0, left_idle1, left_idle2, left_idle3 };

static unsigned char const left_paws[] PROGMEM = {148,
	0x03,0x80,0x2e,0x00,0x85,0x80,0x40,0x30,0x0c,0x02,0x02,0x01,0x83,0x02,0x04,0x08,
	0x04,0x10,0x04,0x20,0x02,0x40,0x01,0x80,0x05,0x00,0x02,0x80,0x36,0x00,0x04,0x01,
	0x04,0x02,0x05,0x04,0x05,0x08,0x04,0x10,0x04,0x20,0x04,0x40,0x04,0x80,0x02,0x00,
	0x8a,0x7c,0x8a,0x01,0x35,0x31,0x09,0x06,0x1a,0xe1,0x01,0x02,0x00,0x02,0x30,0x82,
	0x00,0x80,0x09,0x00,0x02,0x80,0x02,0x00,0x02,0x80,0x01,0x81,0x02,0x02,0x02,0x01,
	0x02,0x00,0x82,0xe1,0x1e,0x56,0x00,0x04,0x01,0x03,0x02,0x06,0x04,0x04,0x08,0x01,
	0x09,0x03,0x11,0x84,0x12,0x22,0x21,0x20,0x02,0x40,0x02,0x41,0x88,0x7c,0xc3,0x84,
	0x99,0x98,0x05,0x06,0x18,0x02,0x00,0x84,0x03,0x0c,0x70,0x80,0x77,0x00,0x06,0x01,
	0x02,0x02,0x82,0x03,0x02,0x05,0x04,0x05,0x08,0x05,0x10,0x05,0x20,0x05,0x40,0x05,
	0x80,0x11,0x00};

static unsigned char const left_tap0[] PROGMEM = {178,
	0x03,0x80,0x1c,0x00,0x02,0x80,0x05,0x00,0x04,0xf8,0x07,0x00,0x85,0x80,0x40,0x30,
	0x0c,0x02,0x02,0x01,0x83,0x02,0x04,0x08,0x04,0x10,0x04,0x20,0x02,0x40,0x01,0x80,
	0x05,0x00,0x02,0x80,0x36,0x00,0x04,0x01,0x04,0x02,0x05,0x04,0x05,0x08,0x04,0x10,
	0x04,0x20,0x82,0x4c,0x4f,0x02,0x5f,0x8e,0x9f,0xbe,0xbc,0xb8,0x00,0x01,0x07,0x87,
	0x43,0x20,0x10,0x08,0x04,0x02,0x02,0x01,0x02,0x00,0x02,0x30,0x82,0x00,0x80,0x09,
	0x00,0x02,0x80,0x02,0x00,0x02,0x80,0x01,0x81,0x02,0x02,0x02,0x01,0x02,0x00,0x82,
	0xe1,0x1e,0x4e,0x00,0x82,0x0c,0x3c,0x05,0xfc,0x85,0x7c,0x3d,0x01,0x0f,0x11,0x05,
	0x10,0x02,0x08,0x02,0x04,0x04,0x08,0x01,0x09,0x03,0x11,0x84,0x12,0x22,0x21,0x20,
	0x02,0x40,0x02,0x41,0x88,0x7c,0x43,0x84,0x99,0x98,0x05,0x06,0x18,0x02,0x00,0x84,
	0x03,0x0c,0x70,0x80,0x50,0x00,0x02,0x03,0x01,0x01,0x24,0x00,0x06,0x01,0x02,0x02,
	0x82,0x03,0x02,0x05,0x04,0x05,0x08,0x05,0x10,0x05,0x20,0x05,0x40,0x05,0x80,0x11,
	0x00};
static unsigned char const left_tap1[] PROGMEM = {151,
	0x03,0x80,0x2e,0x00,0x85,0x80,0x40,0x30,0x0c,0x02,0x02,0x01,0x83,0x02,0x04,0x08,
	0x04,0x10,0x04,0x20,0x02,0x40,0x01,0x80,0x05,0x00,0x02,0x80,0x36,0x00,0x04,0x01,
	0x04,0x02,0x05,0x04,0x05,0x08,0x04,0x10,0x04,0x20,0x04,0x40,0x04,0x80,0x02,0x00,
	0x8a,0x7c,0x8a,0x01,0x35,0x31,0x09,0x06,0x1a,0xe1,0x01,0x02,0x00,0x02,0x30,0x82,
	0x00,0x80,0x09,0x00,0x02,0x80,0x04,0x00,0x01,0x01,0x02,0x02,0x02,0x01,0x02,0x00,
	0x82,0xe1,0x1e,0x56,0x00,0x04,0x01,0x03,0x02,0x06,0x04,0x04,0x08,0x01,0x09,0x03,
	0x11,0x86,0x12,0x22,0x21,0xa0,0x40,0x00,0x02,0x01,0x0a,0x00,0x84,0x03,0x0c,0x70,
	0x80,0x6c,0x00,0x82,0x80,0x87,0x02,0x88,0x03,0x08,0x02,0x04,0x02,0x02,0x82,0x31,
	0x39,0x04,0x79,0x02,0x7a,0x82,0x03,0x02,0x05,0x04,0x05,0x08,0x05,0x10,0x05,0x20,
	0x05,0x40,0x05,0x80,0x11,0x00};
static unsigned char const *left_tap_anim[TAP_FRAMES] = {
	left_tap0, left_tap1 };
#endif // #ifndef RIGHTCAT


// RLE decoding loop that reads count from frame index
// if count < 0x80, next byte is repeated by count
// if count >= 0x80, next (count - 128) bytes are unique
static void decode_frame(unsigned char const *frame) {
	uint16_t cursor = 0;
	uint8_t size    = pgm_read_byte(frame);
	uint8_t i       = 1;

	oled_set_cursor(0,0);
	while (i < size) {
		uint8_t count = pgm_read_byte(frame + i); i++;
		if (count & 0x80) {
			// Next count-128 bytes are unique
			count &= ~(0x80);
			for (uint8_t uniqs = 0; uniqs < count; ++uniqs) {
				uint8_t byte = pgm_read_byte(frame + i); i++;
				oled_write_raw_byte(byte, cursor++);
			}
		} else {
			// Next byte is repeated by count
			uint8_t byte = pgm_read_byte(frame + i); i++;
			for (uint8_t reps = 0; reps < count; ++reps) {
				oled_write_raw_byte(byte, cursor++);
			}
		}
	}
}


static void render_cat_idle(void) {
	static uint8_t index = 0;
	index = index < IDLE_FRAMES - 1 ? index + 1 : 0;

#if defined(LEFTCAT)
	decode_frame(left_idle_anim[index]);
#elif defined(RIGHTCAT)
	decode_frame(idle_anim[index]);
#else
	is_keyboard_left() ? decode_frame(left_idle_anim[index]) : decode_frame(idle_anim[index]);
#endif
}


static void render_cat_paws(void) {
#if defined(LEFTCAT)
	decode_frame(left_paws);
#elif defined(RIGHTCAT)
	decode_frame(paws);
#else
	is_keyboard_left() ? decode_frame(left_paws) : decode_frame(paws);
#endif
}


static void render_cat_tap(void) {
	static uint8_t index = 0;
	index = (index + 1) & 1;

#if defined(LEFTCAT)
	decode_frame(left_tap_anim[index]);
#elif defined(RIGHTCAT)
	decode_frame(tap_anim[index]);
#else
	is_keyboard_left() ? decode_frame(left_tap_anim[index]) : decode_frame(tap_anim[index]);
#endif
}


static void render_bongocat(void) {
	// Animation timer
	static uint16_t anim_timer = 0;

#ifdef WPM_ENABLE
	static uint8_t prev_wpm  = 0;
	// Update tap_timer with sustained WPM
	if (get_current_wpm() > prev_wpm) {
		tap_timer = timer_read32();
	}
	prev_wpm = get_current_wpm();
#endif

	void animate_cat(void) {
		if (timer_elapsed32(tap_timer) < TAP_INTERVAL) {
			render_cat_tap();
		} else if (timer_elapsed32(tap_timer) < PAWS_INTERVAL) {
			render_cat_paws();
		} else {
			render_cat_idle();
		}
	}

	if (timer_elapsed32(tap_timer) > OLED_TIMEOUT) {
		oled_off();
	} else if (timer_elapsed(anim_timer) > FRAME_DURATION) {
		anim_timer = timer_read();
		animate_cat();
	}
}


// Init and rendering calls
oled_rotation_t oled_init_user(oled_rotation_t const rotation) {
	if (is_keyboard_master()) {
#if defined(LEFTCAT)
		return rotation;
#elif defined(RIGHTCAT)
		return OLED_ROTATION_180;
#else
		return is_keyboard_left() ? rotation : OLED_ROTATION_180;
#endif
	} else {
		return OLED_ROTATION_270;
	}
}

bool oled_task_user(void) {
	render_bongocat();
	return false;
}
#endif // OLED_ENABLE