#include <bits/stdc++.h>
#define LL long long
#define pb push_back
using namespace std;

LL gcd(LL a, LL b) {
	if(a == 0) return b;
	return gcd(b%a, a);
}
struct pecahan {
	LL pembilang, penyebut;
	void simplify() {
		if(penyebut < 0) {
			pembilang *= -1;
			penyebut *= -1;
		}
		LL fpb = gcd(pembilang * (pembilang < 0 ? -1 : 1), penyebut);
		pembilang /= fpb;
		penyebut /= fpb;
	}
	pecahan(LL a = 0, LL b = 1) {
		pembilang = a;
		penyebut = b;
		simplify();
	}
};
string to_string(pecahan p) {
	string ret = to_string(p.pembilang);
	if(p.penyebut != 1) ret += "/" + to_string(p.penyebut);
	return ret;
}
ostream& operator<<(ostream& os, pecahan p) {
	os << to_string(p);
	return os;
}
istream& operator>>(istream& is, pecahan &p) {
	string tmp;
	is >> tmp;
	for(int i = 0; i < tmp.size(); i++) {
		if(tmp[i] == '/') {
			p = pecahan(stoll(tmp.substr(0, i)), stoll(tmp.substr(i+1)));
			return is;
		}
	}
	p = pecahan(stoll(tmp));
	return is;
}
pecahan operator*(pecahan a, pecahan b) {
	return pecahan(a.pembilang * b.pembilang, a.penyebut * b.penyebut);
}
pecahan operator/(pecahan a, pecahan b) {
	return pecahan(a * pecahan(b.penyebut, b.pembilang));
}
pecahan operator+(pecahan a, pecahan b) {
	LL fpb = gcd(a.penyebut, b.penyebut);
	a.pembilang = a.pembilang * b.penyebut / fpb;
	b.pembilang = b.pembilang * a.penyebut / fpb;
	return pecahan(a.pembilang + b.pembilang, a.penyebut * b.penyebut / fpb);
}
pecahan operator-(pecahan a, pecahan b) {
	return pecahan(a + (b * -1));
}
bool operator==(pecahan a, pecahan b) {
	return (a.pembilang == b.pembilang && a.penyebut == b.penyebut);
}

typedef vector<vector<pecahan>> matrix;
matrix A, L, U;
int N;
string masukan;
vector<string> ans;

void clear() {
	system("cls");
	cout << "================================================\n";
	cout << "                                                \n";
	cout << "  Program Kalkulator Matriks: LU Decomposition  \n";
	cout << "                                                \n";
	cout << "================================================\n";
	cout << "                                                \n";
}
bool input_matrix() {
	A.clear();
	masukan = "";
	string tmp = "";
	while(tmp.back() != ']') {
		cin >> tmp;
		masukan += tmp;
	}
	if(masukan[0] != '[' || masukan.back() != ']') return 0;
	A.pb({});
	tmp = "";
	for(auto i: masukan) {
		if(i == ',' || i == ']') {
			A.back().pb(stoll(tmp));
			tmp = "";
		} else if(i == ';') {
			A.back().pb(stoll(tmp));
			tmp = "";
			A.pb({});
		} else if(i != '[') {
			tmp += i;
		}
	}
	for(int i = 0; i < A.size(); i++) {
		if(A[i].size() != A[0].size()) return 0;
	}
	if(A.size() != A[0].size()) return 0;
	N = A.size();
	return 1;
}
string fix(string a, int width) {
	while(a.size() < width) {
		a = ' ' + a;
	}
	return a;
}
void add_ans(matrix &a) {
	int width = 0;
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			width = max(width, (int)to_string(a[i][j]).size());
		}
	}
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			ans[i] += fix(to_string(a[i][j]), width);
			if(j < N-1) ans[i] += ' ';
		}
	}
	return;
}
void add_mid(string s) {
	string tmp = fix("", s.size());
	for(int i = 0; i < N; i++) {
		if(i == (N-1)/2) ans[i] += s;
		else ans[i] += tmp;
	}
	return;
}

void solve() {
	U = A;
	L.resize(N);
	for(int i = 0; i < N; i++) {
		L[i].resize(N);
		for(int j = 0; j < N; j++) {
			L[i][j] = (i==j ? 1 : 0);
		}
	}
	for(int i = 0; i < N; i++) {
		if(U[i][i] == 0) {
			cout << "Matriks A tidak bisa dibuat dalam bentuk A = LU, tetapi mungkin bisa dibuat dalam bentuk PA = LU\n";
			return;
		}
		for(int j = i+1; j < N; j++) {
			L[j][i] = U[j][i] / U[i][i];
			for(int k = 0; k < N; k++) {
				U[j][k] = U[j][k] - L[j][i] * U[i][k];
			}
		}
	}
	ans.resize(N);
	add_ans(A);
	add_mid("  =  ");
	add_ans(L);
	add_mid("  x  ");
	add_ans(U);
	for(auto i: ans) {
		cout << i << "\n";
	}
	return;
}

int main() {
	clear();
	while(1) {
		cout << "Pilih mode masukan matriks:\n";
		cout << "1. Dengan format string \'[a,b,c;d,e,f;g,h,i]\'\n";
		cout << "2. Dengan format array \'a b c\'\n";
		cout << "                        d e f \n";
		cout << "                        g h i \n";
		cout << "\n";
		cout << "> ";
		cin >> masukan;
		if(masukan == "1") {
			clear();
			cout << "Masukkan string matriks dengan format \'[a,b,c;d,e,f;g,h,i]\':\n";
			cout << "> ";
			if(input_matrix()) {
				solve();
				break;
			} else {
				clear();
				cout << "Error, string matriks \'" << masukan << "\'tidak sesuai dengan format\n\n";
			}
		} else if(masukan == "2") {
			clear();
			cout << "Masukkan ukuran matriks:\n";
			cout << "> ";
			cin >> N;
			cout << "\n";
			cout << "Masukkan matriks " << N << " x " << N << ":\n";
			A.resize(N);
			for(int i = 0; i < N; i++) {
				A[i].resize(N);
				for(int j = 0; j < N; j++) {
					cin >> A[i][j];
				}
			}
			solve();
			break;
		} else {
			clear();
			cout << "Error, mode masukan matriks hanya berupa nilai \'1\' atau \'2\'\n\n";
		}
	}
}