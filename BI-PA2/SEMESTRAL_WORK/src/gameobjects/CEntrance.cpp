//
// Created by jaroslav on 5/9/20.
//

#include <fstream>
#include <iostream>
#include "CEntrance.h"


std::istream &CEntrance::Load(std::istream &stream){
	int attackerCount;
	if(!(stream >> m_Symbol >> attackerCount))
		return stream;
	// loads all attackers in the queue
	for(int i = 0; i < attackerCount; ++i){
		CAttacker a;
		if(!(stream >> a))
			return stream;
		AddAttackers(a, 1);
	}
	return stream;
}

std::ostream &CEntrance::Print(std::ostream &stream) const{
	stream << m_Symbol << " " << m_EntryQueue.size();
	auto tmpQueue = m_EntryQueue;
	while(!tmpQueue.empty()){
		stream << std::endl << " " << tmpQueue.front() << ((tmpQueue.size()!=1)?"\n":"");
		tmpQueue.pop();
	}
	return stream;
}

void CEntrance::AddAttackers(const CAttacker& attacker, int count) {
	for(int i = 0; i < count; ++i)
		m_EntryQueue.push(attacker);
}

// returns null if the queue is empty, otherwise allocates a new attacker and returns it
CAttacker *CEntrance::NextAttacker() {
	if(m_EntryQueue.empty())
		return nullptr;
	auto *a = new CAttacker(m_EntryQueue.front());
	m_EntryQueue.pop();
	return a;
}

