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

enum planck_layers { _QWERTY, _LOWER, _RAISE, _NUMPAD, _MACRO, _ADJUST, _COLOR };

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define MY_ENT MT(MOD_RSFT, KC_ENT)
#define MR_COMB MO(_MACRO)


uint8_t current_layer = _QWERTY;
uint8_t prev_layer = _ADJUST;
bool layer_changed = false;

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
  COLOR_KC,
  START_COLORS
  // Nothing after start_colors
};
#define SETC(x) START_COLORS + x

#define MAX_CODE_LENGTH 8
void uni_char(const char *code) {
  SEND_STRING(SS_LSFT(SS_LCTRL("u")));
  send_string(code);
  SEND_STRING(SS_TAP(X_ENTER));
}

#define FIRST_EMOJI ROFL
#define LAST_EMOJI ZANY
const char EMOJIS [][7] = {
  "1F923\0", //ROFL, 🤣
  "1F618\0", //KISSES, 😘
  "1F415\0", //DOG, 🐕
  "1F603\0", //SMILE, 😃
  "1F609\0", //WINK, 😉
  "1F44D\0", //THUMBSU, 👍
  "1F92A\0"  //ZANY, 🤪
};


void emoji(uint16_t emoji_i) {
  uni_char(EMOJIS[emoji_i]);
}

#define FIRST_SPANISH A_ACUTE
#define LAST_SPANISH OPEN_X
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

void spanish_char(uint16_t spanish_i) {
  bool shifted = (bool)(get_mods() & (MOD_BIT(KC_LSHIFT)|MOD_BIT(KC_RSHIFT)));
  uni_char(SPANISH[spanish_i * 2 + (int)!shifted]);
}

#ifdef RGB_MATRIX_ENABLE
void set_default_color(uint8_t, uint8_t, uint8_t, uint8_t);
uint32_t default_colors[48];
uint8_t current_hue = 255;
uint8_t current_sat = 255;
uint8_t current_val = 255;
uint8_t current_key = 0;
bool hue_set = false;
bool sat_set = false;
bool key_set = false;
void paint(void);
void pick_hue(void);
void pick_sat(void);
void pick_val(void);
void color_kb(uint8_t mode);
uint8_t step_vals[] = {
    0,   5,  10,  16,  21,  27,  32,  37,  43,  48,  54,  59,
   65,  70,  75,  81,  86,  92,  97, 103, 108, 113, 119, 124,
  130, 135, 141, 146, 151, 157, 162, 168, 173, 179, 184, 189,
  195, 200, 206, 211, 217, 222, 227, 233, 238, 244, 249, 255
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
#endif


// TODO: refactor the following (they may already exist)
#define NUM_KEYS 48
#define ROW_LENGTH 12
#define MAX_LEDS_PER_KEY 8
#define Q_INDEX 1
#define WHITE 0xFFFFFF
#define ORANGE 0xFFA500
#define BLUE 0x0000FF
#define PURPLE 0xFF00FF
#define RED 0xFF0000
#define YELLOW 0xFFFF00
#define GREEN 0x00FF00

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
    _______,    ROFL,    WINK, E_ACUTE, _______,  _______, U_DIAER, U_ACUTE, I_ACUTE, O_ACUTE, _______, SET_QWE,
    _______, A_ACUTE,   SMILE,     DOG, _______,  _______, _______, _______, THUMBSU, _______, _______, _______,
KC_CAPSLOCK,    ZANY, _______, _______, _______,   KISSES, N_TILDE, _______, _______, _______,  OPEN_X, _______,
    _______,    NPKC, _______, _______, _______, COLOR_KC, _______, _______, _______, _______, _______, _______
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
    KC_TAB,  _______, _______, _______, _______, _______, _______, KC_KP_7, KC_KP_8, KC_KP_9, KC_PMNS, KC_BSPC,
    _______, _______, _______, _______, _______, _______, _______, KC_KP_4, KC_KP_5, KC_KP_6, KC_PPLS, _______,
    _______, _______, _______, _______, _______, _______, _______, KC_KP_1, KC_KP_2, KC_KP_3, KC_PSLS, KC_PENT,
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
    _______, _______, _______,   AU_ON,  AU_OFF, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

// Special key for leaving the _COLOR layer by holding it and tapping key 11
// (should act as SET_QWE)
[_COLOR] = LAYOUT_planck_grid(
     SETC(0),  SETC(1),  SETC(2),  SETC(3),  SETC(4),  SETC(5),  SETC(6),  SETC(7),  SETC(8),  SETC(9), SETC(10), SETC(11),
    SETC(12), SETC(13), SETC(14), SETC(15), SETC(16), SETC(17), SETC(18), SETC(19), SETC(20), SETC(21), SETC(22), SETC(23),
    SETC(24), SETC(25), SETC(26), SETC(27), SETC(28), SETC(29), SETC(30), SETC(31), SETC(32), SETC(33), SETC(34), SETC(35),
    SETC(36), SETC(37), SETC(38), SETC(39), SETC(40), SETC(41), SETC(42), SETC(43), SETC(44), SETC(45), SETC(46), SETC(47)
)


};

