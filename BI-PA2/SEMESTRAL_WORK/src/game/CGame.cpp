//
// Created by jaroslav on 5/9/20.
//

#include "CGame.h"

#include <ctime>
#include <sstream>

bool CGame::Print(const CInterface &interface) const {
	return m_State.Print(interface);
}

bool CGame::Load(const std::string& filename) {
	std::ifstream stream(filename);
	if(!stream || !m_State.Load(stream))
		return false;

	m_Filename = filename;

	// erases _saveX suffix from the filename
	if(m_Filename.find("_save") != std::string::npos)
		m_Filename.erase(m_Filename.find("_save"));
	return true;
}

bool CGame::Save() const {
	int i = 0;
	// adds _saveX suffix
	while(i < 1000 && std::ifstream(m_Filename + "_save" + std::to_string(i)))++i;
	if(i == 1000)
		return false;

	std::ofstream stream(m_Filename + "_save" + std::to_string(i));
	return stream && m_State.Save(stream);
}

void CGame::Step() {
	m_State.Step();
}

bool CGame::BuyAttackers(const int &count, const char &type, const char &entrance) {
	return m_State.BuyAttackers(count, type, entrance);
}

GameStatus CGame::GetStatus() const {
	return m_State.GetStatus();
}
