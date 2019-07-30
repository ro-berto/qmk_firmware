/* Copyright 2015-2017 Jack Humbert
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
#include "muse.h"

extern keymap_config_t keymap_config;

enum planck_layers { _QWERTY, _LOWER, _RAISE, _NUMPAD, _MACRO, _ADJUST };

uint8_t current_layer = _QWERTY;
uint8_t prev_layer = _QWERTY;
bool layer_changed = false;

enum planck_keycodes {
  QWERTY = SAFE_RANGE,
  COLEMAK,
  DVORAK,
  PLOVER,
  BACKLIT,
  EXT_PLV,
  SET_QWE,
  NUMPAD_KC
};

enum light_modes {
  QWERTY_MODE,
  CAPS_MODE,
  LOWER_MODE,
  RAISE_MODE,
  NUMPAD_MODE,
  MACRO_MODE,
  ADJUST_MODE
};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define MMACR MO(_MACRO)
#define MY_ENT MT(MOD_RSFT, KC_ENT)
#define MR_COMB MT(MMACR, KC_CAPSLOCK)
#define Q_INDEX 16
#define F_INDEX 16
#define J_INDEX 19
#define MAX_LEDS_PER_KEY 8
#define NUM_KEYS 48
#define ROW_LENGTH 12
#define WHITE 0xFFFFFF
#define ORANGE 0xFFA500
#define BLUE 0x0000FF
#define PURPLE 0xFF00FF
#define RED 0xFF0000

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |CL/MCR| Ctrl | Alt  | GUI  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = LAYOUT_planck_grid(
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
    KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, MY_ENT ,
    MR_COMB, KC_LCTL, KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   _  |   +  |   {  |   }  |  |   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |ISO ~ |ISO | | Home | End  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | Next | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_planck_grid(
    KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR,    KC_ASTR,    KC_LPRN, KC_RPRN, KC_BSPC,
    KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_UNDS,    KC_PLUS,    KC_LCBR, KC_RCBR, KC_PIPE,
    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  S(KC_NUHS), S(KC_NUBS), KC_HOME, KC_END,  _______,
    _______, _______, _______, _______, _______, _______, _______, _______,    KC_MNXT,    KC_VOLD, KC_VOLU, KC_MPLY
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   -  |   =  |   [  |   ]  |  \   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |ISO # |ISO / |Pg Up |Pg Dn |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | Next | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_planck_grid(
    KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR,    KC_ASTR,    KC_LPRN, KC_RPRN, KC_BSPC,
    KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_UNDS,    KC_PLUS,    KC_LCBR, KC_RCBR, KC_PIPE,
    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  S(KC_NUHS), S(KC_NUBS), KC_HOME, KC_END,  _______,
    _______, _______, _______, _______, _______, _______, _______, _______,    KC_MNXT,    KC_VOLD, KC_VOLU, KC_MPLY
),

[_MACRO] = LAYOUT_planck_grid(
    _______,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, NUMPAD_KC, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),
/* Numpad
 * ,------------------------------------------------------------------------------------.
 * |Tab    |      |      |      |      |      |      |   7  |   8  |   9  |   -  |Bkspc |
 * |-------+------+------+------+------+-------------+------+------+------+------+------|
 * |       |      |      |      |      |      |      |   6  |   5  |   4  |   +  |      |
 * |-------+------+------+------+------+------|------+------+------+------+------+------|
 * |       |      |      |      |      |      |      |   3  |   2  |   1  |   /  |enter |
 * |-------+------+------+------+------+------+------+------+------+------+------+------|
 * |SET_QWE|      |      |      |      |             |      |   0  |   .  |   *  |  =   |
 * `------------------------------------------------------------------------------------'
 */
[_NUMPAD] = LAYOUT_planck_grid(
    KC_TAB,  _______, _______, _______, _______, _______, _______, KC_KP_7, KC_KP_7, KC_KP_7, KC_PMNS, KC_BSPC,
    _______, _______, _______, _______, _______, _______, _______, KC_KP_6, KC_KP_5, KC_KP_4, KC_PPLS, _______,
    _______, _______, _______, _______, _______, _______, _______, KC_KP_3, KC_KP_2, KC_KP_1, KC_PSLS, KC_PENT,
    SET_QWE, _______, _______, _______, _______, _______, _______, _______, KC_KP_0, KC_PDOT, KC_PAST, KC_PEQL
),

