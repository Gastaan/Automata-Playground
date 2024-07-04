// <default template>
// "#"pragma GCC optimize("Ofast,unroll-loops")
// "#"pragma comment(linker, "/stack:200000000")
// "#"pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,avx2,fma,tune=native")

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

typedef long long ll;
typedef long double ld;
typedef pair <ll, ll> pll;

#ifdef SINA
#define dbg(...) __f"#"__VA_ARGS__, __VA_ARGS__)
template <typename Arg1> void __f(const char* name, Arg1&& arg1) { cout << name << " : " << arg1 << std::endl; }
template <typename Arg1, typename... Args> void __f (const char* names, Arg1&& arg1, Args&&... args) {
    const char* comma = strchr(names + 1, ',');cout.write(names, comma - names) << " : " << arg1<<" | ";__f(comma+1, args...); }
#define dbg2(x, j, n) cout<< "#"x << " : "; output((j, n), x, 1); cout.flush();
#else
#define dbg(...) 0
#define dbg2(x, j, n) 0
#endif
#define SZ(x) ((ll)((x).size()))
#define File(s, t) freopen(s ".txt", "r", stdin); freopen(t ".txt", "w", stdout);
#define input(j, n, a) for (int _i = (j); _i < (n)+(j); _i++) cin>> a[_i];
#define output(j, n, a, t) for (int _i = (j); _i < (n)+(j); _i++) cout<< a[_i] << (((t) && _i != (n)+(j)-1)? ' ' : '\n');
#define kill(x) return cout<< (x) << end"l", 0
#define cl const ll
#define fr var
#define sc second
#define lc (v << 1)
#define rc (lc | 1)
#define mid ((l + r) >> 1)
#define All(x) (x).begin(, x).end()

cl inf = sizeof(ll) == 4 ? (1e9 + 10) : (3e18), mod = 1e9 + 7, MOD = 998244353;
// </default template>

cl N = 1e5 + 7;

struct Tape {
	string arr [N];
	int index;

	Tape () {
		index = 1;
		fill_n(arr, N, "#");
	}

	void print() {
		for (int i = 0; i < 20; i++) {
			cout<< arr[i];
		}
		cout<< "  " << index << "\n";
	}

	void move (const string &mv) {
		if (mv == "R") index++;
		else if (mv == "L") index--;
	}

	bool match (const string &s) {
		return (s == "<x>" || s == "<y>" || s == "<z>" || s == arr[index]);
	}

	void write (const string &s) {
		if (s == "<x>" || s == "<y>" || s == "<z>") return;
		arr[index] = s;
	}

	string& operator [] (const int &x) { return this->arr[x]; }
};

struct transition {
	string current_state;
	string c1, c2, c3;
	string next_state;
	string nc1, nc2, nc3;
	string mv1, mv2, mv3;
};

vector <transition> transitions;

