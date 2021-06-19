#include "PFF.h"
#include <iostream>
#include <fstream>

using namespace std;

ifstream in_irodak("irodak(1).in");
ifstream in_ugyfelek("ugyfelek(1).in");
ofstream out_sorrend("sorrend.out");
//ofstream out_log("log.out");

class Iroda
{
	int sorszam;	// utoljara tepett sorszam
	int ajto_szam;	// az iroda ajtajanak szama
	int ido;		// egy ugyintezes idotartama
	Szemely aktiv_ugyfel;	// az irodaban tartozkodo szemely
	PFFa<int, Szemely> varoterem;	// varoteremben levo ugyfelek
	int letszam_varoterem;
public:
	int aktiv_ugy;	// az aktualis ugyfel meg ennyi ideig lesz az irodaban
	Iroda();
	Iroda(int, int);
	void belep_ugyfel(Szemely);
	Szemely kilep_ugyfel(int);
	void tovabblepo_ugyfelek(queue<Szemely>&);
	void vissza_ugyfel(Szemely);
	Szemely vege_ugyintezes();
	void irodaba_ugyfel();
	void kiir();
};

Iroda::Iroda() { varoterem = PFFa<int, Szemely>(); }

Iroda::Iroda(int ajto, int t)
{
	ajto_szam = ajto;
	ido = t;
	sorszam = 0;
	aktiv_ugy = 0;
	varoterem = PFFa<int, Szemely>();
	letszam_varoterem = 0;
}

// x kap egy sorszamot es belep a varoterembe
void Iroda::belep_ugyfel(Szemely x)
{
	x.aktualis_sorszam = ++sorszam;		// ha belep valaki, sorszamot kap
	x.celpontok.pop();					// elerte a lista elejerol a celpontot
	varoterem.beszur(sorszam, x);		// x szemely bent lesz a varoteremben
	letszam_varoterem++;				// a varoteremben egyel tobb szemely lesz
}

// az i. sorszamu ugyfel kilep a varoterembol
Szemely Iroda::kilep_ugyfel(int i)
{
	Csucs<int, Szemely>* x = varoterem.keres(i);
	Szemely kilepo_szemely = x->adat;
	varoterem.torol(x);
	letszam_varoterem--;
	return kilepo_szemely;
}

// a varoteremben levok kozul kilepnek a folyosora akiknek van mas inteznivalojuk is
// csak a sor hatso negyedebol mennek ki ugyfelek
void Iroda::tovabblepo_ugyfelek(queue<Szemely>& folyoso)
{
	Csucs<int, Szemely>* it = varoterem.utolso();
	if (it == nullptr)	// nincs senki a varoteremben
		return;
	int n = letszam_varoterem / 2;
	for (int i = 1; i <= n; i++)
	{
		if (!it->adat.celpontok.empty())	// van meg celpontja a szemelynek
		{
			int nr = it->kulcs;				// megjegyezzuk a sorszamat
			folyoso.push(it->adat);
			it->adat.sorszamok.beszur(nr, ajto_szam);
			it = varoterem.elozo(it);
			kilep_ugyfel(nr);
		}
	}
}

// x-nek mar volt sorszama es visszajon a varoterembe
void Iroda::vissza_ugyfel(Szemely x)
{
	x.aktualis_sorszam = x.sorszamok.elso()->kulcs;
	x.sorszamok.torol(x.sorszamok.elso());		// toroljuk a legkisebb sorszamot
	varoterem.beszur(x.aktualis_sorszam, x);	// x szemely bent lesz a varoteremben a regi sorszamaval
	letszam_varoterem++;						// a varoteremben egyel tobb szemely lesz
}

// az ugyfel elhagyja az irodat, kovetkezo jon a helyebe
// visszateritjuk a szemelyt aki vegzett az irodaban
Szemely Iroda::vege_ugyintezes()
{
	Szemely vegzett = aktiv_ugyfel;
	aktiv_ugyfel = Szemely();
	return vegzett;
}

// a soron kovetkezo ugyfel az irodaba lep a varoterembol
void Iroda::irodaba_ugyfel()
{
	if (varoterem.ures())	// nem lesz ugyfel az irodaban
		return;

	Csucs<int, Szemely>* kovetkezo = varoterem.elso();	// jelenlevo legkisebb sorszamu ugyfel
	aktiv_ugyfel = kovetkezo->adat;
	aktiv_ugy = ido;
	varoterem.torol(kovetkezo);
	letszam_varoterem--;
}

