//
// Created by jaroslav on 5/9/20.
//

#include <fstream>
#include "CProjectile.h"

std::istream &CProjectile::Load(std::istream &stream){
	return stream >> m_Symbol
					>> m_UpdateSpeed
					>> m_Damage
					>> m_NextMove
					>> m_Direction;
}

std::ostream &CProjectile::Print(std::ostream &stream) const{
	return stream << m_Symbol << " "
					<< m_UpdateSpeed << " "
					<< m_Damage << " "
					<< m_NextMove << " "
					<< m_Direction;
}

CProjectile::CProjectile() = default;

CProjectile::CProjectile(char symbol, int update_speed, int damage, int next_move, CCoord direction):
	m_Damage(damage),m_Direction(direction){
	m_Symbol = symbol;
	m_UpdateSpeed = update_speed;
	m_NextMove = next_move;
}

int CProjectile::GetDamage() const {
	return m_Damage;
}

CCoord CProjectile::GetDirection() const {
	return m_Direction;
}
