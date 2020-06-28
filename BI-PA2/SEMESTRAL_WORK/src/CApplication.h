//
// Created by jaroslav on 5/9/20.
//

#ifndef TOWER_ATTACK_CAPPLICATION_H
#define TOWER_ATTACK_CAPPLICATION_H


#include "CInterface.h"
#include "CCommand.h"
#include <vector>

class CApplication {
private:
	CInterface m_Interface;
	std::vector<CCommand> m_Commands;
public:
	explicit CApplication(CInterface interface);
	int Run();
};


#endif //TOWER_ATTACK_CAPPLICATION_H
