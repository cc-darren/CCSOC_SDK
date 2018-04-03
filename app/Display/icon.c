/* Copyright (c) 2018 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */

////// !!! nont enough space, so remove all icon about battery except FULL

#include "project.h"
 
#define    ________    0x0
#define    _______X    0x1
#define    ______X_    0x2
#define    ______XX    0x3
#define    _____X__    0x4
#define    _____X_X    0x5
#define    _____XX_    0x6
#define    _____XXX    0x7
#define    ____X___    0x8
#define    ____X__X    0x9
#define    ____X_X_    0xa
#define    ____X_XX    0xb
#define    ____XX__    0xc
#define    ____XX_X    0xd
#define    ____XXX_    0xe
#define    ____XXXX    0xf
#define    ___X____    0x10
#define    ___X___X    0x11
#define    ___X__X_    0x12
#define    ___X__XX    0x13
#define    ___X_X__    0x14
#define    ___X_X_X    0x15
#define    ___X_XX_    0x16
#define    ___X_XXX    0x17
#define    ___XX___    0x18
#define    ___XX__X    0x19
#define    ___XX_X_    0x1a
#define    ___XX_XX    0x1b
#define    ___XXX__    0x1c
#define    ___XXX_X    0x1d
#define    ___XXXX_    0x1e
#define    ___XXXXX    0x1f
#define    __X_____    0x20
#define    __X____X    0x21
#define    __X___X_    0x22
#define    __X___XX    0x23
#define    __X__X__    0x24
#define    __X__X_X    0x25
#define    __X__XX_    0x26
#define    __X__XXX    0x27
#define    __X_X___    0x28
#define    __X_X__X    0x29
#define    __X_X_X_    0x2a
#define    __X_X_XX    0x2b
#define    __X_XX__    0x2c
#define    __X_XX_X    0x2d
#define    __X_XXX_    0x2e
#define    __X_XXXX    0x2f
#define    __XX____    0x30
#define    __XX___X    0x31
#define    __XX__X_    0x32
#define    __XX__XX    0x33
#define    __XX_X__    0x34
#define    __XX_X_X    0x35
#define    __XX_XX_    0x36
#define    __XX_XXX    0x37
#define    __XXX___    0x38
#define    __XXX__X    0x39
#define    __XXX_X_    0x3a
#define    __XXX_XX    0x3b
#define    __XXXX__    0x3c
#define    __XXXX_X    0x3d
#define    __XXXXX_    0x3e
#define    __XXXXXX    0x3f
#define    _X______    0x40
#define    _X_____X    0x41
#define    _X____X_    0x42
#define    _X____XX    0x43
#define    _X___X__    0x44
#define    _X___X_X    0x45
#define    _X___XX_    0x46
#define    _X___XXX    0x47
#define    _X__X___    0x48
#define    _X__X__X    0x49
#define    _X__X_X_    0x4a
#define    _X__X_XX    0x4b
#define    _X__XX__    0x4c
#define    _X__XX_X    0x4d
#define    _X__XXX_    0x4e
#define    _X__XXXX    0x4f
#define    _X_X____    0x50
#define    _X_X___X    0x51
#define    _X_X__X_    0x52
#define    _X_X__XX    0x53
#define    _X_X_X__    0x54
#define    _X_X_X_X    0x55
#define    _X_X_XX_    0x56
#define    _X_X_XXX    0x57
#define    _X_XX___    0x58
#define    _X_XX__X    0x59
#define    _X_XX_X_    0x5a
#define    _X_XX_XX    0x5b
#define    _X_XXX__    0x5c
#define    _X_XXX_X    0x5d
#define    _X_XXXX_    0x5e
#define    _X_XXXXX    0x5f
#define    _XX_____    0x60
#define    _XX____X    0x61
#define    _XX___X_    0x62
#define    _XX___XX    0x63
#define    _XX__X__    0x64
#define    _XX__X_X    0x65
#define    _XX__XX_    0x66
#define    _XX__XXX    0x67
#define    _XX_X___    0x68
#define    _XX_X__X    0x69
#define    _XX_X_X_    0x6a
#define    _XX_X_XX    0x6b
#define    _XX_XX__    0x6c
#define    _XX_XX_X    0x6d
#define    _XX_XXX_    0x6e
#define    _XX_XXXX    0x6f
#define    _XXX____    0x70
#define    _XXX___X    0x71
#define    _XXX__X_    0x72
#define    _XXX__XX    0x73
#define    _XXX_X__    0x74
#define    _XXX_X_X    0x75
#define    _XXX_XX_    0x76
#define    _XXX_XXX    0x77
#define    _XXXX___    0x78
#define    _XXXX__X    0x79
#define    _XXXX_X_    0x7a
#define    _XXXX_XX    0x7b
#define    _XXXXX__    0x7c
#define    _XXXXX_X    0x7d
#define    _XXXXXX_    0x7e
#define    _XXXXXXX    0x7f
#define    X_______    0x80
#define    X______X    0x81
#define    X_____X_    0x82
#define    X_____XX    0x83
#define    X____X__    0x84
#define    X____X_X    0x85
#define    X____XX_    0x86
#define    X____XXX    0x87
#define    X___X___    0x88
#define    X___X__X    0x89
#define    X___X_X_    0x8a
#define    X___X_XX    0x8b
#define    X___XX__    0x8c
#define    X___XX_X    0x8d
#define    X___XXX_    0x8e
#define    X___XXXX    0x8f
#define    X__X____    0x90
#define    X__X___X    0x91
#define    X__X__X_    0x92
#define    X__X__XX    0x93
#define    X__X_X__    0x94
#define    X__X_X_X    0x95
#define    X__X_XX_    0x96
#define    X__X_XXX    0x97
#define    X__XX___    0x98
#define    X__XX__X    0x99
#define    X__XX_X_    0x9a
#define    X__XX_XX    0x9b
#define    X__XXX__    0x9c
#define    X__XXX_X    0x9d
#define    X__XXXX_    0x9e
#define    X__XXXXX    0x9f
#define    X_X_____    0xa0
#define    X_X____X    0xa1
#define    X_X___X_    0xa2
#define    X_X___XX    0xa3
#define    X_X__X__    0xa4
#define    X_X__X_X    0xa5
#define    X_X__XX_    0xa6
#define    X_X__XXX    0xa7
#define    X_X_X___    0xa8
#define    X_X_X__X    0xa9
#define    X_X_X_X_    0xaa
#define    X_X_X_XX    0xab
#define    X_X_XX__    0xac
#define    X_X_XX_X    0xad
#define    X_X_XXX_    0xae
#define    X_X_XXXX    0xaf
#define    X_XX____    0xb0
#define    X_XX___X    0xb1
#define    X_XX__X_    0xb2
#define    X_XX__XX    0xb3
#define    X_XX_X__    0xb4
#define    X_XX_X_X    0xb5
#define    X_XX_XX_    0xb6
#define    X_XX_XXX    0xb7
#define    X_XXX___    0xb8
#define    X_XXX__X    0xb9
#define    X_XXX_X_    0xba
#define    X_XXX_XX    0xbb
#define    X_XXXX__    0xbc
#define    X_XXXX_X    0xbd
#define    X_XXXXX_    0xbe
#define    X_XXXXXX    0xbf
#define    XX______    0xc0
#define    XX_____X    0xc1
#define    XX____X_    0xc2
#define    XX____XX    0xc3
#define    XX___X__    0xc4
#define    XX___X_X    0xc5
#define    XX___XX_    0xc6
#define    XX___XXX    0xc7
#define    XX__X___    0xc8
#define    XX__X__X    0xc9
#define    XX__X_X_    0xca
#define    XX__X_XX    0xcb
#define    XX__XX__    0xcc
#define    XX__XX_X    0xcd
#define    XX__XXX_    0xce
#define    XX__XXXX    0xcf
#define    XX_X____    0xd0
#define    XX_X___X    0xd1
#define    XX_X__X_    0xd2
#define    XX_X__XX    0xd3
#define    XX_X_X__    0xd4
#define    XX_X_X_X    0xd5
#define    XX_X_XX_    0xd6
#define    XX_X_XXX    0xd7
#define    XX_XX___    0xd8
#define    XX_XX__X    0xd9
#define    XX_XX_X_    0xda
#define    XX_XX_XX    0xdb
#define    XX_XXX__    0xdc
#define    XX_XXX_X    0xdd
#define    XX_XXXX_    0xde
#define    XX_XXXXX    0xdf
#define    XXX_____    0xe0
#define    XXX____X    0xe1
#define    XXX___X_    0xe2
#define    XXX___XX    0xe3
#define    XXX__X__    0xe4
#define    XXX__X_X    0xe5
#define    XXX__XX_    0xe6
#define    XXX__XXX    0xe7
#define    XXX_X___    0xe8
#define    XXX_X__X    0xe9
#define    XXX_X_X_    0xea
#define    XXX_X_XX    0xeb
#define    XXX_XX__    0xec
#define    XXX_XX_X    0xed
#define    XXX_XXX_    0xee
#define    XXX_XXXX    0xef
#define    XXXX____    0xf0
#define    XXXX___X    0xf1
#define    XXXX__X_    0xf2
#define    XXXX__XX    0xf3
#define    XXXX_X__    0xf4
#define    XXXX_X_X    0xf5
#define    XXXX_XX_    0xf6
#define    XXXX_XXX    0xf7
#define    XXXXX___    0xf8
#define    XXXXX__X    0xf9
#define    XXXXX_X_    0xfa
#define    XXXXX_XX    0xfb
#define    XXXXXX__    0xfc
#define    XXXXXX_X    0xfd
#define    XXXXXXX_    0xfe
#define    XXXXXXXX    0xff

