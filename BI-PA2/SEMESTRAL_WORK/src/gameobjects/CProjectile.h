//
// Created by jaroslav on 5/9/20.
//

#ifndef TOWER_ATTACK_CPROJECTILE_H
#define TOWER_ATTACK_CPROJECTILE_H


#include "CGameObject.h"
#include "../game/CCoord.h"

// projectile that is shot by the towers
class CProjectile : public CGameObject {
	int m_Damage;
	CCoord m_Direction;
public:
	CProjectile();
	CProjectile(char symbol, int update_speed, int damage, int next_move, CCoord direction);
	int GetDamage() const;
	CCoord GetDirection() const;

	std::istream &Load(std::istream &stream) override;
	std::ostream &Print(std::ostream &stream) const override;
};


#endif //TOWER_ATTACK_CPROJECTILE_H
