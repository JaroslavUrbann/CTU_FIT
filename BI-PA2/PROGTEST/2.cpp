#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST__ */


/*
	CPerson - class for storing info about tax payers
*/
class CPerson{
	public:
		CPerson (string name, string addr, string acct);
		CPerson (string name, string addr);
		CPerson (string acct);
		string name;
		string addr;
		string acct;
		int	   sumIncome = 0;
		int    sumExpense = 0;
};


CPerson::CPerson(string name, string addr, string acct)
				:name(name),
				 addr(addr),
				 acct(acct){}


CPerson::CPerson(string name, string addr)
				:name(name),
				 addr(addr){}


CPerson::CPerson(string acct)
				:acct(acct){}


/*
	CIterator - class for traversing the tax register
			  - takes in begin and end iterators
*/
class CIterator{
	public:
				CIterator 	(vector<CPerson*>::const_iterator beginIt,
							 vector<CPerson*>::const_iterator endIt);
		bool	AtEnd		() const;
		void	Next		();
		string	Name 		() const;
		string	Addr 		() const;
		string	Account 	() const;

	private:
		vector<CPerson*>::const_iterator beginIt, endIt;
};


CIterator::CIterator(vector<CPerson*>::const_iterator beginIt,
					 vector<CPerson*>::const_iterator endIt)
					:beginIt(beginIt),
					 endIt(endIt){}


bool CIterator::AtEnd() const{
	return !distance(beginIt, endIt);
}


void CIterator::Next(){
	beginIt += 1;
}


string CIterator::Name() const{
	return (*beginIt)->name;
}


string CIterator::Addr() const{
	return (*beginIt)->addr;
}


string CIterator::Account() const{
	return (*beginIt)->acct;
}


/*
	CTaxRegister 	- class for manipulations with the tax register
*/
class CTaxRegister{
	public:
		~CTaxRegister();
		bool		Birth		(const string &name,
								 const string &addr,
								 const string &acct);
		bool 		Death		(const string &name,
								 const string &addr);
		bool 		Income		(const string &acct,
								 int           amnt);
		bool		Income 		(const string &name,
								 const string &addr,
								 int		   amnt);
		bool 		Expense		(const string &acct,
						 		 int           amnt);
		bool 		Expense		(const string &name,
						 		 const string &addr,
						 		 int           amnt);
		bool 		Audit		(const string &name,
								 const string &addr,
								 string       &acct,
						 		 int          &sumIncome,
						 		 int          &sumExpense) const;
		CIterator	ListByName	() const;

	private:
		// pointers to register items, sorted by name & address
		vector<CPerson*> regByName;
		// pointers to register items, sorted by account
		vector<CPerson*> regByAcct;
		bool		FindByName	(const CPerson p,
							 	 vector<CPerson*>::const_iterator &posByName) const;
		bool		FindByAcct	(const CPerson p,
							 	 vector<CPerson*>::const_iterator &posByAcct) const;
};


// compares 2 people by name and then by address
bool CmpByName(const CPerson *a, const CPerson &b){
	if (a->name < b.name) return true;
	if (a->name > b.name) return false;
	return a->addr < b.addr;
}

// compares 2 people by account
bool CmpByAcct(const CPerson *a, const CPerson &b){
	return a->acct < b.acct;
}

// destructor, frees all CPerson objects from the register
CTaxRegister::~CTaxRegister(){
	for (auto person = regByName.begin(); person != regByName.end(); ++person)
		delete *person;
}

/*
	takes in a person and returns true there is a person with the same name and address
	in the register,
	also returns an iterator pointing to the person
*/
bool CTaxRegister::FindByName(const CPerson p,
							  vector<CPerson*>::const_iterator &posByName) const{
	posByName = lower_bound(regByName.begin(), regByName.end(), p, CmpByName);
	return posByName != regByName.end() && 
		   (*posByName)->name == p.name && 
		   (*posByName)->addr == p.addr;
}

/*
	takes in a person and returns true there is a person with the same account name
	in the register,
	also returns an iterator pointing to the person
*/
bool CTaxRegister::FindByAcct(const CPerson p,
							  vector<CPerson*>::const_iterator &posByAcct) const{
	posByAcct = lower_bound(regByAcct.begin(), regByAcct.end(), p, CmpByAcct);
	return posByAcct != regByAcct.end() &&
		   (*posByAcct)->acct == p.acct;
}