#define GUI_CONST_STORAGE const

#if 0 // NOT USED

// 40x40 pixel
GUI_CONST_STORAGE unsigned char _acPedometer[] = {
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXX_XXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXX__XX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXX___, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXX__X, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXX_XX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXX_XX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXX_XX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXX_XX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXX__X, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXX_XX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXX__XX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXX__XX, XXXXXXXX, XXXX__XX, XXXXXXXX,
  XXXXXXXX, XXXX__XX, XXXXXXXX, XXX__X_X, XXXXXXXX,
  XXXXXXXX, XXXX____, XXXXXXXX, XX_XXX_X, XXXXXXXX,
  XXXXXXXX, XXXXX___, __XXXXXX, XXXXXX_X, XXXXXXXX,
  XXXXXXXX, XXXXXX__, ______XX, X_XXXX__, XX__XXXX,
  XXXXXXXX, XXXXXXX_, _______X, XXXXX___, XXXX_XXX,
  XXXXXXXX, XXXXXXXX, X_______, ________, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, ________, ________, _XX_XXXX,
  XXXXXXXX, XXXXXXX_, ________, ________, ______XX,
  XXXXXXXX, XXXXXX__, ___XXXXX, X______X, X_____XX,
  XXXXXXXX, XXXXX___, _XXXXXXX, XXX___XX, X_____XX,
  XXXXXXXX, X______X, XXXXXXXX, XXX__XXX, XX__XXXX,
  XXXXXXX_, ____XXXX, XXXXXXXX, XX__XXXX, XXXXXXXX,
  XXXXXX__, _XXXXXXX, XXXXXXXX, XX_XXXXX, XXXXXXXX,
  XXX___XX, XXXXXXXX, XXXXXXXX, XXX__XX_, XXXXXXXX,
  XX___XXX, XXXXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXX__XXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXX__XXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXX_XXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX
};
#endif


