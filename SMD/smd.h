
/*
	Created: 03/26/2023
	Written by Alexandr Sotnikov
	File: smd.h
*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>

class SMDH
{
private:

public:

	bool write_smd(std::string filename)
	{
		std::ofstream filesmd(filename+".smd", std::ios::out);

		filesmd.write("version ", std::strlen("version ")); filesmd.write("1", std::strlen("1")); filesmd.write("\n",1);

		filesmd.write("nodes ", std::strlen("nodes")); filesmd.write("\n",1);
		filesmd.write("  0 ", std::strlen("  0 ")); filesmd.write("\"bonename\" ", std::strlen("bonename ")+2); filesmd.write("-1", std::strlen("-1"));  filesmd.write("\n", 1);
		filesmd.write("end", std::strlen("end")); filesmd.write("\n",1);

		filesmd.write("skeleton", std::strlen("skeleton")); filesmd.write("\n",1);
		filesmd.write("time ", std::strlen("time ")); filesmd.write("0", std::strlen("0"));  filesmd.write("\n",1);
		filesmd.write("  0 ", std::strlen("  0 ")); filesmd.write("0 ", std::strlen("0 ")); filesmd.write("0 ", std::strlen("0 "));  filesmd.write("\n", 1);
		filesmd.write("end", std::strlen("end")); filesmd.write("\n",1);

		filesmd.write("triangles", std::strlen("triangles")); filesmd.write("\n", 1);

		filesmd.write("end", std::strlen("end")); filesmd.write("\n", 1);

		filesmd.close();

		return true;
	}
};