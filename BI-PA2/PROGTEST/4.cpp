#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */

class strng{
	public:
		char *str;
		int ln, al;
		strng();
		strng(const char * s);
		~strng();
		void pb(const char *s);
		void pb(int val);
		strng& operator=(const strng &s);
};

strng::strng():ln(0),al(1){
	str = new char[al];
	str[0] = 0;
}
strng::strng(const char *s):ln(strlen(s)),al(strlen(s)+1){
	str = new char[al];
	strcpy(str, s);
}
strng::~strng(){
	delete [] str;
}
void strng::pb(const char *s){
	int len = strlen(s);
	if (ln + len >= al){
		al = 1.5*(ln + len + 1);
		char *newStr = new char[al];
		strcpy(newStr, str);
		delete [] str;
		str = newStr;
	}
	strcpy(str + ln, s);
	ln += len;
}
void strng::pb(int val){
	char str[20];
	sprintf(str, "%d", val);
	pb(str);
}
strng& strng::operator=(const strng &s){
	if (&s == this) return *this;
	delete [] str;
	al = s.ln + 1;
	ln = s.ln;
	str = new char[al];
	strcpy(str, s.str);
	return *this;
}


class CAcc{
	public:
		int startSum, endSum;
		strng accID;
		strng trans;
		CAcc();
		CAcc(const char *ID, int sum);
		void newTrs(const char *ID, const char *sign, int val, bool to);
		void trim();
		int Balance();
};


CAcc::CAcc(){}
CAcc::CAcc(const char *ID, int sum):startSum(sum),endSum(sum),accID(ID){}
void CAcc::newTrs(const char *ID, const char *sign, int val, bool to){
	if (to) trans.pb("\n - ");
	else trans.pb("\n + ");
	trans.pb(val);
	if (to) trans.pb(", to: ");
	else trans.pb(", from: ");
	trans.pb(ID);
	trans.pb(", sign: ");
	trans.pb(sign);
	if (to) val *= -1;
	endSum += val;
}
void CAcc::trim(){
	startSum = endSum;
	trans.ln = 0;
	trans.str[0] = 0;
}
int CAcc::Balance(){
	return endSum;
}
ostream& operator<<(ostream &os, const CAcc &acc){
	os << acc.accID.str << ":\n   " << acc.startSum;
	os << acc.trans.str;
	os << "\n = " << acc.endSum << endl;
	return os;
}


struct AccArr{
	int users, size, alloc;
	CAcc *accs = nullptr;
	AccArr():users(1),size(0),alloc(0){
		accs = new CAcc[alloc];
	}
	~AccArr(){
		delete [] accs;
	}
	AccArr& operator=(const AccArr &ar){
		if (&ar == this) return *this;
		users = ar.users;
		size = ar.size;
		alloc = ar.size;
		delete [] accs;
		accs = new CAcc[alloc];
		for (int i = 0; i < size; ++i)
			accs[i] = ar.accs[i];
		return *this;
	}
	CAcc *findAcc(CAcc &acc){
		for (int i = 0; i < size; ++i)
			if (!strcmp(accs[i].accID.str, acc.accID.str))
				return accs + i;
		return nullptr;
	}
	void newAcc(CAcc &acc){
		if (size == alloc){
			CAcc *newAccs = new CAcc[alloc+=1];
			for (int i = 0; i < size; ++i)
				newAccs[i] = accs[i];
			delete [] accs;
			accs = newAccs;
		}
		accs[size++] = acc;
	}
};


class CBank{
	public:
		AccArr *shared, mine;
		bool par;
		CBank();
		CBank(const CBank &b);
		~CBank();
		CBank& operator=(const CBank &b);
		bool NewAccount(const char *accID, int initialBalance);
		bool Transaction(const char *depbAccID, const char *credAccId, unsigned int amount, const char *signature);
		bool TrimAccount(const char *accID);
		CAcc Account(const char *accID);
		CAcc * findAcc(CAcc &acc);
		int findCreateAcc(CAcc &acc);
};

