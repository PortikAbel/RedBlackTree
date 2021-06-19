#include "PFF.h"

/*  konstruktor
    k kulcsertekkel es x adattal */
template<class K, class T>
Csucs<K, T>::Csucs(K k, T x)
{
    this->kulcs = k;
    this->adat = x;
    bal = jobb = szulo = nullptr;
    szin = PIROS;
}

// mutatot teriti vissza a csucs nagybatyjara 
template<class K, class T>
Csucs<K, T>* Csucs<K, T>::nagybacsi()
{
    if (szulo == nullptr || szulo->szulo == nullptr)
        return nullptr;
    else
    {
        if (szulo->szulo->bal == szulo)
            return szulo->szulo->jobb;
        else
            return szulo->szulo->bal;
    }
}

// teriti vissza a csucs testverere
template<class K, class T>
Csucs<K, T>* Csucs<K, T>::testver()
{
    if (szulo == nullptr)
        return nullptr;
    else
    {
        if (szulo->bal == this)
            return szulo->jobb;
        else
            return szulo->bal;
    }
}

// a es b csucsok szinenek csereje
template<class K, class T>
void csere_szin(Csucs<K, T>* a, Csucs<K, T>* b) { bool temp = a->szin; a->szin = b->szin; b->szin = temp; }
// a es b csucsok kulcsertekenek csereje
template<class K, class T>
void csere_kulcs(Csucs<K, T>* a, Csucs<K, T>* b) { K temp = a->kulcs; a->kulcs = b->kulcs; b->kulcs = temp; }
// a es b csucsok adattagjanak csereje
template<class K, class T>
void csere_adat(Csucs<K, T>* a, Csucs<K, T>* b) { T temp = a->adat; a->adat = b->adat; b->adat = temp; }

// balra forgatas pt csucs korul
template<class K, class T>
void PFFa<K, T>::forgatBal(Csucs<K, T>*& gyoker, Csucs<K, T>*& pt)
{
    Csucs<K,T>* pt_jobb = pt->jobb;

    pt->jobb = pt_jobb->bal;

    if (pt->jobb != nullptr)
        pt->jobb->szulo = pt;

    pt_jobb->szulo = pt->szulo;

    if (pt->szulo == nullptr)
        gyoker = pt_jobb;

    else if (pt == pt->szulo->bal)
        pt->szulo->bal = pt_jobb;

    else
        pt->szulo->jobb = pt_jobb;

    pt_jobb->bal = pt;
    pt->szulo = pt_jobb;
}

// jobbra forgatas pt csucs korul
template<class K, class T>
void PFFa<K, T>::forgatJobb(Csucs<K, T>*& gyoker, Csucs<K, T>*& pt)
{
    Csucs<K, T>* pt_bal = pt->bal;

    pt->bal = pt_bal->jobb;

    if (pt->bal != nullptr)
        pt->bal->szulo = pt;

    pt_bal->szulo = pt->szulo;

    if (pt->szulo == nullptr)
        gyoker = pt_bal;

    else if (pt == pt->szulo->bal)
        pt->szulo->bal = pt_bal;

    else
        pt->szulo->jobb = pt_bal;

    pt_bal->jobb = pt;
    pt->szulo = pt_bal;
}

/*  Beszuraskor az uj elem szulojet feketere kell valtoztatni,
    megtartva a piros-fekete fa egyensulyat */
