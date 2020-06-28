//
// Created by jaroslav on 5/9/20.
//

#ifndef TOWER_ATTACK_CCOORD_H
#define TOWER_ATTACK_CCOORD_H


#include <iosfwd>

class CCoord {
	int x;
	int y;
public:
	CCoord() = default;
	CCoord(int x, int y):x(x),y(y){}
	int GetX() const;
	int GetY() const;
	friend std::istream& operator>>(std::istream &stream, CCoord &coord);
	friend std::ostream& operator<<(std::ostream &stream, const CCoord &coord);
};


#endif //TOWER_ATTACK_CCOORD_H
