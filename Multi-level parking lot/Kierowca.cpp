#include "Kierowca.h"

using namespace std;

Kierowca::Kierowca(string i_n, int pkt_karne)    : Osoba(i_n), ile_pkt_karnych(pkt_karne)
{

}

int Kierowca::get_ile_pkt_karnych() const
{
    return ile_pkt_karnych;
}

void Kierowca::set_ile_pkt_karnych(int pkt_karne)
{
    ile_pkt_karnych = pkt_karne;
}

ostream& operator<<(ostream& str_wy, const Kierowca& ob)
{
    //na początku wywołujemy operator<< na rzecz klasy podstawowej Osoba, w tym
    //przykładzie wypisuje on tylko jedną linijkę, którą moglibyśmy tutaj
    //po prostu skopiować, ale jest to dobry styl programowania
    operator<<(str_wy, static_cast<const Osoba&>(ob));  //(static_cast rzutuje typ const Kierowca& na typ const Osoba&)

    str_wy << "\nMam " << ob.get_ile_pkt_karnych() << " pkt karnych.";

    return str_wy;
}
