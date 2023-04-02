
/*
	Created: 03/26/2023
	Written by Alexandr Sotnikov
	File: bsp.h
*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

#include "vector.h"

class BSPH
{
private:

	struct planeHeader
	{
		float normal[3];
		float dist;
		int type;
	};

	struct originalFaceHeader
	{
		unsigned short planenum;
		unsigned char side;
		unsigned char onNode;
		int firstEdge;
		short numEdges;
		short texInfo;
		short dispInfo;
		short surfaceFogVolumeID;
		unsigned char styles[4];
		int lightofs;
		float area;
		int LightmapTextureMinsInLuxels[2];
		int LightmapTextureSizeInLuxels[2];
		int origFace; //??????
		unsigned short firstPrimID;
		unsigned int smoothingGropus;
	};

	struct edgeHeader
	{
		unsigned short v[2];
	};

	struct surfedgeHeader
	{
		signed int e;
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

	vertexHeader* vertices;
	int numOfVerts;

	vertexHeader* mapsVertices;

	std::string filenameout;

private:

	float cross_product(vertexHeader a, vertexHeader b)
	{
		return (a.y*b.z - a.z*b.y)+(a.z*b.x-a.x*b.z)+(a.x*b.y-a.y*b.x);
	}

	float dot_product(vertexHeader a, vertexHeader b)
	{
		return (a.x*b.x + a.y*b.y + a.z*b.z);
	}

private:

	bool read_vertices(std::ifstream& file, int offsetofvertices, int sizeofvertices)
	{

		file.seekg(offsetofvertices);

		numOfVerts = sizeofvertices / 12;

		mapsVertices = new vertexHeader[numOfVerts];
		vertices = new vertexHeader[numOfVerts];

		for (int i = 0; i < numOfVerts; ++i)
		{
			file.read((char*)&vertices[i].x, sizeof vertices[i].x);			
			file.read((char*)&vertices[i].y, sizeof vertices[i].y);			
			file.read((char*)&vertices[i].z, sizeof vertices[i].z);			
		}

		return true;
	}

	planeHeader* read_planes(std::ifstream& file, int offsetofplanes, int sizeofplanes)
	{
		planeHeader* header = new planeHeader[sizeofplanes / 20];

		file.seekg(offsetofplanes);

		for (int i = 0; i < sizeofplanes / 20; ++i)
		{
			file.read((char*)&header[i], sizeof header[i]);
		}

		return header;
	}

	bool brushes_to_map(std::ifstream& file, int offsetofbrushes, int offsetofsides, int sizeofbrushes, int sizeofsides, planeHeader* planes)
	{

		std::ofstream fileout(filenameout +".map", std::ios::out);

		fileout.write("{\n", 2);

		fileout.write("\"classname\" ", std::strlen("\"classname\" ")); fileout.write("\"worldspawn\"\n", 13);
		fileout.write("\"defaultteam\" ", 14); fileout.write("\"0\"\n", 4);
		fileout.write("\"newunit\" ", 10); fileout.write("\"0\"\n", 4);
		fileout.write("\"gametitle\" ", 12); fileout.write("\"0\"\n", 4);
		fileout.write("\"startdark\" ", 12); fileout.write("\"0\"\n", 4);
		fileout.write("\"MaxRange\" ", 11); fileout.write("\"4096\"\n", 7);
		fileout.write("\"sounds\" ", 9); fileout.write("\"1\"\n", 4);
		fileout.write("\"mapversion\" ", 13); fileout.write("\"220\"\n", 6);
		fileout.write("\"wad\" ", 6); fileout.write("\"\"\n", 3);
		fileout.write("\"_generator\" ", 13); fileout.write("\"map ported by ceannastar\"\n", 27);

		file.seekg(offsetofsides);

		brushSideHeader* brushSides = new brushSideHeader[(sizeofsides / 8)];

		for (int i = 0; i < sizeofsides / 8; ++i)
		{
			file.read((char*)&brushSides[i], sizeof brushSides[i]);
		}

		file.seekg(offsetofbrushes);

		brushHeader* brushes = new brushHeader[(sizeofbrushes / 12)];

		for (int i = 0; i < sizeofbrushes / 12; ++i)
		{
			file.read((char*)&brushes[i], sizeof brushes[i]);
			{
				fileout.write("{\n", 2);
				for (int k = 0; k < brushes[i].numSides; k++)
				{
					std::vector<vertexHeader> planes1;
					std::vector<vertexHeader> planes2;
					std::vector<vertexHeader> planes3;
					std::string temp;
					for (int j = 0; j < numOfVerts; j++)
					{
						//Все вершины должны лежать на плоскости!
						//Следует учесть, что все вершины являются углами к.-го-нибудь браша
						//fixme: массив вершин учитывает и посторонние к.-инаты!!!!!!!!!!!!!!
						if (((vertices[j].x * planes[brushSides[brushes[i].firstSide + k].planenum].normal[0] + vertices[j].y * planes[brushSides[brushes[i].firstSide + k].planenum].normal[1] + vertices[j].z * planes[brushSides[brushes[i].firstSide + k].planenum].normal[2]) == planes[brushSides[brushes[i].firstSide + k].planenum].dist) && (planes[brushSides[brushes[i].firstSide + k].planenum].type == 0))
						{
							temp = "(" + std::to_string((int)vertices[j].x) + " " + std::to_string((int)vertices[j].y) + " " + std::to_string((int)vertices[j].z) + ") ";
							fileout.write(temp.c_str(), temp.size());
						}
						if (((vertices[j].x * planes[brushSides[brushes[i].firstSide + k].planenum].normal[0] + vertices[j].y * planes[brushSides[brushes[i].firstSide + k].planenum].normal[1] + vertices[j].z * planes[brushSides[brushes[i].firstSide + k].planenum].normal[2]) == planes[brushSides[brushes[i].firstSide + k].planenum].dist) && (planes[brushSides[brushes[i].firstSide + k].planenum].type == 1))
						{
							temp = "(" + std::to_string((int)vertices[j].x) + " " + std::to_string((int)vertices[j].y) + " " + std::to_string((int)vertices[j].z) + ") ";
							fileout.write(temp.c_str(), temp.size());
						}
						if (((vertices[j].x * planes[brushSides[brushes[i].firstSide + k].planenum].normal[0] + vertices[j].y * planes[brushSides[brushes[i].firstSide + k].planenum].normal[1] + vertices[j].z * planes[brushSides[brushes[i].firstSide + k].planenum].normal[2]) == planes[brushSides[brushes[i].firstSide + k].planenum].dist) && (planes[brushSides[brushes[i].firstSide + k].planenum].type == 2))
						{
							temp = "(" + std::to_string((int)vertices[j].x) + " " + std::to_string((int)vertices[j].y) + " " + std::to_string((int)vertices[j].z) + ") ";
							fileout.write(temp.c_str(), temp.size());
						}
					}
					if (temp.size() != 0)
					{
						std::string temp = "C1A0_LABFLRC [ 0 1 0 0 ] [ 0 0 -1 0 ] 0 1 1";
						fileout.write(temp.c_str(), temp.size()); fileout.write("\n", 1);
					}
				}
				fileout.write("}\n", 2);
			}
		}

		fileout.write("}", 1);

		fileout.close();


		return true;
	}


public:

	bool  open_bsp_source(std::string filename)
	{
		{
			filenameout = filename;
			std::ifstream file(filename + ".bsp", std::ios::in | std::ios::binary);

			file.seekg(file.end);
			int filesize = (int)file.tellg();
			file.seekg(file.beg);

			bspHeader* bspFile = new bspHeader;

			file.read((char*)&bspFile->ident, sizeof bspFile->ident);
			file.read((char*)&bspFile->version, sizeof bspFile->version);
			for (int i = 0; i < 64; ++i)
				file.read((char*)&bspFile->lumps[i], sizeof bspFile->lumps[i]);
			file.read((char*)&bspFile->mapRevision, sizeof bspFile->mapRevision);

			planeHeader* planes = read_planes(file, bspFile->lumps[1].fileofs, bspFile->lumps[1].filelen);
			read_vertices(file, bspFile->lumps[3].fileofs, bspFile->lumps[3].filelen);
			brushes_to_map(file, bspFile->lumps[18].fileofs, bspFile->lumps[19].fileofs, bspFile->lumps[18].filelen, bspFile->lumps[19].filelen, planes);

			file.close();
		}
		

		return true;
	}

};