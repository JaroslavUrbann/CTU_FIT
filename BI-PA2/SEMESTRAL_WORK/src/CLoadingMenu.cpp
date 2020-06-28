//
// Created by jaroslav on 5/13/20.
//

#include "CLoadingMenu.h"
#include <experimental/filesystem>
#include <sys/types.h>
#include <sys/stat.h>

namespace fs = std::experimental::filesystem;

bool CLoadingMenu::Print(const CInterface &interface) const {
	std::vector<std::string> filenames;
	// tries to open the save folder
	try{
		filenames = GetFiles();
	}
	catch(const std::exception& e){
		return false;
	}
	// gives the user files to choose from
	interface.Message("Please choose a loading file:\n");
	for(auto &file: filenames) {
		interface.Message(file.substr(m_Savedir.size()));
		interface.Message("    ");
	}
	interface.Message("\n");
	return true;
}

std::vector<std::string> CLoadingMenu::GetFiles() const {
	std::vector<std::string> filenames;
	for (const auto & entry : fs::directory_iterator(m_Savedir))
		filenames.push_back(entry.path());
	return filenames;
}

std::string CLoadingMenu::GetSavedir() const {
	return m_Savedir;
}

