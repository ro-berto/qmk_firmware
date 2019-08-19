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
extern layer_state_t layer_state;

// COMBO Keys
#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define MR_COMB MO(_MACRO)
#define MY_ENT MT(MOD_RSFT, KC_ENT)

// MACRO LAYER STUFF
#define MAX_CODE_LENGTH 8
#define FIRST_EMOJI ROFL
#define LAST_EMOJI ZANY
#define FIRST_SPANISH A_ACUTE
#define LAST_SPANISH OPEN_X

// Color leds stuff
#ifdef RGB_MATRIX_ENABLE
#define ROW_LENGTH 12
#define MAX_LEDS_PER_KEY 8

uint32_t letter_color = 0xFF0000;
void paint(void);
#endif

// Macro layer data
const char EMOJIS [][7] = {
  "1F923\0", //ROFL, 🤣
  "1F618\0", //KISSES, 😘
  "1F415\0", //DOG, 🐕
  "1F603\0", //SMILE, 😃
  "1F609\0", //WINK, 😉
  "1F44D\0", //THUMBSU, 👍
  "1F92A\0"  //ZANY, 🤪
};

// use 2*i for caps, 2*i + 1 for lower case.
const char SPANISH [][4] = {
  "C1\0", //Á
  "E1\0", //á
  "C9\0", //É
  "E9\0", //é
  "CD\0", //Í
  "ED\0", //í
  "D3\0", //Ó
  "F3\0", //ó
  "DA\0", //Ú
  "FA\0", //ú
  "DC\0", //Ü
  "FC\0", //ü
  "D1\0", //Ñ
  "F1\0", //ñ
  "BF\0", //¿
  "A1\0", //¡
};

enum planck_layers {
  _QWERTY,
  _LOWER,
  _RAISE,
  _NUMPAD,
  _MACRO,
  _ADJUST,
  _COLOR
 };