/* Adjust (Lower + Raise)
 * ,-----------------------------------------------------------------------------------.
 * |      | Reset|      |      |      |      |      |      |      |      |      |  Del |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |      |      |Aud on|Audoff|AGnorm|AGswap|Qwerty|Colemk|Dvorak|Plover|      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|Musoff|MIDIon|MIDIof|      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_planck_grid(
    _______, RESET,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
)


};

void color_kb(uint8_t mode);

uint32_t layer_state_set_user(uint32_t state) {
  uint32_t new_state = update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
  for (int i = _QWERTY; i <= _ADJUST; i ++) {
    if (IS_LAYER_ON(i)) {
      current_layer = i;
    }
  }
  return new_state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case NUMPAD_KC:
    if (record->event.pressed) {
      set_single_persistent_default_layer(_NUMPAD);
    }
    return false;
    break;
  case SET_QWE:
    if (record->event.pressed) {
      set_single_persistent_default_layer(_QWERTY);
    }
    return false;
    break;
  }
  return true;
}

bool muse_mode = false;
uint8_t last_muse_note = 0;
uint16_t muse_counter = 0;
uint8_t muse_offset = 70;
uint16_t muse_tempo = 50;

void encoder_update(bool clockwise) {
  if (muse_mode) {
    if (IS_LAYER_ON(_RAISE)) {
      if (clockwise) {
        muse_offset++;
      } else {
        muse_offset--;
      }
    } else {
      if (clockwise) {
        muse_tempo+=1;
      } else {
        muse_tempo-=1;
      }
    }
  } else {
    if (clockwise) {
      #ifdef MOUSEKEY_ENABLE
        tap_code(KC_MS_WH_DOWN);
      #else
        tap_code(KC_PGDN);
      #endif
    } else {
      #ifdef MOUSEKEY_ENABLE
        tap_code(KC_MS_WH_UP);
      #else
        tap_code(KC_PGUP);
      #endif
    }
  }
}

void dip_update(uint8_t index, bool active) {
  switch (index) {
    case 0:
      if (active) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      break;
    case 1:
      if (active) {
        muse_mode = true;
      } else {
        muse_mode = false;
      }
   }
}

void matrix_scan_user(void) {
  if (current_layer != prev_layer) {
    layer_changed = true;
    prev_layer = current_layer;
  }
  if (layer_changed) {
    layer_changed = false;
    switch (current_layer) {
    case _QWERTY:
      color_kb(QWERTY_MODE);
      break;
    case _LOWER:
      color_kb(LOWER_MODE);
      break;
    case _RAISE:
      color_kb(RAISE_MODE);
      break;
    case _NUMPAD:
      color_kb(NUMPAD_MODE);
      break;
    case _MACRO:
      color_kb(MACRO_MODE);
      break;
    case _ADJUST:
      color_kb(ADJUST_MODE);
      break;
    }
  }
}

bool music_mask_user(uint16_t keycode) {
  switch (keycode) {
    case RAISE:
    case LOWER:
      return false;
    default:
      return true;
  }
}

void blank_me(uint8_t *indices, uint32_t *colors) {
  for (int i = 0; i < NUM_KEYS; ++i) {
    indices[i] = 0;
    colors[i] = 0;
  }
  rgb_matrix_set_color_all(0, 0, 0);
}

void apply_keys(uint8_t *indices, uint32_t *colors, uint8_t count) {
  uint8_t r, g, b, row, column;
  uint8_t leds[MAX_LEDS_PER_KEY];
  uint8_t led_count = 0;
  ;
  for (int i = 0; i < count; i++) {
    b = colors[i] & 255;
    g = (colors[i] >> 8) & 255;
    r = (colors[i] >> 16) & 255;
    row = indices[i] / ROW_LENGTH;
    column = indices[i] % ROW_LENGTH;
    led_count = rgb_matrix_map_row_column_to_led(row, column, leds);
    for (int j = 0; j < led_count; j++) {
      rgb_matrix_set_color(leds[j], r, g, b);
    }
  }
}

void color_kb(uint8_t mode) {
  uint8_t indices[NUM_KEYS];
  uint32_t colors[NUM_KEYS];
  uint8_t count = 0;
  blank_me(indices, colors);
  switch (mode) {
  case QWERTY_MODE:
    indices[count] = F_INDEX;
    colors[count] = WHITE;
    count++;
    indices[count] = J_INDEX;
    colors[count] = WHITE;
    count++;
    break;
  case RAISE_MODE:
    for (int i = 0; i < 12 && count < NUM_KEYS; i++) {
      indices[count] = i;
      colors[count] = BLUE;
      count++;
    }
    break;
  case LOWER_MODE:
    for (int i = 12; i < 14 && count < NUM_KEYS; i++) {
      indices[count] = i;
      colors[count] = ORANGE;
      count++;
    }
    break;
  case CAPS_MODE:
    for (int i = 0; i < 48 && count < NUM_KEYS; i++) {
      indices[count] = i;
      colors[count] = RED;
      count++;
    }
    break;
  case MACRO_MODE:
    for (int i = 0; i < 48 && count < NUM_KEYS; i++) {
      if (i % 2 == 0) {
        indices[count] = i;
        colors[count] = PURPLE;
        count++;
      }
    }
    break;
  case ADJUST_MODE:
    indices[count] = Q_INDEX;
    colors[count] = RED;
    count++;
    break;
  case NUMPAD_MODE:
    for (int i = 7; i < 10 && count < NUM_KEYS; i++) {
      for (int j = 0; i < j && count < NUM_KEYS; j++) {

        indices[count] = i + j * 12;
        colors[count] = BLUE;
        count++;
      }
    }
    indices[count] = 44;
    colors[count] = BLUE;
    count++;
    break;
  }
  apply_keys(indices, colors, count);
}

void led_set_user(uint8_t usb_led) {
  static uint8_t old_usb_led = 0;

  _delay_ms(10); // gets rid of tick

  if ((usb_led & (1 << USB_LED_CAPS_LOCK)) &&
      !(old_usb_led & (1 << USB_LED_CAPS_LOCK))) {
    // If CAPS LK LED is turning on...
    color_kb(CAPS_MODE);
  } else if (!(usb_led & (1 << USB_LED_CAPS_LOCK)) &&
             (old_usb_led & (1 << USB_LED_CAPS_LOCK))) {
    // If CAPS LK LED is turning off...
    layer_changed = true;
  }
  old_usb_led = usb_led;
}
