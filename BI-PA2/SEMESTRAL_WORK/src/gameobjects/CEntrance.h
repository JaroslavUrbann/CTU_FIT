//
// Created by jaroslav on 5/9/20.
//

#ifndef TOWER_ATTACK_CENTRANCE_H
#define TOWER_ATTACK_CENTRANCE_H


#include <queue>
#include "CGameObject.h"
#include "CAttacker.h"

// entrance that the user can choose to send attackers to
class CEntrance : public CGameObject {
	// queue of attackers that are waiting to be sent to the playing field in the next few time steps
	std::queue<CAttacker> m_EntryQueue;
public:
	// adds attackers to the queue
	void AddAttackers(const CAttacker& attacker, int count);
	// gets the next attacker from the queue
	CAttacker *NextAttacker();
	std::istream &Load(std::istream &stream) override;
	std::ostream &Print(std::ostream &stream) const override;
};


#endif //TOWER_ATTACK_CENTRANCE_H
