// 
// Vectorscope music generator test case
//
// 2023, Jonathan Tainer
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../track.h"
#include "../vecimg.h"
#include "../vecimg_editor.h"
#include "../synth.h"

#include <raylib.h>

int main(int argc, char** argv) {
	int windowWidth = 1024;
	int windowHeight = 1024;
	int editorSize = 1024;
	InitWindow(windowWidth, windowHeight, "editor test");
	SetTargetFPS(60);

	// Load image if one is specified
	Texture bgTexture = { 0 };
	Vector2 bgPos = { 0.f, 0.f };
	float bgAng = 0.f;
	float bgScale = 0.f;
	if (argc >= 2) {
		bgTexture = LoadTexture(argv[1]);
		float texSize = fmax(bgTexture.width, bgTexture.height);
		bgScale = (float) editorSize / texSize;
	}

	VecEditInit(0, 0, editorSize);
	VecEditSetColors(BLANK, RED, ORANGE, YELLOW);

	while (!WindowShouldClose()) {
		VecEditUpdate();

		BeginDrawing();
		ClearBackground(WHITE);
		if (bgTexture.id > 0) {
			DrawTextureEx(bgTexture, bgPos, bgAng, bgScale, WHITE);
		}
		VecEditDraw();
		EndDrawing();
	}
	CloseWindow();

	// Optionally export the VecImg
	printf("Export CSV file as (leave blank to discard): ");
	fflush(stdout);
	const int n = 256;
	char inputBuffer[n];
	char filename[n];
	fgets(inputBuffer, n, stdin);
	int filenameEntered = sscanf(inputBuffer, "%256s", filename);
	if (filenameEntered != EOF) {
		printf("Exporting image as \"%s\"\n", filename);
		VecEditExport(filename);
	}

	VecEditClose();
	return 0;
}
