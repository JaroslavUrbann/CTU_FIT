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

struct node{
	node *c;
	bool isLeaf;
	uint8_t val[4];
};

struct readBit{
	ifstream f;
	char buffer;
	int buffer_pos;
	readBit(const char *fname):f(fname, ios::binary | ios::in),buffer_pos(-1){}
	bool rb(uint8_t &b){
		if (!f) return false;
		if (buffer_pos < 0){
			if (!f.read(&buffer, sizeof(buffer))) return false;
		 	buffer_pos = 7;
		}
		b = (buffer >> buffer_pos--) & 1;
		return true;
	}
	bool readAscii(uint8_t *val){
		int n_segments = 0;
		int flgbit = 7;
		do{
			if (n_segments > 3) return false;
			val[n_segments] = 0;
			for (ll i = 7; i >= 0; --i){
				uint8_t bit;
				if (!rb(bit)) return false;
				val[n_segments] |= (bit << i);
			}
			flgbit -= (n_segments == 1);
			n_segments++;
		} while(val[0] >> flgbit-- & 1);
		for (ll i = 1; i < n_segments; ++i){
			if (!(val[0] >> (7 - i))) return false;
			if (!(val[i] >> 7) || val[i] >> 6 & 1) return false;
		}
		return true;
	}
};

struct reader{
	readBit rb;
	node n;
	vector<uint8_t*> txt;
	reader(const char *fname):rb(fname),n({nullptr, true, 0}){}
	#ifndef __PROGTEST__
	~reader(){freeTree(&n);}
	#endif /* __PROGTEST__ */

	void freeTree(node *nod){
		if (nod->isLeaf) return;
		freeTree(&nod->c[0]);
		freeTree(&nod->c[1]);
		delete [] nod->c;
	}

	bool writeTxt(const char *fname){
		ofstream outf(fname, ios::binary);
		if (!outf) return false;
		for (ll i = 0; i < (ll)txt.size(); ++i){
			int n_segments = 0;
			int flgbit = 7;
			do{
				if (!outf.write((const char *)&txt[i][n_segments], sizeof(uint8_t))) return false;
				flgbit -= (n_segments == 1);
				n_segments++;
			} while(txt[i][0] >> flgbit-- & 1);
		}
		return true;
	}

	bool readHeaderRec(node &nod){
		uint8_t b;
		if (!rb.rb(b)) return false;
		nod.isLeaf = true;
		if (b) return rb.readAscii(nod.val);
		nod.c = new node [2];
		nod.isLeaf = false;
		return readHeaderRec(nod.c[0]) && readHeaderRec(nod.c[1]);
	}

	bool getChunkSize(int &chunkSize){
		chunkSize = 0;
		for (ll i = 0; i < 12; ++i){
			uint8_t b;
			if (!rb.rb(b)) return false;
			chunkSize <<= 1;
			chunkSize |= b;
		}
		return true;
	}

	bool readChar(){
		node *par = &n;
		while (!par->isLeaf){
			uint8_t b;
			if (!rb.rb(b)) return false;
			par = &par->c[b];
		}
		txt.pb(par->val);
		return true;
	}

	bool readChunk(int chunkSize){
		for (int i = 0; i < chunkSize; ++i)
			if (!readChar()) return false;
		return true;
	}

	bool readChunks(){
		uint8_t chunkFlg;
		do{
			int chunkSize = 4096;
			if (!rb.rb(chunkFlg)) return false;
			if (!chunkFlg && !getChunkSize(chunkSize)) return false;
			if (!readChunk(chunkSize)) return false;
		} while (chunkFlg);
		return true;
	}
};

bool decompressFile ( const char * inFileName, const char * outFileName ){
	reader r(inFileName);
	return r.readHeaderRec(r.n) && r.readChunks() && r.writeTxt(outFileName);
}

bool compressFile ( const char * inFileName, const char * outFileName ){
	return false;
}

#ifndef __PROGTEST__
bool identicalFiles ( const char * fileName1, const char * fileName2 ){
	ifstream f1(fileName1), f2(fileName2);
	char c1, c2;
	while (f1 >> noskipws >> c1 && f2 >> noskipws >> c2)
		if (c1 != c2) return false;
	return true;
}

int main ( void )
{
	// char st[100];
	// cin >> st;
	// cout << decompressFile(st, "tempfile") << endl;
	assert ( decompressFile ( "tests/test0.huf", "tempfile" ) );
	assert ( identicalFiles ( "tests/test0.orig", "tempfile" ) );

	assert ( decompressFile ( "tests/test1.huf", "tempfile" ) );
	assert ( identicalFiles ( "tests/test1.orig", "tempfile" ) );

	assert ( decompressFile ( "tests/test2.huf", "tempfile" ) );
	assert ( identicalFiles ( "tests/test2.orig", "tempfile" ) );

	assert ( decompressFile ( "tests/test3.huf", "tempfile" ) );
	assert ( identicalFiles ( "tests/test3.orig", "tempfile" ) );

	assert ( decompressFile ( "tests/test4.huf", "tempfile" ) );
	assert ( identicalFiles ( "tests/test4.orig", "tempfile" ) );

	assert ( ! decompressFile ( "tests/test5.huf", "tempfile" ) );
	assert ( decompressFile ( "tests/extra0.huf", "tempfile" ) );
	assert ( identicalFiles ( "tests/extra0.orig", "tempfile" ) );

	assert ( decompressFile ( "tests/extra1.huf", "tempfile" ) );
	assert ( identicalFiles ( "tests/extra1.orig", "tempfile" ) );

	assert ( decompressFile ( "tests/extra2.huf", "tempfile" ) );
	assert ( identicalFiles ( "tests/extra2.orig", "tempfile" ) );

	assert ( decompressFile ( "tests/extra3.huf", "tempfile" ) );
	assert ( identicalFiles ( "tests/extra3.orig", "tempfile" ) );

	assert ( decompressFile ( "tests/extra4.huf", "tempfile" ) );
	assert ( identicalFiles ( "tests/extra4.orig", "tempfile" ) );

	assert ( decompressFile ( "tests/extra5.huf", "tempfile" ) );
	assert ( identicalFiles ( "tests/extra5.orig", "tempfile" ) );

	assert ( decompressFile ( "tests/extra6.huf", "tempfile" ) );
	assert ( identicalFiles ( "tests/extra6.orig", "tempfile" ) );

	assert ( decompressFile ( "tests/extra7.huf", "tempfile" ) );
	assert ( identicalFiles ( "tests/extra7.orig", "tempfile" ) );

	assert ( decompressFile ( "tests/extra8.huf", "tempfile" ) );
	assert ( identicalFiles ( "tests/extra8.orig", "tempfile" ) );

	assert ( decompressFile ( "tests/extra9.huf", "tempfile" ) );
	assert ( identicalFiles ( "tests/extra9.orig", "tempfile" ) );
}
#endif /* __PROGTEST__ */