/*
	creates a new record, returns false if the person already exists
*/
bool CTaxRegister::Birth(const string &name,
					 	 const string &addr,
						 const string &acct){
	CPerson *p = new CPerson{name, addr, acct};
	vector<CPerson*>::const_iterator posByName, posByAcct;
	if (FindByName(*p, posByName) || FindByAcct(*p, posByAcct)){
		delete p;
		return false;
	}
	regByName.insert(posByName, p);
	regByAcct.insert(posByAcct, p);
	return true;
}

/*
	finds a person by name and an adress, then gets their account number
	and deletes them, their index in the vector sorted by name & address, 
	and their index int the vector sorted by the account name.
*/
bool CTaxRegister::Death(const string &name,
					 	 const string &addr){
	CPerson p(name, addr);
	vector<CPerson*>::const_iterator posByName, posByAcct;
	// gets an iterator to the array indexed by name & addresss
	if (!FindByName(p, posByName))
		return false;

	// gets the account name
	p.acct = (*posByName)->acct;
	
	// gets and iterator to the array indexed by account
	if (!FindByAcct(p, posByAcct))
		return false;
	
	// deletes all of them
	delete *posByName;
	regByName.erase(posByName);
	regByAcct.erase(posByAcct);
	return true;
}

/*
	finds a person by their account name and adds an amount to
	the sum of their income
*/
bool CTaxRegister::Income(const string &acct,
					 	  int			amnt){
	CPerson p(acct);
	vector<CPerson*>::const_iterator posByAcct;
	if (!FindByAcct(p, posByAcct))
		return false;
	(*posByAcct)->sumIncome += amnt;
	return true;
}

/*
	finds a person by their account name and adds an amount to
	the sum of their expenses
*/
bool CTaxRegister::Expense(const string &acct,
						   int 			 amnt){
	CPerson p(acct);
	vector<CPerson*>::const_iterator posByAcct;
	if (!FindByAcct(p, posByAcct))
		return false;
	(*posByAcct)->sumExpense += amnt;
	return true;
}

/*
	finds a person by their name & address and adds an amount to
	the sum of their income
*/
bool CTaxRegister::Income(const string &name,
						  const string &addr,
						  int		   amnt){
	CPerson p(name, addr);
	vector<CPerson*>::const_iterator posByName;
	if (!FindByName(p, posByName))
		return false;
	(*posByName)->sumIncome += amnt;
	return true;
}

/*
	finds a person by their name & address and adds an amount to
	the sum of their expenses
*/
bool CTaxRegister::Expense(const string &name,
						   const string &addr,
						   int			 amnt){
	CPerson p(name, addr);
	vector<CPerson*>::const_iterator posByName;
	if (!FindByName(p, posByName))
		return false;
	(*posByName)->sumExpense += amnt;
	return true;
}

/*
	finds a person by their name & address, and returns
	their personal information
*/
bool CTaxRegister::Audit(const string &name,
						 const string &addr,
						 string       &acct,
				 		 int          &sumIncome,
				 		 int          &sumExpense) const{
	CPerson p(name, addr, acct);
	vector<CPerson*>::const_iterator posByName;
	if (!FindByName(p, posByName))
		return false;
	acct = (*posByName)->acct;
	sumIncome = (*posByName)->sumIncome;
	sumExpense = (*posByName)->sumExpense;
	return true;
}

/*
	returns a CIterator object for traversing the register
*/
CIterator CTaxRegister::ListByName() const{
	return CIterator(regByName.begin(), regByName.end());
}