//void kiir_nev(Szemely x)
//{
//	out_log << x.csaladnev << ' ' << x.keresztnev << ',';
//}
//
//void Iroda::kiir()
//{
//	kiir_nev(aktiv_ugyfel);
//	out_log << "|";
//	varoterem.vegrehajtInorder(kiir_nev);
//}

// uj ugyfel adatainak beolvasasa
Szemely uj_ugyfel()
{
	Szemely x;
	int k;	// celpont irodak szama
	string cel;
	in_ugyfelek >> x.csaladnev >> x.keresztnev >> k;
	for (int j = 0; j < k; j++)
	{
		in_ugyfelek >> cel;
		x.celpontok.push(cel);
	}
	return x;
}

// a folyoson levo emberek megkeresik a kovetkezo irodat
// minden irodaban csokken egy perccel az aktiv ugyintezes maradek ideje
// a sor hatso negyede kimegy az irodabol
void ido_inc(Iroda irodak[], const int& n, queue<Szemely>& folyoso, PFFa<string, int>& terkep, int& vegzett)
{
	// folyoson levo ugyfelek
	while (!folyoso.empty())
	{
		Szemely x = folyoso.front();
		if (x.sorszamok.ures() && x.celpontok.empty())
		{
			out_sorrend << x.csaladnev << ' ' << x.keresztnev << endl;
			vegzett++;
		}
		else
		{
			int kov_cel;
			if (x.celpontok.empty())								// ha mar huzott mindenhol sorszamot
				kov_cel = x.sorszamok.elso()->adat;					// megkeresi a legkisebb sorszamot
			else
				kov_cel = terkep.keres(x.celpontok.front())->adat;	// kulonben megy a kovetkezo tervezett irodaba
			irodak[kov_cel].belep_ugyfel(x);
		}
		folyoso.pop();
	}

	queue<Szemely> folyoso_;

	for (int i = 0; i < n; i++)
	{
		irodak[i].aktiv_ugy--;
		if (irodak[i].aktiv_ugy == 0)	// most jart le az aktiv ugyintezes, kijon az irodabol az ugyfel
		{
			Szemely x = irodak[i].vege_ugyintezes();
			irodak[i].irodaba_ugyfel();		// bekerik a kovetkezot a varoterembol
			folyoso_.push(x);			// kimegy a folyosora
		}
		else if (irodak[i].aktiv_ugy < 0)	// nincs aktiv ugyintezes
			irodak[i].irodaba_ugyfel();		// bekerik a kovetkezot a varoterembol

		irodak[i].tovabblepo_ugyfelek(folyoso_);
	}

	folyoso = folyoso_;
}

int main()
{
	// irodak beolvasasa
	int db_iroda;
	in_irodak >> db_iroda;
	Iroda* irodahaz = new Iroda[db_iroda];		// az irodak tombje ajto szam szerinti sorrendben 
	PFFa<string, int> terkep;					// iroda nevenek sorszamot megfelelteto halmaz
	string nev;
	queue<Szemely> folyoso;

	int ido;
	for (int i = 0; i < db_iroda; i++)
	{
		in_irodak >> nev >> ido;
		irodahaz[i] = Iroda(i, ido);
		terkep.beszur(nev, i);
	}

	// szemelyek es celpontjaik beolvasasa
	// minden szemely bemegy a folyosora
	int db_ugyfel;
	in_ugyfelek >> db_ugyfel;
	for (int i = 0; i < db_ugyfel; i++)
	{
		Szemely x = uj_ugyfel();
		folyoso.push(x);	// az uj ugyfel belep a folyosora
	}

	int db_vegzett = 0;	// ugyfelek szama, akik elvegeztek az ugyeiket
	int inc = 0;
	while (db_vegzett < db_ugyfel)
	{
		ido_inc(irodahaz, db_iroda, folyoso, terkep, db_vegzett);

		//out_log << inc++ << endl;
		//Csucs<string, int>* iroda = terkep.elso();
		//while (iroda != nullptr)
		//{
		//	out_log << iroda->kulcs << ": ";
		//	irodahaz[iroda->adat].kiir();
		//	out_log << endl;
		//	iroda = terkep.kovetkezo(iroda);
		//}
		//out_log << "a folyoson " << folyoso.size() << "ember van." << endl;
		//out_log << "-----------------------------------------" << endl;
	}
}