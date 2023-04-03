// 
// Vector image editor
//
// 2023, Jonathan Tainer
//

#ifndef VECIMG_EDITOR_H
#define VECIMG_EDITOR_H

#include <raylib.h>

void VecEditInit(float x, float y, float size);

void VecEditSetPos(float x, float y);

void VecEditSetSize(float size);

void VecEditSetColors(Color bg, Color line, Color vert, Color sel);

void VecEditUpdate();

void VecEditDraw();

VecImg* VecEditGetImage();

void VecEditExport(const char* filename);

void VecEditClose();

#endif