GUI_CONST_STORAGE unsigned char _acpedometer1[] = {
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XX_XXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XX_XXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, _X_XXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXX_, _X_XXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXX_, _X_XXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXX_, _X_XXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXX_, _X_XXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXX_, _X_XXXXX, XXXXXXXX,
  XXXXXXXX, __XXX___, XXXXXXX_, _X_XXXXX, XXXXXXXX,
  XXXXXXX_, __XXX___, XXXXXXX_, _X_XXXXX, XXXXXXXX,
  XXXXXXX_, ___XX___, XXXXXXX_, _X_XX__X, XXXXXXXX,
  XXXXXXXX, ____X___, XXXXXXXX, _X_XX__X, XXXXXXXX,
  XXXXXXXX, X___X___, XXXXXXXX, XX_XX__X, XXXXXXXX,
  XXXXXXXX, XX__X___, XXXXX___, XX_XX___, XXXXXXXX,
  XXXXXXXX, XX__X___, XXX_____, _XXXXX__, XXXXXXXX,
  XXXXXXXX, XXX_X___, ________, __XXXX__, XXXXXXXX,
  XXXXXXXX, XXXXX___, ________, __XXXX__, XXXXXXXX,
  XXXXXXXX, XXXXX___, _____X__, ___XXX__, XXXXXXXX,
  XXXXXXXX, XXXXX___, __XXXXX_, ___XX___, XXXXXXXX,
  XXXXXXXX, XXXXX__X, XXXXXXX_, _______X, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, _______X, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, X_____XX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, X_____XX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, X____XXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXX__, _____X_X, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXX____, ___X____, _XXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXX___X, XXXX____, _XXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXX__XX, XXXX____, _XXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXX__XX, XXXXX___, _XXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXX__X, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXX__X, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXX__X, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXX__X, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX
};

GUI_CONST_STORAGE unsigned char _acHRM[] = {
  XXXXXXXX, XXXXXXXX, XXXXXXXX, _______X, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXX__, ________, _XXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXX___, ________, __XXXXXX,
  XXXXXXXX, XXXXXXXX, XXXX____, ________, ___XXXXX,
  XXXXXXXX, XXXXXXXX, XXX_____, ________, ____XXXX,
  XXXXXXXX, XXXXXXXX, XX______, ________, ____XXXX,
  XXXXXXXX, XXXXXXXX, X_______, ________, _____XXX,
  XXXXXXXX, XXXXXXXX, ________, ________, _____XXX,
  XXXXXXXX, XXXXXXX_, ________, ________, _____XXX,
  XXXXXXXX, XXXXX___, ________, ________, ______XX,
  XXXXXXXX, XXXX____, ________, ________, ______XX,
  XXXXXXXX, XXX_____, ________, ________, _____XXX,
  XXXXXXXX, XX______, ________, ________, _____XXX,
  XXXXXXXX, X_______, ________, ________, _____XXX,
  XXXXXXXX, ________, ________, ________, ____XXXX,
  XXXXXXX_, ________, ________, ________, ____XXXX,
  XXXXXX__, ________, ________, ________, ___XXXXX,
  XXXXX___, ________, ________, ________, __XXXXXX,
  XXXX____, ________, ________, ________, _XXXXXXX,
  XXX_____, ________, ________, _______X, XXXXXXXX,
  XXX_____, ________, ________, _______X, XXXXXXXX,
  XXXX____, ________, ________, ________, _XXXXXXX,
  XXXXX___, ________, ________, ________, __XXXXXX,
  XXXXXX__, ________, ________, ________, ___XXXXX,
  XXXXXXX_, ________, ________, ________, ____XXXX,
  XXXXXXXX, ________, ________, ________, ____XXXX,
  XXXXXXXX, X_______, ________, ________, _____XXX,
  XXXXXXXX, XX______, ________, ________, _____XXX,
  XXXXXXXX, XXX_____, ________, ________, _____XXX,
  XXXXXXXX, XXXX____, ________, ________, ______XX,
  XXXXXXXX, XXXXX___, ________, ________, ______XX,
  XXXXXXXX, XXXXXXX_, ________, ________, _____XXX,
  XXXXXXXX, XXXXXXXX, ________, ________, _____XXX,
  XXXXXXXX, XXXXXXXX, X_______, ________, _____XXX,
  XXXXXXXX, XXXXXXXX, XX______, ________, ____XXXX,
  XXXXXXXX, XXXXXXXX, XXX_____, ________, ____XXXX,
  XXXXXXXX, XXXXXXXX, XXXX____, ________, ___XXXXX,
  XXXXXXXX, XXXXXXXX, XXXXX___, ________, __XXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXX__, ________, _XXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, _______X, XXXXXXXX
};


