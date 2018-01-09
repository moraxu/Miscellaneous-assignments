#include <cstdlib>
#include <stdexcept>    //dla standardowo zdefiniowanych wyjątków
#include "Miejsce_parkingowe.h"
#include "Auto.h"
#include "Bilet.h"

using namespace std;

Miejsce_parkingowe::Miejsce_parkingowe(const Auto* poj, const Bilet* bil)
    : dlugosc(rand() % 21 + 5), szerokosc(rand() % 21 + 5), //losujemy 2 wartości z przedziałów 5-25
      pojazd(poj ? poj->virt_konstr_kop() : 0), // <--- tutaj właśnie korzystamy z "konstruktora wirtualnego"
      bilet(bil ? new Bilet(*bil) : 0)
{

}

Miejsce_parkingowe::Miejsce_parkingowe(const Miejsce_parkingowe& ob)    : dlugosc(ob.dlugosc), szerokosc(ob.szerokosc)
{
    pojazd = (ob.pojazd ? ob.pojazd->virt_konstr_kop() : 0);    // <--- tutaj właśnie korzystamy z "konstruktora wirtualnego"
    bilet = (ob.bilet ? new Bilet(*ob.bilet) : 0);
}

Miejsce_parkingowe& Miejsce_parkingowe::operator=(const Miejsce_parkingowe& ob)
{
    if(this == &ob)     //jeżeli chcielibyśmy skopiować ten sam obiekt,
        return *this;   //to opuszczamy funkcję

    delete pojazd;
    pojazd = (ob.pojazd ? ob.pojazd->virt_konstr_kop() : 0);    // <--- tutaj właśnie korzystamy z "konstruktora wirtualnego"

    delete bilet;
    bilet = (ob.bilet ? new Bilet(*ob.bilet) : 0);

    /*
    //nie możemy skopiować poniższych składników ponieważ są one const - czyli stałe:
    dlugosc = ob.dlugosc;
    szerokosc = ob.szerokosc;
    */

    return *this;
}

int Miejsce_parkingowe::get_dlugosc() const
{
    return dlugosc;
}

int Miejsce_parkingowe::get_szerokosc() const
{
    return szerokosc;
}

const Auto* Miejsce_parkingowe::get_pojazd() const
{
    return pojazd;
}

const Bilet* Miejsce_parkingowe::get_bilet() const
{
    return bilet;
}

void Miejsce_parkingowe::ustaw_pojazd_bilet(const Auto* poj, const Bilet* bil)
{
    if(poj && bil)  //jeżeli oba zawierają adresy niezerowe, to ich zawartości kopiujemy do naszych wskaźników
    {
        usun_pojazd_bilet();

        pojazd = poj->virt_konstr_kop();    // <--- tutaj właśnie korzystamy z "konstruktora wirtualnego"
        bilet = new Bilet(*bil);
    }
    else    //w przeciwnym przypadku, rzucamy wyjątek
    {
        throw invalid_argument("null pointer!");
    }
}

bool Miejsce_parkingowe::usun_pojazd_bilet()
{
    if(pojazd && bilet)
    {
        delete pojazd;
        delete bilet;

        pojazd = 0;
        bilet = 0;

        return true;    //pomyślnie usunęliśmy pojazd i bilet z miejsca parkingowego
    }

    return false;   //miejsce było już puste
}

bool Miejsce_parkingowe::czy_zmiesci_sie_auto(const Auto& poj) const
{
    if(poj.get_dlugosc()*poj.get_szerokosc() <= dlugosc*szerokosc)  //żeby zbytnio nie komplikować funkcji,
    {                                                               //przeprowadzimy tylko taki dość banalny test
        return true;
    }
    else
    {
        return false;
    }
}

Miejsce_parkingowe::~Miejsce_parkingowe()
{
    //w destruktorze zwalniamy miejsce pokazywane przez wskaźniki: pojazd i bilet
    //(jeżeli, w którymś z nich jest adres zerowy to nic się nie
    //dzieje - operator delete zignoruje takie wywołanie)
    delete pojazd;
    delete bilet;
}