enum planck_keycodes {
  QWERTY = SAFE_RANGE,
  SET_QWE,
  NPKC,
  // EMOJI_KEYS
  ROFL,
  KISSES,
  DOG,
  SMILE,
  WINK,
  THUMBSU,
  ZANY,
  // SPANISH_KEYS
  A_ACUTE,
  E_ACUTE,
  I_ACUTE,
  O_ACUTE,
  U_ACUTE,
  U_DIAER,
  N_TILDE,
  OPEN_X,
  // Nothing after start_colors
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |CL/MCR| Ctrl | GUI  | Alt  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = LAYOUT_planck_grid(
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
    KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, MY_ENT ,
    MR_COMB, KC_LCTL, KC_LGUI, KC_LALT, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
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
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
    KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,
    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_NUHS, KC_NUBS, KC_PGUP, KC_PGDN, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY
),
/* Macro
 * ,------------------------------------------------------------------------------------.
 * |       |rofl  |wink  |eacute|      |      |udiaer|uacute|iacute|oacute|      |      |
 * |-------+------+------+------+------+-------------+------+------+------+------+------|
 * |       |aacute|smile |dog   |      |      |      |      |thumbs|      |      |      |
 * |-------+------+------+------+------+------|------+------+------+------+------+------|
 * |capslok|zany  |      |      |      |kisses|ntilde|      |      |      | openx|      |
 * |-------+------+------+------+------+------+------+------+------+------+------+------|
 * |       |NumLok|      |      |      |   color     |      |      |      |      |      |
 * `------------------------------------------------------------------------------------'
 */
[_MACRO] = LAYOUT_planck_grid(
    _______,    ROFL,    WINK, E_ACUTE, XXXXXXX,  XXXXXXX, U_DIAER, U_ACUTE, I_ACUTE, O_ACUTE, XXXXXXX, SET_QWE,
    _______, A_ACUTE,   SMILE,     DOG, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, THUMBSU, XXXXXXX, XXXXXXX, _______,
KC_CAPSLOCK,    ZANY, XXXXXXX, XXXXXXX, XXXXXXX,   KISSES, N_TILDE, XXXXXXX, XXXXXXX, XXXXXXX,  OPEN_X, _______,
    _______,    NPKC, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______
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
    KC_TAB,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_KP_7, KC_KP_8, KC_KP_9, KC_PMNS, KC_BSPC,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_KP_4, KC_KP_5, KC_KP_6, KC_PPLS, _______,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_KP_1, KC_KP_2, KC_KP_3, KC_PSLS, KC_PENT,
    SET_QWE, _______, _______, _______, _______, _______, _______, XXXXXXX, KC_KP_0, KC_PDOT, KC_PAST, KC_PEQL
),

/* Adjust (Lower + Raise)
 * ,-----------------------------------------------------------------------------------.
 * |      | Reset|      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |      |      |Aud on|Audoff|      |      |      |      |      |      |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_planck_grid(
    XXXXXXX, RESET,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX,   AU_ON,  AU_OFF, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
)

};

#ifdef AUDIO_ENABLE
  float tone_caps_on[][2]    = SONG(STARTUP_SOUND);
  float tone_caps_off[][2]   = SONG(GOODBYE_SOUND);
#endif

uint8_t current_layer = _QWERTY;
uint8_t prev_layer = _ADJUST;
bool layer_changed = false;

void uni_char(const char *code) {
  SEND_STRING(SS_LSFT(SS_LCTRL("u")));
  send_string(code);
  SEND_STRING(SS_TAP(X_ENTER));
}

void emoji(uint16_t emoji_i) {
  uni_char(EMOJIS[emoji_i]);
}

void spanish_char(uint16_t spanish_i) {
  bool shifted = (bool)(get_mods() & (MOD_BIT(KC_LSHIFT)|MOD_BIT(KC_RSHIFT)));
  uni_char(SPANISH[spanish_i * 2 + (int)!shifted]);
}

uint32_t layer_state_set_user(uint32_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case NPKC:
    if (record->event.pressed) {
      layer_off(_QWERTY);
      layer_off(_COLOR);
      layer_on(_NUMPAD);
    }
    return false;
    break;
  case SET_QWE:
    if (record->event.pressed) {
      layer_off(_NUMPAD);
      layer_off(_COLOR);
      layer_on(_QWERTY);
    }
    return false;
    break;
  //TODO: Merge into a single case.
  case FIRST_EMOJI ... LAST_EMOJI:
    if (record->event.pressed) {
      emoji(keycode - FIRST_EMOJI);
    }
    return false;
    break;
  case FIRST_SPANISH ... LAST_SPANISH:
    if (record->event.pressed) {
      spanish_char(keycode - FIRST_SPANISH);
    }
    return false;
    break;
  }
  return true;
}

void matrix_scan_user(void) {
  for (int i = _QWERTY; i <= _ADJUST; i++) {
    if(layer_state_cmp(layer_state, i)){
      current_layer = i;
    }
  }
  if (current_layer != prev_layer) {
    layer_changed = true;
    prev_layer = current_layer;
  }
  if (layer_changed) {
    layer_changed = false;
#ifdef RGB_MATRIX_ENABLE
    paint();
#endif
  }
}

// Handle CAPSLOCK
void led_set_user(uint8_t usb_led) {
  static uint8_t old_usb_led = 0;

  _delay_ms(10); // gets rid of tick

  if ((usb_led & (1 << USB_LED_CAPS_LOCK)) &&
      !(old_usb_led & (1 << USB_LED_CAPS_LOCK))) {
    // If CAPS LK LED is turning on...
#ifdef RGB_MATRIX_ENABLE
    letter_color = 0xFF0000;
    paint();
#endif
#ifdef AUDIO_ENABLE
    PLAY_SONG(tone_caps_on);
#endif
  } else if (!(usb_led & (1 << USB_LED_CAPS_LOCK)) &&
             (old_usb_led & (1 << USB_LED_CAPS_LOCK))) {
    // If CAPS LK LED is turning off...
#ifdef RGB_MATRIX_ENABLE
    letter_color = 0x009900;
    paint();
#endif
    
#ifdef AUDIO_ENABLE
    PLAY_SONG(tone_caps_off);
#endif
  }
  old_usb_led = usb_led;
}

#ifdef RGB_MATRIX_ENABLE
void color_led_rgb(uint8_t i, uint8_t r, uint8_t g, uint8_t b) {
  uint8_t leds[MAX_LEDS_PER_KEY];
  uint8_t row, column, led_count = 0;

  row = i / ROW_LENGTH;
  column = i % ROW_LENGTH;
  led_count = rgb_matrix_map_row_column_to_led(row, column, leds);
  for (int j = 0; j < led_count; j++) {
    rgb_matrix_set_color(leds[j], r, g, b);
  }
}

void color_cp(uint32_t in, RGB *out) {
  out->r = in >> 16 & 255;
  out->g = in >> 8 & 255;
  out->b = in & 255;
}

void classify(uint16_t code, RGB *res){
  switch(code) {
    case XXXXXXX:
      color_cp(0x000000, res);
      break;
    case KC_A ... KC_Z:
      color_cp(letter_color, res);
      break;
    case KC_1 ... KC_0:
      color_cp(0x000099, res);
      break;
    case KC_MINUS ... KC_SLASH:
      color_cp(0x999900, res);
      break;
    case KC_F1...KC_F12:
      color_cp(0x999999, res);
      break;
    case KC_INSERT...KC_UP:
      color_cp(0x449900, res);
      break;
    case KC_KP_SLASH...KC_KP_DOT:
      color_cp(0x0000FF, res);
      break;
    case KC_BSPACE:
      color_cp(0x770000, res);
      break;
    case KC_NUMLOCK:
      color_cp(0x0000FF, res);
      break;
    case KC_CAPSLOCK:
      color_cp(0xFF0000, res);
      break;
    case KC__MUTE...KC__VOLDOWN:
      color_cp(0xFF9900, res);
      break;
    case KC_AUDIO_MUTE...KC_MEDIA_REWIND:
      color_cp(0xFF9900, res);
      break;
    case KC_LCTRL...KC_RGUI:
      color_cp(0xFFFF99, res);
      break;
    case FIRST_EMOJI...LAST_EMOJI:
      color_cp(0xFFCC00, res);
      break;
    case FIRST_SPANISH...LAST_SPANISH:
      color_cp(0xFF0000, res);
      break;
    case LOWER:
      color_cp(0x000099, res);
      break;
    case RESET:
      color_cp(0xFF0000, res);
      break;
    case MR_COMB:
      color_cp(0xFF00FF, res);
      break;
    default:
      color_cp(0x994400, res);
      break;
  }
}

void paint() {
  keypos_t key;
  uint16_t code;
  RGB this_color;
  uint8_t idx;
  for (int i = 0; i < MATRIX_ROWS; i++){
    for (int j = 0; j < MATRIX_COLS; j++){
      key.row = i;
      key.col = j;
      code = keymap_key_to_keycode(layer_switch_get_layer(key), key);
      classify(code, &this_color);
      idx = i * MATRIX_COLS + j;
      color_led_rgb(idx, this_color.r, this_color.g, this_color.b); 
    }
  }
}
#endif
