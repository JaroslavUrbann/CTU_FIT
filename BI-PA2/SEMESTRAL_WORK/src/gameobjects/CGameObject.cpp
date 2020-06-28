//
// Created by jaroslav on 5/9/20.
//

#include "CGameObject.h"
#include <iostream>

char CGameObject::GetSymbol() const {
	return m_Symbol;
}
void CGameObject::SetNextMove(int nextmove) {
	m_NextMove = nextmove;
}

int CGameObject::GetNextMove() const {
	return m_NextMove;
}

std::istream &CGameObject::Load(std::istream &stream) {
	return stream >> m_Symbol;
}

std::ostream &CGameObject::Print(std::ostream &stream) const {
	return stream << GetSymbol();
}

int CGameObject::GetUpdateSpeed() const {
	return m_UpdateSpeed;
}

CGameObject::CGameObject():m_NextMove(-1){}

CGameObject::~CGameObject() = default;

