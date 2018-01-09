#include <cstdio>   //dla operacji wejscia-wyjscia
#include <climits>  //dla makr oznaczajacych maks. i min. wartosci mogace byc przechowywane w danych zmiennych

using namespace std;

#define MAX_VERTICES 5000   //5000 to maksymalna ilosc wierzcholkow
#define MAX_EDGES 100000    //100000 to maksymalna ilosc krawedzi

///////////////////////////////////////////////////////////////////////
class Vertex    //klasa reprezentujaca wierzcholek
{
public:

    int duty;   //ma on jedyna skladowa oznaczajaca przypisane do niego clo

    Vertex(int d = 0) : duty(d)
    {   }
};
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
class Weighted_edge //klasa reprezentujaca krawedz wazona
{
    int vertex_v, vertex_w; //wierzcholki koncowe
    int edge_weight;    //waga krawedzi (cena przemiany metalu vertex_v na metal vertex_w)

public:
    Weighted_edge(int v = 0, int w = 0, int weight = 0) : vertex_v(v), vertex_w(w), edge_weight(weight) { }

    int v() const
    {
        return vertex_v;
    }

    int w() const
    {
        return vertex_w;
    }

    int wt() const
    {
        return edge_weight;
    }

    int other(int vertex) const //funkcja otrzymujaca jako argument nr jednego z wierzcholka na koncu krawedzi,
    {                           //a zwracajaca nr wierzcholka lezacego naprzeciw
        if(vertex == vertex_v)
            return vertex_w;
        else
            return vertex_v;
    }
};
///////////////////////////////////////////////////////////////////////

class DenseWeightedGRAPH    //klasa reprezentujaca skierowany graf wazony (tzw. dense czyli gesty
{                           //poniewaz poslugujemy sie tutaj macierza sasiedztwa)
    int Vcnt;   //ilosc wierzcholkow w grafie

    Weighted_edge*** adj;   //macierz sasiedztwa to dwuwymiarowa tablica przechowujaca wskazniki
                            //na elementy typu Weighted_edge, jest to dobra praktyka programistyczna
                            //na wypadek gdybysmy musieli w przyszlosci powiekszyc nasza klase
                            //Weighted_edge o nowe skladniki - dzieki temu macierz sasiedztwa nie bedzie
                            //niepotrzebnie dublowala miejsa w pamieci - uzywamy tylko wskaznikow, adj
                            //to wskaznik na taka tablice, zaalokujemy pod nia miejsce w konstruktorze

public:
    DenseWeightedGRAPH(int V) : Vcnt(V)
    {
        adj = new Weighted_edge**[Vcnt];    //tablica dwuwymiarowa to tablica tablic, na poczatku tworzymy glowna tablice...

        for(int i = 0 ; i < Vcnt ; ++i)
        {
            adj[i] = new Weighted_edge*[Vcnt];  //...a pozniej alokujemy miejsce pod jej elementy - tablice przechowujace adresy
                                                //obiektow Weighted_edge

            for(int j = 0 ; j < Vcnt ; ++j)
            {
                adj[i][j] = 0;  //adresy inicjalizujemy wstepnie zerami
            }
        }
    }

    ~DenseWeightedGRAPH()
    {
        for(int i = 0 ; i < Vcnt ; ++i)
        {
            delete [] adj[i];   //w destruktorze dealokujemy miejsce zajmowane przez glowna tablice tablic
        }

        delete [] adj;  //a takze przez poszczegolne tablice
    }

    int V() const { return Vcnt; }

    void insert_edge(Weighted_edge* e)  //funkcja wstawiajaca adres krawedzi do grafu
    {
        int v = e->v(), w = e->w(); //uzyskujemy numery wierzcholkow w krawedzi
        adj[v][w] = e;  //zapisujemy adres krawedzi do macierzy sasiedztwa
    }

    friend class adjIterator;