template<class K, class T>
void PFFa<K, T>::egyensulyoz(Csucs<K, T>*& gyoker, Csucs<K, T>*& pt)
{
    Csucs<K, T>* szulo_pt = nullptr;
    Csucs<K, T>* nagyszulo_pt = nullptr;

    while ((pt != gyoker) && (pt->szin != FEKETE) &&
        (pt->szulo->szin == PIROS))
    {

        szulo_pt = pt->szulo;
        nagyszulo_pt = pt->szulo->szulo;

        /*  1. Eset:
            pt szuloje a baloldali gyereke pt nagyszulojenek. */
        if (szulo_pt == nagyszulo_pt->bal)
        {

            Csucs<K, T>* nagybacsi_pt = nagyszulo_pt->jobb;

            /* 1.1. Eset:
               pt nagybatyja is piros
               Elegseges ujraszinezni */
            if (nagybacsi_pt != nullptr && nagybacsi_pt->szin == PIROS)
            {
                nagyszulo_pt->szin = PIROS;
                szulo_pt->szin = FEKETE;
                nagybacsi_pt->szin = FEKETE;
                pt = nagyszulo_pt;
            }

            else
            {
                /* 1.2. Eset:
                   pt jobboldali gyereke a szulojenek
                   balra forgatas szukseges */
                if (pt == szulo_pt->jobb)
                {
                    forgatBal(gyoker, szulo_pt);
                    pt = szulo_pt;
                    szulo_pt = pt->szulo;
                }

                /* 1.3. Eset:
                   pt baloldali gyereke a szulojenek
                   jobbra forgatas szukseges */
                forgatJobb(gyoker, nagyszulo_pt);
                csere_szin(szulo_pt, nagyszulo_pt);
                pt = szulo_pt;
            }
        }

        /* 2. Eset:
           pt szuloje a jobboldali gyereke pt nagyszulojenek. */
        else
        {
            Csucs<K, T>* nagybacsi_pt = nagyszulo_pt->bal;

            /*  2.1. Eset:
               pt nagybatyja is piros
               Elegseges ujraszinezni */
            if ((nagybacsi_pt != nullptr) && (nagybacsi_pt->szin == PIROS))
            {
                nagyszulo_pt->szin = PIROS;
                szulo_pt->szin = FEKETE;
                nagybacsi_pt->szin = FEKETE;
                pt = nagyszulo_pt;
            }
            else
            {
                /* 2.2. Eset:
                   pt baloldali gyereke a szulojenek
                   jobbra forgatas szukseges */
                if (pt == szulo_pt->bal)
                {
                    forgatJobb(gyoker, szulo_pt);
                    pt = szulo_pt;
                    szulo_pt = pt->szulo;
                }

                /* 2.3. Eset:
                   pt jobboldali gyereke a szulojenek
                   balra forgatas szukseges */
                forgatBal(gyoker, nagyszulo_pt);
                csere_szin(szulo_pt, nagyszulo_pt);
                pt = szulo_pt;
            }
        }
    }

    gyoker->szin = FEKETE;
}

// pt beszurasa 'gyoker' gyokeru binaris faba
template<class K, class T>
Csucs<K, T>* BinFa_Beszuras(Csucs<K, T>* gyoker, Csucs<K, T>* pt)
{
    /* Ha a fa ures, uj gyokerkent teritjuk vissza pt-t */
    if (gyoker == nullptr)
        return pt;

    /* Kulonben megkeressuk a helyet a faban */
    if (pt->kulcs < gyoker->kulcs)
    {
        gyoker->bal = BinFa_Beszuras(gyoker->bal, pt);
        gyoker->bal->szulo = gyoker;
    }
    else if (pt->kulcs > gyoker->kulcs)
    {
        gyoker->jobb = BinFa_Beszuras(gyoker->jobb, pt);
        gyoker->jobb->szulo = gyoker;
    }

    /* Visszateritjuk az eredeti fa gyokeret */
    return gyoker;
}

// Adott ertek-kel rendelkezo csucs beszurasa a piros-fekete faba: 
template<class K, class T>
void PFFa<K, T>::beszur(K kulcs, const T& ertek)
{
    Csucs<K, T>* pt = new Csucs<K, T>(kulcs, ertek);

    /*  Elhelyezzuk az uj csucsot a faban,
        mint normalis binaris keresofaban. */
    gyoker = BinFa_Beszuras(gyoker, pt);

    // Kiegyensulyozzuk a fa szinezeset. 
    egyensulyoz(gyoker, pt);
}

