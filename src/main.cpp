#include "../lib/alphanum.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include <time.h>
#include <vector>
#include <algorithm>

enum class MODE
{
	TIMESTAMP,
	SEQUENTIAL,
	EXIT
};

void timestampRename(const std::string& fileLocation, uint32_t& count);
void sequentialRename(const std::string& fileLocation, uint32_t& count);
void renameFile(const std::filesystem::path& p, int value);
void getListAndSort(const std::string& fileLocation, std::vector<std::filesystem::path>& paths);
void printIntro();
void printOutro(uint32_t& count);
MODE getSelection(const std::string& fileLocation);

int main(int argc, char** argv)
{
	printIntro();

	uint32_t count = 0;

	std::string fileLocation = std::filesystem::current_path().string();

	MODE selection = MODE::EXIT;

	if (argc == 2)
	{
		std::string argValue = argv[1];
		
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
			timestampRename(fileLocation, count);
		break;
		case MODE::SEQUENTIAL:
			sequentialRename(fileLocation, count);
		break;
		default:
			return 1;	
	}

	printOutro(count);

	return 0;
}

void timestampRename(const std::string& fileLocation, uint32_t& count)
{
	std::vector<std::filesystem::path> paths;

	getListAndSort(fileLocation, paths);

	auto timeSeed = time(0);

	for (auto& path : paths)
	{
		if (path.has_extension())
		{
			renameFile(path, static_cast<int>(timeSeed + count));
			++count;
		}
	}
}

void sequentialRename(const std::string& fileLocation, uint32_t& count)
{
	std::vector<std::filesystem::path> paths;

	getListAndSort(fileLocation, paths);

	uint32_t increment = 0;

	std::string previousPath = "";
	for (auto& path : paths)
	{
		if (path.has_extension())
		{
			if (previousPath != path.parent_path().string())
			{
				increment = 1;
				previousPath = path.parent_path().string();
			}
			renameFile(path, increment);
			++count;
			++increment;
		}
	}
}

void renameFile(const std::filesystem::path& p, int value)
{
	std::string newName = p.parent_path().string() + "\\" + std::to_string(value) + p.extension().string();
	std::filesystem::rename(p, newName);

	std::string origName = p.string();
	size_t position = 0;
	while ((position = origName.find("\\\\") != std::string::npos))
	{
		origName.replace(position, 1, "\\");
	}

	std::cout << "Renamed: " << origName << "\nTo     : " << newName << "\n\n";
}

void getListAndSort(const std::string& fileLocation, std::vector<std::filesystem::path>& paths)
{
	for (auto& path : std::filesystem::recursive_directory_iterator(fileLocation))
	{
		if (path.path().has_extension())
		{
			paths.push_back(path.path());
		}
	}

	std::sort(paths.begin(), paths.end(), doj::alphanum_less<std::filesystem::path>());
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

void printOutro(uint32_t& count)
{
	std::cout << "/////////////////////////////////////////////\n";
	std::cout << "// Renaming complete. " << count << " files renamed\n";
	std::cout << "// Press any key to exit \n";
	std::cout << "/////////////////////////////////////////////\n\n";
}

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
