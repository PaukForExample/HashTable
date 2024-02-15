#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <stack>
#include <string>

using namespace std;

template <class T, class C>
class hable { //solves collision by using two hash functions
protected:
	int size = 100; // size of the table
	int enlarger_coef = 75; // elements after reaching higher resize is used (75%)
	int used_elems = 0; //used elements
	struct Element {
	public:
		T key; 
		C value;
		bool ispresent=false;
		bool getstatus() { return ispresent; }
	};
	vector<Element> table;
	int digitco(long long n){
		int r = 0;
		while (n != 0) {
			r++;
			n /= 10;
		}
		return r;
	}
	void resize() {
		stack<Element> are;
		size *= 2;
		enlarger_coef *= 2;
		int i = 0, z = 0;
		while (i != used_elems) {
			if (table[z].getstatus()) {
				are.push(table[z]);
				i++;
			}
			z++;
		}
		table.resize(size);
		while (!are.empty()) {
			int prkey = proghash(are.top().key);
			table[prkey].key = prkey;
			table[prkey].value = are.top().value;
			table[prkey].ispresent = true;
			are.pop();
		}
	}
	int HashFunction1(T ky) { //midsquare method		
		T kys = ky;
		int i = 1;
		int d = digitco(size - 1);
		int r = pow(10, (2 * digitco(ky) - d) / 2);
		if (r == 0) r++;
		ky /= r;
		d = pow(10, d);
		int res = ((ky * ky) % d);
		return res;
	}
private:
	int HashFunction2(T psw) {	// folding hash
		int mod = pow(10, digitco(size - 1));
		int sum = 0;
		while (psw != 0) {
			sum += psw % mod;
			psw /= mod;
		}
		return sum % size;
	}
	int proghash(T psw) { //DON'T FORGET TO make sure that result of this function already solves collision issues
		int res = HashFunction1(psw);
		int kys = res;
		if (!table[res].getstatus()) return res;
		else {
			if (table[res].key == psw) return res;
			else {
				int i = 0;
				while ((!table[res].getstatus()) || (((table[res].getstatus()) && (table[res].key == psw)))) {
					res = res + HashFunction2(kys * i);
					i++;
				}
				return res;
			}
		}
	}

public:
	hable() {
		table.resize(100);
	}
	hable(const hable<T, C>& tale) {
		size = tale.size;
		enlarger_coef = tale.enlarger_coef;
		used_elems = tale.used_elems;
		table.resize(size);
		for (int i = 0; i != size; i++) {
			table[i].key = tale.table[i].key;
			table[i].value = tale.table[i].value;
			table[i].ispresent = tale.table[i].ispresent;
		}
	}
	~hable() {
		this->table.clear();
	}
	void add(T k, C data) {
		int keyres = proghash(k);
		table[keyres].key = k;
		table[keyres].value = data;
		table[keyres].ispresent = true;
		used_elems++;
		if (used_elems >= enlarger_coef) resize();
	}
	C find(T k) { //find by key O(1)
		if (table[proghash(k)].ispresent) return table[proghash(k)].value;
		else return NULL;
	}
	void erase(T k) {
		table[proghash(k)].ispresent = false; //erase by key O(1)
	}
	C operator[](T k) {
		if (table[proghash(k)].ispresent) return table[proghash(k)].value;
		else return NULL;
	}
	friend istream& operator>>(istream&, hable <T, C> hashtable);
	friend ostream& operator<<(ostream& out, hable<T, C> hashtable);
};

template<class T,class C>
istream& operator>> (istream &in, hable <T, C>& hashtable) {
	int k = 0, i = 0;
	in >> hashtable.used_elems;
	while (k != hashtable.used_elems) {
		if (hashtable[i] != nullptr) {
			in >> hashtable.table[i].key >> hashtable.table[i].data;
			k++;
		}
		i++;
	}
	return in;
}
template<class T, class C>
ostream& operator<<(ostream& out, hable<T, C> hashtable) {
	out << hashtable.used_elems();
	int k = 0, i = 0;
	while (k != hashtable.used_elems) {
		if (hashtable[i] != NULL) {
			out << hashtable.table[i].key << hashtable.table[i].value;
			k++;
		}
		i++;
	}
	return out;
}

