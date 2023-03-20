// 
// Vectorscope synthesizer
//
// 2023, Jonathan Tainer
//

#ifndef SYNTH_H
#define SYNTH_H

#include "vecimg.h"
#include "track.h"

typedef struct Signal {
	Vec2* sample;
	unsigned int len;
	unsigned int max;
} Signal;

Signal LoadSignalEmpty(unsigned int max);
Signal LoadSignalVecImg(VecImg* img, Track* track, unsigned int sampleRate);
void SignalAddSample(Signal* signal, Vec2 sample);
void UnloadSignal(Signal* signal);

#endif