inline void load_transitions () {
	// <find $>
	transitions.push_back({"Q0", "a", "#", "#", "Q0", "a", "#", "#", "R", "S", "S"});
	transitions.push_back({"Q0", "b", "#", "#", "Q0", "b", "#", "#", "R", "S", "S"});
	transitions.push_back({"Q0", "$", "#", "#", "Q1", "#", "#", "#", "R", "S", "S"});


	// <copy second string to second tape>
	transitions.push_back({"Q1", "a", "#", "#", "Q1", "#", "a", "#", "R", "R", "S"});
	transitions.push_back({"Q1", "b", "#", "#", "Q1", "#", "b", "#", "R", "R", "S"});
	transitions.push_back({"Q1", "#", "#", "#", "Q2", "#", "#", "#", "L", "L", "S"});


	// <bring back first tape curser to end>
	transitions.push_back({"Q2", "#", "<x>", "#", "Q2", "#", "<x>", "#", "L", "S", "S"});
	transitions.push_back({"Q2", "a", "<x>", "#", "Q3", "a", "<x>", "#", "S", "S", "S"});
	transitions.push_back({"Q2", "b", "<x>", "#", "Q3", "b", "<x>", "#", "S", "S", "S"});


	// <bring back first tape curser to begin>
	transitions.push_back({"Q3", "a", "<y>", "<z>", "Q3", "a", "<y>", "<z>", "L", "S", "S"});
	transitions.push_back({"Q3", "b", "<y>", "<z>", "Q3", "b", "<y>", "<z>", "L", "S", "S"});
	transitions.push_back({"Q3", "#", "<y>", "<z>", "Q4", "#", "<y>", "<z>", "R", "S", "S"});


	// <bring back second tape curser to begin>
	transitions.push_back({"Q4", "<x>", "a", "<z>", "Q4", "<x>", "a", "<z>", "S", "L", "S"});
	transitions.push_back({"Q4", "<x>", "b", "<z>", "Q4", "<x>", "b", "<z>", "S", "L", "S"});
	transitions.push_back({"Q4", "<x>", "#", "<z>", "Q5", "<x>", "#", "<z>", "S", "R", "S"});


	// <main function, consider first and second curser be the beginning of 2 substring, calculate LCS. We have 4 tape terminal char:
	// 	p = 'aa', q = 'ab', r = 'ba', s = 'bb'>
	transitions.push_back({"Q5", "a", "a", "a", "Q5", "a", "A", "p", "R", "R", "R"});
	transitions.push_back({"Q5", "b", "b", "a", "Q5", "b", "B", "q", "R", "R", "R"});

	transitions.push_back({"Q5", "a", "a", "b", "Q5", "a", "A", "r", "R", "R", "R"});
	transitions.push_back({"Q5", "b", "b", "b", "Q5", "b", "B", "s", "R", "R", "R"});

	transitions.push_back({"Q5", "a", "a", "#", "Q5.5", "a", "a", "#", "S", "S", "S"});
	transitions.push_back({"Q5", "b", "b", "#", "Q5.5", "b", "b", "#", "S", "S", "S"});

	transitions.push_back({"Q5", "a", "b", "<z>", "Q6", "a", "b", "<z>", "S", "L", "L"});
	transitions.push_back({"Q5", "b", "a", "<z>", "Q6", "b", "a", "<z>", "S", "L", "L"});
	transitions.push_back({"Q5", "<x>", "#", "<z>", "Q6", "<x>", "#", "<z>", "S", "L", "L"});
	transitions.push_back({"Q5", "#", "<y>", "<z>", "F1", "#", "<y>", "<z>", "S", "S", "L"});


	// <also main function, but found a longer common substring. We have to restore the answer latter>
	transitions.push_back({"Q5.5", "a", "a", "#", "Q5.5", "a", "A", "p", "R", "R", "R"});
	transitions.push_back({"Q5.5", "b", "b", "#", "Q5.5", "b", "B", "q", "R", "R", "R"});

	transitions.push_back({"Q5.5", "a", "b", "<z>", "Q6.5", "a", "b", "<z>", "S", "L", "L"});
	transitions.push_back({"Q5.5", "b", "a", "<z>", "Q6.5", "b", "a", "<z>", "S", "L", "L"});
	transitions.push_back({"Q5.5", "<x>", "#", "<z>", "Q6.5", "<x>", "#", "<z>", "S", "L", "L"});
	transitions.push_back({"Q5.5", "#", "<y>", "<z>", "F2", "#", "<y>", "<z>", "S", "S", "L"});


	// <bring back third tape curser to beginning and restore previous answer>
	transitions.push_back({"Q6", "<x>", "<y>", "p", "Q6", "<x>", "<y>", "a", "S", "S", "L"});
	transitions.push_back({"Q6", "<x>", "<y>", "q", "Q6", "<x>", "<y>", "a", "S", "S", "L"});
	transitions.push_back({"Q6", "<x>", "<y>", "r", "Q6", "<x>", "<y>", "b", "S", "S", "L"});
	transitions.push_back({"Q6", "<x>", "<y>", "s", "Q6", "<x>", "<y>", "b", "S", "S", "L"});
	transitions.push_back({"Q6", "<x>", "<y>", "#", "Q7", "<x>", "<y>", "#", "S", "S", "R"});


	// <bring back third tape curser to beginning and restore new answer>
	transitions.push_back({"Q6.5", "<x>", "<y>", "p", "Q6.5", "<x>", "<y>", "a", "S", "S", "L"});
	transitions.push_back({"Q6.5", "<x>", "<y>", "q", "Q6.5", "<x>", "<y>", "b", "S", "S", "L"});
	transitions.push_back({"Q6.5", "<x>", "<y>", "r", "Q6.5", "<x>", "<y>", "a", "S", "S", "L"});
	transitions.push_back({"Q6.5", "<x>", "<y>", "s", "Q6.5", "<x>", "<y>", "b", "S", "S", "L"});
	transitions.push_back({"Q6.5", "<x>", "<y>", "#", "Q7", "<x>", "<y>", "#", "S", "S", "R"});


	// <turn back second tape "A" -> a , B -> b)>
	transitions.push_back({"Q7", "<x>", "A", "<z>", "Q7", "<x>", "a", "<z>", "S", "L", "S"});
	transitions.push_back({"Q7", "<x>", "B", "<z>", "Q7", "<x>", "b", "<z>", "S", "L", "S"});
	transitions.push_back({"Q7", "<x>", "a", "<z>", "Q8", "<x>", "a", "<z>", "S", "R", "S"});
	transitions.push_back({"Q7", "<x>", "b", "<z>", "Q8", "<x>", "b", "<z>", "S", "R", "S"});
	transitions.push_back({"Q7", "<x>", "#", "<z>", "Q8", "<x>", "#", "<z>", "S", "R", "S"});


	// <move second tape curser to right to test next starting position>
	transitions.push_back({"Q8", "<x>", "<y>", "<z>", "Q9", "<x>", "<y>", "<z>", "S", "R", "S"});


	// <bring back first tape curser to beginning>
	transitions.push_back({"Q9", "a", "<y>", "<z>", "Q9", "a", "<y>", "<z>", "L", "S", "S"});
	transitions.push_back({"Q9", "b", "<y>", "<z>", "Q9", "b", "<y>", "<z>", "L", "S", "S"});
	transitions.push_back({"Q9", "#", "<y>", "<z>", "Q10", "#", "<y>", "<z>", "R", "S", "S"});


	// <if second tape curser is at end, then try next first tape starting position, else continue at Q5>
	transitions.push_back({"Q10", "<x>", "#", "<z>", "Q11", "<x>", "#", "<z>", "S", "L", "S"});
	transitions.push_back({"Q10", "<x>", "a", "<z>", "Q5", "<x>", "a", "<z>", "S", "S", "S"});
	transitions.push_back({"Q10", "<x>", "b", "<z>", "Q5", "<x>", "b", "<z>", "S", "S", "S"});


	// <move first tape starting position to right the go to "Q4">
	transitions.push_back({"Q11", "<x>", "<y>", "<z>", "Q4", "#", "<y>", "<z>", "R", "S", "S"});


	// <restore previous answer and finish>
	transitions.push_back({"F1", "<x>", "<y>", "p", "F1", "<x>", "<y>", "a", "S", "S", "L"});
	transitions.push_back({"F1", "<x>", "<y>", "q", "F1", "<x>", "<y>", "a", "S", "S", "L"});
	transitions.push_back({"F1", "<x>", "<y>", "r", "F1", "<x>", "<y>", "b", "S", "S", "L"});
	transitions.push_back({"F1", "<x>", "<y>", "s", "F1", "<x>", "<y>", "b", "S", "S", "L"});
	transitions.push_back({"F1", "<x>", "<y>", "#", "F", "<x>", "<y>", "#", "S", "S", "R"});


	// <restore new answer and finish>
	transitions.push_back({"F2", "<x>", "<y>", "p", "F2", "<x>", "<y>", "a", "S", "S", "L"});
	transitions.push_back({"F2", "<x>", "<y>", "q", "F2", "<x>", "<y>", "b", "S", "S", "L"});
	transitions.push_back({"F2", "<x>", "<y>", "r", "F2", "<x>", "<y>", "a", "S", "S", "L"});
	transitions.push_back({"F2", "<x>", "<y>", "s", "F2", "<x>", "<y>", "b", "S", "S", "L"});
	transitions.push_back({"F2", "<x>", "<y>", "#", "F", "<x>", "<y>", "#", "S", "S", "R"});
}