CBank::CBank():par(false){
	shared = new AccArr;
}
CBank::CBank(const CBank &b):shared(b.shared),par(b.par){
	shared->users++;
	mine = b.mine;
}
CBank::~CBank(){
	if (!--shared->users) delete shared;
}
CBank& CBank::operator=(const CBank &b){
	if (&b == this) return *this;
	if (!--shared->users) delete shared;
	mine = b.mine;
	shared = b.shared;
	shared->users++;
	return *this;
}
CAcc * CBank::findAcc(CAcc &acc){
	par = shared->users > 1 ? true : par;
	CAcc *r = nullptr;
	if (par) r = mine.findAcc(acc);
	if (!r) r = shared->findAcc(acc);
	return r;
}
bool CBank::NewAccount(const char *accID, int initialBalance){
	CAcc acc(accID, initialBalance);
	if (findAcc(acc)) return false;
	if (par) mine.newAcc(acc);
	else shared->newAcc(acc);
	return true;
}
CAcc CBank::Account(const char *accID){
	CAcc tmp(accID, 0);
	CAcc *r = findAcc(tmp);
	if (!r) throw std::invalid_argument("Am I a joke to you?");
	tmp = *r;
	return tmp;
}
int CBank::findCreateAcc(CAcc &acc){
	par = shared->users > 1 ? true : par;
	CAcc *r = nullptr;
	if (par) r = mine.findAcc(acc);
	if (r) return r - mine.accs;
	r = shared->findAcc(acc);
	if (r && par){
		mine.newAcc(*r);
		return mine.size - 1;
	}
	if (!r) return -1;
	return r - shared->accs;
}
bool CBank::TrimAccount(const char *accID){
	CAcc acc(accID, 0);
	int i = findCreateAcc(acc);
	if (i == -1) return false;
	if (par) mine.accs[i].trim();
	else shared->accs[i].trim();
	return true;
}
bool CBank::Transaction(const char *depbAccID, const char *credAccId, unsigned int amount, const char *signature){
	CAcc tmpA(depbAccID, 0);
	CAcc tmpB(credAccId, 0);
	int i = findCreateAcc(tmpA);
	int j = findCreateAcc(tmpB);
	if (i == -1 || j == -1 || i == j) return false;
	if (par){
		mine.accs[i].newTrs(credAccId, signature, amount, true);
		mine.accs[j].newTrs(depbAccID, signature, amount, false);
	} else{
		shared->accs[i].newTrs(credAccId, signature, amount, true);
		shared->accs[j].newTrs(depbAccID, signature, amount, false);		
	}
	return true;
}


