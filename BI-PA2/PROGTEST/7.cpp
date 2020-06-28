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

template <typename _Type, typename _Comparator = equal_to<typename _Type::value_type>>
class CSearch{
	public:
		_Comparator cmp;
		vec<pair<int,_Type>> needles;
		CSearch<_Type, _Comparator> (_Comparator c = _Comparator()):cmp(c){}
		CSearch (const CSearch &src) = delete;
		CSearch &operator = (const CSearch &src) = delete;
		void Add(int id, const _Type &needle){ needles.push_back({id, needle}); }
		set<int> Search (const _Type &h) const{
			set<int> res; bool gucci = true;
			for (auto &n: needles)
				for (auto ith = h.begin(), itn = n.ss.begin(); ith != h.end(); ++ith, itn = n.ss.begin(), gucci = true)
					for (auto ithh = ith; ithh != h.end() && itn != n.ss.end() && !res.count(n.ff); ++ithh, ++itn)
						if (!(gucci &= cmp(*ithh, *itn))) break;
						else if (next(itn) == n.ss.end()) res.insert(n.ff);
			return res;
		}
};