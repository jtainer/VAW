//
// Parse vector image data from CSV files
//
// 2023, Jonathan Tainer
//

#include <stdio.h>
#include <stdlib.h>
#include "vecimg.h"

VecImg CreateVecImg(unsigned int max) {
	VecImg img = { 0 };
	img.max = max;
	img.vec = malloc(sizeof(Vec2) * max);
	if (img.vec == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failed\n");
		img.max = 0;
	}
	return img;
}

void ImgInsertVec(VecImg* img, Vec2 vec) {
	if (img->len >= img->max) {
		Vec2* tmp = realloc(img->vec, sizeof(Vec2) * img->max * 2);
		if (tmp == NULL) {
			fprintf(stderr, "ERROR: Memory reallocation failed\n");
		}
		else {
			img->vec = tmp;
			img->max *= 2;
		}
	}
	if (img->len < img->max) {
		img->vec[img->len] = vec;
		img->len++;
	}
	return;
}

VecImg LoadVecImg(const char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "ERROR: Failed to load file \"%s\"\n", filename);
		return (VecImg) { 0 };
	}

	VecImg img = CreateVecImg(DEFAULT_IMAGE_LENGTH);

	const unsigned int n = 256;
	char buf[n];
	int line = 0;
	while (fgets(buf, n, file)) {
		line++;
		Vec2 tmp = { 0 };
		int count = sscanf(buf, "%f,%f", &tmp.x, &tmp.y);
		
		// Detect invalid format in image
		if (count != 2) {
			fprintf(stderr, "ERROR: Vector format not recognized\n");
			fprintf(stderr, "\t\"%s\" line %d: %s\n", filename, line, buf);
			UnloadVecImg(&img);
			break;
		}

		ImgInsertVec(&img, tmp);
	}

	fclose(file);
	return img;
}

void ExportVecImg(VecImg* img, const char* filename) {
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		fprintf(stderr, "ERROR: Failed to open file for writing\n");
		return;
	}

	for (unsigned int i = 0; i < img->len; i++) {
		fprintf(file, "%f,%f\n", img->vec[i].x, img->vec[i].y);
	}

	fclose(file);
	return;
}

void UnloadVecImg(VecImg* img) {
	free(img->vec);
	img->max = 0;
	img->len = 0;
	return;
}