GUI_CONST_STORAGE unsigned char _acCalorie[] = {
  ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________,
  ________, ____XXX_, ________, ________, ________,
  ________, XXXXXXXX, XXXX____, ________, ________,
  ______XX, XXXXXXXX, XXXXXXX_, ________, ________,
  _____XXX, XXXXXXXX, XXXXXX__, ________, ________,
  _____XXX, XXXXXXXX, XXXX____, ________, ________,
  ____XXXX, XXXXXXXX, XXX_____, ________, ________,
  ____XXXX, XXXXXXXX, XXXXXXXX, ________, ________,
  ___XXXXX, XXXXXXXX, XXXXXXXX, XX______, ________,
  ___XXXXX, XXXXXXXX, XXXXXXXX, XXXX____, ________,
  ___XXX__, ____XXXX, XXXXXXXX, XXXXX___, ________,
  ___X____, ______XX, XXXXXXXX, XXXXXXX_, ________,
  ________, _______X, XXXXXXXX, XXXXXXXX, ________,
  ________, ________, _XXXXXXX, XXXXXXXX, X_______,
  ________, ________, ___XXXXX, XXXXXXXX, XX______,
  ________, ________, ____XXXX, XXXXXXXX, XXX_____,
  ________, ________, _____XXX, XXXXXXXX, XXXX____,
  ________, ________, ____XXXX, XXXXXXXX, XXXXXX__,
  ________, ________, __XXXXXX, XXXXXXXX, XXXXXXX_,
  ________, ________, _XXXXXXX, XXXXXXXX, XXX_____,
  __X_____, ________, XXXXXXXX, XXXXXXXX, ________,
  __XX____, _______X, XXXXXXXX, XXXXXX__, ________,
  __XXXX__, ______XX, XXXXXXXX, XXXX____, ________,
  ___XXXXX, _____XXX, XXXXXXXX, XX______, ________,
  ___XXXXX, XXXXXXXX, XXXXXXXX, X_______, ________,
  ___XXXXX, XXXXXXXX, XXXXXX__, ________, ________,
  ____XXXX, XXXXXXXX, XXXXXX__, ________, ________,
  ____XXXX, XXXXXXXX, XXXXXXXX, XXX_____, ________,
  _____XXX, XXXXXXXX, XXXXXXXX, XX______, ________,
  _____XXX, XXXXXXXX, XXXXXXXX, ________, ________,
  _______X, XXXXXXXX, XXXXXX__, ________, ________,
  ________, XXXXXXXX, XXXX____, ________, ________,
  ________, ___XXXXX, X_______, ________, ________,
  ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________,
  ________, ________, ________, ________, ________
};

GUI_CONST_STORAGE unsigned char _acswiming[] = {
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXX_XX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXX_XXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXX__XXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XX__XXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, X___XXXX, XXXXXX__, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, X___XXXX, XXXXXX__, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, X___XXXX, XXXXXX__, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXX_, __XXXXXX, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXX_, __XXXXXX, XXXXXXXX,
  XXXXXXXX, ___XXX__, XXXXXXXX, ___XXXXX, XXXXXXXX,
  XXXXXXXX, ___XXX__, _XXXXXXX, ___XXXXX, XXXXXXXX,
  XXXXXXXX, ___XXX__, _XXXXXXX, ____XXXX, XXXXXXXX,
  XXXXXXXX, ___XXXX_, __XXXXXX, X___XXXX, XXXXXXXX,
  XXXXXXXX, ___XXXX_, __XXXXXX, X____XXX, XXXXXXXX,
  XXXXXXXX, ___XXXX_, ___XXXXX, X____XXX, XXXXXXXX,
  XXXXXXXX, ____XXX_, ____XXXX, X_____XX, XXXXXXXX,
  XXXXXXXX, ____XXXX, ____XX__, ______XX, XXXXXXXX,
  XXXXXXXX, X___XXXX, ________, _____XXX, XXXXXXXX,
  XXXXXXXX, X___XXXX, ________, ___XXXXX, XXXXXXXX,
  XXXXXXXX, X___XXXX, ________, __XXXXXX, XXXXXXXX,
  XXXXXXXX, X____XXX, X_______, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XX___XXX, X_____XX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XX___XXX, X____XXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XX___XXX, X__XXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXX___XX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXX___XX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXX___XX, XXXX____, __XXXXXX, XXXXXXXX,
  XXXXXXXX, XXX___XX, XXX_____, ___XXXXX, XXXXXXXX,
  XXXXXXXX, XXX___XX, XXX_____, ___XXXXX, XXXXXXXX,
  XXXXXXXX, XXX___XX, XXX_____, ___XXXXX, XXXXXXXX,
  XXXXXXXX, XXX___XX, XXX_____, ___XXXXX, XXXXXXXX,
  XXXXXXXX, XXX___XX, XXX_____, ___XXXXX, XXXXXXXX,
  XXXXXXXX, XXX__XXX, XXXX____, __XXXXXX, XXXXXXXX,
  XXXXXXXX, XXX__XXX, XXXXX___, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXX__XXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXX_XXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXX_XXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XX_XXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX
};

