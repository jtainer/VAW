//
// Vector image format for creating oscilloscope music
//
// 2023, Jonathan Tainer
//

#ifndef VECIMG_H
#define VECIMG_H

#define DEFAULT_IMAGE_LENGTH 512

typedef struct Vec2 {
	float x, y;
} Vec2;

typedef struct VecImg {
	Vec2* vec;
	unsigned int len;
	unsigned int max;
} VecImg;

VecImg CreateVecImg(unsigned int max);
void ImgInsertVec(VecImg* img, Vec2 vec);
VecImg LoadVecImg(const char* filename);
VecImg CopyVecImg(VecImg* source);
void ExportVecImg(VecImg* img, const char* filename);
void UnloadVecImg(VecImg* img);

#endif
