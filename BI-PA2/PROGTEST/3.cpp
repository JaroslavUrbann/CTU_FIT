#ifndef __PROGTEST__
#include <bits/stdc++.h>
using namespace std;
#endif /* __PROGTEST__ */

typedef long long ll;
typedef long double ld;
#define pb(x) push_back(x)
#define ff first
#define ss second
#define vec vector
#define all(x) (x).begin(), (x).end()
#define inf 9223372036854775807
#define mod 998244353

class CBigInt{
	public:
		string v;
		bool p = true;
		CBigInt();
		CBigInt(int num);
		CBigInt(string num);
		CBigInt(const char *num);
		ll size() const;
		CBigInt& operator+=(const CBigInt &a);
		CBigInt& operator*=(const CBigInt &a);
		CBigInt& operator=(const CBigInt &a);
};

ll CBigInt::size() const{ return v.size(); }

void clr(CBigInt &a){
	for (ll i = a.size() - 1; i > 0 && a.v[i] == '0'; --i)
		a.v.pop_back();
	if (a.size() == 1 && a.v[0] == '0') a.p = true;
}

CBigInt::CBigInt():v("0"),p(true){}

CBigInt::CBigInt(int num):p(num>=0){
	if (num<0) v = to_string(num*-1);
	else v = to_string(num);
	reverse(all(v));
	clr(*this);
}

CBigInt::CBigInt(const char *num):CBigInt(string(num)){}

CBigInt::CBigInt(string num){
	if (!num.length()) throw std::invalid_argument( "I mean why?" );
	p = true;
	if (num[0] == '-'){
		v = num.substr(1, num.length());
		p = false;
	}
	else v = num;
	if (!v.size()) throw std::invalid_argument( "not cool man" );
	for (size_t i = 0; i < v.length(); ++i)
		if (!isdigit(v[i])) 
			throw std::invalid_argument( "why are you doing this to me?" );
	reverse(all(v));
	clr(*this);
}


bool operator==(const CBigInt &a, const CBigInt &b){
	return a.p == b.p && a.v == b.v;
}

bool operator!=(const CBigInt &a, const CBigInt &b){
	return !(a == b);
}

bool operator>(const CBigInt &a, const CBigInt &b){
	if (a.p && !b.p) return true;
	if (!a.p && b.p) return false;
	if (a.size() != b.size()) return !a.p ^ (a.size() > b.size());
	string aa = a.v;
	string bb = b.v;
	reverse(all(aa));
	reverse(all(bb));
	return !a.p ^ (aa > bb);
}

bool operator<(const CBigInt &a, const CBigInt &b){
	return !(a > b || a == b);
}

bool operator>=(const CBigInt &a, const CBigInt &b){
	return a > b || a == b;
}

bool operator<=(const CBigInt &a, const CBigInt &b){
	return a < b || a == b;
}

istream& operator>>(istream& is, CBigInt &a)
{
	is >> ws;
	char c = '+';
	bool prev_p = a.p;
	a.p = is.peek() != '-';
	if (is.peek() == '-') is >> c;
	bool f = true;
	if (isdigit(is.peek())){
		a.v = "";
		while(isdigit(is.peek())){
			is >> c;
			if (!f || c != '0'){
				a.v.pb(c);
				f = false;
			}
		}
		reverse(all(a.v));		
	}
	else {
		a.p = prev_p;
		is.setstate(ios::failbit);
	}
	return is;
}

ostream& operator<<(ostream& os, const CBigInt &a){
	if (!a.size()) return os;
	if (!a.p) os << '-';
	string s = a.v;
	reverse(all(s));
	os << s;
	return os;
}

CBigInt pls(const CBigInt &a, const CBigInt &b){
	ll c = 0, s = max(a.size(), b.size());
	CBigInt r; r.p = a.p;
	r.v = "";
	for (ll i = 0; i < s || c; ++i){
		if (i < a.size()) c += a.v[i] - '0';
		if (i < b.size()) c += b.v[i] - '0';
		r.v.pb(c % 10 + '0');
		c /= 10;
	}
	clr(r);
	return r;
}

CBigInt mns(const CBigInt &a, const CBigInt &b){
	CBigInt r; r.p = a.p;
	r.v = "";
	ll c = 0;
	for (ll i = 0; i < a.size(); ++i){
		ll val = a.v[i] - '0' - c;
		if (i < b.size()) val -= b.v[i] - '0';
		if (val < 0) c = 1;
		else c = 0;
		r.v.pb((val + 10) % 10 + '0');
	}
	clr(r);
	return r;
}

CBigInt operator+(const CBigInt &a, const CBigInt &b){
	if (a.p == b.p) return pls(a, b);
	if (a.v >= b.v) return mns(a, b);
	else return mns(b, a);
}

