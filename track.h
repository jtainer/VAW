// 
// Track data object
// Used by synthesizer to modulate an arbitrary waveform
//
// 2023, Jonathan Tainer
//

#ifndef TRACK_H
#define TRACK_H

#define DEFAULT_TRACK_LENGTH 256

typedef struct Envelope {
	float frq;
	float mag;
	float dur;
	float atk;
	float dec;
	float sus;
	float rel;
} Envelope;

typedef struct Track {
	Envelope* env;
	unsigned int len;
	unsigned int max;
} Track;

Track LoadTrack(const char* filename);
void UnloadTrack(Track* track);

#endif
