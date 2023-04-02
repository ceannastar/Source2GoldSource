
/*
	Created: 03/26/2023
	Written by Alexandr Sotnikov
	File: vector.h
*/

#pragma once

struct vertexHeader
{
	float x;
	float y;
	float z;
};

struct planesverts
{
	vertexHeader verts[3];
};
