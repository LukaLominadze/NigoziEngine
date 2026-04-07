#pragma once

#include <filesystem>
#include <fstream>

namespace Nigozi 
{
	namespace FileDialogue
	{
		// filterList are file extension specifiers that can be seperated with ';'
		// for ex: "png;jpeg;webp"
		std::filesystem::path OpenFileDialog(const char* filterList, const char* defaultPath = NULL);
		std::filesystem::path OpenFolderDialog(const char* defaultPath = NULL);
		std::filesystem::path OpenSaveDialog(const char* filterList, const char* defaultPath = NULL);
	}
}