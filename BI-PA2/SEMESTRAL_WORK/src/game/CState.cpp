//
// Created by jaroslav on 5/9/20.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "CState.h"
#include "../gameobjects/CWall.h"
#include "../gameobjects/CPath.h"
#include "../gameobjects/CExit.h"


void CState::Step() {
	// updates projectiles
	while(!m_Projectiles.empty()){
		CProjectile *p = m_Projectiles.top();

		// other projectiles don't need to be updated
		if(p->GetNextMove() < m_StepsLeft)
			break;

		// update projectile
		m_Projectiles.pop();
		p->SetNextMove(m_StepsLeft - p->GetUpdateSpeed());
		std::pair<bool,CAttacker*> updated = m_Map.MoveProjectile(p);

		// projectile wasn't destroyed
		if(!updated.first)
			m_Projectiles.push(p);

		// projectile killed an attacker
		else if(updated.second){
			// updates attackers
			std::priority_queue<CAttacker*,std::vector<CAttacker*>,cmp<CAttacker>> tmp;
			// empty and fill the whole queue
			while(!m_Attackers.empty()){
				CAttacker *a = m_Attackers.top();
				m_Attackers.pop();
				// doesn't push back the destroyed ones
				if(a != updated.second)
					tmp.push(a);
				else delete a;
			}
			m_Attackers = tmp;
		}
	}

	// updates towers
	while(!m_Towers.empty()){
		CTower *t = m_Towers.top();
		// towers were fully updated
		if(t->GetNextMove() < m_StepsLeft)
			break;

		std::vector<CGameObject*> updated = m_Map.ShootProjectile(t);

		// removes attackers that were killed
		std::priority_queue<CAttacker*,std::vector<CAttacker*>,cmp<CAttacker>> tmp;
		while(!m_Attackers.empty()){
			CAttacker *a = m_Attackers.top();
			m_Attackers.pop();
			bool contains = false;

			// checks if the attacker was killed
			for(auto it = updated.begin(); it != updated.end();++it)
				if(*it == a){
					contains = true;
					updated.erase(it);
					delete a;
					break;
				}

			// pushes the tower back otherwise
			if(!contains)
				tmp.push(a);
		}
		m_Attackers = tmp;

		// pushes new projectiles in
		for(auto p: updated)
			m_Projectiles.push(dynamic_cast<CProjectile*>(p));

		// updates the tower
		m_Towers.pop();
		t->SetNextMove(m_StepsLeft - t->GetUpdateSpeed());
		m_Towers.push(t);
	}

	// place new towers
	if(m_StepsLeft % 400 == 0 && !m_TowerTypes.empty() && m_TowerTypes.size() < 6){
		srand((unsigned)time(nullptr));

		// gets new coord
		CCoord coord{rand() % m_Map.GetWidth(), rand() % m_Map.GetHeight()};

		// checks if the place is free
		if(dynamic_cast<CPath*>(m_Map.Get(coord))){

			// gets a tower type
			int tower_index = rand() % m_TowerTypes.size();
			auto tower_type = m_TowerTypes.begin();
			std::advance(tower_type, tower_index);
			auto *t = new CTower(tower_type->second);
			m_Map.Set(coord, t);

			// checks if the map is still playable
			if(!m_Map.IsTraversable())
				m_Map.Set(coord, new CPath(' '));
			else m_Towers.push(t);
		}
	}

	// recalculates distances
	m_Map.IsTraversable();

	// updates attackers
	std::vector<CGameObject*> attackers;

	// get all attackers that need to be updated
	while(!m_Attackers.empty()){
		CAttacker *a = m_Attackers.top();

		// others don't need to be updated
		if(a->GetNextMove() < m_StepsLeft)
			break;

		// update next move and push to the vector
		a->SetNextMove(m_StepsLeft - a->GetUpdateSpeed());
		attackers.push_back(a);
		--m_LivesLeft;
		m_Attackers.pop();
	}

	// put attackers back
	attackers = m_Map.MoveTowardsExit(attackers);
	for(int i = 0; i < (int)attackers.size(); ++i, ++m_LivesLeft)
			m_Attackers.push(dynamic_cast<CAttacker*>(attackers[i]));

	// spawn attackers
	std::priority_queue<CEntrance*,std::vector<CEntrance*>,cmp<CEntrance>> entrances;
	while(!m_Entrances.empty()){
		auto newAttackers = m_Map.SpawnAttackers(m_Entrances.top());
		for(auto a: newAttackers)
			m_Attackers.push(a);
		entrances.push(m_Entrances.top());
		m_Entrances.pop();
	}
	m_Entrances = entrances;

	m_Money += 5;

	if(--m_StepsLeft <= 0)
		m_Status = GameStatus::LOST;

	if(m_LivesLeft <= 0)
		m_Status = GameStatus::WON;
}

GameStatus CState::GetStatus() const {
	return m_Status;
}