#ifndef __PROGTEST__
int main ( void )
{
  string acct;
  int    sumIncome, sumExpense;
  CTaxRegister b1;
  assert ( b1 . Birth ( "John Smith", "Oak Road 23", "123/456/789" ) );
  assert ( b1 . Birth ( "Jane Hacker", "Main Street 17", "Xuj5#94" ) );
  assert ( b1 . Birth ( "Peter Hacker", "Main Street 17", "634oddT" ) );
  assert ( b1 . Birth ( "John Smith", "Main Street 17", "Z343rwZ" ) );
  assert ( b1 . Income ( "Xuj5#94", 1000 ) );
  assert ( b1 . Income ( "634oddT", 2000 ) );
  assert ( b1 . Income ( "123/456/789", 3000 ) );
  assert ( b1 . Income ( "634oddT", 4000 ) );
  assert ( b1 . Income ( "Peter Hacker", "Main Street 17", 2000 ) );
  assert ( b1 . Expense ( "Jane Hacker", "Main Street 17", 2000 ) );
  assert ( b1 . Expense ( "John Smith", "Main Street 17", 500 ) );
  assert ( b1 . Expense ( "Jane Hacker", "Main Street 17", 1000 ) );
  assert ( b1 . Expense ( "Xuj5#94", 1300 ) );
  assert ( b1 . Audit ( "John Smith", "Oak Road 23", acct, sumIncome, sumExpense ) );
  assert ( acct == "123/456/789" );
  assert ( sumIncome == 3000 );
  assert ( sumExpense == 0 );
  assert ( b1 . Audit ( "Jane Hacker", "Main Street 17", acct, sumIncome, sumExpense ) );
  assert ( acct == "Xuj5#94" );
  assert ( sumIncome == 1000 );
  assert ( sumExpense == 4300 );
  assert ( b1 . Audit ( "Peter Hacker", "Main Street 17", acct, sumIncome, sumExpense ) );
  assert ( acct == "634oddT" );
  assert ( sumIncome == 8000 );
  assert ( sumExpense == 0 );
  assert ( b1 . Audit ( "John Smith", "Main Street 17", acct, sumIncome, sumExpense ) );
  assert ( acct == "Z343rwZ" );
  assert ( sumIncome == 0 );
  assert ( sumExpense == 500 );
  CIterator it = b1 . ListByName ();
  assert ( ! it . AtEnd ()
           && it . Name () == "Jane Hacker"
           && it . Addr () == "Main Street 17"
           && it . Account () == "Xuj5#94" );
  it . Next ();
  assert ( ! it . AtEnd ()
           && it . Name () == "John Smith"
           && it . Addr () == "Main Street 17"
           && it . Account () == "Z343rwZ" );
  it . Next ();
  assert ( ! it . AtEnd ()
           && it . Name () == "John Smith"
           && it . Addr () == "Oak Road 23"
           && it . Account () == "123/456/789" );
  it . Next ();
  assert ( ! it . AtEnd ()
           && it . Name () == "Peter Hacker"
           && it . Addr () == "Main Street 17"
           && it . Account () == "634oddT" );
  it . Next ();
  assert ( it . AtEnd () );

  assert ( b1 . Death ( "John Smith", "Main Street 17" ) );

  CTaxRegister b2;
  assert ( b2 . Birth ( "John Smith", "Oak Road 23", "123/456/789" ) );
  assert ( b2 . Birth ( "Jane Hacker", "Main Street 17", "Xuj5#94" ) );
  assert ( !b2 . Income ( "634oddT", 4000 ) );
  assert ( !b2 . Expense ( "John Smith", "Main Street 18", 500 ) );
  assert ( !b2 . Audit ( "John Nowak", "Second Street 23", acct, sumIncome, sumExpense ) );
  assert ( !b2 . Death ( "Peter Nowak", "5-th Avenue" ) );
  assert ( !b2 . Birth ( "Jane Hacker", "Main Street 17", "4et689A" ) );
  assert ( !b2 . Birth ( "Joe Hacker", "Elm Street 23", "Xuj5#94" ) );
  assert ( b2 . Death ( "Jane Hacker", "Main Street 17" ) );
  assert ( b2 . Birth ( "Joe Hacker", "Elm Street 23", "Xuj5#94" ) );
  assert ( b2 . Audit ( "Joe Hacker", "Elm Street 23", acct, sumIncome, sumExpense ) );
  assert ( acct == "Xuj5#94" );
  assert ( sumIncome == 0 );
  assert ( sumExpense == 0 );
  assert ( !b2 . Birth ( "Joe Hacker", "Elm Street 23", "AAj5#94" ) );

  return 0;
}
#endif /* __PROGTEST__ */
