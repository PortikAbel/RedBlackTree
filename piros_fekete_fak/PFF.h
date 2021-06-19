#ifndef PF_FA_H
#define PF_FA_H

#include <string>
#include <queue>

enum Szin {PIROS, FEKETE};

template<class K, class T>
struct Csucs
{
	K kulcs;
	T adat;
	bool szin;
	Csucs<K, T>* szulo, * bal, * jobb;

    Csucs(K, T);
    Csucs<K, T>* nagybacsi();
    Csucs<K, T>* testver();
};

template<class K, class T>
class PFFa
{
	Csucs<K, T>* gyoker;
	void forgatBal(Csucs<K, T>*&, Csucs<K, T>*&);
	void forgatJobb(Csucs<K, T>*&, Csucs<K, T>*&);
	void egyensulyoz(Csucs<K, T>*&, Csucs<K, T>*&);
	void egyensulyozDuplaFekete(Csucs<K, T>*);
	Csucs<K, T>* hasonlit(Csucs<K, T>*, K);
	void inorder(Csucs<K, T>*, void (*f)(T));
public:
	PFFa() { gyoker = nullptr; }

	void beszur(K, const T&);
	Csucs<K, T>* keres(K);
	Csucs<K, T>* kovetkezo(Csucs<K, T>*);
	Csucs<K, T>* elozo(Csucs<K, T>*);
	Csucs<K, T>* elso();
	Csucs<K, T>* utolso();
	bool letezik(K);
	bool ures() { return gyoker == nullptr; }
    void torol(Csucs<K, T>*);
	void vegrehajtInorder(void (*f)(T)) { inorder(gyoker, f); }
};

template Csucs<int, int>;
template PFFa<int, int>;
struct Szemely
{
	std::string csaladnev, keresztnev;
	std::queue<std::string> celpontok;
	int aktualis_sorszam;
	PFFa<int, int> sorszamok;
};

template Csucs<int, Szemely>;
template PFFa<int, Szemely>;

template Csucs<std::string, int>;
template PFFa<std::string, int>;

#endif
