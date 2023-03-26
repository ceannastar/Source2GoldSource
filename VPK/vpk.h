
/*
	Created: 03/26/2023
	Written by Alexandr Sotnikov
	File: map.h
*/

#pragma once

#include <iostream>
#include <fstream>
//#include <string>

#define SIGNATURE 0x55aa1234

class VPKH
{
private:



public:

	VPKH() {}
	~VPKH() {}

	bool open_vpk(std::string filename)
	{
		std::ifstream file(filename+".vpk", std::ios::in | std::ios::binary);

		



		file.close();

		return true;
	}

};
