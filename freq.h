// 
// Frequency table and calculations
//
// 2022, Jonathan Tainer
//

#ifndef FREQ_TABLE
#define FREQ_TABLE

extern float freq_table[];

#define C 0
#define D 2
#define E 4
#define F 5
#define G 7
#define A 9
#define B 11

#define NATL 0
#define SHRP 1
#define FLAT -1

#define FREQ(NOTE, OCTAVE, ACCIDENTAL) freq_table[12 * OCTAVE + NOTE + ACCIDENTAL]

#endif
