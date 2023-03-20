// 
// Vectorscope music generator test case
//
// 2023, Jonathan Tainer
//

#include <stdio.h>
#include <stdlib.h>
#include "../track.h"
#include "../vecimg.h"
#include "../synth.h"

// TODO: Replace raylib with my own WAV loader
#include <raylib.h>

#define SAMPLE_RATE 44100
#define SAMPLE_SIZE 32
#define CHANNELS 2
#define OUTPUT_FILE "output.wav"

int main() {
	Track track = LoadTrack("assets/track.csv");
	printf("Loaded %u envelopes:\n", track.len);
	for (unsigned int i = 0; i < track.len; i++) {
		printf("%f\n", track.env[i].frq);
	}
	printf("\n");
	
	VecImg img = LoadVecImg("assets/image.csv");
	printf("Loaded %u vectors:\n", img.len);
	for (unsigned int i = 0; i < img.len; i++) {
		printf("<%f, %f>\n", img.vec[i].x, img.vec[i].y);
	}

	Signal signal = LoadSignalVecImg(&img, &track, SAMPLE_RATE);
	printf("Generated %u samples\n", signal.len);

	Wave wave;
	wave.frameCount = signal.len;
	wave.sampleRate = SAMPLE_RATE;
	wave.sampleSize = SAMPLE_SIZE;
	wave.channels = CHANNELS;
	wave.data = signal.sample;
	ExportWave(wave, OUTPUT_FILE);

	UnloadSignal(&signal);
	UnloadTrack(&track);
	UnloadVecImg(&img);
	return 0;
}