struct Turing {
	string state;
	Tape tape1, tape2, tape3;

	Turing() {
		state = "Q0";
	}

	void insert(const string &s) {
		for (int i = 0; i < SZ(s); i++)	tape1[1 + i] = s[i];
	}
} T;

inline void print() {
	cout<< "\n";
	cout<< T.state << '\n';
	T.tape1.print();
	T.tape2.print();
	T.tape3.print();
	cout<< "\n";
}

string s;

int main () {
    ios::sync_with_stdio(0); cin.tie(0);

	load_transitions();

	print();

	cout<< "enter input (s1$s2): ";
	cin>> s;
	T.insert(s);

	print();

	while (T.state != "F") {
		for (auto x : transitions) if (T.state == x.current_state) {
			if (T.tape1.match(x.c1) && T.tape2.match(x.c2) && T.tape3.match(x.c3)) {
				T.state = x.next_state;
				T.tape1.write(x.nc1);
				T.tape2.write(x.nc2);
				T.tape3.write(x.nc3);
				T.tape1.move(x.mv1);
				T.tape2.move(x.mv2);
				T.tape3.move(x.mv3);
				break;
			}
		}
		print();
	}

	cout<< "answer= ";
	T.tape3.print();

    cerr<< "\nTime elapsed: " << 1000 * clock() / CLOCKS_PER_SEC << "ms\t" __TIMESTAMP__ "\n";

    return 0;
}
/*

*/