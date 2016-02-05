/*
 Copyright (c) 2015, 2016
        Hugo "hrkz" Frezat <hugo.frezat@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*/

#include "imagine/interact/impl/keyboard_native.h"

namespace ig       {
namespace impl     {
namespace keyboard {

static const key_t KeyTbl[] =
{                     // keyboard mapping table
                      // Dec |  Hex | Windows Virtual key
  key_t::unknown,     //   0   0x00
  key_t::unknown,     //   1   0x01   VK_LBUTTON
  key_t::unknown,     //   2   0x02   VK_RBUTTON
  key_t::cancel,      //   3   0x03   VK_CANCEL
  key_t::unknown,     //   4   0x04   VK_MBUTTON
  key_t::unknown,     //   5   0x05   VK_XBUTTON1
  key_t::unknown,     //   6   0x06   VK_XBUTTON2
  key_t::unknown,     //   7   0x07   -- unassigned --
  key_t::backspace,   //   8   0x08   VK_BACK
  key_t::tab,         //   9   0x09   VK_TAB
  key_t::unknown,     //  10   0x0A   -- reserved --
  key_t::unknown,     //  11   0x0B   -- reserved --
  key_t::clear,       //  12   0x0C   VK_CLEAR
  key_t::ret,         //  13   0x0D   VK_RETURN
  key_t::unknown,     //  14   0x0E   -- unassigned --
  key_t::unknown,     //  15   0x0F   -- unassigned --
  key_t::shift,       //  16   0x10   VK_SHIFT
  key_t::control,     //  17   0x11   VK_CONTROL
  key_t::alt,         //  18   0x12   VK_MENU
  key_t::pause,       //  19   0x13   VK_PAUSE
  key_t::caps_lock,   //  20   0x14   VK_CAPITAL
  key_t::unknown,     //  21   0x15   VK_KANA / VK_HANGUL
  key_t::unknown,     //  22   0x16   -- unassigned --
  key_t::unknown,     //  23   0x17   VK_JUNJA
  key_t::unknown,     //  24   0x18   VK_FINAL
  key_t::unknown,     //  25   0x19   VK_HANJA / VK_KANJI
  key_t::unknown,     //  26   0x1A   -- unassigned --
  key_t::escape,      //  27   0x1B   VK_ESCAPE
  key_t::unknown,     //  28   0x1C   VK_CONVERT
  key_t::unknown,     //  29   0x1D   VK_NONCONVERT
  key_t::unknown,     //  30   0x1E   VK_ACCEPT
  key_t::mode_switch, //  31   0x1F   VK_MODECHANGE
  key_t::space,       //  32   0x20   VK_SPACE
  key_t::page_up,     //  33   0x21   VK_PRIOR
  key_t::page_down,   //  34   0x22   VK_NEXT
  key_t::end,         //  35   0x23   VK_END
  key_t::home,        //  36   0x24   VK_HOME
  key_t::left,        //  37   0x25   VK_LEFT
  key_t::up,          //  38   0x26   VK_UP
  key_t::right,       //  39   0x27   VK_RIGHT
  key_t::down,        //  40   0x28   VK_DOWN
  key_t::select,      //  41   0x29   VK_SELECT
  key_t::printer,     //  42   0x2A   VK_PRINT
  key_t::execute,     //  43   0x2B   VK_EXECUTE
  key_t::print,       //  44   0x2C   VK_SNAPSHOT
  key_t::insert,      //  45   0x2D   VK_INSERT
  key_t::del,         //  46   0x2E   VK_DELETE
  key_t::help,        //  47   0x2F   VK_HELP

  key_t::unknown,     //  48   0x30   VK_0
  key_t::unknown,     //  49   0x31   VK_1
  key_t::unknown,     //  50   0x32   VK_2
  key_t::unknown,     //  51   0x33   VK_3
  key_t::unknown,     //  52   0x34   VK_4
  key_t::unknown,     //  53   0x35   VK_5
  key_t::unknown,     //  54   0x36   VK_6
  key_t::unknown,     //  55   0x37   VK_7
  key_t::unknown,     //  56   0x38   VK_8
  key_t::unknown,     //  57   0x39   VK_9
  key_t::unknown,     //  58   0x3A   -- unassigned --
  key_t::unknown,     //  59   0x3B   -- unassigned --
  key_t::unknown,     //  60   0x3C   -- unassigned --
  key_t::unknown,     //  61   0x3D   -- unassigned --
  key_t::unknown,     //  62   0x3E   -- unassigned --
  key_t::unknown,     //  63   0x3F   -- unassigned --
  key_t::unknown,     //  64   0x40   -- unassigned --
  key_t::a,           //  65   0x41   VK_A
  key_t::b,           //  66   0x42   VK_B
  key_t::c,           //  67   0x43   VK_C
  key_t::d,           //  68   0x44   VK_D
  key_t::e,           //  69   0x45   VK_E
  key_t::f,           //  70   0x46   VK_F
  key_t::g,           //  71   0x47   VK_G
  key_t::h,           //  72   0x48   VK_H
  key_t::i,           //  73   0x49   VK_I
  key_t::j,           //  74   0x4A   VK_J
  key_t::k,           //  75   0x4B   VK_K
  key_t::l,           //  76   0x4C   VK_L
  key_t::m,           //  77   0x4D   VK_M
  key_t::n,           //  78   0x4E   VK_N
  key_t::o,           //  79   0x4F   VK_O
  key_t::p,           //  80   0x50   VK_P
  key_t::q,           //  81   0x51   VK_Q
  key_t::r,           //  82   0x52   VK_R
  key_t::s,           //  83   0x53   VK_S
  key_t::t,           //  84   0x54   VK_T
  key_t::u,           //  85   0x55   VK_U
  key_t::v,           //  86   0x56   VK_V
  key_t::w,           //  87   0x57   VK_W
  key_t::x,           //  88   0x58   VK_X
  key_t::y,           //  89   0x59   VK_Y
  key_t::z,           //  90   0x5A   VK_Z
  key_t::meta,        //  91   0x5B   VK_LWIN
  key_t::meta,        //  92   0x5C   VK_RWIN
  key_t::menu,        //  93   0x5D   VK_APPS
  key_t::unknown,     //  94   0x5E   -- reserved --
  key_t::sleep,       //  95   0x5F   VK_SLEEP
  key_t::num0,        //  96   0x60   VK_NUMPAD0
  key_t::num1,        //  97   0x61   VK_NUMPAD1
  key_t::num2,        //  98   0x62   VK_NUMPAD2
  key_t::num3,        //  99   0x63   VK_NUMPAD3
  key_t::num4,        // 100   0x64   VK_NUMPAD4
  key_t::num5,        // 101   0x65   VK_NUMPAD5
  key_t::num6,        // 102   0x66   VK_NUMPAD6
  key_t::num7,        // 103   0x67   VK_NUMPAD7
  key_t::num8,        // 104   0x68   VK_NUMPAD8
  key_t::num9,        // 105   0x69   VK_NUMPAD9
  key_t::asterisk,    // 106   0x6A   VK_MULTIPLY
  key_t::plus,        // 107   0x6B   VK_ADD
  key_t::comma,       // 108   0x6C   VK_SEPARATOR
  key_t::minus,       // 109   0x6D   VK_SUBTRACT
  key_t::period,      // 110   0x6E   VK_DECIMAL
  key_t::slash,       // 111   0x6F   VK_DIVIDE
  key_t::f1,          // 112   0x70   VK_F1
  key_t::f2,          // 113   0x71   VK_F2
  key_t::f3,          // 114   0x72   VK_F3
  key_t::f4,          // 115   0x73   VK_F4
  key_t::f5,          // 116   0x74   VK_F5
  key_t::f6,          // 117   0x75   VK_F6
  key_t::f7,          // 118   0x76   VK_F7
  key_t::f8,          // 119   0x77   VK_F8
  key_t::f9,          // 120   0x78   VK_F9
  key_t::f10,         // 121   0x79   VK_F10
  key_t::f11,         // 122   0x7A   VK_F11
  key_t::f12,         // 123   0x7B   VK_F12
  key_t::f13,         // 124   0x7C   VK_F13
  key_t::f14,         // 125   0x7D   VK_F14
  key_t::f15,         // 126   0x7E   VK_F15
  key_t::f16,         // 127   0x7F   VK_F16
  key_t::f17,         // 128   0x80   VK_F17
  key_t::f18,         // 129   0x81   VK_F18
  key_t::f19,         // 130   0x82   VK_F19
  key_t::f20,         // 131   0x83   VK_F20
  key_t::f21,         // 132   0x84   VK_F21
  key_t::f22,         // 133   0x85   VK_F22
  key_t::f23,         // 134   0x86   VK_F23
  key_t::f24,         // 135   0x87   VK_F24
  key_t::unknown,     // 136   0x88   -- unassigned --
  key_t::unknown,     // 137   0x89   -- unassigned --
  key_t::unknown,     // 138   0x8A   -- unassigned --
  key_t::unknown,     // 139   0x8B   -- unassigned --
  key_t::unknown,     // 140   0x8C   -- unassigned --
  key_t::unknown,     // 141   0x8D   -- unassigned --
  key_t::unknown,     // 142   0x8E   -- unassigned --
  key_t::unknown,     // 143   0x8F   -- unassigned --
  key_t::num_lock,    // 144   0x90   VK_NUMLOCK
  key_t::scroll_lock, // 145   0x91   VK_SCROLL
  key_t::unknown,     // 146   0x92   VK_OEM_FJ_JISHO
  key_t::unknown,     // 147   0x93   VK_OEM_FJ_MASSHOU
  key_t::unknown,     // 148   0x94   VK_OEM_FJ_TOUROKU
  key_t::unknown,     // 149   0x95   VK_OEM_FJ_LOYA
  key_t::unknown,     // 150   0x96   VK_OEM_FJ_ROYA
  key_t::unknown,     // 151   0x97   -- unassigned --
  key_t::unknown,     // 152   0x98   -- unassigned --
  key_t::unknown,     // 153   0x99   -- unassigned --
  key_t::unknown,     // 154   0x9A   -- unassigned --
  key_t::unknown,     // 155   0x9B   -- unassigned --
  key_t::unknown,     // 156   0x9C   -- unassigned --
  key_t::unknown,     // 157   0x9D   -- unassigned --
  key_t::unknown,     // 158   0x9E   -- unassigned --
  key_t::unknown,     // 159   0x9F   -- unassigned --
  key_t::shift,       // 160   0xA0   VK_LSHIFT
  key_t::shift,       // 161   0xA1   VK_RSHIFT
  key_t::control,     // 162   0xA2   VK_LCONTROL
  key_t::control,     // 163   0xA3   VK_RCONTROL
  key_t::alt,         // 164   0xA4   VK_LMENU
  key_t::alt,         // 165   0xA5   VK_RMENU
  key_t::back,        // 166   0xA6   VK_BROWSER_BACK
  key_t::forward,     // 167   0xA7   VK_BROWSER_FORWARD
  key_t::refresh,     // 168   0xA8   VK_BROWSER_REFRESH
  key_t::stop,        // 169   0xA9   VK_BROWSER_STOP
  key_t::search,      // 170   0xAA   VK_BROWSER_SEARCH
  key_t::favorites,   // 171   0xAB   VK_BROWSER_FAVORITES
  key_t::home_page,   // 172   0xAC   VK_BROWSER_HOME
  key_t::volume_mute, // 173   0xAD   VK_VOLUME_MUTE
  key_t::volume_down, // 174   0xAE   VK_VOLUME_DOWN
  key_t::volume_up,   // 175   0xAF   VK_VOLUME_UP
  key_t::media_next,  // 176   0xB0   VK_MEDIA_NEXT_TRACK
  key_t::media_previous, //177 0xB1   VK_MEDIA_PREV_TRACK
  key_t::media_stop,  // 178   0xB2   VK_MEDIA_STOP
  key_t::media_play,  // 179   0xB3   VK_MEDIA_PLAY_PAUSE
  key_t::launch_mail, // 180   0xB4   VK_LAUNCH_MAIL
  key_t::launch_media,// 181   0xB5   VK_LAUNCH_MEDIA_SELECT
  key_t::launch0,     // 182   0xB6   VK_LAUNCH_APP1
  key_t::launch1,     // 183   0xB7   VK_LAUNCH_APP2
  key_t::unknown,     // 184   0xB8   -- reserved --
  key_t::unknown,     // 185   0xB9   -- reserved --
  key_t::unknown,     // 186   0xBA   VK_OEM_1
  key_t::unknown,     // 187   0xBB   VK_OEM_PLUS
  key_t::unknown,     // 188   0xBC   VK_OEM_COMMA
  key_t::unknown,     // 189   0xBD   VK_OEM_MINUS
  key_t::unknown,     // 190   0xBE   VK_OEM_PERIOD
  key_t::unknown,     // 191   0xBF   VK_OEM_2
  key_t::unknown,     // 192   0xC0   VK_OEM_3
  key_t::unknown,     // 193   0xC1   -- reserved --
  key_t::unknown,     // 194   0xC2   -- reserved --
  key_t::unknown,     // 195   0xC3   -- reserved --
  key_t::unknown,     // 196   0xC4   -- reserved --
  key_t::unknown,     // 197   0xC5   -- reserved --
  key_t::unknown,     // 198   0xC6   -- reserved --
  key_t::unknown,     // 199   0xC7   -- reserved --
  key_t::unknown,     // 200   0xC8   -- reserved --
  key_t::unknown,     // 201   0xC9   -- reserved --
  key_t::unknown,     // 202   0xCA   -- reserved --
  key_t::unknown,     // 203   0xCB   -- reserved --
  key_t::unknown,     // 204   0xCC   -- reserved --
  key_t::unknown,     // 205   0xCD   -- reserved --
  key_t::unknown,     // 206   0xCE   -- reserved --
  key_t::unknown,     // 207   0xCF   -- reserved --
  key_t::unknown,     // 208   0xD0   -- reserved --
  key_t::unknown,     // 209   0xD1   -- reserved --
  key_t::unknown,     // 210   0xD2   -- reserved --
  key_t::unknown,     // 211   0xD3   -- reserved --
  key_t::unknown,     // 212   0xD4   -- reserved --
  key_t::unknown,     // 213   0xD5   -- reserved --
  key_t::unknown,     // 214   0xD6   -- reserved --
  key_t::unknown,     // 215   0xD7   -- reserved --
  key_t::unknown,     // 216   0xD8   -- unassigned --
  key_t::unknown,     // 217   0xD9   -- unassigned --
  key_t::unknown,     // 218   0xDA   -- unassigned --
  key_t::unknown,     // 219   0xDB   VK_OEM_4
  key_t::unknown,     // 220   0xDC   VK_OEM_5
  key_t::unknown,     // 221   0xDD   VK_OEM_6
  key_t::unknown,     // 222   0xDE   VK_OEM_7
  key_t::unknown,     // 223   0xDF   VK_OEM_8
  key_t::unknown,     // 224   0xE0   -- reserved --
  key_t::unknown,     // 225   0xE1   VK_OEM_AX
  key_t::unknown,     // 226   0xE2   VK_OEM_102
  key_t::unknown,     // 227   0xE3   VK_ICO_HELP
  key_t::unknown,     // 228   0xE4   VK_ICO_00
  key_t::unknown,     // 229   0xE5   VK_PROCESSKEY
  key_t::unknown,     // 230   0xE6   VK_ICO_CLEAR
  key_t::unknown,     // 231   0xE7   VK_PACKET
  key_t::unknown,     // 232   0xE8   -- unassigned --

  key_t::unknown,     // 233   0xE9   VK_OEM_RESET
  key_t::unknown,     // 234   0xEA   VK_OEM_JUMP
  key_t::unknown,     // 235   0xEB   VK_OEM_PA1
  key_t::unknown,     // 236   0xEC   VK_OEM_PA2
  key_t::unknown,     // 237   0xED   VK_OEM_PA3
  key_t::unknown,     // 238   0xEE   VK_OEM_WSCTRL
  key_t::unknown,     // 239   0xEF   VK_OEM_CUSEL
  key_t::unknown,     // 240   0xF0   VK_OEM_ATTN
  key_t::unknown,     // 241   0xF1   VK_OEM_FINISH
  key_t::unknown,     // 242   0xF2   VK_OEM_COPY
  key_t::unknown,     // 243   0xF3   VK_OEM_AUTO
  key_t::unknown,     // 244   0xF4   VK_OEM_ENLW
  key_t::unknown,     // 245   0xF5   VK_OEM_BACKTAB
  key_t::unknown,     // 246   0xF6   VK_ATTN
  key_t::unknown,     // 247   0xF7   VK_CRSEL
  key_t::unknown,     // 248   0xF8   VK_EXSEL
  key_t::unknown,     // 249   0xF9   VK_EREOF
  key_t::play,        // 250   0xFA   VK_PLAY
  key_t::zoom,        // 251   0xFB   VK_ZOOM
  key_t::unknown,     // 252   0xFC   VK_NONAME
  key_t::unknown,     // 253   0xFD   VK_PA1
  key_t::clear,       // 254   0xFE   VK_OEM_CLEAR
  key_t::unknown
};

modifier_ft modifiers()
{
  modifier_ft mods{modifier_t::none};

  if (GetAsyncKeyState(VK_SHIFT)   < 0) mods |= modifier_t::shift;
  if (GetAsyncKeyState(VK_CONTROL) < 0) mods |= modifier_t::ctrl;
  if (GetAsyncKeyState(VK_MENU)    < 0) mods |= modifier_t::alt;
  return mods;
}

key_t key(WPARAM wparam)
{
  return KeyTbl[wparam];
}

} // namespace keyboard
} // namespace impl
} // namespace ig
