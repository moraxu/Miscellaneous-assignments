#include <sstream>  //dla strumieni płynących do/czytających z obiektów klasy string
#include "Bilet.h"

using namespace std;

string Bilet::zlacz_nr_stanowiska(unsigned poziom, unsigned miejsce) const
{
    //definiujemy tzw. strumień wyjściowy piszący do stringu,
    //używamy go podobnie np. do strumienia cout, wszystko co
    //do niego zapiszemy, trafi do stringu, do którego odwołamy się potem jako str_do_stringu.str()
    ostringstream str_do_stringu;

    str_do_stringu << poziom << 'x' << miejsce;

    return str_do_stringu.str();
}

Bilet::Bilet(int oplata, unsigned poziom, unsigned miejsce)
        : czas_wjazdu(time(0)), oplata_za_godz(oplata), nr_stanowiska(zlacz_nr_stanowiska(poziom, miejsce))
{

}

string Bilet::get_czas_wjazdu() const
{
    return ctime(&czas_wjazdu); //funkcja ctime po otrzymaniu obiektu typu time_t*, zwraca
                                //ciąg tekstowy typu char* oznaczający odpowiednią datę w formacie tekstowym,
                                //my ten ciąg char* zwracamy jako obiekt typu string
}

int Bilet::get_oplata_za_godz() const
{
    return oplata_za_godz;
}

double Bilet::aktualna_naleznosc() const    //funkcja obliczająca naszą aktualną należność (za pobyt na miejscu parkingowym)
{
    return ((time(0) - czas_wjazdu) / 60.0) * oplata_za_godz;   //w programie traktujemy sekundy jako minuty
}

string Bilet::get_nr_stanowiska() const
{
    return nr_stanowiska;
}

ostream& operator<<(ostream& str_wy, const Bilet& ob)
{
    str_wy << "[Bilet na nr stanowiska " << ob.get_nr_stanowiska() << ",\n"
           << "czas przyjazdu: " << ob.get_czas_wjazdu() << "akt. naleznosc: ";

    ios_base::fmtflags stare_flagi = str_wy.setf(ios::fixed, ios::floatfield);
    streamsize stara_precyzja = str_wy.precision(2);    //ustawiamy precyzję na 2 cyfry po przecinku dziesiętnym
                                                        //na najbliższą operację wypisania liczby zmiennoprzecinkowej

    str_wy << ob.aktualna_naleznosc() << " zl.]";

    str_wy.flags(stare_flagi);  //po wypisaniu liczby, powracamy do starych ustawień strumienia
    str_wy.precision(stara_precyzja);

    return str_wy;
}