bool CState::Load(std::ifstream &stream) {
	int height, width;
	if(!(stream >> m_StepsLeft >> m_LivesLeft >> m_Money >> height >> width))
		return false;

	if(m_StepsLeft < 0 || m_Money < 0 || m_LivesLeft <= 0)
		return false;

	// reading the map
	if(height < 0 || height > 15 || width < 0 || width > 100)
		return false;
	m_Map = CMap(height, width);

	// reading tower types
	int tower_types;
	if(!(stream >> tower_types))
		return false;
	for(int i = 0; i < tower_types; ++i){
		CTower t;
		if(!(stream >> t))
			return false;
		m_TowerTypes[t.GetSymbol()] = t;
	}

	// reading attacker types
	int attacker_types;
	if(!(stream >> attacker_types))
		return false;
	for(int i = 0; i < attacker_types; ++i){
		CAttacker a;
		if(!(stream >> a))
			return false;
		m_AttackerTypes[a.GetSymbol()] = a;
	}

	// reading towers
	if(!LoadObjects<CTower>(stream, &m_Towers))
		return false;

	// reading attackers
	if(!LoadObjects<CAttacker>(stream, &m_Attackers))
		return false;

	// reading projectiles
	if(!LoadObjects<CProjectile>(stream, &m_Projectiles))
		return false;

	// reading walls
	if(!LoadObjects<CWall>(stream))
		return false;

	// reading paths
	if(!LoadObjects<CPath>(stream))
		return false;

	// reading entrances
	if(!LoadObjects<CEntrance>(stream, &m_Entrances))
		return false;
	if(m_Entrances.empty())
		return false;

	// reading exits
	if(!LoadObjects<CExit>(stream, &m_Exits))
		return false;
	if(m_Exits.empty())
		return false;

	if(!m_Map.IsFilled())
		return false;

	m_Status = GameStatus::IN_PROGRESS;

	return m_Map.IsTraversable();
}

bool CState::Save(std::ostream &stream) const{
	stream << m_StepsLeft << std::endl
			<< m_LivesLeft << std::endl
			<< m_Money << std::endl
			<< m_Map.GetHeight() << std::endl
			<< m_Map.GetWidth() << "\n\n";

	// prints tower types
	stream << m_TowerTypes.size() << std::endl;
	for(const auto& t: m_TowerTypes)
		stream << " " << t.second << std::endl;
	stream << std::endl;

	// prints attacker types
	stream << m_AttackerTypes.size() << std::endl;
	for(const auto& a: m_AttackerTypes)
		stream << " " << a.second << std::endl;
	stream << std::endl;

	// prints all other objects
	m_Map.PrintObjects<CTower>(stream);
	m_Map.PrintObjects<CAttacker>(stream);
	m_Map.PrintObjects<CProjectile>(stream);
	m_Map.PrintObjects<CWall>(stream);
	m_Map.PrintObjects<CPath>(stream);
	m_Map.PrintObjects<CEntrance>(stream);
	m_Map.PrintObjects<CExit>(stream);

	return bool(stream);
}

bool CState::Print(const CInterface &interface) const {
	interface.Message("Steps left: ");
	interface.Message(std::to_string(m_StepsLeft));
	interface.Message("\nLives left: ");
	interface.Message(std::to_string(m_LivesLeft));
	interface.Message("\nMoney left: ");
	interface.Message(std::to_string(m_Money));
	interface.Message('\n');

	for(int i = 0; i < m_Map.GetHeight(); ++i, interface.Message('\n'))
		for(int j = 0; j < m_Map.GetWidth(); ++j)
			interface.Message(m_Map.Get(CCoord(j,i))->GetSymbol());

	int maxw = 3;

	interface.Message("\nAttacker types:\n");
	for(const auto &a: m_AttackerTypes){
		std::stringstream ss;
		ss << "Name: " << a.first
			<< ",  Update speed: " << std::setw(maxw) << a.second.GetUpdateSpeed()
			<< ",  Health: " << std::setw(maxw) << a.second.GetHealth()
			<< ",  Price: " << std::setw(maxw) << a.second.GetPrice()
			<< std::endl;
		interface.Message(ss.str());
	}

	interface.Message("\nTower types:\n");
	for(const auto &a: m_TowerTypes){
		std::stringstream ss;
		ss << "Name: " << a.first
		   << ",  Fire rate: " << std::setw(maxw) << a.second.GetUpdateSpeed()
		   << ",  Damage: " << std::setw(maxw) << a.second.GetDamage()
		   << ",  Projectile update speed: " << std::setw(maxw) << a.second.GetUpdateSpeed()
		   << std::endl;
		interface.Message(ss.str());
	}
	interface.Message('\n');
	return true;
}

bool CState::BuyAttackers(const int &count, const char &type, const char &entrance) {
	// checks if the type is correct
	if(!m_AttackerTypes.count(type))
		return false;

	if(count <= 0 || count > 10000)
		return false;

	std::priority_queue<CEntrance*,std::vector<CEntrance*>,cmp<CEntrance>> entrances;
	bool done = false;
	// finds the right entrance
	while(!m_Entrances.empty()){
		entrances.push(m_Entrances.top());
		if(m_Entrances.top()->GetSymbol() == entrance){
			// checks if player has enough money
			if(m_AttackerTypes[type].GetPrice() * count <= m_Money){
				done = true;
				m_Money -= m_AttackerTypes[type].GetPrice() * count;
				m_Entrances.top()->AddAttackers(m_AttackerTypes[type], count);
			}
		}
		m_Entrances.pop();
	}
	m_Entrances = entrances;

	return done;
}

template<typename T>
bool CState::LoadObjects(std::ifstream &stream, std::priority_queue<T*,std::vector<T*>,cmp<T>> *q) {
	int count;
	// gets number of objects
	if(!(stream >> count))
		return false;

	// loads all objects
	for(int i = 0; i < count; ++i){
		CCoord c{};
		auto *e = new T;
		if(!(stream >> c >> *e)){
			delete e;
			return false;
		}

		// inserts objects into the map
		if(!(m_Map.Set(c, e))){
			delete e;
			return false;
		}
		// inserts objects into their queue
		if(q) q->push(e);
	}
	return true;
}

