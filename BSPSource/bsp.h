
/*
	Created: 03/26/2023
	Written by Alexandr Sotnikov
	File: map.h
*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>

class BSPH
{
private:

	struct vertexHeader
	{
		float x;
		float y;
		float z;
	};

	struct brushHeader
	{
		int firstSide;
		int numSides;
		int contents;
	};

	struct brushSideHeader
	{
		unsigned short planenum;
		short texinfo;
		short dispinfo;
		short bevel;
	};

	struct lumpHeader
	{
		int fileofs;
		int filelen;
		int version;
		char byte[4];
	};

	struct bspHeader
	{
		int ident;
		int version;
		lumpHeader lumps[64];
		int mapRevision;
	};

private:

	bool read_brushes(std::ifstream& file, int offsetofbrushes, int offsetofsides, int sizeofbrushes, int sizeofsides)
	{

		file.seekg(offsetofbrushes);

		brushHeader* brushes = new brushHeader[(sizeofbrushes /12)];
		brushSideHeader* brushSides = new brushSideHeader[(sizeofsides/8)];

		for (int i = 0; i < sizeofbrushes / 12; ++i)
		{
			file.read((char*)&brushes, sizeof brushes);
			std::cout << brushes[i].firstSide << std::endl;
		}

		return true;
	}

public:

	bool  open_bsp_source(std::string filename)
	{

		std::ifstream file(filename+".bsp", std::ios::in | std::ios::binary);

		file.seekg(file.end);
		int filesize = file.tellg();
		file.seekg(file.beg);

		bspHeader* bspFile = new bspHeader;

		file.read((char*)&bspFile->ident, sizeof bspFile->ident);
		file.read((char*)&bspFile->version, sizeof bspFile->version);
		for(int i = 0; i < 64; ++i)
			file.read((char*)&bspFile->lumps[i], sizeof bspFile->lumps[i]);
		file.read((char*)&bspFile->mapRevision, sizeof bspFile->mapRevision);

		read_brushes(file, bspFile->lumps[18].fileofs, bspFile->lumps[19].fileofs, bspFile->lumps[18].filelen, bspFile->lumps[19].filelen);

		file.close();

		return true;
	}

};