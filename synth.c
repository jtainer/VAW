// 
// Vectorscope synthesizer
//
// 2023, Jonathan Tainer
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "synth.h"

Signal LoadSignalEmpty(unsigned int max) {
	Signal signal = { 0 };
	signal.max = max;
	signal.sample = malloc(sizeof(Vec2) * max);
	if (signal.sample == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failed\n");
		signal.max = 0;
	}
	return signal;
}

void SignalAddSample(Signal* signal, Vec2 sample) {
	if (signal->len >= signal->max) {
		Vec2* tmp = realloc(signal->sample, sizeof(Vec2) * signal->max * 2);
		if (tmp == NULL) {
			fprintf(stderr, "ERROR: Memory reallocation failed\n");
		}
		else {
			signal->sample = tmp;
			signal->max *= 2;
		}
	}
	if (signal->len < signal->max) {
		signal->sample[signal->len] = sample;
		signal->len++;
	}
}

Signal LoadSignalVecImg(VecImg* img, Track* track, unsigned int sampleRate) {
	Signal signal = LoadSignalEmpty(sampleRate);
	
	unsigned int vecIndex = 0;
	for (unsigned int i = 0; i < track->len; i++) {
		unsigned int duration = track->env[i].dur * sampleRate;
		float stepSize = img->len * track->env[i].frq / sampleRate;
		for (unsigned int t = 0; t < duration; t++) {
			// No interpolation, just repeat samples
			// unsigned int tmpIndex = (t * stepSize) + vecIndex;
			// tmpIndex %= img->len;
			// SignalAddSample(&signal, img->vec[tmpIndex]);
			
			// Linear interpolation between vertices
			float exactIndex = (t * stepSize) + (float) vecIndex;
			unsigned int beginIndex = floorf(exactIndex);
			unsigned int endIndex = ceilf(exactIndex);
			beginIndex %= img->len;
			endIndex %= img->len;
			float endFactor = exactIndex - floorf(exactIndex);
			float beginFactor = 1.f - endFactor;
			Vec2 begin = img->vec[beginIndex];
			Vec2 end = img->vec[endIndex];
			Vec2 interp;
			interp.x = beginFactor * begin.x + endFactor * end.x;
			interp.y = beginFactor * begin.y + endFactor * end.y;
			interp.x *= track->env[i].mag;
			interp.y *= track->env[i].mag;
			SignalAddSample(&signal, interp);
		}
		vecIndex += duration * stepSize;
		vecIndex %= img->len;
	}

	return signal;
}

void UnloadSignal(Signal* signal) {
	free(signal->sample);
	signal->len = 0;
	signal->max = 0;
}
