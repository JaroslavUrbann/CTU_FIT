//
// Created by jaroslav on 5/9/20.
//

#ifndef TOWER_ATTACK_CMAP_H
#define TOWER_ATTACK_CMAP_H

#include <vector>
#include <map>
#include <set>
#include <ostream>
#include "../gameobjects/CGameObject.h"
#include "CCoord.h"
#include "../gameobjects/CProjectile.h"
#include "../gameobjects/CTower.h"
#include "../gameobjects/CEntrance.h"

class CMap {
	int m_Height;
	int m_Width;
	// only 1 entity per field is allowed
	std::vector<std::vector<CGameObject*>> m_Map;
	std::vector<std::vector<int>> m_Distances;
	std::map<CGameObject*,CCoord> m_Position;
	// checks if the coordinate is within bounds
	bool IsIn(CCoord coord) const;
public:
	CMap();
	~CMap();
	CMap(int height, int width);
	int GetHeight() const;
	int GetWidth() const;
	// gets the object at the given coordinate
	CGameObject *Get(const CCoord &coord) const;
	bool Set(const CCoord &coord, CGameObject *entity);
	// checks if the attackers can still reach the exit
	bool IsTraversable();
	// checks if the map is fully filled
	bool IsFilled() const;
	// moves entities towards the exit, returns those who haven't gone through
	std::vector<CGameObject*> MoveTowardsExit(std::vector<CGameObject*> entities);
	// moves a projectile, returns whether the projectile was destroyed and an attacker that was killed
	std::pair<bool,CAttacker*> MoveProjectile(CProjectile *projectile);
	// shoots projectiles from the tower, returns new projectiles and killed attackers
	std::vector<CGameObject*> ShootProjectile(CTower *tower);
	// spawns new attackers, returns spawned attackers
	std::vector<CAttacker*> SpawnAttackers(CEntrance *entrance);

	// prints all entities of a given type
	template<typename T>
	void PrintObjects(std::ostream &stream) const{
		std::vector<T*> items;
		for(int i = 0; i < GetHeight(); ++i)
			for(int j = 0; j < GetWidth(); ++j)
				if(dynamic_cast<T*>(m_Map[i][j]))
					items.push_back(dynamic_cast<T*>(m_Map[i][j]));

		stream << items.size() << std::endl;
		for(auto item: items)
			stream << " " << m_Position.find(item)->second << " " << *item << std::endl;
		stream << std::endl;
	}
};

#endif //TOWER_ATTACK_CMAP_H
