//
// Created by jaroslav on 5/9/20.
//

#ifndef TOWER_ATTACK_CTOWER_H
#define TOWER_ATTACK_CTOWER_H


#include "CGameObject.h"

class CTower : public CGameObject {
	int m_Damage;
	int m_Range;
	int m_ProjectileSpeed;
public:
	int GetDamage() const;
	int GetRange() const;
	int GetProjectileSpeed() const;

	std::istream &Load(std::istream &stream) override;
	std::ostream &Print(std::ostream &stream) const override;
};


#endif //TOWER_ATTACK_CTOWER_H
