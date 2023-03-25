
/*
	Created: 03/25/2023
	Written by Alexandr Sotnikov
	File: map.h
*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>

class MAPH
{
private:

public:

	MAPH() {}
	~MAPH() {}

	bool write_map_header(std::string filename)
	{
		std::ofstream file(filename+".map", std::ios::out);

		file.write("{\n", 2);

		file.write("\"classname\" ", std::strlen("\"classname\" ")); file.write("\"worldspawn\"\n", 13);
		file.write("\"defaultteam\" ", 14); file.write("\"0\"\n", 4);
		file.write("\"newunit\" ", 10); file.write("\"0\"\n", 4);
		file.write("\"gametitle\" ", 12); file.write("\"0\"\n", 4);
		file.write("\"startdark\" ", 12); file.write("\"0\"\n", 4);
		file.write("\"MaxRange\" ", 11); file.write("\"4096\"\n", 7);
		file.write("\"sounds\" ", 9); file.write("\"1\"\n", 4);
		file.write("\"mapversion\" ", 13); file.write("\"220\"\n", 6);
		file.write("\"wad\" ", 6); file.write("\"\"\n", 3);
		file.write("\"_generator\" ", 13); file.write("\"map ported by ceannastar\"\n", 27);

		file.write("{\n", 2);

		file.write("() () () TEXTURE [] [] 0 1 1\n", std::strlen("() () () TEXTURE [] [] 0 1 1")+1);
		file.write("() () () TEXTURE [] [] 0 1 1\n", std::strlen("() () () TEXTURE [] [] 0 1 1")+1);
		file.write("() () () TEXTURE [] [] 0 1 1\n", std::strlen("() () () TEXTURE [] [] 0 1 1")+1);

		file.write("}\n", 2);

		file.write("}", 1);

		file.close();

		return true;
	}
};