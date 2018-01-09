#ifndef AUTO_H_INCLUDED
#define AUTO_H_INCLUDED

#include <string>
#include "Kierowca.h"

class Auto
{
    int szerokosc, dlugosc;
    std::string nazwa;

    Kierowca wlasciciel;
    std::string rejestracja;

public:

    Auto(std::string nzw = "", int szer = 0, int dlug = 0, std::string rejstr = "", const Kierowca& wlasc = Kierowca());

    Kierowca get_wlasciciel() const;
    void set_wlasciciel(const Kierowca& wlasc);

    int get_szerokosc() const;
    void set_szerokosc(int nowa_szer);

    int get_dlugosc() const;
    void set_dlugosc(int nowa_dlug);

    std::string get_rejestracja() const;
    void set_rejestracja(std::string rejestr);

    std::string get_nazwa() const;
    void set_nazwa(std::string nowa_nazwa);

    //tzw. funkcje czysto wirtualne, czyniące z tej klasy klasę abstrakcyjną (nie można
    //tworzyć obiektów tej klasy, a jedynie wskaźniki i referencje na nią),
    //w funkcjach tych będziemy wyświetlać tylko proste komunikaty (w klasach pochodnych), ma to na celu
    //wyłącznie zobrazowanie działania funkcji wirtualnych
    virtual void przedstaw_sie() const = 0;
    virtual void zapal_silnik() const = 0;
    virtual void zgas_silnik() const = 0;
    virtual void przyspiesz() const = 0;
    virtual void zwolnij() const = 0;

    //2 poniższe funkcje wirtualne prezentują często używany idiom na tzw. "konstruktor wirtualny",
    //który nie istnieje oficjalnie w języku C++, zdefiniujemy je w każdej klasie pochodnej od klasy Auto

    virtual Auto* virt_konstr() const = 0;      //"wirtualny konstruktor"
    virtual Auto* virt_konstr_kop() const = 0;  //"wirtualny konstruktor kopiujący"

    virtual ~Auto();    //wirtualny destruktor, definiujemy go tylko w celu
                        //uniknięcia ostrzeżeń kompilacji w g++, będzie miał on puste ciało
};

#endif // AUTO_H_INCLUDED
