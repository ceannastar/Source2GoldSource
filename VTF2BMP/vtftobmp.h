
/*
	Created: 03/27/2023
	Written by Alexandr Sotnikov
	File: vtf.h
*/

#pragma once

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>

#include <atlbase.h>
#include <atlconv.h>

class VTFH
{
private:

	struct vtfHeader
	{
		int magic;
		unsigned int version[2];
		unsigned int headerSize;
		unsigned short width;
		unsigned short height;
		unsigned int flags;
		unsigned short frames;
		unsigned short firstFrame;
		int padding0;
		float reflectivity[3];
		unsigned char paddin1[4];
		float bumpmapScale;
		unsigned int highResImageFormat;
		unsigned char mipmapCount;
		unsigned int lowResImageFormat;
		unsigned char lowResImageWidth;
		unsigned char lowResImageHeight;
	};

	enum IMAGE_FORMAT
	{
		IMAGE_FORMAT_NONE = -1,
		IMAGE_FORMAT_RGBA8888 = 0,
		IMAGE_FORMAT_ABGR8888,
		IMAGE_FORMAT_RGB888,
		IMAGE_FORMAT_BGR888,
		IMAGE_FORMAT_RGB565,
		IMAGE_FORMAT_I8,
		IMAGE_FORMAT_IA88,
		IMAGE_FORMAT_P8,
		IMAGE_FORMAT_A8,
		IMAGE_FORMAT_RGB888_BLUESCREEN,
		IMAGE_FORMAT_BGR888_BLUESCREEN,
		IMAGE_FORMAT_ARGB8888,
		IMAGE_FORMAT_BGRA8888,
		IMAGE_FORMAT_DXT1,
		IMAGE_FORMAT_DXT3,
		IMAGE_FORMAT_DXT5,
		IMAGE_FORMAT_BGRX8888,
		IMAGE_FORMAT_BGR565,
		IMAGE_FORMAT_BGRX5551,
		IMAGE_FORMAT_BGRA4444,
		IMAGE_FORMAT_DXT1_ONEBITALPHA,
		IMAGE_FORMAT_BGRA5551,
		IMAGE_FORMAT_UV88,
		IMAGE_FORMAT_UVWQ8888,
		IMAGE_FORMAT_RGBA16161616F,
		IMAGE_FORMAT_RGBA16161616,
		IMAGE_FORMAT_UVLX8888
	};

	struct pixelHeader
	{
		int x;
		int y;
		char r;
		char g;
		char b;
	};

public:
	
	pixelHeader* pixel;

private:

public:


