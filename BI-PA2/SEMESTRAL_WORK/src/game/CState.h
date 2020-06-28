//
// Created by jaroslav on 5/9/20.
//

#ifndef TOWER_ATTACK_CSTATE_H
#define TOWER_ATTACK_CSTATE_H


#include <map>
#include "../gameobjects/CProjectile.h"
#include "../gameobjects/CTower.h"
#include "CCoord.h"
#include "../gameobjects/CEntrance.h"
#include "CMap.h"
#include "../CInterface.h"
#include "../gameobjects/CPath.h"
#include "../gameobjects/CWall.h"
#include "../gameobjects/CExit.h"

enum GameStatus{
	IN_PROGRESS,
	LOST,
	WON
};

// comparison function for priority queues
template<typename T>
struct cmp{
	bool operator()(T *a, T *b){
		return a && b && a->GetNextMove() < b->GetNextMove();
	}
};


class CState {
	int m_StepsLeft;
	CMap m_Map;
	GameStatus m_Status;
	int m_LivesLeft;
	int m_Money;
	// attacker & tower types that are represented with a char
	std::map<char,CAttacker> m_AttackerTypes;
	std::map<char,CTower> m_TowerTypes;
	// pointers to game objects in the map for faster updating
	std::priority_queue<CProjectile*,std::vector<CProjectile*>,cmp<CProjectile>> m_Projectiles;
	std::priority_queue<CTower*,std::vector<CTower*>,cmp<CTower>> m_Towers;
	std::priority_queue<CAttacker*,std::vector<CAttacker*>,cmp<CAttacker>> m_Attackers;
	std::priority_queue<CEntrance*,std::vector<CEntrance*>,cmp<CEntrance>> m_Entrances;
	std::priority_queue<CExit*,std::vector<CExit*>,cmp<CExit>> m_Exits;
	// generic method for loading game objects
	template<typename T>
	bool LoadObjects(std::ifstream &stream, std::priority_queue<T*,std::vector<T*>,cmp<T>> *q = nullptr);
public:
	void Step();
	GameStatus GetStatus() const;
	bool Load(std::ifstream &stream);
	bool Save(std::ostream &stream) const;
	bool Print(const CInterface &interface) const;
	bool BuyAttackers(const int &count, const char &type, const char &entrance);
};

#endif //TOWER_ATTACK_CSTATE_H
