/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_NAVNUM,
    LAYER_SYMBOLS,
    LAYER_FUNMEDIA,
    LAYER_POINTER,
};

// Automatically enable sniping-mode on the pointer layer.
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#define ENT_NAV LT(LAYER_NAVNUM, KC_ENT)
#define SPC_SYM LT(LAYER_SYMBOLS, KC_SPC)
#define ESC_FUN LT(LAYER_FUNMEDIA, KC_ESC)
#define _L_PTR(KC) LT(LAYER_POINTER, KC)

// clang-format off

/** Convenience row shorthands. */
#define _______________DEAD_HALF_ROW_______________ XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
#define ______________HOME_ROW_GACS_L______________ KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, KC_RALT
#define ______________HOME_ROW_GACS_R______________ KC_RALT, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI
#define ___________TRANSPARENT_THUMB_ROW___________ _______, _______, _______, _______, _______

/** \brief COLEMAK-DH layout (3 rows, 10 columns). */
#define LAYOUT_LAYER_BASE                                                                     \
       KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,    KC_J,    KC_L,    KC_U,    KC_Y, KC_BSPC, \
       KC_A,    KC_R,    KC_S,    KC_T,    KC_G,    KC_M,    KC_N,    KC_E,    KC_I,    KC_O, \
       KC_X,    KC_C,    KC_D,    KC_V,    KC_Z,    KC_K,    KC_H, KC_COMM,  KC_DOT, KC_SLSH, \
                      OS_LSFT, ENT_NAV, ESC_FUN,  KC_TAB, SPC_SYM

/** \brief Navigation and numeral layout. */
#define LAYOUT_LAYER_NAVNUM                                                              \
     KC_PWR,    KC_7,    KC_8,    KC_9, XXXXXXX, XXXXXXX, KC_HOME,   KC_UP,  KC_END, KC_PGUP, \
    XXXXXXX,    KC_4,    KC_5,    KC_6,KC_COMMA, XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, \
       KC_0,    KC_1,    KC_2,    KC_3,  KC_DOT, _______________DEAD_HALF_ROW_______________, \
                      ___________TRANSPARENT_THUMB_ROW___________

/** \brief Symbols layer. */
#define LAYOUT_LAYER_SYMBOLS                                                                  \
    XXXXXXX, KC_AMPR, KC_ASTR, KC_PIPE, XXXXXXX, KC_SCLN, KC_LCBR, KC_RCBR, KC_MINS, KC_PLUS, \
    XXXXXXX,  KC_DLR, KC_PERC, KC_CIRC, KC_TILD,  KC_EQL, KC_LPRN, KC_RPRN, KC_DQUO, KC_QUOT, \
    XXXXXXX, KC_EXLM,   KC_AT, KC_HASH, XXXXXXX, KC_BSLS, KC_LBRC, KC_RBRC, KC_COLN, KC_UNDS, \
                      ___________TRANSPARENT_THUMB_ROW___________

/** \brief Function layer. */
#define LAYOUT_LAYER_FUNMEDIA                                                                \
    KC_F12,   KC_F7,   KC_F8,   KC_F9, KC_PSCR, XXXXXXX, XXXXXXX, KC_VOLU, XXXXXXX, XXXXXXX, \
    KC_F11,   KC_F4,   KC_F5,   KC_F6, KC_SCRL, XXXXXXX, KC_MPRV, KC_VOLD, KC_MNXT, KC_MSEL, \
    KC_F10,   KC_F1,   KC_F2,   KC_F3, KC_PAUS, XXXXXXX, XXXXXXX, KC_MUTE, XXXXXXX, XXXXXXX, \
                      _______, _______, _______, KC_MSTP, KC_MPLY

/** \brief Mouse emulation and pointer functions. */
#define LAYOUT_LAYER_POINTER                                                                  \
    QK_BOOT,  EE_CLR, XXXXXXX, DPI_MOD, S_D_MOD, S_D_MOD, DPI_MOD, XXXXXXX,  EE_CLR, QK_BOOT, \
    ______________HOME_ROW_GACS_L______________, ______________HOME_ROW_GACS_R______________, \
    _______, DRGSCRL, SNIPING, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, SNIPING, DRGSCRL, _______, \
                      MS_BTN2, MS_BTN1, MS_BTN3, MS_BTN3, MS_BTN1

