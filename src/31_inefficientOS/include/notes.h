#ifndef NOTES_H
#define NOTES_H
// Note frequencies (in Hz)
#define C0 16
#define Cs0 17
#define D0 18
#define Ds0 19
#define E0 21
#define F0 22
#define Fs0 23
#define G0 25
#define Gs0 26
#define A0 27
#define As0 29  // A-sharp (same as B-flat)
#define B0 31

#define C1 33
#define Cs1 35
#define D1 37
#define Ds1 39
#define E1 41
#define F1 44
#define Fs1 46
#define G1 49
#define Gs1 52
#define A1 55
#define As1 58  // A-sharp (same as B-flat)
#define B1 62

#define C2 65
#define Cs2 69
#define D2 73
#define Ds2 78
#define E2 82
#define F2 87
#define Fs2 92
#define G2 98
#define Gs2 104
#define A2 110
#define As2 117  // A-sharp (same as B-flat)
#define B2 123

// Adding octave 3 (was missing)
#define C3 131
#define Cs3 139
#define D3 147
#define Ds3 156
#define E3 165
#define F3 175
#define Fs3 185
#define G3 196
#define Gs3 208
#define A3 220
#define As3 233  // A-sharp (same as B-flat)
#define B3 247

#define C4 262
#define Cs4 277
#define D4 294
#define Ds4 311
#define E4 330
#define F4 349
#define Fs4 370
#define G4 392
#define Gs4 415
#define A4 440
#define As4 466  // A-sharp (same as B-flat)
#define B4 494

#define C5 523
#define Cs5 554
#define D5 587
#define Ds5 622
#define E5 659
#define F5 698
#define Fs5 740
#define G5 784
#define Gs5 831
#define A5 880
#define As5 932  // A-sharp (same as B-flat)
#define B5 988

#define C6 1047
#define Cs6 1109
#define D6 1175
#define Ds6 1245
#define E6 1319
#define F6 1397
#define Fs6 1480
#define G6 1568
#define Gs6 1661
#define A6 1760
#define As6 1865  // A-sharp (same as B-flat)
#define B6 1976

#define C7 2093
#define Cs7 2217
#define D7 2349
#define Ds7 2489
#define E7 2637
#define F7 2794
#define Fs7 2960
#define G7 3136
#define Gs7 3322
#define A7 3520
#define As7 3729  // A-sharp (same as B-flat)
#define B7 3951

#define C8 4186
#define Cs8 4435
#define D8 4699
#define Ds8 4978
#define E8 5274
#define F8 5588
#define Fs8 5919
#define G8 6272
#define Gs8 6645
#define A8 7040
#define As8 7459  // A-sharp (same as B-flat)
#define B8 7902

#define C9 8372
#define Cs9 8870
#define D9 9397
#define Ds9 9956
#define E9 10548
#define F9 11175
#define Fs9 11839
#define G9 12543
#define Gs9 13290
#define A9 14080
#define As9 14917  // A-sharp (same as B-flat)
#define B9 15804

// Define flat notes for compatibility
#define Db0 Cs0
#define Eb0 Ds0
#define Gb0 Fs0
#define Ab0 Gs0
#define Bb0 As0

#define Db1 Cs1
#define Eb1 Ds1
#define Gb1 Fs1
#define Ab1 Gs1
#define Bb1 As1

#define Db2 Cs2
#define Eb2 Ds2
#define Gb2 Fs2
#define Ab2 Gs2
#define Bb2 As2

#define Db3 Cs3
#define Eb3 Ds3
#define Gb3 Fs3
#define Ab3 Gs3
#define Bb3 As3

#define Db4 Cs4
#define Eb4 Ds4
#define Gb4 Fs4
#define Ab4 Gs4
#define Bb4 As4

#define Db5 Cs5
#define Eb5 Ds5
#define Gb5 Fs5
#define Ab5 Gs5
#define Bb5 As5

#define Db6 Cs6
#define Eb6 Ds6
#define Gb6 Fs6
#define Ab6 Gs6
#define Bb6 As6

#define Db7 Cs7
#define Eb7 Ds7
#define Gb7 Fs7
#define Ab7 Gs7
#define Bb7 As7

#define Db8 Cs8
#define Eb8 Ds8
#define Gb8 Fs8
#define Ab8 Gs8
#define Bb8 As8

#define Db9 Cs9
#define Eb9 Ds9
#define Gb9 Fs9
#define Ab9 Gs9
#define Bb9 As9

// Legacy compatibility with A_SHARP format
#define A_SHARP4 As4
#define A_FLAT4 Ab4
#define G_SHARP4 Gs4
#define G_FLAT4 Gb4
#define F_SHARP4 Fs4
#define E_FLAT4 Eb4
#define D_SHARP4 Ds4
#define D_FLAT4 Db4
#define C_SHARP4 Cs4
#define B_FLAT4 Bb4

#define A_SHARP5 As5
#define A_FLAT5 Ab5
#define G_SHARP5 Gs5
#define G_FLAT5 Gb5
#define F_SHARP5 Fs5
#define E_FLAT5 Eb5
#define D_SHARP5 Ds5
#define D_FLAT5 Db5
#define C_SHARP5 Cs5
#define B_FLAT5 Bb5

#define A_SHARP3 As3
#define A_FLAT3 Ab3
#define G_SHARP3 Gs3
#define G_FLAT3 Gb3
#define F_SHARP3 Fs3
#define E_FLAT3 Eb3
#define D_SHARP3 Ds3
#define D_FLAT3 Db3
#define C_SHARP3 Cs3
#define B_FLAT3 Bb3

// Rest
#define R 0  // Rest (no sound)
#endif // NOTES_H