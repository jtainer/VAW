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
	Track track = LoadTrack("assets/track1.csv");
	
	VecImg img = LoadVecImg("assets/triangle.csv");

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
