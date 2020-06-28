//
// Created by jaroslav on 5/9/20.
//

#include <fstream>
#include "CPath.h"


std::istream &CPath::Load(std::istream &stream){
	return stream >> std::noskipws >> m_Symbol >> std::skipws;
}


CPath::CPath(char symbol) {
	m_Symbol = symbol;
}
