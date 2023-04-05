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

static float GetEnvelopeAmplitude(float t, float atk, float dec, float sus, float prev) {
	if (t < atk) {
		float slope = (1.f - prev) / atk;
		return prev + t * slope;
	}
	else if (t < atk + dec) {
		float slope = (sus - 1.f) / dec;
		return 1.f + (t - atk) * slope;
	}
	else {
		return sus;
	}
}

Signal LoadSignalVecImg(VecImg* img, Track* track, unsigned int sampleRate) {
	Signal signal = LoadSignalEmpty(sampleRate);
	
	unsigned int vecIndex = 0;
	for (unsigned int i = 0; i < track->len; i++) {
		unsigned int duration = track->env[i].dur * sampleRate;
		
		// ADSR parameters for current envelope
		float atk = track->env[i].atk * sampleRate;
		float dec = track->env[i].dec * sampleRate;
		float sus = track->env[i].sus;
		float rel = track->env[i].rel;
		
		// Final magnitude of previous envelope
		float prev = (i > 0) ? track->env[i-1].sus : 0.f;

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

			// Apply ADSR
			float amp = GetEnvelopeAmplitude(t * stepSize, atk, dec, sus, prev) * track->env[i].mag;
			interp.x *= amp;
			interp.y *= amp;
			SignalAddSample(&signal, interp);
		}

		// vecIndex += duration * stepSize;
		vecIndex += track->env[i].dur * track->env[i].frq * img->len;
		vecIndex %= img->len;
	}

	return signal;
}

void UnloadSignal(Signal* signal) {
	free(signal->sample);
	signal->len = 0;
	signal->max = 0;
}
