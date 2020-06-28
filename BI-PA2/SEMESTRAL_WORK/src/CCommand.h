//
// Created by jaroslav on 5/9/20.
//

#ifndef TOWER_ATTACK_CCOMMAND_H
#define TOWER_ATTACK_CCOMMAND_H


#include "game/CGame.h"
#include "CInterface.h"

class CCommand {
private:
	std::string m_Help;
	std::function<bool(const CInterface &, CGame &, const std::string &parameters)> m_Execute;
	// checks if a string is a valid command
	std::function<bool(const std::string &)> m_IsValid;
public:
	CCommand(std::string help,
			std::function<bool(const CInterface &, CGame &, const std::string &parameters)> execute,
			std::function<bool(const std::string &)> isvalid);
	std::string Help() const;
	bool IsValid(const std::string &command);
	bool Execute(const CInterface &interface, CGame &game, const std::string& parameters);
};


#endif //TOWER_ATTACK_CCOMMAND_H