GUI_CONST_STORAGE unsigned char _acmessage[] = {
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXX____, ________, ________, ________, __XXXXXX,
  XX______, ________, ________, ________, ____XXXX,
  XX______, _XXXXXXX, XXXXXXXX, XXXXXXX_, ____XXXX,
  XX______, _XXXXXXX, XXXXXXXX, XXXXXXX_, ____XXXX,
  XX______, _XXXXXXX, XXXXXXXX, XXXXXXX_, ____XXXX,
  XX______, _XXXXXXX, XXXXXXXX, XXXXXXX_, ____XXXX,
  XX______, _XXXXXXX, XXXXXXXX, XXXXXXXX, ____XXXX,
  XX______, _XXXXXXX, XXXXXX__, _____XXX, X___XXXX,
  XX__XX__, _XXXXXXX, XXXX____, _______X, XX__XXXX,
  XX_XXXX_, _XXXXXXX, XXX_____, ________, XXX_XXXX,
  XX_XXXX_, _XXXXXXX, XX______, ________, _XXXXXXX,
  XX__XX__, _XXXXXXX, XX______, ________, __XXXXXX,
  XX______, _XXXXXXX, X______X, ____X___, __XXXXXX,
  XX______, _XXXXXXX, X_____XX, ___XX___, ___XXXXX,
  XX______, _XXX____, ______XX, ___XX___, ___XXXXX,
  XX______, _XXX____, ______XX, ___XX___, ___XXXXX,
  XX______, _XXXX___, ______XX, ___XX___, ___XXXXX,
  XX______, _XXXXX__, ______XX, ___XX___, ___XXXXX,
  XX______, ____XXX_, ______XX, ___XX___, ___XXXXX,
  XXX_____, _____XXX, ______XX, ___XX___, ___XXXXX,
  XXXXXXXX, XXXXXXXX, X_______, ___XX___, ___XXXXX,
  XXXXXXXX, XXXXXXXX, X_______, ___XX___, ___XXXXX,
  XXXXXXXX, XXXXXXXX, X_______, ___XX___, ___XXXXX,
  XXXXXXXX, XXXXXXXX, X_______, ___XX___, ___XXXXX,
  XXXXXXXX, XXXXXXXX, X_______, ___XX___, ___XXXXX,
  XXXXXXXX, XXXXXXXX, X_______, ________, __XXXXXX,
  XXXXXXXX, XXXXXXXX, XX______, ________, __XXXXXX,
  XXXXXXXX, XXXXXXXX, XX______, ________, _XXXXXXX,
  XXXXXXXX, XXXXXXXX, XXX_____, ________, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXX___, _______X, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXX_, _____XXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX
};

GUI_CONST_STORAGE unsigned char _acDistance[] = {
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXX__, ___XXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXX____, _____XXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XX______, ______XX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, ________, XX_____X, XXXXXXXX, XXXXXXXX,
  XXXXXX__, ______XX, XXXX____, XXXXXXXX, XXXXXXXX,
  XXXX____, _____XXX, XXXX____, XXXXXXXX, XXXXXXXX,
  XX______, _____XXX, XXXXX___, XXXXXXXX, XXXXXXXX,
  ________, _____XXX, XXXXX___, XXXXXXXX, XXXXXXXX,
  XX______, _____XXX, XXXXX___, XXXXXXXX, XXXXXXXX,
  _XXX____, _____XXX, XXXX____, XXXXXXXX, XXXXXXXX,
  ___XXX__, ______XX, XXX_____, XXXXXXXX, XXXXXXXX,
  _____XXX, ________, _______X, XXXXXXXX, XXXXXXXX,
  X______X, XX______, ______XX, XXXXXXXX, XXXXXXXX,
  X_______, XXXX____, _____XXX, XXXXXXXX, XXXXXXXX,
  X_______, XXXXXXX_, ___XXXXX, XXXXXXXX, XXXXXXXX,
  X_______, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XX______, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XX______, _XXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXX_____, __XXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXX_____, ___XXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXX____, ______X_, _______X, XXXXXXXX, XXXXXXXX,
  XXXXX___, ________, ________, _XXXXXXX, XXXXXXXX,
  XXXXXX__, ________, ________, __XXXXXX, XXXXXXXX,
  XXXXXXX_, ________, ____XXX_, __XXXXXX, XXXXXXXX,
  XXXXXXXX, X_______, __XXXXXX, X__XXXXX, XXXX_XXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XX_XXXXX, X______X,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXX_XXX_, ___XX___,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXX___, __XXXX__,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXX____, __XXXX__,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXX__, ___XXX__,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, _______X,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XX____XX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX
};


GUI_CONST_STORAGE unsigned char _acbattery[] = {
  XXXXXXXX, XX______, ________, ______XX, XXXXXXXX,
  XXXXXXXX, X_______, ________, _______X, XXXXXXXX,
  XXXXXXXX, ________, ________, ________, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___X____, ________, ____X___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ____XXXX, XXXXXXXX, XXXX____, XXXXXXXX,
  XXXXXXXX, ________, _XXXXXX_, ________, XXXXXXXX,
  XXXXXXXX, X_______, _XXXXXX_, _______X, XXXXXXXX,
  XXXXXXXX, XX______, _XXXXXX_, ______XX, XXXXXXXX,
  XXXXXXXX, XXXXXX__, ________, __XXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXX__, ________, __XXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXX__, ________, __XXXXXX, XXXXXXXX
};


GUI_CONST_STORAGE unsigned char _acincommingcall[] = {
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXX__X, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXX_____, __XXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XX_X____, ____XXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, X___X___, ______XX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, X____X__, _______X, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XX____X_, ________, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXX____X, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXX____, XXX_____, __XXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXX__X, XXXX____, ___XXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXX__, ____XXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXX_, _____XXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, _____XXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, ______XX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, X_____XX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, X______X, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, X______X, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, _X_____X, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXX_, __X_____, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXX_, ___X____, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, ____X__X, XXXXXXXX,
  XXXXXXXX, XXXX_XX_, XX_XXXXX, X____X_X, XXXXXXXX,
  XXXXXXXX, XXXX_XX_, XX_XXXXX, XX____XX, XXXXXXXX,
  XXXXXXXX, XXXX_XX_, XX__XXXX, XXX__XXX, XXXXXXXX,
  XXXXXXXX, XXXX_XX_, _XX__XXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXX__XX, __XX__XX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXX_XX, ___XXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXX__X, X___XXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXX__, XXX___XX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, _XXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, ____XXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XX____XX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX
};