    class adjIterator   //klasa, ktora posluzy nam jako uchwyt do uzyskania
                        //wszystkich sasiadow danego wierzcholka
    {
        const DenseWeightedGRAPH& G;
        int v, i;

    public:
        //G to referencja na graf, na ktorym zamierzamy pracowac, v to numer
        //wierzcholka, po ktorego sasiadach chcemy iterowac
        adjIterator(const DenseWeightedGRAPH& G, int v) : G(G), v(v), i(-1) {   }

        Weighted_edge* beg()    //zwraca pierwszego sasiada (w macierzy sasiedztwa) dla wierzcholka nr v
        {
            i = -1;
            return nxt();
        }

        Weighted_edge* nxt()    //zwraca kolejnego sasiada
        {
            for(++i ; i < G.V() ; ++i)
                if(G.adj[v][i] != 0)
                    return G.adj[v][i];
            return 0;
        }

        bool end() const    //sprawdza czy przegladnelismy juz wszystkich sasiadow wierzcholka nr v
        {
            return i >= G.V();
        }
    };
};
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
class SPT_Dijkstra  //klasa realizujaca algorytm dijkstry - wszukuje on najkrotsze sciezki
                    //do wszystkich wierzcholkow od wierzcholka startowego
{
    const DenseWeightedGRAPH& G;    //referencja na graf, na ktorym bedziemy pracowac
    const int start_vertex; //wierzcholek startowy, od ktorego szukamy najkrotsze sciezki

    unsigned long wt[MAX_VERTICES]; //tablica przechowujaca dlugosci najkrotszych sciezek do kazdego wierzcholka od wierzcholka startowego
    Weighted_edge* spt[MAX_VERTICES];   //spt[i] przechowuje adres ostatniej krawedzi na drodze od "start_vertex" do "i",
                                        //w konstruktorze inicjalizujemy adresy zerami, wiec przyda nam sie ona aby sprawdzic
                                        //czy w ogole istnieje sciezka od "start_vertex" do "i"

    struct edge //struktura potrzebna na uzytek klasy dijkstra_set (definicja nizej),
    {           //przechowuje ona nr wierzcholka i dlugosc ostatnio znalezionej najkrotszej sciezki do niego
        int vertex;
        unsigned long weight;

        edge(int v = 0, unsigned long wt = 0) : vertex(v), weight(wt) { }

        bool operator<(const edge& e2) const
        {
            return weight < e2.weight || ((e2.weight == weight) && vertex < e2.vertex);
        }

        bool operator==(const edge& e2) const
        {
            return (weight == e2.weight) && (vertex == e2.vertex);
        }
    };

    class dijkstra_set  //klasa imitujaca std::set (wstawianie, usuwanie elementow i wyszukiwanie najmniejszego)
    {
        int set_size;   //aktualny rozmiar
        edge arr[MAX_VERTICES]; //tablica bedzie przechowywala co najwyzej MAX_VERTICES krawedzi

    public:

        dijkstra_set()
        {
            set_size = 0;
        }

        bool empty() const
        {
            return set_size == 0;
        }

        edge return_smallest_element() const    //szukamy krawedzi o najmniejszej wadze
        {
            edge min_el(INT_MAX, ULONG_MAX);
            for(int i = 0 ; i < set_size ; ++i)
            {
                if(arr[i] < min_el)
                    min_el = arr[i];
            }
            return min_el;
        }

        void insert(edge e) //wstawiamy nowa krawedz e
        {
            arr[set_size++] = e;
        }

        void erase(edge e)  //usuwamy krawedz e (jesli jest u nas przechowywana)
        {
            int i;
            for(i = 0 ; i < set_size ; ++i)
            {
                if(arr[i] == e) //szukamy indeks pod ktorym byc moze jest krawedz
                    break;
            }

            if(i < set_size)    //jesli znalezlismy taka krawedz...
            {
                if(set_size > 1)    //...a ilosc krawedzi >1,
                {                   //to przesuwamy wszystkie elementy z prawego konca tablicy o jeden element tak, aby zatarly nasz
                    for( ; i < set_size-1 ; ++i)
                    {
                        arr[i] = arr[i+1];
                    }
                }

                --set_size;
            }
        }
    };

    void dijkstra_algorithm()
    {
        int v = start_vertex;
        wt[v] = 0;  //ustawiamy dlugosc najkrotszej sciezki do wierzcholka startowego jako 0 (z def. algorytmu)

        dijkstra_set vertex_queue;
        vertex_queue.insert(edge(v, wt[v]));

        while(!vertex_queue.empty())
        {
            edge smallest = vertex_queue.return_smallest_element(); //pobieramy najkrotsza sciezke do jakiegos wierzcholka,
                                                                    //ktora obecnie przechowuje nasz dijkstra_set
            v = smallest.vertex;
            vertex_queue.erase(smallest);   //po pobraniu sciezki, usuwamy ja ze zbioru

            DenseWeightedGRAPH::adjIterator A(G, v);
            for(Weighted_edge* e = A.beg() ; !A.end() ; e = A.nxt())    //przetwarzamy kazdego sasiada
            //wierzcholka v, sasiad jest zapisany jako klasa Weighted_edge
            {
                unsigned long P = wt[v] + e->wt();  //wt() zwraca wage krawedzi do sasiada, po dodaniu do niej wt[v]
                                                    //otrzymujemy dlugosc OBECNIE PRZETWARZANEJ sciezki do danego sasiada

                int w = e->other(v);    //funkcja zwraca wierzcholek lezacy naprzeciwko wierzcholka "v" w krawedzi "e"

                if(P < wt[w])   //jezeli OBECNIE PRZETWARZANA sciezka do danego sasiada jest krotsza od poprzedniej...
                {
                    vertex_queue.erase(edge(w, wt[w])); //...to usuwamy poprzednia sciezke z naszego zbioru

                    //uaktualniamy dwa elementy w naszych tablicach
                    wt[w] = P;
                    spt[w] = e;

                    vertex_queue.insert(edge(w, wt[w]));    //i dodajemy nowa najkrotsza sciezke do zbioru
                }
            }
        }
    }

public:
    SPT_Dijkstra(const DenseWeightedGRAPH& G, int start)    : G(G), start_vertex(start)
    {
        for(int i = 0 ; i < G.V() ; ++i)
        {
            wt[i] = ULONG_MAX;  //wstepnie inicjalizujemy dlugosci najkrotszych sciezek najwieksza mozliwa liczba
            spt[i] = 0;
        }

        dijkstra_algorithm();
    }

    Weighted_edge* pathR(int v) const   //zgodnie z komentarzem przy definicji skladowej spt, ta funkcje mozemy
    {                                   //wykorzystac aby dowiedziec sie czy start_vertex jest polaczony z wierzcholkiem nr v
        return spt[v];                  //(jesli jest to funkcja nie zwroci adresu zerowego)
    }

    unsigned long dist(int v) const //funkcja zwraca dlugosc najkrotszej sciezki od start_vertex do v
    {
        return wt[v];
    }
};
///////////////////////////////////////////////////////////////////////

