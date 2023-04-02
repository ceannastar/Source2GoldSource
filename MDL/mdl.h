
/*
	Created: 03/25/2023
	Written by Alexandr Sotnikov
	File: mdl.h
*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

class MDLH
{
private:

	struct vvdHeader
	{
		int id;
		int version;
		int checksum;
		int numLODS;
		int numLODVertexes[8];
		int numFixups;
		int fixupTableStart;
		int vertexDataStart;
		int tangentDataStart;
	};

	struct boneHeader
	{
		float weight[3];
		char bone[3];
		unsigned char numBones;
	};

	struct vertexHeader
	{
		boneHeader boneWeights;
		float vecPos[3];
		float vecNormal[3];
		float texCoord[2];
	};

	struct studioHeader
	{
		int id;
		int version;
		int checksum;
		char name[64];

		int dataLength;

		float eyePos[3];
		float illumPos[3];
		float hull_min[3];
		float hull_max[3];
		float view_bbmin[3];
		float view_bbmax[3];

		int flags;

		int bone_count;
		int bone_offset;

		int bonecontroller_count;
		int bonecontroller_offset;

		int hitbox_count;
		int hitbox_offset;

		int localanim_count;
		int localanim_offset;

		int localseq_count;
		int localseq_offset;

		int activityListVersion;
		int eventsindexed;

		int texture_count;
		int texture_offset;
	};

	struct mstudiotextureHeader
	{
		int name_offset;
		int flags;
		int used;
		int unused1; 
		int material;
		int client_material;
		int unused[10];
	};

	struct vertexHeaderSMD
	{
		int bone;
		float vertices[3];
		float normals[3];
		float uv[3];
	};

	struct vtxHeader
	{
		int version;

		int vertCacheSize;
		unsigned short maxBonexPerStrip;
		unsigned short maxBonexPerTri;
		int maxBonexPerVert;

		int checkSum;

		int numLODs;

		int materialReplacementListOffset;

		int numBodyParts;
		int bodyPartOffset;
	};

	struct bodypartHeader
	{
		int numModels;
		int modelOffset;
	};

	struct modelHeader
	{
		int numLODs;
		int lodOffset;
	};

	struct modelLODHeader
	{
		int numMeshes;
		int meshOffset;
		float switchPoint;
	};

	struct meshHeader
	{
		int numStripGroups;
		int stripGroupHeaderOffset;
		unsigned char flags;
	};

	struct stripgroupHeader
	{
		int numVerts;
		int vertOffset;

		int numIndices;
		int indexOffset;

		int numStrips;
		int stripOffset;

		unsigned char flags;

		int numTopologyIndices;
		int topologyOffset;
	};

	struct stripHeader
	{
		int numIndices;
		int indexOffset;

		int numVerts;
		int vertOffset;

		short numBones;

		unsigned char flags;

		int numBoneStateChanges;
		int boneStateChangeOffset;
	};

private:

	vertexHeader* vertices = nullptr;
	int numVerts = 0;

public:

	bool read_mdl(std::string filename)
	{

		std::ifstream file(filename + ".mdl", std::ios::in | std::ios::binary);

		file.seekg(file.end);
		int filesize = (int)file.tellg();
		file.seekg(file.beg);

		studioHeader* studio = new studioHeader;

		file.read((char*)&studio->id, sizeof studio->id);
		file.read((char*)&studio->version, sizeof studio->version);
		std::cout << studio->version << std::endl;
		file.read((char*)&studio->checksum, sizeof studio->checksum);
		for(int i = 0; i < 64; ++i)
			file.read((char*)&studio->name[i], sizeof studio->name[i]);
		file.read((char*)&studio->dataLength, sizeof studio->dataLength);
		for (int i = 0; i < 3; ++i)
			file.read((char*)&studio->eyePos[i], sizeof studio->eyePos[i]);
		for (int i = 0; i < 3; ++i)
			file.read((char*)&studio->illumPos[i], sizeof studio->illumPos[i]);	
		for (int i = 0; i < 3; ++i)
			file.read((char*)&studio->hull_min[i], sizeof studio->hull_min[i]);
		for (int i = 0; i < 3; ++i)
			file.read((char*)&studio->hull_max[i], sizeof studio->hull_max[i]);
		for (int i = 0; i < 3; ++i)
			file.read((char*)&studio->view_bbmin[i], sizeof studio->view_bbmin[i]);	
		for (int i = 0; i < 3; ++i)
			file.read((char*)&studio->view_bbmax[i], sizeof studio->view_bbmax[i]);

		file.read((char*)&studio->flags, sizeof studio->flags);

		file.read((char*)&studio->bone_count, sizeof studio->bone_count);
		file.read((char*)&studio->bone_offset, sizeof studio->bone_offset);
		
		file.read((char*)&studio->bonecontroller_count, sizeof studio->bonecontroller_count);
		file.read((char*)&studio->bonecontroller_offset, sizeof studio->bonecontroller_offset);
				
		file.read((char*)&studio->hitbox_count, sizeof studio->hitbox_count);
		file.read((char*)&studio->hitbox_offset, sizeof studio->hitbox_offset);
						
		file.read((char*)&studio->localanim_count, sizeof studio->localanim_count);
		file.read((char*)&studio->localanim_offset, sizeof studio->localanim_offset);
								
		file.read((char*)&studio->localseq_count, sizeof studio->localseq_count);
		file.read((char*)&studio->localseq_offset, sizeof studio->localseq_offset);
										
		file.read((char*)&studio->activityListVersion, sizeof studio->activityListVersion);
		file.read((char*)&studio->eventsindexed, sizeof studio->eventsindexed);
												
		file.read((char*)&studio->texture_count, sizeof studio->texture_count);
		file.read((char*)&studio->texture_offset, sizeof studio->texture_offset);

		mstudiotextureHeader* textures = new mstudiotextureHeader[studio->texture_count];

		file.seekg(studio->texture_offset);

		for (int i = 0; i < studio->texture_count; ++i)
		{
			file.read((char*)&textures[i].name_offset, sizeof textures[i].name_offset);
			file.read((char*)&textures[i].flags, sizeof textures[i].flags);
			file.read((char*)&textures[i].used, sizeof textures[i].used);
			file.read((char*)&textures[i].unused1, sizeof textures[i].unused1);
			file.read((char*)&textures[i].material, sizeof textures[i].material);
			file.read((char*)&textures[i].client_material, sizeof textures[i].client_material);
			for (int k = 0; k < 10; ++k)
				file.read((char*)&textures[i].unused[k], sizeof textures[i].unused[k]);
			file.seekg((int)file.tellg() + textures[i].name_offset);
			
		}

		file.close();

		return true;
	}

	bool read_vvd(std::string filename)
	{
		std::ifstream file(filename+".vvd", std::ios::in | std::ios::binary);

		file.seekg(file.end);
		int filesize = (int)file.tellg();
		file.seekg(file.beg);

		vvdHeader* vvd = new vvdHeader;

		file.read((char*)&vvd->id, sizeof vvd->id);
		file.read((char*)&vvd->version, sizeof vvd->version);
		file.read((char*)&vvd->checksum, sizeof vvd->checksum);
		file.read((char*)&vvd->numLODS, sizeof vvd->numLODS);
		for(int i = 0; i < 8; ++i)
			file.read((char*)&vvd->numLODVertexes[i], sizeof vvd->numLODVertexes[i]);
		file.read((char*)&vvd->numFixups, sizeof vvd->numFixups);
		file.read((char*)&vvd->fixupTableStart, sizeof vvd->fixupTableStart);
		file.read((char*)&vvd->vertexDataStart, sizeof vvd->vertexDataStart);
		file.read((char*)&vvd->tangentDataStart, sizeof vvd->tangentDataStart);

		file.seekg(vvd->vertexDataStart);

		numVerts = vvd->numLODVertexes[0];

		vertices = new vertexHeader[vvd->numLODVertexes[0]];

		for (int k = 0; k < vvd->numLODVertexes[0]; ++k)
		{

			for (int i = 0; i < 3; ++i)
				file.read((char*)&vertices[k].boneWeights.weight[i], sizeof vertices[k].boneWeights.weight[i]);

			for (int i = 0; i < 3; ++i)
				file.read((char*)&vertices[k].boneWeights.bone[i], sizeof vertices[k].boneWeights.bone[i]);

			file.read((char*)&vertices[k].boneWeights.numBones, sizeof vertices[k].boneWeights.numBones);

			for (int i = 0; i < 3; ++i)
				file.read((char*)&vertices[k].vecPos[i], sizeof vertices[k].vecPos[i]);

			for (int i = 0; i < 3; ++i)
				file.read((char*)&vertices[k].vecNormal[i], sizeof vertices[k].vecNormal[i]);

			for (int i = 0; i < 2; ++i)
				file.read((char*)&vertices[k].texCoord[i], sizeof vertices[k].texCoord[i]);
		}

		file.close();

		return true;
	}

	bool read_vtx(std::string filename)
	{

		std::ifstream file(filename+".dx80.vtx", std::ios::in | std::ios::binary);

		vtxHeader* vtx = new vtxHeader;

		file.read((char*)&vtx->version, sizeof vtx->version);

		file.read((char*)&vtx->vertCacheSize, sizeof vtx->vertCacheSize);
		file.read((char*)&vtx->maxBonexPerStrip, sizeof vtx->maxBonexPerStrip);
		file.read((char*)&vtx->maxBonexPerTri, sizeof vtx->maxBonexPerTri);
		file.read((char*)&vtx->maxBonexPerVert, sizeof vtx->maxBonexPerVert);

		file.read((char*)&vtx->checkSum, sizeof vtx->checkSum);

		file.read((char*)&vtx->numLODs, sizeof vtx->numLODs);

		file.read((char*)&vtx->materialReplacementListOffset, sizeof vtx->materialReplacementListOffset);

		file.read((char*)&vtx->numBodyParts, sizeof vtx->numBodyParts);
		file.read((char*)&vtx->bodyPartOffset, sizeof vtx->bodyPartOffset);

		file.seekg(vtx->bodyPartOffset);

		bodypartHeader* bodypart = new bodypartHeader;

		file.read((char*)&bodypart->numModels, sizeof bodypart->numModels);
		file.read((char*)&bodypart->modelOffset, sizeof bodypart->modelOffset);

		file.seekg(vtx->bodyPartOffset+bodypart->modelOffset);

		modelHeader* model = new modelHeader;

		file.read((char*)&model->numLODs, sizeof model->numLODs);
		file.read((char*)&model->lodOffset, sizeof model->lodOffset);

		file.seekg(vtx->bodyPartOffset + bodypart->modelOffset+ model->lodOffset);

		modelLODHeader* modelLOD = new modelLODHeader;

		file.read((char*)&modelLOD->numMeshes, sizeof modelLOD->numMeshes);
		file.read((char*)&modelLOD->meshOffset, sizeof modelLOD->meshOffset);
		file.read((char*)&modelLOD->switchPoint, sizeof modelLOD->switchPoint);

		file.seekg(vtx->bodyPartOffset + bodypart->modelOffset + model->lodOffset+ modelLOD->meshOffset);

		meshHeader* mesh = new meshHeader;

		file.read((char*)&mesh->numStripGroups, sizeof mesh->numStripGroups);
		file.read((char*)&mesh->stripGroupHeaderOffset, sizeof mesh->stripGroupHeaderOffset);
		file.read((char*)&mesh->flags, sizeof mesh->flags);

		file.seekg(vtx->bodyPartOffset + bodypart->modelOffset + model->lodOffset + modelLOD->meshOffset+ mesh->stripGroupHeaderOffset);

		stripgroupHeader* stripGroup = new stripgroupHeader;

		file.read((char*)&stripGroup->numVerts, sizeof stripGroup->numVerts);
		file.read((char*)&stripGroup->vertOffset, sizeof stripGroup->vertOffset);

		file.read((char*)&stripGroup->numIndices, sizeof stripGroup->numIndices);
		file.read((char*)&stripGroup->indexOffset, sizeof stripGroup->indexOffset);

		file.read((char*)&stripGroup->numStrips, sizeof stripGroup->numStrips);
		file.read((char*)&stripGroup->stripOffset, sizeof stripGroup->stripOffset);

		file.seekg(vtx->bodyPartOffset + bodypart->modelOffset + model->lodOffset + modelLOD->meshOffset + mesh->stripGroupHeaderOffset+ stripGroup->indexOffset);

		unsigned short* edges = new unsigned short[stripGroup->numIndices];

		for (int i = 0; i < stripGroup->numIndices; i++)
			file.read((char*)&edges[i], sizeof edges[i]);

		read_vvd(filename);

		std::ofstream filesmd(filename + ".smd", std::ios::out);

		filesmd.write("version ", std::strlen("version ")); filesmd.write("1", std::strlen("1")); filesmd.write("\n", 1);

		filesmd.write("nodes ", std::strlen("nodes")); filesmd.write("\n", 1);
		filesmd.write("  0 ", std::strlen("  0 ")); filesmd.write("\"root\" ", std::strlen("root ") + 2); filesmd.write("-1", std::strlen("-1"));  filesmd.write("\n", 1);
		filesmd.write("end", std::strlen("end")); filesmd.write("\n", 1);

		filesmd.write("skeleton", std::strlen("skeleton")); filesmd.write("\n", 1);
		filesmd.write("time ", std::strlen("time ")); filesmd.write("0", std::strlen("0"));  filesmd.write("\n", 1);
		filesmd.write("  0 ", std::strlen("  0 ")); filesmd.write("0 ", std::strlen("0 ")); filesmd.write("0 ", std::strlen("0 ")); filesmd.write("0 ", std::strlen("0 ")); filesmd.write("0 ", std::strlen("0 ")); filesmd.write("0 ", std::strlen("0 ")); filesmd.write("0 ", std::strlen("0 "));   filesmd.write("\n", 1);
		filesmd.write("end", std::strlen("end")); filesmd.write("\n", 1);

		filesmd.write("triangles", std::strlen("triangles")); filesmd.write("\n", 1);

		filesmd.write("end", std::strlen("end")); filesmd.write("\n", 1);

		filesmd.close();
		
		file.close();

		return true;
	}
};