CBigInt operator-(const CBigInt &a, const CBigInt &b){
	CBigInt c(b.v);
	c.p = !b.p;
	return a + c;
}

CBigInt operator*(const CBigInt &a, const CBigInt &b){
	CBigInt r;
	for (ll i = 0; i < a.size(); ++i){
		CBigInt ir;
		ir.v = string(i, '0');
		ll c = 0;
		for (ll j = 0; j < b.size() || c; ++j){
			if (j < b.size()) c += (a.v[i] - '0') * (b.v[j] - '0');
			ir.v.pb(c % 10 + '0');
			c /= 10;
		}
		r += ir;
	}
	r.p = (a.p == b.p);
	clr(r);
	return r;
}

CBigInt & CBigInt::operator+=(const CBigInt &a){
	return (*this = *this + a);
}

CBigInt & CBigInt::operator*=(const CBigInt &a){
	return (*this = *this * a);
}

CBigInt & CBigInt::operator=(const CBigInt &a){
	this->p = a.p;
	this->v = a.v;
	return *this;
}


#ifndef __PROGTEST__
static bool equal ( const CBigInt & x, const char * val )
{
  ostringstream oss;
  oss << x;
  return oss . str () == val;
}
int main ( void )
{
  CBigInt a, b;
  istringstream is;
  a = 10;
  a += 20;
  assert ( equal ( a, "30" ) );
  a *= 5;
  assert ( equal ( a, "150" ) );
  b = a + 3;
  assert ( equal ( b, "153" ) );
  b = a * 7;
  assert ( equal ( b, "1050" ) );
  assert ( equal ( a, "150" ) );

  a = 10;
  a += -20;
  assert ( equal ( a, "-10" ) );
  a *= 5;
  assert ( equal ( a, "-50" ) );
  b = a + 73;
  assert ( equal ( b, "23" ) );
  b = a * -7;
  assert ( equal ( b, "350" ) );
  assert ( equal ( a, "-50" ) );

  a = "12345678901234567890";
  a += "-99999999999999999999";
  assert ( equal ( a, "-87654321098765432109" ) );
  a *= "54321987654321987654";
  assert ( equal ( a, "-4761556948575111126880627366067073182286" ) );
  a *= 0;
  assert ( equal ( a, "0" ) );
  a = 10;
  b = a + "400";
  assert ( equal ( b, "410" ) );
  b = a * "15";
  assert ( equal ( b, "150" ) );
  assert ( equal ( a, "10" ) );

  is . clear ();
  is . str ( " 1234" );
  assert ( is >> b );
  assert ( equal ( b, "1234" ) );
  is . clear ();
  is . str ( " 12 34" );
  assert ( is >> b );
  assert ( equal ( b, "12" ) );
  is . clear ();
  is . str ( "999z" );
  assert ( is >> b );
  assert ( equal ( b, "999" ) );
  is . clear ();
  is . str ( "abcd" );
  assert ( ! ( is >> b ) );
  is . clear ();
  is . str ( "- 758" );
  assert ( ! ( is >> b ) );
  try
  {
    a = "-xyz";
    assert ( "missing an exception" == NULL );
  }
  catch ( const invalid_argument & e )
  {
  }

  a = "73786976294838206464";
  assert ( a < "1361129467683753853853498429727072845824" );
  assert ( a <= "1361129467683753853853498429727072845824" );
  assert ( ! ( a > "1361129467683753853853498429727072845824" ) );
  assert ( ! ( a >= "1361129467683753853853498429727072845824" ) );
  assert ( ! ( a == "1361129467683753853853498429727072845824" ) );
  assert ( a != "1361129467683753853853498429727072845824" );
  assert ( ! ( a < "73786976294838206464" ) );
  assert ( a <= "73786976294838206464" );
  assert ( ! ( a > "73786976294838206464" ) );
  assert ( a >= "73786976294838206464" );
  assert ( a == "73786976294838206464" );
  assert ( ! ( a != "73786976294838206464" ) );
  assert ( a < "73786976294838206465" );
  assert ( a <= "73786976294838206465" );
  assert ( ! ( a > "73786976294838206465" ) );
  assert ( ! ( a >= "73786976294838206465" ) );
  assert ( ! ( a == "73786976294838206465" ) );
  assert ( a != "73786976294838206465" );
  a = "2147483648";
  assert ( ! ( a < -2147483648 ) );
  assert ( ! ( a <= -2147483648 ) );
  assert ( a > -2147483648 );
  assert ( a >= -2147483648 );
  assert ( ! ( a == -2147483648 ) );
  assert ( a != -2147483648 );

  return 0;
}
#endif /* __PROGTEST__ */
