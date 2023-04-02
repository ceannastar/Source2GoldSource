
/*
	Created: 03/27/2023
	Written by Alexandr Sotnikov
	File: vpk.h
*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>



class VPKH
{
private:

	struct vpkHeader
	{
		unsigned int magic;
		unsigned int Version;
		unsigned int TreeSize;
		unsigned int FileDataSectionSize;
		unsigned int ArchiveMD5SectionSize;
		unsigned int OtherMD5SectionSize;
		unsigned int SignatureSectionSize;
	};

	struct vkpDirectoryEntry
	{
		unsigned int CRC;
		unsigned short PreloadBytes;

		unsigned short ArchiveIndex;
		unsigned int EntryOffset;

		unsigned int EntryLength;

		unsigned short Terminator;
	};

private:

	bool read_string(std::ifstream& file, std::string& string)
	{
		for (;;)
		{
			char byte;
			file.read((char*)&byte, 1);
			if (byte == '\0')
				return false;
			string = string + byte;
		}
		return true;
	}

	std::string read_directory(std::ifstream& file)
	{
		std::string extension;
		std::string path;
		std::string filename;

		for(;;)
		{
			if (!read_string(file, extension))
				break;
		}
		for (;;)
		{
			if (!read_string(file, path))
				break;
		}
		for (;;)
		{
			if (!read_string(file, filename))
				break;
		}
		if(path.size()!=1)
			return path+"/"+filename+"."+extension;
		else
			return filename + "." + extension;
	}

public:

	bool load_vpk(std::string filename)
	{
		std::ifstream file(filename + ".vpk", std::ios::in | std::ios::binary);

		file.seekg(file.end);
		int filesize = file.tellg();
		file.seekg(file.beg);

		vpkHeader* vHeader = new vpkHeader;

		file.read((char*)vHeader, sizeof vHeader);

		if (vHeader->magic == 0x55aa1234)
		{
			file.seekg(sizeof vpkHeader + vHeader->TreeSize);
			std::cout << read_directory(file) << std::endl;
			
			vkpDirectoryEntry* vpkDirEntry = new vkpDirectoryEntry;
			file.read((char*)&vpkDirEntry->CRC, sizeof vpkDirEntry->CRC);
			file.read((char*)&vpkDirEntry->PreloadBytes, sizeof vpkDirEntry->PreloadBytes);
			file.read((char*)&vpkDirEntry->ArchiveIndex, sizeof vpkDirEntry->ArchiveIndex);
			file.read((char*)&vpkDirEntry->EntryOffset, sizeof vpkDirEntry->EntryOffset);
			file.read((char*)&vpkDirEntry->EntryLength, sizeof vpkDirEntry->EntryLength);
			file.read((char*)&vpkDirEntry->Terminator, sizeof vpkDirEntry->Terminator);
			if (vpkDirEntry->Terminator == 0xffff)
			{
				if (vpkDirEntry->ArchiveIndex = 0x7fff)
					std::cout << "WORKED!" << std::endl;
			}


		}
		else if(vHeader->Version != 2)
		{
			std::cout << "Wrond file format!" << std::endl;
		}

		file.close();
	}

};