int main()
{
    FILE* input_file = fopen("dane", "r");

    int Vcnt, Ecnt;

    fscanf(input_file, "%d", &Vcnt);

    //bedziemy budowac dwa grafy: pierwszy zwyczajny i drugi o nazwie rgraph z ODWROCONYMI krawedziami
    DenseWeightedGRAPH graph(Vcnt), rgraph(Vcnt);

    Vertex* vertices = new Vertex[Vcnt];

    for(int i = 0 ; i < Vcnt ; ++i)
    {
        int duty;
        fscanf(input_file, "%d", &duty);

        vertices[i] = Vertex(duty/2);
    }

    fscanf(input_file, "%d", &Ecnt);

    Weighted_edge* edges = new Weighted_edge[Ecnt*2];   //tworzymy dwa razy tyle krawedzi ile potrzeba,
                                                        //poniewaz musimy zbudowac dodatkowo drugi graf z odwroconymi krawedziami

    for(int i = 0 ; i < Ecnt*2 ; i+=2)
    {
        int v, w, cost;
        fscanf(input_file, "%d%d%d", &v, &w, &cost);

        //od wczytanych nr wierzcholkow odejmujemy 1 aby miec numeracje od 0

        edges[i] = Weighted_edge(v-1, w-1, cost);
        edges[i+1] = Weighted_edge(w-1, v-1, cost); //odwrocona krawedz
        graph.insert_edge(&edges[i]);
        rgraph.insert_edge(&edges[i+1]);    //dodajemy odwrocona krawedz do odwroconego grafu
    }

    //algorytm polega na tym aby dla kazdego wierzcholka wyliczyc 2 najkrotsze sciezki:
    //od zlota do danego wierzcholka i od danego wierzcholka do zlota
    //pierwszy zbior sciezek liczymy uruchamiajac algorytm dijkstry dla zwyczajnego grafu z wierzcholkiem startowym nr 0 (zloto),
    //drugi zbior sciezek liczymy uruchamiajac algorytm dijkstry dla ODWROCONEGO grafu z wierzcholkiem starotwym rowniez 0 (zloto)
    //- taki zbior najkrotszych sciezek, pomimo ze liczony od wierzcholka 0, daje tak naprawde dlugosci najkrotszych sciezek
    //od kazdego innego wierzcholka wlasnie do zlota (bo pracowalismy na ODWROCONYM grafie)

    //po dodaniu takich dwoch najkrotszych sciezek do siebie i dodaniu jeszcze cla dla danego wierzcholka,
    //sprawdzamy czy wynik dodawania tych 3 elementow jest najmniejszy - jesli tak, to go zapisujemy

    //najmniejszy wynik ze wszystkich to odpowiedz dla naszego zadania

    SPT_Dijkstra spaths_from_gold(graph, 0),    //pierwszy zbior najkrotszych sciezek
                 spaths_others(rgraph, 0);      //drugi zbior najkrotszych sciezek (na grafie odwroconym)

    unsigned long min_total_cost = vertices[0].duty;    //uwzgledniamy clo dla samego zlota bez zadnych
                                                        //przemian metali - byc moze to jest prawidlowa odpowiedz
    for(int i = 1 ; i < Vcnt ; ++i)
    {
        //przed dodaniem trzech wspomnianych wyzej elementow sprawdzamy jeszcze czy istnieje w ogole sciezka od zlota do wierzcholka
        //nr "i" i od wierzcholka nr "i" do zlota
        if(spaths_from_gold.pathR(i) != 0 && spaths_others.pathR(i) != 0)
        {
            unsigned long current_total = vertices[i].duty + spaths_from_gold.dist(i) + spaths_others.dist(i);

            if(current_total < min_total_cost)
                min_total_cost = current_total;
        }

    }

    printf("%lu\n", min_total_cost);

    delete [] vertices;
    delete [] edges;

    fclose(input_file);

    return 0;
}
