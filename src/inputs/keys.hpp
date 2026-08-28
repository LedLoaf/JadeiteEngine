#pragma once
#include <SDL2/SDL.h>

namespace jadeite
{
constexpr int KEY_UNKNOWN = -1;

constexpr int KEY_BACKSPACE = SDLK_BACKSPACE;
constexpr int KEY_TAB = SDLK_TAB;
constexpr int KEY_CLEAR = SDLK_CLEAR;
constexpr int KEY_RETURN = SDLK_RETURN;
constexpr int KEY_PAUSE = SDLK_PAUSE;
constexpr int KEY_ESCAPE = SDLK_ESCAPE;
constexpr int KEY_SPACE = SDLK_SPACE;
constexpr int KEY_EXCLAIM = SDLK_EXCLAIM;
constexpr int KEY_QUOTEDBL = SDLK_QUOTEDBL;
constexpr int KEY_HASH = SDLK_HASH;
constexpr int KEY_DOLLAR = SDLK_DOLLAR;
constexpr int KEY_AMPERSAND = SDLK_AMPERSAND;
constexpr int KEY_QUOTE = SDLK_QUOTE;
constexpr int KEY_LEFTPAREN = SDLK_LEFTPAREN;
constexpr int KEY_RIGHTPAREN = SDLK_RIGHTPAREN;
constexpr int KEY_ASTERISK = SDLK_ASTERISK;
constexpr int KEY_PLUS = SDLK_PLUS;
constexpr int KEY_COMMA = SDLK_COMMA;
constexpr int KEY_MINUS = SDLK_MINUS;
constexpr int KEY_PERIOD = SDLK_PERIOD;
constexpr int KEY_SLASH = SDLK_SLASH;
constexpr int KEY_0 = SDLK_0;
constexpr int KEY_1 = SDLK_1;
constexpr int KEY_2 = SDLK_2;
constexpr int KEY_3 = SDLK_3;
constexpr int KEY_4 = SDLK_4;
constexpr int KEY_5 = SDLK_5;
constexpr int KEY_6 = SDLK_6;
constexpr int KEY_7 = SDLK_7;
constexpr int KEY_8 = SDLK_8;
constexpr int KEY_9 = SDLK_9;
constexpr int KEY_COLON = SDLK_COLON;
constexpr int KEY_SEMICOLON = SDLK_SEMICOLON;
constexpr int KEY_LESS = SDLK_LESS;
constexpr int KEY_EQUALS = SDLK_EQUALS;
constexpr int KEY_GREATER = SDLK_GREATER;
constexpr int KEY_QUESTION = SDLK_QUESTION;
constexpr int KEY_AT = SDLK_AT;	

constexpr int KEY_LEFTBRACKET = SDLK_LEFTBRACKET;
constexpr int KEY_BACKSLASH = SDLK_BACKSLASH;
constexpr int KEY_RIGHTBRACKET = SDLK_RIGHTBRACKET;
constexpr int KEY_CARET = SDLK_CARET;
constexpr int KEY_UNDERSCORE = SDLK_UNDERSCORE;
constexpr int KEY_BACKQUOTE = SDLK_BACKQUOTE;
constexpr int KEY_A = SDLK_a;
constexpr int KEY_B = SDLK_b;
constexpr int KEY_C = SDLK_c;
constexpr int KEY_D = SDLK_d;
constexpr int KEY_E = SDLK_e;
constexpr int KEY_F = SDLK_f;
constexpr int KEY_G = SDLK_g;
constexpr int KEY_H = SDLK_h;
constexpr int KEY_I = SDLK_i;
constexpr int KEY_J = SDLK_j;
constexpr int KEY_K = SDLK_k;
constexpr int KEY_L = SDLK_l;
constexpr int KEY_M = SDLK_m;
constexpr int KEY_N = SDLK_n;
constexpr int KEY_O = SDLK_o;
constexpr int KEY_P = SDLK_p;
constexpr int KEY_Q = SDLK_q;
constexpr int KEY_R = SDLK_r;
constexpr int KEY_S = SDLK_s;
constexpr int KEY_T = SDLK_t;
constexpr int KEY_U = SDLK_u;
constexpr int KEY_V = SDLK_v;
constexpr int KEY_W = SDLK_w;
constexpr int KEY_X = SDLK_x;
constexpr int KEY_Y = SDLK_y;
constexpr int KEY_Z = SDLK_z;
constexpr int KEY_DELETE = SDLK_DELETE;

constexpr int KEY_CAPSLOCK = SDLK_CAPSLOCK;

constexpr int KEY_F1 = SDLK_F1;
constexpr int KEY_F2 = SDLK_F2;
constexpr int KEY_F3 = SDLK_F3;
constexpr int KEY_F4 = SDLK_F4;
constexpr int KEY_F5 = SDLK_F5;
constexpr int KEY_F6 = SDLK_F6;
constexpr int KEY_F7 = SDLK_F7;
constexpr int KEY_F8 = SDLK_F8;
constexpr int KEY_F9 = SDLK_F9;
constexpr int KEY_F10 = SDLK_F10;
constexpr int KEY_F11 = SDLK_F11;
constexpr int KEY_F12 = SDLK_F12;

constexpr int KEY_SCROLLOCK = SDLK_SCROLLLOCK;

constexpr int KEY_INSERT = SDLK_INSERT;
constexpr int KEY_HOME = SDLK_HOME;
constexpr int KEY_PAGEUP = SDLK_PAGEUP;
constexpr int KEY_END = SDLK_END;
constexpr int KEY_PAGEDOWN = SDLK_PAGEDOWN;

constexpr int KEY_RIGHT = SDLK_RIGHT;
constexpr int KEY_LEFT = SDLK_LEFT;
constexpr int KEY_DOWN = SDLK_DOWN;
constexpr int KEY_UP = SDLK_UP;

constexpr int KEY_NUMLOCK = SDLK_NUMLOCKCLEAR;
constexpr int KEY_KP_DIVIDE = SDLK_KP_DIVIDE;
constexpr int KEY_KP_MULTIPLY = SDLK_KP_MULTIPLY;
constexpr int KEY_KP_MINUS = SDLK_KP_MINUS;
constexpr int KEY_KP_PLUS = SDLK_KP_PLUS;
constexpr int KEY_KP_ENTER = SDLK_KP_ENTER;
constexpr int KEY_KP1 = SDLK_KP_1;
constexpr int KEY_KP2 = SDLK_KP_2;
constexpr int KEY_KP3 = SDLK_KP_3;
constexpr int KEY_KP4 = SDLK_KP_4;
constexpr int KEY_KP5 = SDLK_KP_5;
constexpr int KEY_KP6 = SDLK_KP_6;
constexpr int KEY_KP7 = SDLK_KP_7;
constexpr int KEY_KP8 = SDLK_KP_8;
constexpr int KEY_KP9 = SDLK_KP_9;
constexpr int KEY_KP0 = SDLK_KP_0;
constexpr int KEY_KP_PERIOD = SDLK_KP_PERIOD;

constexpr int KEY_LCTRL = SDLK_LCTRL;
constexpr int KEY_LSHIFT = SDLK_LSHIFT;
constexpr int KEY_LALT = SDLK_LALT;
constexpr int KEY_RCTRL = SDLK_RCTRL;
constexpr int KEY_RSHIFT = SDLK_RSHIFT;
constexpr int KEY_RALT = SDLK_RALT;

} // jadeite
