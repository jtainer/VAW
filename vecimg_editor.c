// 
// Vector image editor
//
// 2023, Jonathan Tainer
//

#include <raylib.h>
#include <raymath.h>
#include "vecimg.h"
#include "vecimg_editor.h"

#define COLOR_BG	BLACK
#define COLOR_LINE	WHITE
#define COLOR_VERT	RED
#define COLOR_SELECT	GREEN

#define MAX_VERTS 1024

typedef struct {
	VecImg image;
	int selection;
	Vector2 offset;
	float size;
	Color bgColor;
	Color lineColor;
	Color vertColor;
	Color selectColor;
} EDITOR;

static EDITOR editor = { (VecImg) { 0 }, -1, (Vector2) { 0.f, 0.f }, 0.f, COLOR_BG, COLOR_LINE, COLOR_VERT, COLOR_SELECT };

void VecEditInit(float x, float y, float size) {
	editor.offset.x = x;
	editor.offset.y = y;
	editor.size = size;
	editor.image = CreateVecImg(MAX_VERTS);
}

void VecEditSetPos(float x, float y) {
	editor.offset.x = x;
	editor.offset.y = y;
}

void VecEditSetSize(float size) {
	editor.size = size;
}

void VecEditSetColors(Color bg, Color line, Color vert, Color sel) {
	editor.bgColor = bg;
	editor.lineColor = line;
	editor.vertColor = vert;
	editor.selectColor = sel;
}

void VecEditUpdate() {

	// Move mouse into vector image coordinate space
	// Scale x and y so they are in the range (-1,1)
	float scale = editor.size / 2.f;
	Vector2 offset = editor.offset;
	offset.x += scale;
	offset.y += scale;
	Vector2 mouse = GetMousePosition();
	mouse = Vector2Subtract(mouse, offset);
	mouse = Vector2Scale(mouse, 1.f / scale);
	
	Vector2* vec = (Vector2*) editor.image.vec;
	float vertexRadius = 0.02f;

	if (mouse.x < -1 || mouse.x > 1 || mouse.y < -1 || mouse.y > 1) {
		editor.selection = -1;
	}
	else if (editor.selection >= 0) {
		if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			editor.selection = -1;
		}
		else if (editor.selection < editor.image.len) {
			vec[editor.selection] = mouse;
		}
	}
	else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		for (unsigned int i = 0; i < editor.image.len; i++) {
			if (Vector2Distance(vec[i], mouse) < vertexRadius) {
				if (IsKeyDown(KEY_LEFT_SHIFT)) {
					ImgDeleteVec(&editor.image, i);
				}
				else {
					editor.selection = i;
					vec[i] = mouse;
					break;
				}
			}
		}
	}
	else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
		ImgInsertVec(&editor.image, (Vec2) { mouse.x, mouse.y });
	}
}

void VecEditDraw() {

	// Background
	DrawRectangle(editor.offset.x, editor.offset.y, editor.size, editor.size, editor.bgColor);

	// Lines
	Vector2* vec = (Vector2*) editor.image.vec;
	float scale = editor.size / 2.f;
	Vector2 offset = editor.offset;
	offset.x += scale;
	offset.y += scale;
	for (unsigned int i = 0; i < editor.image.len; i++) {
		unsigned int beginIndex = i;
		unsigned int endIndex = (i + 1) % editor.image.len;
		Vector2 begin = vec[beginIndex];
		Vector2 end = vec[endIndex];		
		begin = Vector2Scale(begin, scale);
		end = Vector2Scale(end, scale);
		begin = Vector2Add(begin, offset);
		end = Vector2Add(end, offset);
		DrawLineV(begin, end, editor.lineColor);

	}

	// Vertices
	Vector2 mouse = GetMousePosition();
	float rad = editor.size / 100.f;
	for (unsigned int i = 0; i < editor.image.len; i++) {
		Vector2 vert = vec[i];
		vert = Vector2Scale(vert, scale);
		vert = Vector2Add(vert, offset);
		Color color = (Vector2Distance(vert, mouse) < rad) ? editor.selectColor : editor.vertColor;
		DrawCircleV(vert, rad, color);
	}
}

VecImg* VecEditGetImage() {
	return &editor.image;
}

void VecEditExport(const char* filename) {
	ExportVecImg(&editor.image, filename);
}

void VecEditClose() {
	UnloadVecImg(&editor.image);
}
