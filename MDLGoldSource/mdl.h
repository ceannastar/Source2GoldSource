
/*
	Created: 04/01/2023
	Written by Alexandr Sotnikov
	File: mdl.h
*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>

class MDLH
{
private:

public:

	bool load_mdl(std::string filename)
	{
		std::ifstream file(filename+".mdl", std::ios::in | std::ios::binary);

		file.seekg(file.end);
		int filesize = file.tellg();
		file.seekg(file.beg);



		file.close();
	}
};
