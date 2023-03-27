// 
// Vector image editor
//
// 2023, Jonathan Tainer
//

#ifndef VECIMG_EDITOR_H
#define VECIMG_EDITOR_H

void VecEditInit(float x, float y, float size);

void VecEditSetPos(float x, float y);

void VecEditSetSize(float size);

void VecEditUpdate();

void VecEditDraw();

VecImg* VecEditGetImage();

void VecEditClose();

#endif