#ifndef __PROGTEST__
int main ( void )
{
  ostringstream os;
  char accCpy[100], debCpy[100], credCpy[100], signCpy[100];
  CBank x0;
  assert ( x0 . NewAccount ( "123456", 1000 ) );
  assert ( x0 . NewAccount ( "987654", -500 ) );
  assert ( x0 . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" ) );
  assert ( x0 . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" ) );
  assert ( x0 . NewAccount ( "111111", 5000 ) );
  assert ( x0 . Transaction ( "111111", "987654", 290, "Okh6e+8rAiuT5=" ) );
  assert ( x0 . Account ( "123456" ). Balance ( ) ==  -2190 );
  assert ( x0 . Account ( "987654" ). Balance ( ) ==  2980 );
  assert ( x0 . Account ( "111111" ). Balance ( ) ==  4710 );
  os . str ( "" );
  os << x0 . Account ( "123456" );
  assert ( ! strcmp ( os . str () . c_str (), "123456:\n   1000\n - 300, to: 987654, sign: XAbG5uKz6E=\n - 2890, to: 987654, sign: AbG5uKz6E=\n = -2190\n" ) );
  os . str ( "" );
  os << x0 . Account ( "987654" );
  assert ( ! strcmp ( os . str () . c_str (), "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 290, from: 111111, sign: Okh6e+8rAiuT5=\n = 2980\n" ) );
  os . str ( "" );
  os << x0 . Account ( "111111" );
  assert ( ! strcmp ( os . str () . c_str (), "111111:\n   5000\n - 290, to: 987654, sign: Okh6e+8rAiuT5=\n = 4710\n" ) );
  assert ( x0 . TrimAccount ( "987654" ) );
  assert ( x0 . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" ) );
  os . str ( "" );
  os << x0 . Account ( "987654" );
  assert ( ! strcmp ( os . str () . c_str (), "987654:\n   2980\n + 123, from: 111111, sign: asdf78wrnASDT3W\n = 3103\n" ) );

  CBank x2;
  strncpy ( accCpy, "123456", sizeof ( accCpy ) );
  assert ( x2 . NewAccount ( accCpy, 1000 ));
  strncpy ( accCpy, "987654", sizeof ( accCpy ) );
  assert ( x2 . NewAccount ( accCpy, -500 ));
  strncpy ( debCpy, "123456", sizeof ( debCpy ) );
  strncpy ( credCpy, "987654", sizeof ( credCpy ) );
  strncpy ( signCpy, "XAbG5uKz6E=", sizeof ( signCpy ) );
  assert ( x2 . Transaction ( debCpy, credCpy, 300, signCpy ) );
  strncpy ( debCpy, "123456", sizeof ( debCpy ) );
  strncpy ( credCpy, "987654", sizeof ( credCpy ) );
  strncpy ( signCpy, "AbG5uKz6E=", sizeof ( signCpy ) );
  assert ( x2 . Transaction ( debCpy, credCpy, 2890, signCpy ) );
  strncpy ( accCpy, "111111", sizeof ( accCpy ) );
  assert ( x2 . NewAccount ( accCpy, 5000 ));
  strncpy ( debCpy, "111111", sizeof ( debCpy ) );
  strncpy ( credCpy, "987654", sizeof ( credCpy ) );
  strncpy ( signCpy, "Okh6e+8rAiuT5=", sizeof ( signCpy ) );
  assert ( x2 . Transaction ( debCpy, credCpy, 2890, signCpy ) );
  assert ( x2 . Account ( "123456" ). Balance ( ) ==  -2190 );
  assert ( x2 . Account ( "987654" ). Balance ( ) ==  5580 );
  assert ( x2 . Account ( "111111" ). Balance ( ) ==  2110 );
  os . str ( "" );
  os << x2 . Account ( "123456" );
  assert ( ! strcmp ( os . str () . c_str (), "123456:\n   1000\n - 300, to: 987654, sign: XAbG5uKz6E=\n - 2890, to: 987654, sign: AbG5uKz6E=\n = -2190\n" ) );
  os . str ( "" );
  os << x2 . Account ( "987654" );
  assert ( ! strcmp ( os . str () . c_str (), "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 2890, from: 111111, sign: Okh6e+8rAiuT5=\n = 5580\n" ) );
  os . str ( "" );
  os << x2 . Account ( "111111" );
  assert ( ! strcmp ( os . str () . c_str (), "111111:\n   5000\n - 2890, to: 987654, sign: Okh6e+8rAiuT5=\n = 2110\n" ) );
  assert ( x2 . TrimAccount ( "987654" ) );
  strncpy ( debCpy, "111111", sizeof ( debCpy ) );
  strncpy ( credCpy, "987654", sizeof ( credCpy ) );
  strncpy ( signCpy, "asdf78wrnASDT3W", sizeof ( signCpy ) );
  assert ( x2 . Transaction ( debCpy, credCpy, 123, signCpy ) );
  os . str ( "" );
  os << x2 . Account ( "987654" );
  assert ( ! strcmp ( os . str () . c_str (), "987654:\n   5580\n + 123, from: 111111, sign: asdf78wrnASDT3W\n = 5703\n" ) );

  CBank x4;
  assert ( x4 . NewAccount ( "123456", 1000 ) );
  assert ( x4 . NewAccount ( "987654", -500 ) );
  assert ( !x4 . NewAccount ( "123456", 3000 ) );
  assert ( !x4 . Transaction ( "123456", "666", 100, "123nr6dfqkwbv5" ) );
  assert ( !x4 . Transaction ( "666", "123456", 100, "34dGD74JsdfKGH" ) );
  assert ( !x4 . Transaction ( "123456", "123456", 100, "Juaw7Jasdkjb5" ) );
  try
  {
    x4 . Account ( "666" ). Balance ( );
    assert ( "Missing exception !!" == NULL );
  }
  catch ( ... )
  {
  }
  try
  {
    os << x4 . Account ( "666" ). Balance ( );
    assert ( "Missing exception !!" == NULL );
  }
  catch ( ... )
  {
  }
  assert ( !x4 . TrimAccount ( "666" ) );

  CBank x6;
  assert ( x6 . NewAccount ( "123456", 1000 ) );
  assert ( x6 . NewAccount ( "987654", -500 ) );
  assert ( x6 . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" ) );
  assert ( x6 . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" ) );
  assert ( x6 . NewAccount ( "111111", 5000 ) );
  assert ( x6 . Transaction ( "111111", "987654", 2890, "Okh6e+8rAiuT5=" ) );
  CBank x7 ( x6 );
  assert ( x6 . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" ) );
  assert ( x7 . Transaction ( "111111", "987654", 789, "SGDFTYE3sdfsd3W" ) );
  // cout << x6 . Account ( "111111" );
  // cout << x7 . Account ( "111111" );
  assert ( x6 . NewAccount ( "99999999", 7000 ) );
  assert ( x6 . Transaction ( "111111", "99999999", 3789, "aher5asdVsAD" ) );
  assert ( x6 . TrimAccount ( "111111" ) );
  assert ( x6 . Transaction ( "123456", "111111", 221, "Q23wr234ER==" ) );
  os . str ( "" );
  // cout << x6 . Account ( "111111" );
  os << x6 . Account ( "111111" );
  assert ( ! strcmp ( os . str () . c_str (), "111111:\n   -1802\n + 221, from: 123456, sign: Q23wr234ER==\n = -1581\n" ) );
  os . str ( "" );
  os << x6 . Account ( "99999999" );
  assert ( ! strcmp ( os . str () . c_str (), "99999999:\n   7000\n + 3789, from: 111111, sign: aher5asdVsAD\n = 10789\n" ) );
  os . str ( "" );
  os << x6 . Account ( "987654" );
  assert ( ! strcmp ( os . str () . c_str (), "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 2890, from: 111111, sign: Okh6e+8rAiuT5=\n + 123, from: 111111, sign: asdf78wrnASDT3W\n = 5703\n" ) );
  os . str ( "" );
  os << x7 . Account ( "111111" );
  assert ( ! strcmp ( os . str () . c_str (), "111111:\n   5000\n - 2890, to: 987654, sign: Okh6e+8rAiuT5=\n - 789, to: 987654, sign: SGDFTYE3sdfsd3W\n = 1321\n" ) );
  try
  {
    os << x7 . Account ( "99999999" ). Balance ( );
    assert ( "Missing exception !!" == NULL );
  }
  catch ( ... )
  {
  }
  os . str ( "" );
  os << x7 . Account ( "987654" );
  assert ( ! strcmp ( os . str () . c_str (), "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 2890, from: 111111, sign: Okh6e+8rAiuT5=\n + 789, from: 111111, sign: SGDFTYE3sdfsd3W\n = 6369\n" ) );

  CBank x8;
  CBank x9;
  assert ( x8 . NewAccount ( "123456", 1000 ) );
  assert ( x8 . NewAccount ( "987654", -500 ) );
  assert ( x8 . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" ) );
  assert ( x8 . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" ) );
  assert ( x8 . NewAccount ( "111111", 5000 ) );
  assert ( x8 . Transaction ( "111111", "987654", 2890, "Okh6e+8rAiuT5=" ) );
  x9 = x8;
  assert ( x8 . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" ) );
  assert ( x9 . Transaction ( "111111", "987654", 789, "SGDFTYE3sdfsd3W" ) );
  assert ( x8 . NewAccount ( "99999999", 7000 ) );
  assert ( x8 . Transaction ( "111111", "99999999", 3789, "aher5asdVsAD" ) );
  assert ( x8 . TrimAccount ( "111111" ) );
  os . str ( "" );
  os << x8 . Account ( "111111" );
  assert ( ! strcmp ( os . str () . c_str (), "111111:\n   -1802\n = -1802\n" ) );
  os . str ( "" );
  os << x9 . Account ( "111111" );
  assert ( ! strcmp ( os . str () . c_str (), "111111:\n   5000\n - 2890, to: 987654, sign: Okh6e+8rAiuT5=\n - 789, to: 987654, sign: SGDFTYE3sdfsd3W\n = 1321\n" ) );

  return 0;
}
#endif /* __PROGTEST__ */