// n csucs kulcsertekehez hasonlitja x-et
template<class K, class T>
Csucs<K, T>* PFFa<K, T>::hasonlit(Csucs<K, T>* n, K x)
{
    if (x == n->kulcs)
        return n;
    else if (x < n->kulcs)
    {
        if (n->bal == nullptr)
            return n;
        return hasonlit(n->bal, x);
    }
    else
    {
        if (n->jobb == nullptr)
            return n;
        return hasonlit(n->jobb, x);
    }
}
// vegrehajtjuk az f fgv-t a fa minden elemet 
// a kulcsertekuk szerinti sorrendben
template<class K, class T>
void PFFa<K, T>::inorder(Csucs<K, T>* x, void(*f)(T))
{
    if (x == nullptr)
        return;
    inorder(x->bal, f);
    f(x->adat);
    inorder(x->jobb, f);
}

// mutatot terit vissza az x kulcserteku csucshoz
template<class K, class T>
Csucs<K, T>* PFFa<K, T>::keres(K x)
{
    return hasonlit(gyoker, x);
}

// visszateriti, hogy letezik-e x kulcserteku csucs
template<class K, class T>
bool PFFa<K, T>::letezik(K x)
{
    return hasonlit(gyoker, x)->kulcs == x;
}

// A csucs legkisebb erteku leszarmazottja (vagy onmaga, ha nincs kisebb)
template<class K, class T>
Csucs<K, T>* Min_Leszarmazott(Csucs<K, T>* x) {
    Csucs<K, T>* temp = x;

    while (temp->bal != nullptr)
        temp = temp->bal;

    return temp;
}

// A csucs legnagyobb erteku leszarmazottja (vagy onmaga, ha nincs nagyobb)
template<class K, class T>
Csucs<K, T>* Max_Leszarmazott(Csucs<K, T>* x) {
    Csucs<K, T>* temp = x;

    while (temp->jobb != nullptr)
        temp = temp->jobb;

    return temp;
}

// mutatot terit vissza az x kulcserteket koveto kulcserteku csucsra
template<class K, class T>
Csucs<K, T>* PFFa<K, T>::kovetkezo(Csucs<K, T>* x)
{
    if (x->jobb == nullptr)
    {
        if (x->szulo != nullptr && x == x->szulo->bal)  // x baloldali leszarmazott -> szulo kulcserteke a kovetkezo
            return x->szulo;
        else                    // kulonben nincs x utani elem
            return nullptr;
    }
    return Min_Leszarmazott(x->jobb);
}

// mutatot terit vissza az x kulcserteket megelozo kulcserteku csucsra
template<class K, class T>
Csucs<K, T>* PFFa<K, T>::elozo(Csucs<K, T>* x)
{
    if (x->bal == nullptr)
    {
        if (x->szulo != nullptr && x == x->szulo->jobb)    // x jobboldali leszarmazott -> szulo kulcserteke az elozo
            return x->szulo;
        else                        // kulonben nincs x elotti elem
            return nullptr;
    }
    return Max_Leszarmazott(x->bal);
}

// mutatot terit vissza a fa legkisebb kulcserteku elemere
template<class K, class T>
Csucs<K, T>* PFFa<K, T>::elso()
{
    if (gyoker == nullptr)
        return nullptr;
    return Min_Leszarmazott(gyoker);
}

// mutatot terit vissza a fa legnagyobb kulcserteku elemere
template<class K, class T>
Csucs<K, T>* PFFa<K, T>::utolso()
{
    if (gyoker == nullptr)
        return nullptr;
    return Max_Leszarmazott(gyoker);
}

// mutatot terit vissza az x csucs helyere irhato csucshoz
template<class K, class T>
Csucs<K, T>* Helyettesito_Elem(Csucs<K, T>* x) {
    // ha ket gyereke van
    if (x->bal != nullptr && x->jobb != nullptr)
        return Min_Leszarmazott(x->jobb);

    // ha level
    if (x->bal == nullptr && x->jobb == nullptr)
        return nullptr;

    // pontosan egy gyereke van
    if (x->bal != nullptr)
        return x->bal;
    else
        return x->jobb;
}

