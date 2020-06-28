#ifndef __PROGTEST__
#include <bits/stdc++.h>
using namespace std;
#endif // __PROGTEST__

typedef long long ll;
typedef long double ld;
#define pb(x) push_back(x)
#define ff first
#define ss second
#define vec vector
#define all(x) (x).begin(), (x).end()
#define inf 9223372036854775807
#define mod 998244353
#define endl '\n'


class CCell{
	public:
		const static ll ALIGN_LEFT = 0;
		const static ll ALIGN_RIGHT = 1;
		const static ll ALIGN_CENTER = 2;
		vec<string> content;
		ll al;
		CCell(ll al = 2):al(al){};
		virtual ~CCell(){};
		virtual ll geth(ll res = 0) const{ return content.size(); }
		virtual ll getw(ll res = 0) const{
			for (auto &c: content) res = max(res, (ll)c.size());
			return res;
		}
		virtual string align(string s, ll w) const{
			if (al == ALIGN_LEFT) return s + string(w - (ll)s.size(), ' ');
			if (al == ALIGN_RIGHT) return string(w - (ll)s.size(), ' ') + s;
			return string((w - (ll)s.size())/2, ' ') + s + string(w - (ll)s.size() - (w - (ll)s.size())/2, ' ');
		}
		virtual string str(ll r, ll height){
			if (al == ALIGN_CENTER) r -= (height - (ll)content.size())/2;
			return r < 0 || r >= (ll)content.size() ? string(getw(), ' ') : content[r];
		}
		virtual CCell *cpy() const{ return new CCell(*this); }
};
class CEmpty : public CCell{
	CCell *cpy () const{ return new CEmpty(*this); }
};


class CText : public CCell{
	public:
		char hahahaha;
		CText(string s, ll al):CCell(al){ SetText(s); }
		void SetText(string s){
			stringstream ss(s);
			content.clear();
			while (getline(ss, s, '\n')) content.pb(s);
		}
		CCell *cpy() const{ return new CText(*this); }
};

class CImage : public CCell{
	public:
		char hahahaha;
		CImage AddRow(string s){
			content.pb(s);
			return *this;
		}
		CCell *cpy() const{ return new CImage(*this); }
};

class CTable : public CCell{
	public:
		ll h, w;
		vec<vec<CCell*>> t;
		vec<string> st;
		CTable(ll r, ll c):CCell(0),h(r),w(c),t(h, vec<CCell*>(w)){
			for (ll i = 0; i < h; ++i)
				for (ll j = 0; j < w; ++j)
					t[i][j] = new CEmpty();
		}
		CTable(const CTable &ct){ cp(ct); }
		~CTable(){ del(); }
		CTable operator=(const CTable &ct){
			if (this == &ct) return *this;
			del(); cp(ct);
			return *this;
		}
		void cp(const CTable &ct){
			h = ct.h; w = ct.w; al = ct.al;
			t.resize(h, vec<CCell*>(w));
			for (ll i = 0; i < h; ++i)
				for (ll j = 0; j < w; ++j)
					t[i][j] = ct.t[i][j]->cpy();			
		}
		void del(){
			for (ll i = 0; i < h; ++i)
				for (ll j = 0; j < w; ++j)
					delete t[i][j];			
		}
		CCell &GetCell(ll r, ll c){ return *t[r][c]; }

		void SetCell(ll r, ll c, const CCell &cell){
			auto prev = t[r][c];
			t[r][c] = cell.cpy();
			delete prev;
		}

		ll geth(ll res = 0) const{
			for (ll r = 0, ir = 0; r < h; ++r, res += ir, ir = 0)
				for (ll c = 0; c < w; ++c)
					ir = max(ir, t[r][c]->geth());
			return res + h + 1;
		}

		ll getw(ll res = 0) const{
			for (ll c = 0, ir = 0; c < w; ++c, res += ir, ir = 0)
				for (ll r = 0; r < h; ++r)
					ir = max(ir, t[r][c]->getw());
			return res + w + 1;
		}

		void construct(){
			st.clear();
			vec<ll> rh(h, 0), rw(w, 0);
			for (ll r = 0; r < h; ++r){
				for (ll c = 0; c < w; ++c){
					rh[r] = max(rh[r], t[r][c]->geth());
					rw[c] = max(rw[c], t[r][c]->getw());
				}
			} st.pb("+");
			for (ll c = 0; c < w; ++c)
				st[0] += string(rw[c], '-') + "+";
			for (ll r = 0; r < h; ++r, st.pb(st[0]))
				for (ll rr = 0; rr < rh[r]; ++rr)
					for (ll c = 0; c < w; ++c)
						if (!c) st.pb("|" + t[r][c]->align(t[r][c]->str(rr, rh[r]), rw[c]) + "|");
						else st.back() += t[r][c]->align(t[r][c]->str(rr, rh[r]), rw[c]) + "|";
		}

		string str(ll row, ll height) {
			if (!row) construct();
			return row < (ll)st.size() ? st[row] : string(w, ' ');
		}
		
		friend ostream &operator <<(ostream &os, CTable &t){
			t.str(0, 0);
			for (auto &r: t.st)	os << r << endl;
			return os;
		}
		CCell *cpy() const{ return new CTable(*this); }
};