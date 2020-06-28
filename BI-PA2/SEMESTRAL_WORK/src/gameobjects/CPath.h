//
// Created by jaroslav on 5/9/20.
//

#ifndef TOWER_ATTACK_CPATH_H
#define TOWER_ATTACK_CPATH_H


#include "CGameObject.h"

class CPath : public CGameObject {
public:
	CPath() = default;
	explicit CPath(char symbol);
	std::istream &Load(std::istream &stream) override;
};


#endif //TOWER_ATTACK_CPATH_H
