// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "alphanum.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include <chrono>
#include <vector>
#include <algorithm>

enum class MODE
{
	TIMESTAMP,
	SEQUENTIAL,
	EXIT
};

uint64_t timestampRename(const std::string& fileLocation);
uint64_t sequentialRename(const std::string& fileLocation);
bool renameFile(const std::filesystem::path& p, uint64_t value);
std::vector<std::filesystem::path> getListAndSort(const std::string& fileLocation);
void printIntro();
void printOutro(const uint64_t& updateCount);
MODE getSelection(const std::string& fileLocation);

int main(int argc, char** argv)
{
	printIntro();

	uint64_t updateCount { 0 }; //counts the total number of files renamed

	std::string fileLocation { std::filesystem::current_path().string() };
	
	MODE selection { MODE::EXIT };

	if (argc == 2)
	{
		std::string argValue { argv[1] };
		
		if (argValue == "-1")
		{
			selection = MODE::TIMESTAMP;
		}
		else if (argValue == "-2")
		{
			selection = MODE::SEQUENTIAL;
		}
	}

	if (selection == MODE::EXIT)
	{
		selection = getSelection(fileLocation);
	}

	switch (selection)
	{
		case MODE::TIMESTAMP:
			updateCount = timestampRename(fileLocation);
			break;
		case MODE::SEQUENTIAL:
			updateCount = sequentialRename(fileLocation);
			break;
		default:
			return 1;
			break;
	}

	printOutro(updateCount);

	return 0;
}

//takes the current system time and uses this as a starting point to increment the filename
uint64_t timestampRename(const std::string& fileLocation)
{
	uint64_t updateCount { 0 };
	std::vector<std::filesystem::path> paths { getListAndSort(fileLocation) };

	uint64_t timeSeed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	for (auto& path : paths)
	{
		if (path.has_extension())
		{
			if(renameFile(path, timeSeed + updateCount))
			{
				++updateCount;
			}
		}
	}

	return updateCount;
}

//renames the files from 1 - X
//if the new file name has the same name as an existing file, it will not updated it
//to help prevent this, this function calls timestampRename first and give everything a new name
uint64_t sequentialRename(const std::string& fileLocation)
{
	timestampRename(fileLocation);
	uint64_t updateCount { 0 };

	std::vector<std::filesystem::path> paths { getListAndSort(fileLocation) };

	uint64_t increment { 0 };

	std::string previousPath {};

	for (auto& path : paths)
	{
		if (path.has_extension())
		{
			if (previousPath != path.parent_path().string())
			{
				increment = 1;
				previousPath = path.parent_path().string();
			}
			
			if(renameFile(path, increment))
			{
				++updateCount;				
			}
			++increment;
		}
	}

	return updateCount;
}

//doesn't rename the file if the name already exists
bool renameFile(const std::filesystem::path& p, uint64_t value)
{
	bool returnValue { false };

	std::string newName { p.parent_path().string() + "\\" + std::to_string(value) + p.extension().string() };

	if(!std::filesystem::exists(newName))
	{
		std::filesystem::rename(p, newName);
		returnValue = true;
	}
	else
	{
		std::cout << "Failed to rename: " << p << " to " << newName << " . File name already exists\n";
		return returnValue;
	}

	std::string origName { p.string() };
	size_t position { origName.find("\\\\") };
	
	while (position != std::string::npos)
	{
		origName.replace(position, 1, "\\");
	}

	std::cout << "Renamed: " << origName << "\nTo     : " << newName << "\n\n";

	return returnValue;
}

//returns a sorted vector of all file within the supplied location
//sort is based on alpha numeric
std::vector<std::filesystem::path> getListAndSort(const std::string& fileLocation)
{
	std::vector<std::filesystem::path> paths;

	for (auto& path : std::filesystem::recursive_directory_iterator(fileLocation))
	{
		if (path.path().has_extension())
		{
			paths.push_back(path.path());
		}
	}

	std::sort(paths.begin(), paths.end(), doj::alphanum_less<std::filesystem::path>());

	return paths;
}

void printIntro()
{
	std::cout << "////////////////////////////////////////////////////////////////\n";
	std::cout << "// ______ _ _     ______                                      //\n";
	std::cout << "// |  ___(_) |    | ___ \\                                     //\n";
	std::cout << "// | |_   _| | ___| |_/ /___ _ __   __ _ _ __ ___   ___ _ __  //\n";
	std::cout << "// |  _| | | |/ _ \\    // _ \\ '_ \\ / _` | '_ ` _ \\ / _ \\ '__| //\n";
	std::cout << "// | |   | | |  __/ |\\ \\  __/ | | | (_| | | | | | |  __/ |    //\n";
	std::cout << "// \\_|   |_|_|\\___\\_| \\_\\___|_| |_|\\__,_|_| |_| |_|\\___|_|    //\n";
	std::cout << "//                                                            //\n";
	std::cout << "////////////////////////////////////////////////////////////////\n\n";
}

void printOutro(const uint64_t& updateCount)
{
	std::cout << "/////////////////////////////////////////////\n";
	std::cout << "// Renaming complete. " << updateCount << " files renamed\n";
	std::cout << "// Press any key to exit \n";
	std::cout << "/////////////////////////////////////////////\n\n";
}

//user interface for mode selection
MODE getSelection(const std::string& fileLocation)
{
	std::string selection;

	while (true)
	{
		std::cout << "Choose an option below to rename all files in \"" << fileLocation << "\"\n\n";
		std::cout << "1. Timestamp\n";
		std::cout << "2. Sequential number\n";
		std::cout << "q. Quit\n";

		std::cin >> selection;

		switch (selection[0])
		{
		case '1':
			return MODE::TIMESTAMP;
			break;
		case '2':
			return MODE::SEQUENTIAL;
			break;
		case 'q':
		case 'Q':
			return MODE::EXIT;
			break;
		}
		std::cout << "Value not valid\n\n";
	}
}
