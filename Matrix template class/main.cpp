#include <iostream>
#include <fstream>      //dla obsługi plików dyskowych
#include <algorithm>    //dla funkcji sort(), swap()
#include <cstdlib>      //makra EXIT_SUCCESS, EXIT_FAILURE
#include <cstring>      //dla funkcji memcpy()

using namespace std;

//--------------------------------------------------------------------------------------------------------------------------------------
//klasa szablonowa imitująca tablicę dwuwymiarową:

template <typename T>
class Tablica
{
    unsigned wiersze, kolumny;
    T* tab;

public:
    Tablica(const unsigned wier = 0, const unsigned kol = 0);
    Tablica(const Tablica& ob);
    Tablica& operator=(const Tablica& ob);
    ~Tablica();

    unsigned ile_wierszy() const;
    unsigned ile_kolumn() const;

    bool czytaj_z_pliku(const char* nazwa_pliku);
    void drukuj_na_ekran() const;
    void nadpisz_element(const unsigned wiersz, const unsigned kolumna, const T& wartosc);
    void dodaj_wiersz();
    void dodaj_kolumne();
    bool usun_wiersz(const unsigned wiersz);
    bool usun_kolumne(const unsigned kolumna);
    void zamien_elementy(const unsigned wiersz1, const unsigned kolumna1, const unsigned wiersz2, const unsigned kolumna2);
    void przestaw_kolumny(const unsigned kolumna1, const unsigned kolumna2);
    void przestaw_wiersze(const unsigned wiersz1, const unsigned wiersz2);
    void sortuj_tablice();
};
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
Tablica<T>::Tablica(const unsigned wier, const unsigned kol)
    : wiersze((wier && kol) ? wier : 0), kolumny((wier && kol) ? kol : 0), tab( (wiersze && kolumny) ? new T[wiersze*kolumny]() : 0)
{
    //w konstruktorze korzystamy tylko z listy inicjalizacyjnej (podanej powyżej), aby zainicjalizować rozmiary tablicy oraz wskaźnik
    //adresem tablicy o wymiarach wier*kol - jest to tablica jednowymiarowa, z której korzystać będziemy jak z dwuwymiarowej

    //jeżeli którykolwiek z argumentów będzie miał wartość domyślną, czyli zero, to nie alokujemy miejsca na tablicę,
    //przypisując tym samym do wskaźnika adres zerowy i 0 do każdego wymiaru
}
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
Tablica<T>::Tablica(const Tablica& ob)  //konstruktor kopiujący, potrzebujemy go, ponieważ obiekty klasy przechowują wskaźniki
                                        //pokazujące na dynamicznie zaalokowany obszar pamięci
    : wiersze(ob.wiersze), kolumny(ob.kolumny), tab(ob.tab ? static_cast<T*>(memcpy(new T[wiersze*kolumny], ob.tab, wiersze*kolumny*sizeof(T))) : 0)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
