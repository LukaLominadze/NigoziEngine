#include "ngpch.h"
#include "FileDialogue.h"

#include "nfd.h"

#include "StringUtils.h"

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
			if (fileTokens[1] == filter) {
				free(outPath);
				return result;
			}
		}
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