template<class T, class C>
class opash :public hable<T, C> {
private:																										
	int proghash(T k) {
		int res = HashFunction1(k);
		while ((!this->table[res % size].getstatus()) || (this->table[res % size].key != k)) res++;
		return res % size;
	}
public:
	opash() { this->table.resize(100); }
	opash(const opash<T, C>& tale) :hash(hash<T, C>& tale) {
		this->size = tale.size;
		this->enlarger_coef = tale.enlarger_coef;
		this->used_elems = tale.used_elems;
		this->table.resize(this->size);
		for (int i = 0; i != this->size; i++) {
			this->table[i].key = tale.table[i].key;
			this->table[i].value = tale.table[i].value;
			this->table[i].ispresent = tale.table[i].ispresent;
		}
	}
	~opash() { this->table.clear(); }
};

template<class T, class C>
istream& operator>> (istream& in, opash <T, C>& hashtable) {
	int k = 0, i = 0;
	in >> hashtable.used_elems;
	while (k != hashtable.used_elems) {
		if (hashtable[i] != nullptr) {
			in >> hashtable.table[i].key >> hashtable.table[i].data;
			k++;
		}
		i++;
	}
	return in;
}
template<class T, class C>
ostream& operator<< (ostream& out, opash<T, C> hashtable) {
	out << hashtable.used_elems();
	int k = 0, i = 0;
	while (k != hashtable.used_elems) {
		if (hashtable[i] != nullptr) {
			out << hashtable.table[i].key << hashtable.table[i].data;
			k++;
		}
		i++;
	}
	return out;
}

template<class T, class C>
class lash :public hable<T, C> {
protected:
	struct Element {
	public:
		T key;
		C value;
		bool ispresent = false;
		bool getstatus() { return ispresent; }

	};
	vector<list<Element>> table;
	void resize() {
		stack<Element> are;
		this->size *= 2;
		this->enlarger_coef *= 2;
		int z = 0, i = 0;
		while (z != this->used_elems) {
			while (!table[i].empty()) {
				are.push(table[i].front());
				this->table[i].pop_front();
				z++;
			}
			i++;
		}
		this->table.clear();
		this->table.resize(this->size);
		while (!are.empty()) {
			table[this->HashFunction1(are.top().key)].push_back(are.top());
			are.pop();
		}
		
	}
	int proghash(T k) {
		int r = this->HashFunction1(k);
		return r;
	}
public:
	lash(){ this->table.resize(100); }
	lash(const opash<T, C>& tale) :hash(hash<T, C>& tale) {
		this->size = tale.size;
		this->enlarger_coef = tale.enlarger_coef;
		this->used_elems = tale.used_elems;
		table.resize(size);
		for (int i = 0; i != this->size; i++) {
			table[i] = tale[i];
		}
	}
	~lash() { this->table.clear(); }
	void add(T k, C data) {
		int keyres = proghash(k);
		auto quick = table[keyres].begin();
		for (auto i = this->table[keyres].begin(); i != table[keyres].end(); i++) {
			if (i->key == k) {
				i->value = data;
				return;
			}
		}
		Element a;
		a.key = k;
		a.value = data;
		table[keyres].push_back(a);
		this->used_elems++;
		if (this->used_elems >= this->enlarger_coef) resize();
	}
	C find(T k) {
		int res = this->HashFunction1(k);
		for (auto i = table[res].begin(); i != table[res].end(); i++) {
			if (i->key == k) {
				return i->value;
			}
		}
	}
	void erase(T k) {
		int res = HashFunction1(k);
		for (auto i = table[i].begin(); i != table[i].end(); i++) {
			if (*i.key == k) {
				table[i].remove(i);
			}
		}
	}
	C operator[](T k) {
		int res = HashFunction1(k);
		for (auto i = table[res].begin(); i != table[res].end(); i++) {
			if (*i.key == k) return *i.data;
		}
	}
};

template<class T, class C>
istream& operator>> (istream& in, lash <T, C>& hashtable) {
	int k = 0, i = 0;
	in >> hashtable.used_elems;
	while (k != hashtable.used_elems) {
		if (hashtable[i] != nullptr) {
			in >> hashtable.table[i].key >> hashtable.table[i].data;
			k++;
		}
		i++;
	}
	return in;
}
template<class T, class C>
ostream& operator<< (ostream& out, lash<T, C> hashtable) {
	out << hashtable.used_elems();
	int k = 0, i = 0;
	while (k != hashtable.used_elems) {
		if (hashtable[i] != nullptr) {
			out << hashtable.table[i].key << hashtable.table[i].data;
			k++;
		}
		i++;
	}
	return out;
}


