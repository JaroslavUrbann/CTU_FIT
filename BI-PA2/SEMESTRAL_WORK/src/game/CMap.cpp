//
// Created by jaroslav on 5/9/20.
//

#include <algorithm>
#include "CMap.h"
#include "../gameobjects/CExit.h"
#include "../gameobjects/CWall.h"
#include "../gameobjects/CPath.h"
#include "CCoord.h"


CMap::CMap(int height, int width):m_Height(height),m_Width(width),
				m_Map(std::vector<std::vector<CGameObject*>>(height, std::vector<CGameObject*>(width, nullptr))){}

int CMap::GetHeight() const {
	return m_Height;
}

int CMap::GetWidth() const {
	return m_Width;
}

// returns a pointer to an object in the given coordinate
CGameObject *CMap::Get(const CCoord &coord) const {
	if(!IsIn(coord))
		return nullptr;

	return m_Map[coord.GetY()][coord.GetX()];
}

// checks if the attackers can still reach the exit
bool CMap::IsTraversable() {

	// gets all exits
	std::vector<CCoord> exits;
	for(int i = 0; i < m_Height; ++i)
		for(int j = 0; j < m_Width; ++j)
			if(dynamic_cast<CExit*>(m_Map[i][j]))
				exits.emplace_back(j, i);

	// checks if there are any
	if(exits.empty())
		return false;

	m_Distances = std::vector<std::vector<int>>(m_Height, std::vector<int>(m_Width, 100000));

	// checks if at least one of them leads to at least one entrance via bfs
	// graph edges
	int dx[4] = {1, -1, 0, 0};
	int dy[4] = {0, 0, 1, -1};
	bool found = false;
	for(auto coord: exits){
		m_Distances[coord.GetY()][coord.GetX()] = 1;
		std::queue<CCoord> q;
		q.push(coord);

		while(!q.empty()){
			// gets new node from the queue
			auto c = q.front();
			q.pop();

			// search ends if an entrance is reached
			if(dynamic_cast<CEntrance*>(Get(c)))
				found = true;

			// expands search to neighboring nodes
			for(int i = 0; i < 4; ++i){
				int nx = c.GetX() + dx[i];
				int ny = c.GetY() + dy[i];

				// checks if the node is traversable
				if(nx >= 0 && nx < m_Width && ny >= 0 && ny < m_Height
					&& m_Distances[ny][nx] - 1 > m_Distances[c.GetY()][c.GetX()]
					&& (dynamic_cast<CPath*>(Get(CCoord(nx,ny))) || dynamic_cast<CAttacker*>(Get(CCoord(nx,ny)))
						|| dynamic_cast<CEntrance*>(Get(CCoord(nx,ny))))){

					// inserts it into the queue
					m_Distances[ny][nx] = m_Distances[c.GetY()][c.GetX()] + 1;
					q.push(CCoord(nx,ny));
				}
			}
		}
	}

	return found;
}


// puts a game object into the field
bool CMap::Set(const CCoord &coord, CGameObject *entity) {
	if(!IsIn(coord) || !entity){
		delete entity;
		return false;
	}

	m_Position.erase(m_Map[coord.GetY()][coord.GetX()]);
	delete m_Map[coord.GetY()][coord.GetX()];
	m_Map[coord.GetY()][coord.GetX()] = entity;
	m_Position[entity] = coord;
	return true;
}

// checks if the map is fully filled
bool CMap::IsFilled() const {
	for(int i = 0; i < m_Height; ++i)
		for(int j = 0; j < m_Width; ++j)
			if(!m_Map[i][j])
				return false;

	return true;
}

// moves given entities towards the exit
std::vector<CGameObject *> CMap::MoveTowardsExit(std::vector<CGameObject *> entities) {
	// sort entities by their closeness to the exit
	std::sort(entities.begin(), entities.end(), [&](CGameObject *a, CGameObject *b){
		auto posA = m_Position[a];
		auto posB = m_Position[b];
		return m_Distances[posA.GetY()][posA.GetX()] < m_Distances[posB.GetY()][posB.GetX()];
	});

	// direction vectors
	int dx[4] = {1, -1, 0, 0};
	int dy[4] = {0, 0, 1, -1};

	// entities that haven't gone through the exit
	std::vector<CGameObject*>left;

	// moves all entities
	for(auto e: entities){
		left.push_back(e);
		auto pos = m_Position[e];
		int best = -1;
		int bestval = 1000;

		// checks all surrounding squares
		for(int i = 0; i < 4; ++i){
			int x = pos.GetX() + dx[i];
			int y = pos.GetY() + dy[i];

			// updates the best distance
			if(dynamic_cast<CPath*>(Get(CCoord(x,y))) && m_Distances[y][x] < bestval) {
				best = i;
				bestval = m_Distances[y][x];
			}

			// checks if there is an exit near by
			if(dynamic_cast<CExit*>(Get(CCoord(x,y)))){
				best = -1;
				auto *p = new CPath(' ');
				Set(pos, p);
				left.pop_back();
				break;
			}
		}

		// moves the entity by swapping it with a nearby square
		if(~best){
			m_Position[e] = m_Position[m_Map[pos.GetY() + dy[best]][pos.GetX() + dx[best]]];
			m_Position[m_Map[pos.GetY() + dy[best]][pos.GetX() + dx[best]]] = pos;
			std::swap(m_Map[pos.GetY()][pos.GetX()], m_Map[pos.GetY() + dy[best]][pos.GetX() + dx[best]]);
		}
	}
	return left;
}

