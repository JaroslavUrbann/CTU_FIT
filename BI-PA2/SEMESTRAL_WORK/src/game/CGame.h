//
// Created by jaroslav on 5/9/20.
//

#ifndef TOWER_ATTACK_CGAME_H
#define TOWER_ATTACK_CGAME_H


#include "../CInterface.h"
#include "CState.h"
#include <string>

// interface for making changes to the game state
class CGame {
	// filename for loading & saving
	std::string m_Filename;
	CState m_State;
public:
	bool Print(const CInterface &interface) const;
	bool Load(const std::string& filename);
	bool Save() const;
	void Step();
	GameStatus GetStatus() const;
	bool BuyAttackers(const int &count, const char &type, const char &entrance);
};


#endif //TOWER_ATTACK_CGAME_H
