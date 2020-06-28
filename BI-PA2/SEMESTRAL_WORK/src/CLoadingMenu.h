//
// Created by jaroslav on 5/13/20.
//

#ifndef TOWER_ATTACK_CLOADINGMENU_H
#define TOWER_ATTACK_CLOADINGMENU_H


#include <vector>
#include "CInterface.h"

class CLoadingMenu {
	// returns names of possible loading files
	const std::string m_Savedir = "data/saves/";
	std::vector<std::string> GetFiles() const;
public:
	// prints save files that the user can choose
	bool Print(const CInterface &interface) const;
	std::string GetSavedir() const;
};


#endif //TOWER_ATTACK_CLOADINGMENU_H
