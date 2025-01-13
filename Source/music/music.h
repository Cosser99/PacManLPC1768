#ifndef MUSIC_H
#define MUSIC_H


//Default: 1.65
#define SPEEDUP 1.6

#define TIMERSCALER 1

#define SECOND 0x17D7840 * TIMERSCALER


typedef char BOOL;
#define TRUE 1
#define FALSE 0
//
#define TIMER_FREQUENCY 25000000  // 25 MHz (timer frequency)
#define NOTE_DIVISOR 45  	
#define N_ENTRY(note) (TIMER_FREQUENCY / ((note) * NOTE_DIVISOR)) 
typedef enum note_durations
{
	time_semibiscroma = (unsigned int)(SECOND * SPEEDUP / 64.0f + 0.5), // 1/128
	time_biscroma = (unsigned int)(SECOND * SPEEDUP / 32.0f + 0.5), // 1/64
	time_semicroma = (unsigned int)(SECOND * SPEEDUP / 16.0f + 0.5), // 1/32
	time_croma = (unsigned int)(SECOND * SPEEDUP / 8.0f + 0.5), // 1/16
	time_semiminima = (unsigned int)(SECOND * SPEEDUP / 4.0f + 0.5), // 1/4
	time_minima = (unsigned int)(SECOND * SPEEDUP / 2.0f + 0.5), // 1/2
	time_semibreve = (unsigned int)(SECOND * SPEEDUP + 0.5), // 1
} NOTE_DURATION;
typedef enum frequencies
{
	B0  = N_ENTRY(31),
	C1  = N_ENTRY(33),
	CS1 = N_ENTRY(35),
	D1  = N_ENTRY(37),
	DS1 = N_ENTRY(39),
	E1  = N_ENTRY(41),
	F1  = N_ENTRY(44),
	FS1 = N_ENTRY(46),
	G1  = N_ENTRY(49),
	GS1 = N_ENTRY(52),
	A1  = N_ENTRY(55),
	AS1 = N_ENTRY(58),
	B1  = N_ENTRY(62),
	C2  = N_ENTRY(65),
	CS2 = N_ENTRY(69),
	D2  = N_ENTRY(73),
	DS2 = N_ENTRY(78),
	E2  = N_ENTRY(82),
	F2  = N_ENTRY(87),
	FS2 = N_ENTRY(93),
	G2  = N_ENTRY(98),
	GS2 = N_ENTRY(104),
	A2  = N_ENTRY(110),
	AS2 = N_ENTRY(117),
	B2  = N_ENTRY(123),
	C3B = N_ENTRY(127),
	C3  = N_ENTRY(131),
	CS3 = N_ENTRY(139),
	D3  = N_ENTRY(147),
	DS3 = N_ENTRY(156),
	E3  = N_ENTRY(165),
	F3  = N_ENTRY(175),
	FS3 = N_ENTRY(185),
	G3  = N_ENTRY(196),
	GS3 = N_ENTRY(208),
	A3  = N_ENTRY(220),
	AS3 = N_ENTRY(233),
	B3  = N_ENTRY(247),
	C4  = N_ENTRY(262),
	CS4 = N_ENTRY(277),
	D4  = N_ENTRY(294),
	DS4 = N_ENTRY(311),
	E4  = N_ENTRY(330),
	F4  = N_ENTRY(349),
	FS4 = N_ENTRY(370),
	G4  = N_ENTRY(392),
	GS4 = N_ENTRY(415),
	A4  = N_ENTRY(440),
	AS4 = N_ENTRY(466),
	B4  = N_ENTRY(494),
	C5  = N_ENTRY(523),
	CS5 = N_ENTRY(554),
	D5  = N_ENTRY(587),
	DS5 = N_ENTRY(622),
	E5  = N_ENTRY(659),
	F5  = N_ENTRY(698),
	FS5 = N_ENTRY(740),
	G5  = N_ENTRY(784),
	GS5 = N_ENTRY(831),
	A5  = N_ENTRY(880),
	AS5 = N_ENTRY(932),
	B5  = N_ENTRY(988),
	C6  = N_ENTRY(1047),
	CS6 = N_ENTRY(1109),
	D6  = N_ENTRY(1175),
	DS6 = N_ENTRY(1245),
	E6  = N_ENTRY(1319),
	F6  = N_ENTRY(1397),
	FS6 = N_ENTRY(1480),
	G6  = N_ENTRY(1568),
	GS6 = N_ENTRY(1661),
	A6  = N_ENTRY(1760),
	AS6 = N_ENTRY(1865),
	B6  = N_ENTRY(1976),
	C7  = N_ENTRY(2093),
	CS7 = N_ENTRY(2217),
	D7  = N_ENTRY(2349),
	DS7 = N_ENTRY(2489),
	E7  = N_ENTRY(2637),
	F7  = N_ENTRY(2794),
	FS7 = N_ENTRY(2960),
	G7  = N_ENTRY(3136),
	GS7 = N_ENTRY(3322),
	A7  = N_ENTRY(3520),
	AS7 = N_ENTRY(3729),
	B7  = N_ENTRY(3951),
	C8	 = N_ENTRY(4186),
	CS8 = N_ENTRY(4435),
	D8	 = N_ENTRY(4699),
	DS8 = N_ENTRY(4978),
	end = -1,
	pause = 0	// DO NOT SOUND
} FREQUENCY;
/*
typedef enum frequencies	// k= freq.tim / (freq.nota*45)   es: K=4240 = 25000000/(131*45)
{
	a2b = 5351,	// 103Hz	k=5351 a2b	La bemolle
	b2 = 4500,	// 123Hz	k=4500 b2		Si 2
	c3b = 4370,	// 127Hz	k)4370 c3b	Do bemolle 3
	c3 = 4240,	// 131Hz	k=4240 c3		Do 3
	d3 = 3779,	// 147Hz	k=3779 d3		Re 3
	e3 = 3367,	// 165Hz	k=3367 e3		Mi 3
	f3 = 3175,	// 175Hz	k=3175 f3		Fa 3
	g3 = 2834,	// 196Hz	k=2834 g3		Sol	3
	a3b = 2670, // 208Hz  k=2670 a4b	La bemolle 3
	a3 = 2525,	// 220Hz	k=2525 a3		La3
	b3 = 2249,	// 247Hz	k=2249 b3		Si 3
	c4 = 2120,	// 262Hz	k=2120 c4		Do 4
	d4 = 1890,	// 294Hz	k=1890 d4		Re 4
	e4 = 1684,	// 330Hz	k=1684 e4		Mi 4
	f4 = 1592,	// 349Hz	k=1592 f4		Fa 4
	g4 = 1417,	// 392Hz	k=1417 g4		Sol 4
	a4 = 1263,	// 440Hz	k=1263 a4		La 4
	b4 = 1125,	// 494Hz	k=1125 b4		Si 4
	c5 = 1062, 	// 523Hz	k=1062 c5		Do 5
	pause = 0		// DO NOT SOUND
} FREQUENCY;
*/

typedef struct 
{
	FREQUENCY freq;
	NOTE_DURATION duration;
} NOTE;



void playNote(NOTE note);
BOOL isNotePlaying(void);

#endif
/* EOF */
