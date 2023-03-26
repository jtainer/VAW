// 
// Vectorscope music generator test case
//
// 2023, Jonathan Tainer
//

#include <stdio.h>
#include <stdlib.h>
#include "../track.h"
#include "../vecimg.h"
#include "../vecimg_editor.h"
#include "../synth.h"

#include <raylib.h>

int main() {
	int windowWidth = 1920;
	int windowHeight = 1080;
	InitWindow(windowWidth, windowHeight, "editor test");
	SetTargetFPS(60);

	VecEditInit(0, 0, 1024);

	while (!WindowShouldClose()) {
		VecEditUpdate();

		BeginDrawing();
		ClearBackground(WHITE);
		VecEditDraw();
		EndDrawing();
	}

	VecEditClose();
	CloseWindow();

	return 0;
}
