//
// Created by jaroslav on 5/10/20.
//

#include <fstream>
#include "CAttacker.h"

std::istream &CAttacker::Load(std::istream &stream){
	return stream >> m_Symbol
					>> m_UpdateSpeed
					>> m_Health
					>> m_NextMove
					>> m_Price;
}

std::ostream &CAttacker::Print(std::ostream &stream) const {
	return stream << m_Symbol << " "
				  << m_UpdateSpeed << " "
				  << m_Health << " "
				  << m_NextMove << " "
				  << m_Price;
}

int CAttacker::GetHealth() const {
	return m_Health;
}

int CAttacker::GetPrice() const {
	return m_Price;
}

void CAttacker::SetHealth(int health) {
	m_Health = health;
}

CAttacker::CAttacker(const CAttacker &other) {
	m_Symbol = other.m_Symbol;
	m_UpdateSpeed = other.GetUpdateSpeed();
	m_NextMove = other.GetNextMove();
	m_Health = other.GetHealth();
	m_Price = other.GetPrice();
}

CAttacker::CAttacker() = default;

