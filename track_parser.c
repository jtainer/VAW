// 
// Parse track data from CSV file
//
// 2023, Jonathan Tainer
//

#include <stdio.h>
#include <stdlib.h>
#include "track.h"
#include "freq.h"

typedef struct EnvelopeFileFormat {
	char note;	// Upper or lower case note letters are both valid
	char acc;	// 'N' for natural, 'S' for sharp, 'F' for flat, lower case also valid
	int oct;	// Octave number
	float dur;
	float atk;
	float dec;
	float sus;
	float rel;
} EnvelopeFileFormat;

static Track CreateTrack(unsigned int max) {
	Track track = { 0 };
	track.max = max;
	track.env = malloc(sizeof(Envelope) * max);
	if (track.env == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failed\n");
		track.max = 0;
	}
	return track;
}

void UnloadTrack(Track* track) {
	free(track->env);
	track->max = 0;
	track->len = 0;
}

static void TrackInsertEnvelope(Track* track, Envelope env) {
	if (track->len >= track->max) {
		Envelope* tmp = realloc(track->env, sizeof(Envelope) * track->max * 2);
		if (tmp == NULL) {
			fprintf(stderr, "ERROR: Memory reallocation failed\n");
		}
		else {
			track->env = tmp;
			track->max *= 2;
		}
	}
	if (track->len < track->max) {
		track->env[track->len] = env;
		track->len++;
	}
}

Track LoadTrack(const char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "ERROR: Failed to load file \"%s\"\n", filename);
		return (Track) { 0 };
	}

	Track track = CreateTrack(DEFAULT_TRACK_LENGTH);

	const unsigned int n = 256;
	char buf[n];
	int line = 0;
	while (fgets(buf, n, file)) {
		line++;
		EnvelopeFileFormat format = { 0 };
		int count = sscanf(buf, "%c%c%d,%f,%f,%f,%f,%f",
				&format.note, &format.acc,
				&format.oct, &format.dur,
				&format.atk, &format.dec,
				&format.sus, &format.rel);

		// Detect invalid format in track
		if (count != 8) {
			fprintf(stderr, "ERROR: Track format not recognized\n");
			fprintf(stderr, "\t\"%s\" line %d: %s\n", filename, line, buf);
			UnloadTrack(&track);
			break;
		}

		// Detect invalid note character
		if (!((format.note >= 'a' &&format.note <= 'g') ||
					(format.note >= 'A' && format.note <= 'G'))) {
			fprintf(stderr, "ERROR: Invalid note in track\n");
			fprintf(stderr, "\t\"%s\" line %d: %s\n", filename, line, buf);
			UnloadTrack(&track);
			break;
		}

		// Detect invalid accidental character
		if (!(format.acc == 'N' || format.acc == 'n' ||
					format.acc == 'S' || format.acc == 's' ||
					format.acc == 'F' || format.acc == 'f')) {
			fprintf(stderr, "ERROR: Invalid accidental in track\n");
			fprintf(stderr, "\t\"%s\" line %d: %s\n", filename, line, buf);
			UnloadTrack(&track);
			break;
		}

		// Detect octave number out of range
		if (format.oct < 0 || format.oct > 8) {
			fprintf(stderr, "ERROR: Octave number outside range 0-8\n");
			fprintf(stderr, "\t\"%s\" line %d: %s\n", filename, line, buf);
			UnloadTrack(&track);
			break;

		}

		// Detect negative values
		if (format.dur < 0 || format.atk < 0 || format.dec < 0 ||
				format.sus < 0 || format.rel < 0) {
			fprintf(stderr, "ERROR: Negative floating point value\n");
			fprintf(stderr, "\t\"%s\" line %d: %s\n", filename, line, buf);
			UnloadTrack(&track);
			break;
		}

		// At this point we can be certain the format data is valid

		// Calculate indices of frequency lookup table
		Envelope env = { 0 };
		int noteIndex = 0, accIndex = 0, octIndex = format.oct;
		switch (format.note) {
			case 'A':
			case 'a':
				noteIndex = A;
				break;
			case 'B':
			case 'b':
				noteIndex = B;
				break;
			case 'C':
			case 'c':
				noteIndex = C;
				break;
			case 'D':
			case 'd':
				noteIndex = D;
				break;
			case 'E':
			case 'e':
				noteIndex = E;
				break;
			case 'F':
			case 'f':
				noteIndex = F;
				break;
			case 'G':
			case 'g':
				noteIndex = G;
				break;
		}
		switch (format.acc) {
			case 'N':
			case 'n':
				accIndex = NATL;
				break;
			case 'S':
			case 's':
				accIndex = SHRP;
				break;
			case 'F':
			case 'f':
				accIndex = FLAT;
				break;
		}
		env.frq = FREQ(noteIndex, octIndex, accIndex);
		env.dur = format.dur;
		env.atk = format.atk;
		env.dec = format.dec;
		env.sus = format.sus;
		env.rel = format.rel;
		
		TrackInsertEnvelope(&track, env);
	}

	fclose(file);
	return track;
}