#ifdef AUDIO_ENABLE
  float tone_caps_on[][2]    = SONG(STARTUP_SOUND);
  float tone_caps_off[][2]   = SONG(GOODBYE_SOUND);
#endif


uint32_t layer_state_set_user(uint32_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case COLOR_KC:
    if (record->event.pressed) {
      layer_off(_QWERTY);
      layer_off(_NUMPAD);
      layer_on(_COLOR);
    }
    return false;
    break;
  case NPKC:
    if (record->event.pressed) {
      //set_single_persistent_default_layer(_NUMPAD);
      layer_off(_QWERTY);
      layer_off(_COLOR);
      layer_on(_NUMPAD);
    }
    return false;
    break;
  case SET_QWE:
    if (record->event.pressed) {
      //set_single_persistent_default_layer(_QWERTY);
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
#ifdef RGB_MATRIX_ENABLE
  case SETC(0) ... SETC(47):
    if (record->event.pressed) {

      if (!key_set) {
        current_key = keycode - SETC(0);
        key_set = true;
        pick_hue();
      } else if (!hue_set) {
        current_hue = step_vals[keycode - SETC(0)];
        hue_set = true;
        pick_sat();
      } else if (!sat_set) {
        current_sat = step_vals[keycode - SETC(0)];
        sat_set = true;
        pick_val();
      } else {
        current_val = step_vals[keycode - SETC(0)];
        set_default_color(current_key, current_hue, current_sat, current_val);
        key_set = false;
        hue_set = false;
        sat_set = false;
      }
      return false;
    }
    return true;
    break;
#endif
  }
  return true;
}

