//
// Created by jaroslav on 5/10/20.
//

#ifndef TOWER_ATTACK_CATTACKER_H
#define TOWER_ATTACK_CATTACKER_H


#include "CGameObject.h"

class CAttacker : public CGameObject {
	int m_Health;
	int m_Price;
public:
	CAttacker();
	CAttacker(const CAttacker &other);
	int GetHealth() const;
	int GetPrice() const;

	void SetHealth(int health);
	std::istream &Load(std::istream &stream) override;
	std::ostream &Print(std::ostream &stream) const override;
};


#endif //TOWER_ATTACK_CATTACKER_H