GUI_CONST_STORAGE unsigned char _acsms[] = {
  XXXXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, X_______, ____XXXX, XXXXXXXX,
  XXXXXXXX, XXXXXX__, ________, ______XX, XXXXXXXX,
  XXXXXXXX, XXXXX___, ________, _______X, XXXXXXXX,
  XXXXXXXX, XXX_____, ________, ________, XXXXXXXX,
  XXXXXXXX, XX______, ________, ________, _XXXXXXX,
  XXXXXXXX, X_______, __X__XXX, ________, __XXXXXX,
  XXXXXXXX, X_______, __X__X__, X_______, __XXXXXX,
  X_______, ________, __X_XX__, X_______, ___XXXXX,
  X_______, ________, __XXX___, X_______, ___XXXXX,
  X_______, ________, ________, ________, ____XXXX,
  X_______, ________, ________, ________, ____XXXX,
  XX______, ________, ________, ________, _____XXX,
  XXX_____, ________, __XXXXXX, X_______, _____XXX,
  XXX_____, ________, ______XX, X_______, _____XXX,
  XXXX____, ________, ____XXXX, ________, _____XXX,
  XXXXX___, ________, __XX____, ________, _____XXX,
  XXXXXX__, ________, ___XXX__, ________, _____XXX,
  XXXXXX__, ________, ______XX, X_______, _____XXX,
  XXXXXX__, ________, __XXXXXX, X_______, _____XXX,
  XXXXXXX_, ________, __XXX___, ________, _____XXX,
  XXXXXXX_, ________, ________, ________, _____XXX,
  XXXXXXX_, ________, ________, ________, ____X__X,
  XXXXXXXX, ________, __X___XX, ________, ____X__X,
  XXXXXXXX, ________, __X__XXX, X_______, ___XX__X,
  XXXXXXXX, X_______, __X__X__, X_______, ___X___X,
  XXXXXXXX, X_______, __X_XX__, X_______, __XX___X,
  XXXXXXXX, XX______, __XXX___, ________, _XX____X,
  XXXXXXXX, XXX_____, ________, ________, XX____XX,
  XXXXXXXX, XXXX____, ________, _______X, X_____XX,
  XXXXXXXX, X__XX___, ________, ______XX, ______XX,
  XXXXXXXX, X___XXX_, ________, ____XXX_, _____XXX,
  XXXXXXXX, ______XX, X_______, __XXX___, _____XXX,
  XXXXXXXX, ________, XXXXXXXX, XXX_____, ____XXXX,
  XXXXXXXX, ________, _____XX_, ________, ____XXXX,
  XXXXXXXX, ________, ________, ________, ___XXXXX,
  XXXXXXXX, _____XXX, ________, ________, __XXXXXX,
  XXXXXXXX, __XXXXXX, X_______, ________, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXX_____, _______X, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXX__, ____XXXX, XXXXXXXX
};

GUI_CONST_STORAGE unsigned char _aclongsit[] = {
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXX_____, ________, __XXXXXX, XXXXXXXX, XXXXXXXX,
  XX______, ________, _______X, XXXXXXXX, XXXXXXXX,
  XX______, ________, ________, __XXXXXX, XXXXXXXX,
  XX______, ________, ________, _____XXX, XXXXXXXX,
  XXXX_XXX, XXXXXX__, ________, _______X, XXXXXXXX,
  XXXXXXXX, XXXXXX__, ________, ________, _XXXXXXX,
  XXXXXXXX, XXXXXX__, ________, ________, __XXXXXX,
  XXXXXXXX, X_______, ________, ________, ___XXXXX,
  XXXX____, ________, ________, ______XX, ____XXXX,
  XXX_____, ________, ________, ______XX, X____XXX,
  XX______, ________, ________, _____XXX, XX___XXX,
  XX______, __XXXXXX, ___X____, ________, _X____XX,
  XXX___XX, XXXXXXXX, ___XXX__, ________, __X___XX,
  XXXXXXXX, XXXXXXXX, ________, __X_____, ______XX,
  XXXXXXXX, XXXXXXXX, X_______, XXX_____, _______X,
  XXXXXXXX, XXXXXXXX, X______X, XXX_____, ___X___X,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXX_____, ___X__XX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXX_____, __XXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXX____, _XXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX
};

