#pragma once

#include <Arduino.h>

// =========================================================================
// 音名定義（オクターブ 3 〜 6）
// -------------------------------------------------------------------------
//  - シャープ（♯）は「S」で表現します（例：C#4 ➔ NOTE_CS4）
//  - フラット（♭）は「F」で表現します（例：Db4 ➔ NOTE_DF4）
// =========================================================================

#define REST 0

// --- オクターブ 3 ---
#define NOTE_C3   131
#define NOTE_CS3  139
#define NOTE_DF3  139  // CS3 と同じ
#define NOTE_D3   147
#define NOTE_DS3  156
#define NOTE_EF3  156  // DS3 と同じ
#define NOTE_E3   165
#define NOTE_F3   175
#define NOTE_FS3  185
#define NOTE_GF3  185  // FS3 と同じ
#define NOTE_G3   196
#define NOTE_GS3  208
#define NOTE_AF3  208  // GS3 と同じ
#define NOTE_A3   220
#define NOTE_AS3  233
#define NOTE_BF3  233  // AS3 と同じ
#define NOTE_B3   247

// --- オクターブ 4 ---
#define NOTE_C4   262
#define NOTE_CS4  277
#define NOTE_DF4  277  // CS4 と同じ
#define NOTE_D4   294
#define NOTE_DS4  311
#define NOTE_EF4  311  // DS4 と同じ
#define NOTE_E4   330
#define NOTE_F4   349
#define NOTE_FS4  370
#define NOTE_GF4  370  // FS4 と同じ
#define NOTE_G4   392
#define NOTE_GS4  415
#define NOTE_AF4  415  // GS4 と同じ
#define NOTE_A4   440
#define NOTE_AS4  466
#define NOTE_BF4  466  // AS4 と同じ
#define NOTE_B4   494

// --- オクターブ 5 ---
#define NOTE_C5   523
#define NOTE_CS5  554
#define NOTE_DF5  554  // CS5 と同じ
#define NOTE_D5   587
#define NOTE_DS5  622
#define NOTE_EF5  622  // DS5 と同じ
#define NOTE_E5   659
#define NOTE_F5   698
#define NOTE_FS5  740
#define NOTE_GF5  740  // FS5 と同じ
#define NOTE_G5   784
#define NOTE_GS5  831
#define NOTE_AF5  831  // GS5 と同じ
#define NOTE_A5   880
#define NOTE_AS5  932
#define NOTE_BF5  932  // AS5 と同じ
#define NOTE_B5   988

// --- オクターブ 6 ---
#define NOTE_C6   1047
#define NOTE_CS6  1109
#define NOTE_DF6  1109 // CS6 と同じ
#define NOTE_D6   1175
#define NOTE_DS6  1245
#define NOTE_EF6  1245 // DS6 と同じ
#define NOTE_E6   1319
#define NOTE_F6   1397
#define NOTE_FS6  1480
#define NOTE_GF6  1480 // FS6 と同じ
#define NOTE_G6   1568
#define NOTE_GS6  1661
#define NOTE_AF6  1661 // GS6 と同じ
#define NOTE_A6   1760
#define NOTE_AS6  1865
#define NOTE_BF6  1865 // AS6 と同じ
#define NOTE_B6   1976
#define NOTE_C7   2093

#define REST 0 // 休符

// --- 音符の定義 ---
#define NOTE_1   1
#define NOTE_2   2
#define NOTE_4   4
#define NOTE_8   8
#define NOTE_16  16

// --- 構造体の定義 ---
/* 
struct Note {
  int pitch;
  int duration;
  bool isDotted; 
  bool isTied;
};


inline Note N(int p, int d) { return {p, d, false, false}; }
inline Note ND(int p, int d) { return {p, d, true, false}; }
inline Note R(int d) { return {REST, d, false, false}; }
inline Note NT(int p, int d) { return {p, d, false, true}; }
inline Note NDT(int p, int d) { return {p, d, true, true}; }
 */
