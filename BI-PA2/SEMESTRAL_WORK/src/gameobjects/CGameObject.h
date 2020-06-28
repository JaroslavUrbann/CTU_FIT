//
// Created by jaroslav on 5/9/20.
//

#ifndef TOWER_ATTACK_CGAMEOBJECT_H
#define TOWER_ATTACK_CGAMEOBJECT_H

#include <string>

class CGameObject {
protected:
	int m_UpdateSpeed;
	int m_NextMove;
	char m_Symbol;
public:
	CGameObject();
	virtual ~CGameObject();
	char GetSymbol() const;
	virtual int GetUpdateSpeed() const;
	virtual int GetNextMove() const;
	virtual void SetNextMove(int nextmove);
	virtual std::istream &Load(std::istream &stream);
	virtual std::ostream &Print(std::ostream &stream) const;
};

inline std::istream& operator>>(std::istream &stream, CGameObject &object){
	return object.Load(stream);
}

inline std::ostream& operator<<(std::ostream &stream, const CGameObject &object){
	return object.Print(stream);
}

#endif //TOWER_ATTACK_CGAMEOBJECT_H
