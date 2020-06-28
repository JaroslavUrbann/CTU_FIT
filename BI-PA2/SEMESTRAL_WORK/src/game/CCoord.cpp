//
// Created by jaroslav on 5/9/20.
//

#include "CCoord.h"
#include <iostream>

int CCoord::GetX() const {
	return x;
}

int CCoord::GetY() const {
	return y;
}

std::istream& operator>>(std::istream &stream, CCoord &coord) {
	return stream >> coord.y >> coord.x;
}

std::ostream& operator<<(std::ostream &stream, const CCoord &coord){
	return stream << coord.GetY() << " " << coord.GetX();
}