// moves a projectile, returns whether it was destroyed and an attacker that was killed
std::pair<bool,CAttacker*> CMap::MoveProjectile(CProjectile *projectile) {
	// gets original position of the projectile
	int ox = m_Position[projectile].GetX();
	int oy = m_Position[projectile].GetY();
	// gets new position of the projectile
	int y = oy + projectile->GetDirection().GetY();
	int x = ox + projectile->GetDirection().GetX();

	std::pair<bool,CAttacker*> res = {false, nullptr};

	CGameObject *o = Get(CCoord(x,y));
	// checks if the new position is a free path
	if(dynamic_cast<CPath*>(o)) {
		m_Position[o] = CCoord(ox, oy);
		m_Position[projectile] = CCoord(x,y);
		std::swap(m_Map[oy][ox], m_Map[y][x]);
	}
	else {
		// removes health from an attacker if it hits one
		if(dynamic_cast<CAttacker*>(o)){
			auto *a = dynamic_cast<CAttacker*>(o);
			a->SetHealth(a->GetHealth() - projectile->GetDamage());
			if(a->GetHealth() <= 0){
				res.second = a;
				auto *p = new CPath(' ');
				m_Position.erase(m_Map[CCoord(x,y).GetY()][CCoord(x,y).GetX()]);
				m_Map[CCoord(x,y).GetY()][CCoord(x,y).GetX()] = p;
				m_Position[p] = CCoord(x,y);
			}
		}
		res.first = true;
		Set(CCoord(ox,oy), new CPath(' '));
	}
	return res;
}

// shoots projectiles out of a given tower
std::vector<CGameObject*> CMap::ShootProjectile(CTower *tower) {
	// resulting projectiles and destroyed attackers
	std::vector<CGameObject*> res;

	// direction vectors
	int dx[4] = {1, -1, 0, 0};
	int dy[4] = {0, 0, 1, -1};
	for(int i = 0; i < 4; ++i){
		// projectile position
		int y = m_Position[tower].GetY() + dy[i];
		int x = m_Position[tower].GetX() + dx[i];

		// projectile shot an attacker
		if(dynamic_cast<CAttacker*>(Get(CCoord(x,y)))){
			CAttacker *a = dynamic_cast<CAttacker*>(Get(CCoord(x,y)));
			a->SetHealth(a->GetHealth() - tower->GetDamage());

			// attacker was killed
			if(a->GetHealth() <= 0){
				res.push_back(a);
				auto *p = new CPath(' ');
				m_Position.erase(m_Map[CCoord(x,y).GetY()][CCoord(x,y).GetX()]);
				m_Map[CCoord(x,y).GetY()][CCoord(x,y).GetX()] = p;
				m_Position[p] = CCoord(x,y);
			}
			continue;
		}

		// if not a path
		if(!dynamic_cast<CPath*>(Get(CCoord(x,y))))
			continue;

		// checks is the tower should be active
		bool active = false;
		for(int j = 0; j < tower->GetRange(); ++j){
			for(int k = -j; k < j + 1; ++k){
				int ny = dy[i] ? y + dy[i]*j : y + k;
				int nx = dx[i] ? x + dx[i]*j : x + k;
				if(dynamic_cast<CAttacker*>(Get(CCoord(nx,ny))))
					active = true;
			}
		}

		// places a projectile if the tower is active
		if(active){
			auto *p = new CProjectile('o', tower->GetProjectileSpeed(),
											tower->GetDamage(),
											tower->GetNextMove() - tower->GetProjectileSpeed(),
											CCoord(dx[i], dy[i]));
			Set(CCoord(x,y), p);
			res.push_back(p);
		}
	}
	return res;
}

bool CMap::IsIn(CCoord coord) const {
	return coord.GetX() >= 0 && coord.GetY() >= 0 && coord.GetX() < m_Width && coord.GetY() < m_Height;
}

std::vector<CAttacker *> CMap::SpawnAttackers(CEntrance *entrance) {
	std::vector<CAttacker*> attackers;

	int dx[4] = {1, -1, 0, 0};
	int dy[4] = {0, 0, 1, -1};
	// checks if surrounding squares are empty, and inserts an attacker if they are
	for(int i = 0; i < 4; ++i){
		int y = m_Position[entrance].GetY() + dy[i];
		int x = m_Position[entrance].GetX() + dx[i];
		if(dynamic_cast<CPath*>(Get(CCoord(x,y)))){
			CAttacker *a = entrance->NextAttacker();
			if(a){
				Set(CCoord(x,y), a);
				attackers.push_back(a);
			}
		}
	}

	return attackers;
}

CMap::CMap() {

}

CMap::~CMap() {
	for(int i = 0; i < GetHeight(); ++i)
		for(int j = 0; j < GetWidth(); ++j)
			delete m_Map[i][j];
}