GUI_CONST_STORAGE unsigned char _acAlarm[] = {
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXX__, ___XXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXX____, _____XXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXX_____, ______XX,
  X____XXX, XXXX____, _______X, XXXXX___, _______X,
  ______XX, X_______, ________, __XXXX__, _______X,
  _______X, ________, ________, ___XXXX_, ________,
  X_______, ________, ________, _____XXX, ________,
  XX______, ________, ________, ______XX, X_______,
  XXX_____, _____XXX, XXXXXX__, _______X, XX______,
  XXX_____, ___XXXXX, XXXXXXXX, _______X, XXX_____,
  XXX_____, _XXXXXXX, XXXXXXXX, XX______, XXX____X,
  XX______, XXXXXXXX, XXXXXXXX, XXX_____, _XXX___X,
  XX______, XXXXXXXX, XXXXXXXX, XXX_____, _XXX__XX,
  X______X, XXXXXXXX, XXXXXXXX, XXXX____, __XXXXXX,
  X_____XX, XXXXXXXX, XXXXXXXX, XXXXX___, __XXXXXX,
  X_____XX, XXXXXX__, XXXXXXXX, XXXXX___, __XXXXXX,
  ______XX, XXXXX___, __XXXXXX, XXXXX___, ___XXXXX,
  _____XXX, XXXXX___, ___XXXXX, XXXXXX__, ___XXXXX,
  _____XXX, XXXXX___, ________, ___XXX__, ___XXXXX,
  _____XXX, XXXXXX__, ________, ___XXX__, ___XXXXX,
  _____XXX, XXXXXXX_, ________, ___XXX__, ___XXXXX,
  _____XXX, XXXXXXXX, ________, ___XXX__, ___XXXXX,
  _____XXX, XXXXXXXX, XXXXXXXX, XXXXXX__, ___XXXXX,
  ______XX, XXXXXXXX, XXXXXXXX, XXXXX___, ___XXXXX,
  X_____XX, XXXXXXXX, XXXXXXXX, XXXXX___, __XXXXXX,
  X_____XX, XXXXXXXX, XXXXXXXX, XXXXX___, __XXXXXX,
  X______X, XXXXXXXX, XXXXXXXX, XXXX____, __XXXXXX,
  XX______, XXXXXXXX, XXXXXXXX, XXX_____, _XXX__XX,
  XX______, XXXXXXXX, XXXXXXXX, XXX_____, _XXX___X,
  XXX_____, _XXXXXXX, XXXXXXXX, XX______, XXX____X,
  XXX_____, ___XXXXX, XXXXXXXX, _______X, XXX_____,
  XXX_____, _____XXX, XXXXXX__, _______X, XX______,
  XX______, ________, ________, ______XX, X_______,
  X_______, ________, ________, _____XXX, ________,
  _______X, ________, ________, ___XXXX_, ________,
  ______XX, X_______, ________, __XXXX__, _______X,
  X____XXX, XXXX____, _______X, XXXXX___, _______X,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXX_____, ______XX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXX____, _____XXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXX__, ___XXXXX
};

#if 0 // NOT USED
// 24x24 pixel
GUI_CONST_STORAGE unsigned char _acBatteryCharging24x24[] = {
  XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXX__, ________, __XXXXXX,
  XXXXXX__, ________, __XXXXXX,
  XXXXXX_X, XXXXXXXX, X_XXXXXX,
  XXXXXX_X, XXXXXXXX, X_XXXXXX,
  XXXXXX_X, XXXXXXXX, X_XXXXXX,
  XXXXXX_X, XXXXXXXX, X_XXXXXX,
  XXXXXX_X, XXXX_XXX, X_XXXXXX,
  XXXXXX_X, XXXX_XXX, X_XXXXXX,
  XXXXXX_X, XXXX__XX, X_XXXXXX,
  XXXXXX_X, XX____XX, X_XXXXXX,
  XXXXXX_X, XX__XXXX, X_XXXXXX,
  XXXXXX_X, XXX_XXXX, X_XXXXXX,
  XXXXXX_X, XXX_XXXX, X_XXXXXX,
  XXXXXX_X, XXXXXXXX, X_XXXXXX,
  XXXXXX_X, XXXXXXXX, X_XXXXXX,
  XXXXXX_X, XXXXXXXX, X_XXXXXX,
  XXXXXX__, XXXXXXXX, __XXXXXX,
  XXXXXX__, ________, __XXXXXX,
  XXXXXXXX, XX____XX, XXXXXXXX,
  XXXXXXXX, XX____XX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX
};


// 16x16 pixel
GUI_CONST_STORAGE unsigned char _acBatteryCharging16x16[] = {
  XXXXXXXX, XXXXXXXX,
  XXXX____, ____XXXX,
  XXXX____, ____XXXX,
  XXXX_XXX, XXX_XXXX,
  XXXX_XXX, XXX_XXXX,
  XXXX_XXX, XXX_XXXX,
  XXXX_XXX, _XX_XXXX,
  XXXX_XX_, _XX_XXXX,
  XXXX_XX_, XXX_XXXX,
  XXXX_XX_, XXX_XXXX,
  XXXX_XXX, XXX_XXXX,
  XXXX_XXX, XXX_XXXX,
  XXXX__XX, XX__XXXX,
  XXXXXX__, __XXXXXX,
  XXXXXXX_, _XXXXXXX,
  XXXXXXXX, XXXXXXXX
};

#endif

GUI_CONST_STORAGE unsigned char _achrmclose[] = {
  XXXXXXXX, X_XXXXXX, XXXXXXXX, XXXXX_XX, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXX____, XXXXXXXX,
  XXXXXXX_, ____XXXX, XXXXXXXX, XXX_____, _XXXXXXX,
  XXXXXX__, _____XXX, XXXXXXXX, XX______, _XXXXXXX,
  XXXXXXX_, ______XX, XXXXXXXX, X_______, XXXXXXXX,
  XXXXXXXX, _______X, XXXXXXXX, _______X, XXXXXXXX,
  XXXXXXXX, X_______, XXXXXXX_, ______XX, XXXXXXXX,
  XXXXXXXX, XX______, _XXXXX__, _____XXX, XXXXXXXX,
  XXXXXXXX, XXX_____, __XXX___, ____XXXX, XXXXXXXX,
  XXXXXXXX, XXXX____, ___X____, ___XXXXX, XXXXXXXX,
  XXXXXXXX, XXXXX___, ________, __XXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXX__, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, _______X, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXX__, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXX___, ________, __XXXXXX, XXXXXXXX,
  XXXXXXXX, XXXX____, ___X____, ___XXXXX, XXXXXXXX,
  XXXXXXXX, XXX_____, __XXX___, ____XXXX, XXXXXXXX,
  XXXXXXXX, XX______, _XXXXX__, _____XXX, XXXXXXXX,
  XXXXXXXX, X_______, XXXXXXX_, ______XX, XXXXXXXX,
  XXXXXXXX, _______X, XXXXXXXX, _______X, XXXXXXXX,
  XXXXXXX_, ______XX, XXXXXXXX, X_______, XXXXXXXX,
  XXXXXX__, _____XXX, XXXXXXXX, XX______, _XXXXXXX,
  XXXXXX__, ____XXXX, XXXX____, XXX_____, _XXXXXXX,
  XXXXXXX_, ___XXXXX, XXX_____, __XX____, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XX______, __XXX__X, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, X_______, __XXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, ________, __XXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, __XXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXX__, ________, __XXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXX___, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXX___, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXX__, ________, __XXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, __XXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, ________, __XXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, X_______, __XXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XX______, __XXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXX_____, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXX____, XXXXXXXX, XXXXXXXX
};