// V csucs torlese a fabol
template<class K, class T>
void PFFa<K, T>::torol(Csucs<K, T>* V)
{
    Csucs<K, T>* U = Helyettesito_Elem(V);

    // U es V fekete 
    bool UV_Fekete = ((U == nullptr || U->szin == FEKETE) && (V->szin == FEKETE));
    Csucs<K, T>* szulo = V->szulo;

    if (U == nullptr) {
        // ha U nullpointer, akkor V-nek nincs belso leszarmazottja
        if (V == gyoker) {
            // V gyoker, null-ra allitjuk 
            gyoker = nullptr;
        }
        else {
            if (UV_Fekete) {
                // U es V fekete 
                // V-ben egynsulyozzuk a dupla fekete esetet 
                egyensulyozDuplaFekete(V);
            }

            // különben V piros, törölhetjük

            // V-t kitoroljuk a fabol
            if (V == V->szulo->bal) {
                szulo->bal = nullptr;
            }
            else {
                szulo->jobb = nullptr;
            }
        }
        delete V;
        return;
    }

    if (V->bal == nullptr or V->jobb == nullptr) {
        // V-nek van 1 gyereke 
        if (V == gyoker) {
            // V a gyoker, V felveszi U erteket, majd U-t toroljuk 
            V->kulcs = U->kulcs;
            V->adat = U->adat;
            V->bal = V->jobb = nullptr;
            delete U;
        }
        else {
            // V-t kivesszuk es a helyere kerul U
            if (V == V->szulo->bal) {
                szulo->bal = U;
            }
            else {
                szulo->jobb = U;
            }
            delete V;
            U->szulo = szulo;
            if (UV_Fekete) {
                // U es V fekete, U-ben egynsulyozzuk a dupla fekete helyzetet 
                egyensulyozDuplaFekete(U);
            }
            else {
                // U vagy V piros, U fekete lesz 
                U->szin = FEKETE;
            }
        }
        return;
    }

    // V-nek ket gyereke van, felcsereljuk az erteket U-val, majd azt toroljuk 
    csere_kulcs(U, V);
    csere_adat(U, V);
    torol(U);
}

// torles eseten felmerulo dupla fekete helyzet javitasa
template<class K, class T>
void PFFa<K, T>::egyensulyozDuplaFekete(Csucs<K, T>* x) {
    if (x == gyoker)
        // elertuk a gyokeret 
        return;

    Csucs<K, T>* testver = x->testver(), * szulo = x->szulo;
    if (testver->szin == PIROS) {
        // a testver piros 
        szulo->szin = PIROS;
        testver->szin = FEKETE;
        if (testver == testver->szulo->bal) {
            // x jobboldali gyerek 
            forgatJobb(gyoker, szulo);
        }
        else {
            // x baloldali gyerek
            forgatBal(gyoker, szulo);
        }
        egyensulyozDuplaFekete(x);
    }
    else {
        // a testver fekete 
        if ((testver->bal != nullptr && testver->bal->szin == PIROS) 
            || (testver->jobb != nullptr && testver->jobb->szin == PIROS)) {
            // A testvernek van piros gyereke
            if (testver->bal != nullptr and testver->bal->szin == PIROS) {
            // A testver bal gyereke piros
                if (testver == testver->szulo->bal) {
                    // bal-bal eset 
                    testver->bal->szin = testver->szin;
                    testver->szin = szulo->szin;
                    forgatJobb(gyoker, szulo);
                }
                else {
                    // jobb-bal eset 
                    testver->bal->szin = szulo->szin;
                    forgatJobb(gyoker, testver);
                    forgatBal(gyoker, szulo);
                }
            }
            else {
            // A testver jobb gyereke piros
                if (testver == testver->szulo->bal) {
                    // bal-jobb eset 
                    testver->jobb->szin = szulo->szin;
                    forgatBal(gyoker, testver);
                    forgatJobb(gyoker, szulo);
                }
                else {
                    // jobb-jobb eset
                    testver->jobb->szin = testver->szin;
                    testver->szin = szulo->szin;
                    forgatBal(gyoker, szulo);
                }
            }
            szulo->szin = FEKETE;
        }
        else {
            // a testvernek 2 fekete gyereke van 
            testver->szin = PIROS;
            if (szulo->szin == FEKETE)
                egyensulyozDuplaFekete(szulo);
            else
                szulo->szin = FEKETE;
        }
    }
}