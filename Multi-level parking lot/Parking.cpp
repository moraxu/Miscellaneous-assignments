#include "Parking.h"
#include "Auto.h"
#include "Bilet.h"
using namespace std;

Parking::Parking(unsigned ilosc_poziomow, unsigned ilosc_miejsc)
                    : wektor(ilosc_poziomow, vector<Miejsce_parkingowe>(ilosc_miejsc))  //konstrukcja dwuwymiarowego wektora
{

}

const vector< vector<Miejsce_parkingowe> >& Parking::zwroc_wektor() const   //zwraca referencję do stałego wektora
{
    return wektor;
}

bool Parking::wjedz_na_poziom(unsigned nr_poziomu, const Auto& poj, int oplata)
{
    if(nr_poziomu < wektor.size())
    {
        for(unsigned nr_miejsca = 0 ; nr_miejsca < wektor[nr_poziomu].size() ; ++nr_miejsca)
        {
            //jeżeli miejsce na parkingu jest wolne (get_pojazd() zwraca adres zerowy) i dane auto
            //zmieści się na tym miejscu parkingowym, to możemy go tam dodać (wystawiając przy okazji bilet)
            if(!wektor[nr_poziomu][nr_miejsca].get_pojazd() && wektor[nr_poziomu][nr_miejsca].czy_zmiesci_sie_auto(poj))
            {
                Bilet bilet(oplata, nr_poziomu+1, nr_miejsca+1);

                wektor[nr_poziomu][nr_miejsca].ustaw_pojazd_bilet(&poj, &bilet);

                cout << "\n";

                //demonstracja działania funkcji wirtualnych
                poj.zwolnij();
                poj.zgas_silnik();

                cout << "\nPojazd zaparkowano na miejscu parkingowym " << nr_poziomu+1 << "x" << nr_miejsca+1 << ".\n\n";
                return true;
            }
        }

        cout << "\t\nWszystkie miejsca sa zajete lub auto jest za duze na dany poziom!\n\n";
    }
    else
    {
        cout << "\tNie ma takiego poziomu!\n\n";
    }

    return false;
}

bool Parking::wyjedz(string rejestracja)
{
    const Auto* poj;

    for(unsigned nr_poziomu = 0 ; nr_poziomu < wektor.size() ; ++nr_poziomu)
    {
        for(unsigned nr_miejsca = 0 ; nr_miejsca < wektor[nr_poziomu].size() ; ++nr_miejsca)
        {
            poj = wektor[nr_poziomu][nr_miejsca].get_pojazd();

            if(poj) //jeżeli miejsce jest zajęte
            {
                if(poj->get_rejestracja() == rejestracja)   //i rejestracje się zgadzają
                {
                    cout << "\n";

                    //demonstracja działania funkcji wirtualnych
                    poj->zapal_silnik();
                    poj->przyspiesz();

                    cout << "\n" << *(wektor[nr_poziomu][nr_miejsca].get_bilet()) << "\n\n";

                    wektor[nr_poziomu][nr_miejsca].usun_pojazd_bilet();

                    return true;
                }
            }
        }
    }

    cout << "\n\t\tNie znaleziono rejestracji na parkingu wielopoziomowym!\n\n";
    return false;
}