	bool read_vtf(std::string filename)
	{
		std::ifstream file(filename + ".vtf", std::ios::in | std::ios::binary);
	
		if (!file.is_open())
		{
			MessageBox(0, L"VTF file is not exist",  L"ERROR", 0);
			return false;
		}

		vtfHeader* vtf = new vtfHeader;

		file.read((char*)&vtf->magic, sizeof vtf->magic);
		file.read((char*)&vtf->version[0], sizeof vtf->version[0]);
		file.read((char*)&vtf->version[1], sizeof vtf->version[1]);
		file.read((char*)&vtf->headerSize, sizeof vtf->headerSize);
		file.read((char*)&vtf->width, sizeof vtf->width);
		file.read((char*)&vtf->height, sizeof vtf->height);
		file.read((char*)&vtf->flags, sizeof vtf->flags);
		file.read((char*)&vtf->frames, sizeof vtf->frames);
		file.read((char*)&vtf->firstFrame, sizeof vtf->firstFrame);
		file.read((char*)&vtf->padding0, sizeof vtf->padding0);
		for(int i = 0; i < 3; ++i)
			file.read((char*)&vtf->reflectivity[i], sizeof vtf->reflectivity[i]);
		for (int i = 0; i < 4; ++i)
		file.read((char*)&vtf->paddin1[i], sizeof vtf->paddin1[i]);
		file.read((char*)&vtf->bumpmapScale, sizeof vtf->bumpmapScale);
		file.read((char*)&vtf->highResImageFormat, sizeof vtf->highResImageFormat);
		file.read((char*)&vtf->mipmapCount, sizeof vtf->mipmapCount);
		file.read((char*)&vtf->lowResImageFormat, sizeof vtf->lowResImageFormat);
		file.read((char*)&vtf->lowResImageWidth, sizeof vtf->lowResImageWidth);
		file.read((char*)&vtf->lowResImageHeight, sizeof vtf->lowResImageHeight);

		if (vtf->magic == 0x00465456)
			MessageBox(0, L"DONE!", L"DONE!", 0);

		std::string version = "Version: "+std::to_string(vtf->version[0]) + "." + std::to_string(vtf->version[1]) + "\nHeaderSize: " + std::to_string(vtf->headerSize) +
			std::to_string(vtf->version[1])+"\nHighResImageFormat: "+std::to_string(vtf->highResImageFormat)+"\nHighResWidth: "+ std::to_string((int)vtf->width)+ "\nHighResHeight: " + std::to_string((int)vtf->height) + "\nMipMap Count: " + std::to_string(vtf->mipmapCount) + "\nLowResWidth: " + std::to_string(vtf->lowResImageWidth) + "\nLowResHeight: " + std::to_string(vtf->lowResImageHeight);
		CA2W unicodeStr(version.c_str());
		MessageBox(0, unicodeStr, L"Information", 0);

		file.seekg(vtf->headerSize);

		unsigned char* color = new unsigned char[vtf->width * vtf->height];

		for (int i = 0; i < vtf->width * vtf->height; i++)
		{
			if ((i+2)>= vtf->width * vtf->height)
				break;
				file.read((char*)&color[i], 1);
				file.read((char*)&color[i+1], 1);
				file.read((char*)&color[i+2], 1);
				i++;
		}

		file.close();

		std::ofstream bmp("image.bmp", std::ios::out | std::ios::binary);

		unsigned short bfType = 0x4D42;
		unsigned short bfReversed1 = 0;
		unsigned short bfReversed2 = 0;
		unsigned int bfOffBits = 54;

		long biWidth = vtf->width;
		long biHeight = vtf->height;
		unsigned short biPlanes = 1;
		unsigned short biBitCount = 8;
		unsigned int biCompression = 0;
		unsigned int biSizeImage = 0;
		long biXPelsPerMeter = 0;
		long biYPelsPerMeter = 0;
		unsigned int biClrUsed = 1;
		unsigned int biClrImportant = 1;
		unsigned int biSize = sizeof biSize + sizeof biWidth + sizeof biHeight + sizeof biPlanes + sizeof biBitCount + sizeof biCompression + sizeof biSizeImage + sizeof biXPelsPerMeter + sizeof biYPelsPerMeter + sizeof biClrUsed + sizeof biClrImportant;

		unsigned int bfSize = sizeof biSize + sizeof biWidth + sizeof biHeight + sizeof biPlanes + sizeof biBitCount + sizeof biCompression + sizeof biSizeImage + sizeof biXPelsPerMeter + sizeof biYPelsPerMeter + sizeof biClrUsed + sizeof biClrImportant + sizeof bfType + sizeof bfReversed1 + sizeof bfReversed2 + sizeof bfOffBits + sizeof bfSize;

		bmp.write((char*)&bfType, sizeof bfType);
		bmp.write((char*)&bfSize, sizeof bfSize);
		bmp.write((char*)&bfReversed1, sizeof bfReversed1);
		bmp.write((char*)&bfReversed2, sizeof bfReversed2);
		bmp.write((char*)&bfOffBits, sizeof bfOffBits);

		bmp.write((char*)&biSize, sizeof biSize);
		bmp.write((char*)&biWidth, sizeof biWidth);
		bmp.write((char*)&biHeight, sizeof biHeight);
		bmp.write((char*)&biPlanes, sizeof biPlanes);
		bmp.write((char*)&biBitCount, sizeof biBitCount);
		bmp.write((char*)&biCompression, sizeof biCompression);
		bmp.write((char*)&biSizeImage, sizeof biSizeImage);
		bmp.write((char*)&biXPelsPerMeter, sizeof biXPelsPerMeter);
		bmp.write((char*)&biYPelsPerMeter, sizeof biYPelsPerMeter);
		bmp.write((char*)&biClrUsed, sizeof biClrUsed);
		bmp.write((char*)&biClrImportant, sizeof biClrImportant);

		for (int i = 0; i < vtf->width * vtf->height; i++)
		{
			if ((i+2)>= vtf->width * vtf->height)
				break;
			bmp.write((char*)&color[i], 1);
			bmp.write((char*)&color[i+1], 1);
			bmp.write((char*)&color[i+2], 1);
		}

		bmp.close();

		return true;
	}
};