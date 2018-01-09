#include <iostream>
using namespace std;

#define MAX 300

class Stos{
    double elementy[MAX];
    unsigned rozmiar;

    public:
    Stos():rozmiar(0){}
    bool Pelny(){return (rozmiar == MAX);}
    bool Pusty(){return (rozmiar == 0);}
    operator double(){ return elementy[rozmiar-1];}

    bool Poloz(const double & element){
        return (Pelny() ? false : (elementy[rozmiar++]=element, true));
    }

    bool Zdejmij(double & element){
        return (Pusty() ? false : (element=elementy[--rozmiar], true));
    }

    void drukuj(){
        double tu_zdjety;
        while(Pusty()==false){
            Zdejmij(tu_zdjety);
            cout<<tu_zdjety<<" ";
        }
    }
};

int main(){
    const int rozmiar_tablicy = 11;
    double A[rozmiar_tablicy] = {578.6, 32.4, 0.7, 45.3, 12.3, 156.4, 78.8, 93.2, 1.4, 56.32, 23.23};

    cout<<"zawartosc tablicy A: ";
    for(int ind=0 ; ind<rozmiar_tablicy ; ind++) cout<<A[ind]<<" ";
    cout<<endl;

    Stos L, P;

    double do_przenoszenia;
    L.Poloz(A[0]);
    for(int ind=1 ; ind<rozmiar_tablicy ; ind++){
        for(; (L.Pusty()==false) && L>A[ind] ; L.Zdejmij(do_przenoszenia), P.Poloz(do_przenoszenia));
        for(; (P.Pusty()==false) && P<A[ind] ; P.Zdejmij(do_przenoszenia), L.Poloz(do_przenoszenia));
        L.Poloz(A[ind]);
    }

    while(L.Pusty()==false){
        L.Zdejmij(do_przenoszenia);
        P.Poloz(do_przenoszenia);
    }

    cout<<"stos P: "; P.drukuj();
    return 0;
}
