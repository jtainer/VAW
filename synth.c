// 
// Vectorscope synthesizer
//
// 2023, Jonathan Tainer
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "synth.h"

typedef struct {
	int freqInterpEnabled;
	float freqInterpRate;

	// TODO: implement pitch shifting, this isnt actually used yet
	float pitchShiftFactor;
} SYNTH;

static SYNTH synth = { 1, 1.0004f, 1.f };

void SynthEnableFreqInterp(float rate) {
	synth.freqInterpEnabled = 1;
	synth.freqInterpRate = rate;
}

void SynthDisableFreqInterp() {
	synth.freqInterpEnabled = 0;
}

void SynthSetPitchShift(float factor) {
	synth.pitchShiftFactor = factor;
}

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

static float GetEnvelopeAmplitude(float t, float prev, float max, float atk, float dec, float sus) {
	if (t < atk) {
		float slope = (max - prev) / atk;
		return prev + t * slope;
	}
	else if (t < atk + dec) {
		float slope = (sus - max) / dec;
		return fmin(max + (t - atk) * slope, max);
	}
	else {
		return fmin(sus, max);
	}
}

Signal LoadSignalVecImg(VecImg* img, Track* track, unsigned int sampleRate) {
	Signal signal = LoadSignalEmpty(sampleRate);

	// Track frequency between frames to smooth transitions
	float currFreq = track->env[0].frq;

	// Track vector array index between envelopes
	// Used float to accurately interpolate between vectors
	float exactIndex = 0.f;

	for (unsigned int i = 0; i < track->len; i++) {
		unsigned int duration = track->env[i].dur * sampleRate;
		
		// ADSR parameters for current envelope
		float atk = track->env[i].atk * sampleRate;
		float dec = track->env[i].dec * sampleRate;
		float sus = track->env[i].sus;
		float rel = track->env[i].rel;
		
		// Track time within current envelope (count number of samples)
		float envelopeTime = 0.f;
		for (unsigned int t = 0; t < duration; t++) {
			// Smooth changes in frequency if enabled
			float targetFreq = track->env[i].frq;
			if (!synth.freqInterpEnabled) {
				currFreq = targetFreq;
			}
			else if (currFreq < targetFreq) {
				currFreq *= synth.freqInterpRate;
				currFreq = fmin(currFreq, targetFreq);
			}
			else if (currFreq > targetFreq) {
				currFreq /= synth.freqInterpRate;
				currFreq = fmax(currFreq, targetFreq);
			}

			float stepSize = img->len * currFreq / sampleRate;
	
			// No interpolation, just repeat samples
			// unsigned int tmpIndex = (t * stepSize) + vecIndex;
			// tmpIndex %= img->len;
			// SignalAddSample(&signal, img->vec[tmpIndex]);
			
			// Linear interpolation between vertices
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
			float prev = (i > 0) ? track->env[i-1].sus * track->env[i-1].mag : 0.f;
			float max = track->env[i].mag;
			float amp = GetEnvelopeAmplitude(t, prev, max, atk, dec, sus);
			interp.x *= amp;
			interp.y *= amp;
			SignalAddSample(&signal, interp);

			exactIndex += stepSize;
			envelopeTime += stepSize;
		}
		exactIndex = fmod(exactIndex, img->len);
	}

	return signal;
}

void UnloadSignal(Signal* signal) {
	free(signal->sample);
	signal->len = 0;
	signal->max = 0;
}
