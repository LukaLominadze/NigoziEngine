#include "ngpch.h"
#include "FileDialogue.h"

#include "nfd.h"

#include "StringUtils.h"

namespace Nigozi 
{
	std::filesystem::path FileDialogue::OpenFileDialog(const char* filterList, const char* defaultPath)
	{
		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_OpenDialog(filterList, defaultPath, &outPath);
		if (result == NFD_OKAY)
		{
			std::filesystem::path result(outPath);
			auto tokens = StringUtils::SplitString(result.string(), "/");
			if (tokens.size() == 1) {
				tokens = StringUtils::SplitString(result.string(), "\\");
			}
			auto fileTokens = StringUtils::SplitString(tokens.at(tokens.size() - 1), ".");
			if (fileTokens.size() < 2) {
				return std::filesystem::path{};
			}
			auto filterTokens = StringUtils::SplitString(filterList, ";");
			for (auto filter : filterTokens) {
				if (filter.find(',') != std::string::npos) {
					auto subTokens = StringUtils::SplitString(filter, ",");
					for (auto _filter : subTokens) {
						if (fileTokens[1] == _filter) {
							return result;
						}
					}
					continue;
				}
				if (fileTokens[1] == filter) {
					return result;
				}
			}
			free(outPath);
			return std::filesystem::path{};
		}
		else if (result == NFD_CANCEL)
		{
			return std::filesystem::path{};
		}
		else
		{
			return std::filesystem::path{};
		}
		return std::filesystem::path{};
	}

	std::filesystem::path FileDialogue::OpenFolderDialog(const char* defaultPath)
	{
		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_PickFolder(defaultPath, &outPath);
		if (result == NFD_OKAY)
		{
			std::filesystem::path result(outPath);
			free(outPath);
			if (result.empty()) {
				return std::filesystem::path{};
			}
			return result;
		}
		else if (result == NFD_CANCEL)
		{
			return std::filesystem::path{};
		}
		else
		{
			return std::filesystem::path{};
		}
		return std::filesystem::path{};
	}

	std::filesystem::path FileDialogue::OpenSaveDialog(const char* filterList, const char* defaultPath)
	{
		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_SaveDialog(filterList, defaultPath, &outPath);
		if (result == NFD_OKAY)
		{
			std::filesystem::path result(outPath);
			free(outPath);
			if (result.empty()) {
				return std::filesystem::path{};
			}
			return result;
		}
		else if (result == NFD_CANCEL)
		{
			return std::filesystem::path{};
		}
		else
		{
			return std::filesystem::path{};
		}
		return std::filesystem::path{};
	}
}