void matrix_scan_user(void) {
  for (int i = _QWERTY; i <= _COLOR; i++) {
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
    switch (current_layer) {
    case _COLOR:
      paint();
      break;
    case _QWERTY:
      paint();
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
#endif
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

#ifdef RGB_MATRIX_ENABLE

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


// TODO exclude if no rgbmatrix
void color_kb(uint8_t mode) {
  uint8_t indices[NUM_KEYS];
  uint32_t colors[NUM_KEYS];
  uint8_t count = 0;
  //Google Colors
  uint8_t blue_indices[] = {
    0,1,2,12,13,14,24,25,26, //G
    19,20,31,32,43,44 //g
  };
  uint8_t red_indices[] = {
    15,16,27,28, //o
    22,23,34,35 //e
  };
  uint8_t yellow_indices[] = {
    17,18,29,30 //o
  };
  uint8_t green_indices[] = {
    9,21,33 //l
  };
  blank_me(indices, colors);
  switch (mode) {
  case QWERTY_MODE:
    for (int i = 0; i < 15; i ++) {
      indices[count] = blue_indices[i];
      colors[count] = BLUE;
      count++;
    }
    for (int i = 0; i < 8; i ++) {
      indices[count] = red_indices[i];
      colors[count] = RED;
      count++;
    }
    for (int i = 0; i < 4; i ++) {
      indices[count] = yellow_indices[i];
      colors[count] = YELLOW;
      count++;
    }
    for (int i = 0; i < 3; i ++) {
      indices[count] = green_indices[i];
      colors[count] = GREEN;
      count++;
    }
    break;
  case RAISE_MODE:
    for (int i = 0; i < 12 && count < NUM_KEYS; i++) {
      indices[count] = i;
      colors[count] = BLUE;
      count++;
    }
    break;
  case LOWER_MODE:
    for (int i = 12; i < 24 && count < NUM_KEYS; i++) {
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
      if (i % 2 == (i / 12 % 2)) {
        indices[count] = i;
        colors[count] = PURPLE;
        count++;
      }
    }
    break;
  case ADJUST_MODE:
    indices[count] = Q_INDEX;
    colors[count] = WHITE;
    count++;
    indices[count] = 15;
    colors[count] = GREEN;
    count++;
    indices[count] = 16;
    colors[count] = RED;
    count++;
    break;
  case NUMPAD_MODE:
    for (int i = 7; i < 10 && count < NUM_KEYS; i++) {
      for (int j = 0; j < 3 && count < NUM_KEYS; j++) {
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
#endif

//TODO: use sound if enabled
void led_set_user(uint8_t usb_led) {
  static uint8_t old_usb_led = 0;

  _delay_ms(10); // gets rid of tick

  if ((usb_led & (1 << USB_LED_CAPS_LOCK)) &&
      !(old_usb_led & (1 << USB_LED_CAPS_LOCK))) {
    // If CAPS LK LED is turning on...
#ifdef RGB_MATRIX_ENABLE
    color_kb(CAPS_MODE);
#endif
#ifdef AUDIO_ENABLE
    PLAY_SONG(tone_caps_on);
#endif
  } else if (!(usb_led & (1 << USB_LED_CAPS_LOCK)) &&
             (old_usb_led & (1 << USB_LED_CAPS_LOCK))) {
    // If CAPS LK LED is turning off...
    layer_changed = true;
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

void color_led_hsv(uint8_t i, uint8_t h, uint8_t s, uint8_t v){
  HSV x;
  x.h = h;
  x.s = s;
  x.v = v;
  RGB rgb = hsv_to_rgb(x);
  color_led_rgb(i, rgb.r, rgb.g, rgb.b);
}


void paint(void){
  uint8_t r, g, b;
  for(int i = 0; i < 48; i++){
    b = default_colors[i] & 255;
    g = (default_colors[i] >> 8) & 255;
    r = (default_colors[i] >> 16) & 255;
    color_led_rgb(i, r, g, b);
  }
}


void set_default_color(uint8_t i, uint8_t h, uint8_t s, uint8_t v){
  // Jam key at index 1.
  if (i==1 && h == 5 && s == 5 && v == 5) {
    layer_off(_COLOR);
    layer_on(_QWERTY);
    return;
  }
  HSV x;
  RGB rgb;
  uint32_t val;

  x.h = h;
  x.s = s;
  x.v = v;
  rgb = hsv_to_rgb(x);

  val = i;
  val = val << 8;
  val |= rgb.r;
  val = val << 8;
  val |= rgb.g;
  val = val << 8;
  val |= rgb.b;
  default_colors[i] = val;
  paint();
}

void pick_hue() {
  current_sat = 255;
  current_val = 255;
  for (int i = 0; i < 48; i++) {
    color_led_hsv(i, step_vals[i], current_sat, current_val);
  }
}

void pick_sat() {
  for (int i = 0; i < 48; i++) {
    color_led_hsv(i, current_hue, step_vals[i], current_val);
  }
}

void pick_val() {
  for (int i = 0; i < 48; i++) {
    color_led_hsv(i, current_hue, current_sat, step_vals[i]);
  }
}
#endif