Tablica<T>& Tablica<T>::operator=(const Tablica& ob)    //przeładowany operator przypisania, potrzebujemy go, ponieważ obiekty klasy przechowują wskaźniki
                                                        //pokazujące na dynamicznie zaalokowany obszar pamięci
{
    if(this == &ob)
        return *this;   //opuszczamy funkcję, jeśli operator został wywołany na rzecz tych samych obiektów

    delete [] tab;

    wiersze = ob.wiersze;
    kolumny = ob.kolumny;
    tab = (ob.tab ? static_cast<T*>(memcpy(new T[wiersze*kolumny], ob.tab, wiersze*kolumny*sizeof(T))) : 0);

    return *this;
}
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
Tablica<T>::~Tablica()
{
    //w destruktorze zwalniamy tylko miejsce zajmowane przez naszą tablicę
    delete [] tab;
}
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
unsigned Tablica<T>::ile_wierszy() const
{
    return wiersze;
}
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
unsigned Tablica<T>::ile_kolumn() const
{
    return kolumny;
}
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool Tablica<T>::czytaj_z_pliku(const char* nazwa_pliku)
{
    ifstream str_czytajacy(nazwa_pliku);

    if(!str_czytajacy.is_open())    //jeżeli nie udało otworzyć się strumienia czytającego z pliku
    {
        return false;   //to zwracamy wartość false
    }

    //w przeciwnym wypadku wczytujemy tyle wartości, ile pomieści tablica (chyba, że napotkamy wcześniej koniec pliku)

    for(unsigned rozmiar = wiersze*kolumny, i = 0 ; i < rozmiar && str_czytajacy.peek() != EOF ; ++i)
    {
        str_czytajacy >> tab[i];
    }

    str_czytajacy.close();
    return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void Tablica<T>::drukuj_na_ekran() const
{
    cout << "Zawartosc tablicy:\n";
    for(unsigned akt_wiersz = 0 ; akt_wiersz < wiersze ; ++akt_wiersz)
    {
        cout << "\n\t";
        for(unsigned akt_kolumna = 0 ; akt_kolumna < kolumny ; ++akt_kolumna)
        {
            cout << tab[akt_wiersz * kolumny + akt_kolumna] << ' '; //tak kompilator oblicza adres elementu tablicy dwuwymiarowej
                                                                    //gdy stosujemy notację tab[nr_wiersza][nr_kolumny], tutaj tej notacji
                                                                    //nie możemy zastosować (tablica jest jednowymiarowa), stąd ten zapis
        }
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void Tablica<T>::nadpisz_element(const unsigned wiersz, const unsigned kolumna, const T& wartosc)
{
    tab[wiersz * kolumny + kolumna] = wartosc;  //podobna notacja jak w funkcji składowej Tablica<T>::drukuj_na_ekran()
}
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void Tablica<T>::dodaj_wiersz()
{
    if(!tab)    //jeżeli tablica ma wymiary 0x0 czyli jest pusta...
    {
        tab = new T[1]; //...to powiększamy jej rozmiar do 1x1
        wiersze = kolumny = 1;
        return;
    }

    T* nowa_tab = new T[(wiersze+1)*kolumny](); //tworzymy nową tablicę, powiększoną o 1 wiersz na końcu

    memcpy(nowa_tab, tab, wiersze*kolumny*sizeof(T));   //kopiujemy zawartość starej tablicy do nowej (powiększonej)

    delete [] tab;  //zwalniamy miejsce zajmowane przez starą tablicę
    tab = nowa_tab; //aktualizujemy wskaźnik

    ++wiersze;
}
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void Tablica<T>::dodaj_kolumne()
{
    if(!tab)    //jeżeli tablica ma wymiary 0x0 czyli jest pusta...
    {
        tab = new T[1]; //...to powiększamy jej rozmiar do 1x1
        wiersze = kolumny = 1;
        return;
    }

    T* nowa_tab = new T[wiersze*(kolumny+1)](); //tworzymy nową tablicę, powiększoną o 1 kolumnę na końcu

    for(unsigned i = 0 ; i < wiersze ; ++i)
    {
        memcpy(nowa_tab+(i*(kolumny+1)), tab+(i*kolumny), kolumny*sizeof(T));   //kopiujemy tablicę wierszami, aby ostatnia
                                                                                //kolumna nowej tablicy zawierała puste elementy
    }

    delete [] tab;  //zwalniamy miejsce zajmowane przez starą tablicę
    tab = nowa_tab; //aktualizujemy wskaźnik

    ++kolumny;
}
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool Tablica<T>::usun_wiersz(const unsigned wiersz)
{
    if(wiersze == 1)    //jeżeli usuwamy ostatni wiersz tablicy to tym samym dealokujemy miejsce przez nią zajmowaną
    {
        wiersze = kolumny = 0;
        delete [] tab;
        tab = 0;
        return false;
    }

    T* nowa_tab = new T[(wiersze-1)*kolumny]();

    //dwa wywołania funkcji memcpy:

    //pierwsze kopiuje zawartość starej tablicy DO wiersza, który zostanie pominięty
    memcpy(nowa_tab, tab, wiersz*kolumny*sizeof(T));

    //drugie kopiuje zawartość starej tablicy zaczynając od wiersza PO wierszu, który został pominięty
    memcpy(nowa_tab+(wiersz*kolumny), tab+((wiersz+1)*kolumny), (wiersze-wiersz-1)*kolumny*sizeof(T));

    delete [] tab;  //zwalniamy miejsce zajmowane przez starą tablicę
    tab = nowa_tab;

    --wiersze;

    return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
bool Tablica<T>::usun_kolumne(const unsigned kolumna)
{
    if(kolumna == 1)    //jeżeli usuwamy ostatnią kolumnę tablicy to tym samym dealokujemy miejsce przez nią zajmowaną
    {
        wiersze = kolumny = 0;
        delete [] tab;
        tab = 0;
        return false;
    }

    T* nowa_tab = new T[wiersze*(kolumny-1)]();

    for(unsigned i = 0 ; i < wiersze ; ++i)
    {
        //dwa wywołania funkcji memcpy na każdy wiersz starej tablicy:

        //pierwsze kopiuje zawartość starej tablicy DO kolumny, która zostanie pominięta
        memcpy(nowa_tab+(i*(kolumny-1)), tab+(i*kolumny), kolumna*sizeof(T));

        //drugie kopiuje zawartość starej tablicy zaczynając od kolumny PO kolumnie, która została pominięta
        memcpy(nowa_tab+(i*(kolumny-1))+kolumna, tab+(i*kolumny)+kolumna+1, (kolumny-kolumna-1)*sizeof(T));
    }

    delete [] tab;  //zwalniamy miejsce zajmowane przez starą tablicę
    tab = nowa_tab;

    --kolumny;

    return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void Tablica<T>::zamien_elementy(const unsigned wiersz1, const unsigned kolumna1, const unsigned wiersz2, const unsigned kolumna2)
{
    swap(tab[wiersz1 * kolumny + kolumna1], tab[wiersz2 * kolumny + kolumna2]);
}
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void Tablica<T>::przestaw_kolumny(const unsigned kolumna1, const unsigned kolumna2)
{
    for(unsigned i = 0 ; i < wiersze ; ++i)
    {
        zamien_elementy(i, kolumna1, i, kolumna2);
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void Tablica<T>::przestaw_wiersze(const unsigned wiersz1, const unsigned wiersz2)
{
    for(unsigned i = 0 ; i < kolumny ; ++i)
    {
        zamien_elementy(wiersz1, i, wiersz2, i);
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void Tablica<T>::sortuj_tablice()
{
    sort(tab, tab+(wiersze*kolumny));
}
//--------------------------------------------------------------------------------------------------------------------------------------

int main()
{
    cout << "Tworzymy tablice dwuwymiarowa 5x6 i wczytujemy liczby z pliku tablica.txt.\n";

    Tablica<int> liczby(5, 6);
    if(!liczby.czytaj_z_pliku("tablica.txt"))
    {
        cout << "\nNie można otworzyć pliku tablica.txt!";
        return EXIT_FAILURE;
    }

    liczby.drukuj_na_ekran();

    unsigned wiersz, kolumna;
    char temp;

    cout << "\n\nPodaj indeks elementu do nadpisania (w formacie: wiersz-kolumna, numeracja od 0) >>> ";
    cin >> wiersz >> temp >> kolumna;

    if(wiersz > liczby.ile_wierszy() || kolumna > liczby.ile_kolumn())
    {
        cout << "\nNiepoprawny indeks!";
        return EXIT_FAILURE;
    }

    int liczba;

    cout << "Podaj nowa liczbe >>> ";
    cin >> liczba;

    liczby.nadpisz_element(wiersz, kolumna, liczba);

    cout << "\nDodajemy nowy wiersz.\n";
    liczby.dodaj_wiersz();
    liczby.drukuj_na_ekran();

    cout << "\n\nDodajemy nowa kolumne.\n";
    liczby.dodaj_kolumne();
    liczby.drukuj_na_ekran();

    cout << "\n\nUsuwamy dwa pierwsze wiersze.\n";
    liczby.usun_wiersz(0);
    liczby.usun_wiersz(0);
    liczby.drukuj_na_ekran();

    cout << "\n\nUsuwamy dwie przedostatnie kolumny.\n";
    liczby.usun_kolumne(liczby.ile_kolumn()-2);
    liczby.usun_kolumne(liczby.ile_kolumn()-2);
    liczby.drukuj_na_ekran();

    cout << "\n\nZamieniamy element [0][0] z [0][1].\n";
    liczby.zamien_elementy(0, 0, 0, 1);
    liczby.drukuj_na_ekran();

    cout << "\n\nPrzestawiamy kolumny nr 0 i 2.\n";
    liczby.przestaw_kolumny(0, 2);
    liczby.drukuj_na_ekran();

    cout << "\n\nPrzestawiamy wiersze nr 0 i 2.\n";
    liczby.przestaw_wiersze(0, 2);
    liczby.drukuj_na_ekran();

    cout << "\n\nSortujemy tablice.\n";
    liczby.sortuj_tablice();
    liczby.drukuj_na_ekran();

    cout << "\n\nTworzymy dwie nowe tablice wykorzystujac konstr. kop. i przeladowany operator=.\n";

    Tablica<int> nowa1(liczby); //startuje konstruktor kopiujący
    Tablica<int> nowa2;
    nowa2 = liczby; //startuje przeładowany operator=

    nowa1.drukuj_na_ekran();
    cout << "\n\n";
    nowa2.drukuj_na_ekran();

    return EXIT_SUCCESS;
}
