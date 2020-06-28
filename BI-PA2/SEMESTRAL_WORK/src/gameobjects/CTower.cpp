//
// Created by jaroslav on 5/9/20.
//

#include <fstream>
#include "CTower.h"

std::istream &CTower::Load(std::istream &stream){
	return stream >> m_Symbol
					>> m_UpdateSpeed
					>> m_Damage
					>> m_NextMove
					>> m_Range
					>> m_ProjectileSpeed;
}

std::ostream &CTower::Print(std::ostream &stream) const{
	return stream << m_Symbol << " "
				  << m_UpdateSpeed << " "
				  << m_Damage << " "
				  << m_NextMove << " "
				  << m_Range << " "
				  << m_ProjectileSpeed;
}

int CTower::GetDamage() const {
	return m_Damage;
}

int CTower::GetRange() const {
	return m_Range;
}

int CTower::GetProjectileSpeed() const {
	return m_ProjectileSpeed;
}