#if 1  // NOT USED!
GUI_CONST_STORAGE unsigned char _accharging[] = {
#if 1    
XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, X_______, ________, _______X, XXXXXXXX,
  XXXXXXXX, ________, ________, ________, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, __X_____, ________, ____XX__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ____X___, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ____X___, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ___XX___, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ___XX___, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, __XXX___, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, _XXXX___, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, _XXXX___, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, XXXXXXXX, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, XXXXXXXX, _____X__, XXXXXXXX,
  XXXXXXXX, __X____X, XXXXXXX_, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, __XXXXX_, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, __XXXX__, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, __XXXX__, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, __XXX___, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, __XX____, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, __XX____, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, __X_____, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, __X_____, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, ________, ________, ________, XXXXXXXX,
  XXXXXXXX, X_______, ________, _______X, XXXXXXXX,
  XXXXXXXX, XXXXXX__, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX
#else  
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, X_______, ________, ________, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXXX__, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXX_XX, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XXX____X, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XX_____X, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, X_______, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, ________, _XXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XX____XX, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XX____XX, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, X_____XX, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, X____XXX, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, X____XXX, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, _____XXX, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, ________, ___XXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXX_, ________, __XXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXX_, ________, __XXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XXXX____, __XXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XXXX____, _XXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XXXX____, _XXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XXX_____, _XXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XXX_____, _XXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XX______, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XX______, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XX______, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXXX_, _XXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, _XXXXXXX,
  XXXXXXXX, ________, _XXXXXX_, ________, XXXXXXXX,
  XXXXXXXX, XX______, _XXXXXX_, _______X, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, _XXXXXX_, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, X______X, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX
#endif  
};



GUI_CONST_STORAGE unsigned char _acbattery_LowPower[] = {
#if 1
XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XX______, ________, ______XX, XXXXXXXX,
  XXXXXXXX, ________, ________, ________, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, ________, ________, ________, XXXXXXXX,
  XXXXXXXX, X_______, ________, ________, XXXXXXXX,
  XXXXXXXX, XXX_____, ________, _____XXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX
#else
  XXXXXXXX, XX______, ________, ______XX, XXXXXXXX,
  XXXXXXXX, X_______, ________, _______X, XXXXXXXX,
  XXXXXXXX, ________, ________, ________, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, ____XXXX, XXXXXXXX, XXXX____, XXXXXXXX,
  XXXXXXXX, ________, _XXXXXX_, ________, XXXXXXXX,
  XXXXXXXX, X_______, _XXXXXX_, _______X, XXXXXXXX,
  XXXXXXXX, XX______, _XXXXXX_, ______XX, XXXXXXXX,
  XXXXXXXX, XXXXXX__, ________, __XXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXX__, ________, __XXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXX__, ________, __XXXXXX, XXXXXXXX
#endif  
};
#endif

GUI_CONST_STORAGE unsigned char _acbatteryfull[] = {
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,  
  XXXXXXXX, XX______, ________, ______XX, XXXXXXXX,
  XXXXXXXX, ________, ________, ________, XXXXXXXX,
  XXXXXXXX, ___XXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, ________, ________, ________, XXXXXXXX,
  XXXXXXXX, ________, ________, _______X, XXXXXXXX,
  XXXXXXXX, XXX_____, ________, _____XXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX
};


GUI_CONST_STORAGE unsigned char _acbatteryLevel4[] = {
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,    
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, X_______, ________, _______X, XXXXXXXX,
  XXXXXXXX, ________, ________, ________, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, __X_____, ________, ____XX__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, ____XX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, ________, ________, ________, XXXXXXXX,
  XXXXXXXX, X_______, ________, _______X, XXXXXXXX,
  XXXXXXXX, XXXXXX__, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX    
};
GUI_CONST_STORAGE unsigned char _acbatteryLevel3[] = {
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, X_______, ________, _______X, XXXXXXXX,
  XXXXXXXX, ________, ________, ________, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, ________, ________, ________, XXXXXXXX,
  XXXXXXXX, X_______, ________, _______X, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX   
};
GUI_CONST_STORAGE unsigned char _acbatteryLevel2[] = {
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, X_______, ________, _______X, XXXXXXXX,
  XXXXXXXX, ________, ________, ________, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, __X_____, ________, ____XX__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, ____XX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, ________, ________, ________, XXXXXXXX,
  XXXXXXXX, ________, ________, _______X, XXXXXXXX,
  XXXXXXXX, XXXXXX__, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX  
};
GUI_CONST_STORAGE unsigned char _acbatteryLevel1[] = {
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, 
  XXXXXXXX, X_______, ________, _______X, XXXXXXXX,
  XXXXXXXX, ________, ________, ________, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXX___, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __X_____, ________, _____X__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, __XXXXXX, XXXXXXXX, XXXXXX__, XXXXXXXX,
  XXXXXXXX, ________, ________, ________, XXXXXXXX,
  XXXXXXXX, X_______, ________, _______X, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX
};

