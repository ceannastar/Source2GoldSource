
/*
	Created: 03/26/2023
	Written by Alexandr Sotnikov
	File: map.h
*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

class BSPH
{
private:

	struct planeHeader
	{
		float normal[3];
		float dist;
		int type;
	};

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

	bool getting_vertices_of_plane(planeHeader* planes, int sizeofplanes)
	{
		vertexHeader* vertices = new vertexHeader;
		for (int i = 0; i < sizeofplanes; i++)
		{
			for (int j = 0; j < sizeofplanes; j++)
			{
				for (int k = 0; k < sizeofplanes; k++)
				{
					if ((i==j)||(i==k)||(j==k))
						break;
					if ((planes[i].type != planes[j].type) && (planes[i].type != planes[k].type) && (planes[j].type != planes[k].type))
					{
							float x = planes[i].normal[0] * planes[i].dist + planes[j].normal[0] * planes[j].dist + planes[k].normal[0] * planes[k].dist;
							float y = planes[i].normal[1] * planes[i].dist + planes[j].normal[1] * planes[j].dist + planes[k].normal[1] * planes[k].dist;
							float z = planes[i].normal[2] * planes[i].dist + planes[j].normal[2] * planes[j].dist + planes[k].normal[2] * planes[k].dist;
							std::cout << "( " << x << " " << y << " " << z << " )"  << std::endl;
					}
				}
			}
		}

		return true;
	}

	planeHeader* read_planes(std::ifstream& file, int offsetofplanes, int sizeofplanes)
	{
		planeHeader* header = new planeHeader[sizeofplanes/20];

		file.seekg(offsetofplanes);

		for (int i = 0; i < sizeofplanes / 20; ++i)
		{
			file.read((char*)&header[i], sizeof header[i]);
		}
		
		return header;
	}

	vertexHeader* read_vertices(std::ifstream& file, int offsetovertices, int sizeofvertices)
	{
		vertexHeader* vertices = new vertexHeader[sizeofvertices/12];

		file.seekg(offsetovertices);

		for (int i = 0; i < sizeofvertices / 12; ++i)
		{
			file.read((char*)&vertices[i], sizeof vertices[i]);
			//std::cout << "(" << vertices[i].x << ";" << vertices[i].y << ";" << vertices[i].z << ") " << std::endl;
		}
		//std::cout << std::endl;

		return vertices;
	}

	bool read_brushes(std::ifstream& file, int offsetofbrushes, int offsetofsides, int sizeofbrushes, int sizeofsides, planeHeader* planes)
	{

		file.seekg(offsetofsides);

		brushSideHeader* brushSides = new brushSideHeader[(sizeofsides / 8)];

		for (int i = 0; i < sizeofsides/8; ++i)
		{
			file.read((char*)&brushSides[i], sizeof brushSides[i]);
		}
		
		file.seekg(offsetofbrushes);

		brushHeader* brushes = new brushHeader[(sizeofbrushes /12)];

		for (int i = 0; i < sizeofbrushes / 12; ++i)
		{
			planeHeader* tempplanes = new planeHeader[sizeofsides / 8];
			file.read((char*)&brushes[i], sizeof brushes[i]);
			for (int i = 0; i < sizeofsides / 8; ++i)
			{
				file.read((char*)&brushSides[i], sizeof brushSides[i]);
				tempplanes[i] = planes[brushSides[i].planenum];
				//std::cout << "(" << planes[brushSides[i].planenum].normal[0] << ";" << planes[brushSides[i].planenum].normal[1] << ";" << planes[brushSides[i].planenum].normal[2] << ") " << planes[brushSides[i].planenum].dist << " " << planes[brushSides[i].planenum].type << std::endl;
			}
			getting_vertices_of_plane(tempplanes, sizeofsides / 8);
			std::cout << std::endl;
		}
		
		return true;
	}

public:

	bool  open_bsp_source(std::string filename)
	{

		std::ifstream file(filename+".bsp", std::ios::in | std::ios::binary);

		file.seekg(file.end);
		int filesize = (int)file.tellg();
		file.seekg(file.beg);

		bspHeader* bspFile = new bspHeader;

		file.read((char*)&bspFile->ident, sizeof bspFile->ident);
		file.read((char*)&bspFile->version, sizeof bspFile->version);
		for(int i = 0; i < 64; ++i)
			file.read((char*)&bspFile->lumps[i], sizeof bspFile->lumps[i]);
		file.read((char*)&bspFile->mapRevision, sizeof bspFile->mapRevision);

		planeHeader* planes = read_planes(file, bspFile->lumps[1].fileofs, bspFile->lumps[1].filelen);
		//read_vertices(file, bspFile->lumps[3].fileofs, bspFile->lumps[3].filelen);
		read_brushes(file, bspFile->lumps[18].fileofs, bspFile->lumps[19].fileofs, bspFile->lumps[18].filelen, bspFile->lumps[19].filelen, planes);

		file.close();

		return true;
	}

};