/**
 * \brief Sidedness definition.
 *
 * Define sidedness for use by QMK's chordal hold.
 */
#define CHORDAL_HOLD_LAYOUT                                                                   \
        'L',     'L',     'L',     'L',     'L',     'R',    'R',    'R',    'R',    'R',     \
        'L',     'L',     'L',     'L',     'L',     'R',    'R',    'R',    'R',    'R',     \
        'L',     'L',     'L',     'L',     'L',     'R',    'R',    'R',    'R',    'R',     \
                          '*',     '*',     '*',     '*',    '*'
/**
 * \brief Add Home Row mod to a layout.
 *
 * Expects a 10-key per row layout.  Adds support for GACS (Gui, Alt, Ctl, Shift)
 * home row.  The layout passed in parameter must contain at least 20 keycodes.
 *
 * This is meant to be used with `LAYER_ALPHAS_QWERTY` defined above, eg.:
 *
 *     HOME_ROW_MOD_GACS(LAYER_ALPHAS_QWERTY)
 */
#define _HOME_ROW_MOD_GACS(                                            \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
      LGUI_T(L10), LALT_T(L11), LCTL_T(L12), LSFT_T(L13), RALT_T(L14),  \
      RALT_T(R15), RSFT_T(R16), RCTL_T(R17), LALT_T(R18), RGUI_T(R19), \
      __VA_ARGS__
#define HOME_ROW_MOD_GACS(...) _HOME_ROW_MOD_GACS(__VA_ARGS__)

/**
 * \brief Add pointer layer keys to a layout.
 *
 * Expects a 10-key per row layout.  The layout passed in parameter must contain
 * at least 30 keycodes.
 *
 * This is meant to be used with `LAYER_ALPHAS_QWERTY` defined above, eg.:
 *
 *     POINTER_MOD(LAYER_ALPHAS_QWERTY)
 */
#define _POINTER_MOD(                                                  \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    L20, L21, L22, L23, L24, R25, R26, R27, R28, R29,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
             L10,         L11,         L12,         L13,         L14,  \
             R15,         R16,         R17,         R18,         R19,  \
      _L_PTR(L20),        L21,         L22,         L23,         L24,  \
             R25,         R26,         R27,         R28,  _L_PTR(R29), \
      __VA_ARGS__
#define POINTER_MOD(...) _POINTER_MOD(__VA_ARGS__)

#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM =
  LAYOUT_wrapper(CHORDAL_HOLD_LAYOUT);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_wrapper(POINTER_MOD(HOME_ROW_MOD_GACS(LAYOUT_LAYER_BASE))),
  [LAYER_NAVNUM] = LAYOUT_wrapper(HOME_ROW_MOD_GACS(LAYOUT_LAYER_NAVNUM)),
  [LAYER_SYMBOLS] = LAYOUT_wrapper(LAYOUT_LAYER_SYMBOLS),
  [LAYER_FUNMEDIA] = LAYOUT_wrapper(LAYOUT_LAYER_FUNMEDIA),
  [LAYER_POINTER] = LAYOUT_wrapper(LAYOUT_LAYER_POINTER),
};
// clang-format on

/**
 * \brief Process mod tap combinations that require 16 bit action codes.
 *
 * Mod tap and layer tap mask modifiers from keycodes. That also applies to special alias
 * codes like KC_DLR etc.
 *
 * This can be circumvented by intercepting the action and sending the tapped code manually.
 */
// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//     switch (keycode) {
//         case LT(LAYER_FUNMEDIA, OS_LSFT):
//             if (record->tap.count && record->event.pressed) {
//                 tap_code16(OS_LSFT); // Send OS_LSFT on tap
//                 return false;        // Return false to ignore further processing of key
//             }
//             break;
//     }
//     return true;
// }

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case ENT_NAV:
            return 0;
        case SPC_SYM:
            return 0;
        case ESC_FUN:
            return 0;
        default:
            return QUICK_TAP_TERM;
    }
}
