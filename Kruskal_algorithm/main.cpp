#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>

using namespace std;

//szczegoly dzialania algorytmu Kruskala:
//http://en.wikipedia.org/wiki/Kruskal's_algorithm

///////////////////////////////////////////////////////////////////////
class Weighted_edge //klasa reprezentujaca wazona krawedz pomiedzy dwoma wierzcholkami grafu
{
    int vertex_v, vertex_w;
    int edge_weight;

public:
    Weighted_edge(int v, int w, int weight) : vertex_v(v), vertex_w(w), edge_weight(weight) { }

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

    int other(int vertex) const //funkcja zwracajaca przeciwlegly wierzcholek krawedzi do wierzcholka nr "vertex"
    {
        if(vertex == vertex_v)
            return vertex_w;
        else
            return vertex_v;
    }

    friend std::ostream& operator<<(std::ostream& str, const Weighted_edge& e)  //operator<< wypisujacy dane krawedzi
    {
        str << e.vertex_v+1 << '-' << e.vertex_w+1 << " [waga: " << e.edge_weight << "]";
        return str;
    }

    bool operator<(const Weighted_edge& e2) const
    {
        return edge_weight < e2.edge_weight;
    }
};
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
template <class Weighted_edge>
class SparseMultiWeightedGRAPH_vectors   //klasa reprezentujaca graf oparty na tzw. liscie sasiedztwa
{
    int Vcnt, Ecnt;

    std::vector<std::vector<Weighted_edge*> > adj;

public:
    SparseMultiWeightedGRAPH_vectors(int V) : Vcnt(V), Ecnt(0), adj(V)
    {   }

    int V() const { return Vcnt; }
    int E() const { return Ecnt; }

    void insert(Weighted_edge* e)   //funkcja wstawiajaca adres krawedzi do reprezentacji grafu
    {
        int v = e->v(), w = e->w();
        adj[v].push_back(e);
        adj[w].push_back(e);
        ++Ecnt;
    }

    friend class adjIterator;

    class adjIterator   //klasa ta to typ, ktory definiujemy jesli chcemy iterowac po sasiadach danego wierzcholka
    {
        const SparseMultiWeightedGRAPH_vectors<Weighted_edge>& G;
        int v;
        typename std::vector<Weighted_edge*>::size_type it;

    public:
        //"v" to numer wierzcholka, po ktorego sasiadach chcemy iterowac
        adjIterator(const SparseMultiWeightedGRAPH_vectors<Weighted_edge>& G, int v) : G(G), v(v), it(0) { }

        Weighted_edge* beg()
        {
            return (G.adj[v].empty() ? 0 : G.adj[v][it=0]);
        }

        Weighted_edge* nxt()
        {
            return (++it != G.adj[v].size() ? G.adj[v][it] : 0);
        }

        bool end()
        {
            return it == G.adj[v].size();
        }
    };
};
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
class UF                        //weighted quick-union
{                               //(http://pl.wikipedia.org/wiki/Struktura_zbior%C3%B3w_roz%C5%82%C4%85cznych)
    std::vector<int> id, size;

public:
    UF(int N) : id(N), size(N, 1)
    {
        for(int i = 0 ; i < N ; ++i)
            id[i] = i;
    }

    int find(int x) const
    {
        while(x != id[x])
            x = id[x];

        return x;
    }

    bool connected(int p, int q) const
    {
        return find(p) == find(q);
    }

    void unite(int p, int q)
    {
        int i = find(p), j = find(q);

        if(i == j)  //p i q sa juz polaczone
            return;

        if(size[i] < size[j])   //zawsze podlaczamy mniejsze drzewo do wiekszego
        {
            id[i] = j;
            size[j] += size[i];
        }
        else
        {
            id[j] = i;
            size[i] += size[j];
        }
    }
};
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
//ta funkcja zwraca nam wektor krawedzi danego grafu G
template <class Graph, class Weighted_edge, class Weighted_edge_ptr>
std::vector<Weighted_edge_ptr> return_weighted_edges(const Graph& G)
{
    std::vector<Weighted_edge_ptr> edges(G.E());

    for(int v = 0, E = 0 ; v < G.V() ; ++v)
    {
        typename Graph::adjIterator A(G, v);
        for(Weighted_edge* e = A.beg() ; !A.end() ; e = A.nxt())
        {
            if(v < e->other(v))
                edges[E++] = Weighted_edge_ptr(e);
        }
    }

    return edges;
}
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
template <class Graph, class Weighted_edge> //preprocessing time (assuming that UF is a weighted
class MST_Kruskal                           //quick-union algorithm) - O[E(lgE + lgV)], space - O(V + E)
{
    //musimy stworzyc kolejny typ do przechowywania samych adresow krawedzi w grafie
    //aby moc umiescic w nim operator< - przyda sie nam on w funkcji sortujacej krawedzie wzgledem ich wagi
    struct Weighted_edge_ptr
    {
        Weighted_edge* pointer;

        Weighted_edge_ptr(Weighted_edge* p = 0) : pointer(p) { }

        bool operator<(const Weighted_edge_ptr& ptr2) const
        {
            return *pointer < *ptr2.pointer;
        }
    };

    const Graph& G;
    int edges_in_mst;
    std::vector<Weighted_edge_ptr> mst; //wektor przechowujacy wskazniki do krawedzi wchodzacych w sklad MST

    void kruskal_algorithm()
    {
        std::vector<Weighted_edge_ptr> edge_pointers = return_weighted_edges<Graph, Weighted_edge, Weighted_edge_ptr>(G);
        UF uf(G.V());

        std::sort(edge_pointers.begin(), edge_pointers.end());  //sortujemy krawedzie grafu wzgledem ich wagi

        for(int i = 0, V = G.V(), E = G.E() ; i < E && edges_in_mst < (V-1) ; ++i)
        {
            if(!uf.connected(edge_pointers[i].pointer->v(), edge_pointers[i].pointer->w())) //jesli wierzcholki v i w sa juz polaczone,
            {                                                                               //to polaczenie ich bezposrednio spowodowaloby cykl
                uf.unite(edge_pointers[i].pointer->v(), edge_pointers[i].pointer->w());
                mst[edges_in_mst++] = edge_pointers[i];
            }
        }
    }

public:
    MST_Kruskal(const Graph& G) : G(G), edges_in_mst(0), mst(G.V())
    {
        kruskal_algorithm();
    }

    void show() const
    {
        for(int e = 0 ; e < edges_in_mst ; ++e) //drukujemy krawedzie grafu wchodzace w sklad jego MST
        {
            std::cout << "Krawedz: " << *mst[e].pointer << '\n';
        }
    }
};
///////////////////////////////////////////////////////////////////////

int main()
{
    ifstream in_str("dane.txt");
    string line;

    if(!in_str.is_open())
    {
        cout << "Wystapil blad podczas otwierania pliku!\n";
        return EXIT_FAILURE;
    }

    int vertices;
    in_str >> vertices;

    getline(in_str, line, '\n');    //ignorujemy reszte znakow w pierwszej linii pliku, az do napotkania znaku nowej linii

    SparseMultiWeightedGRAPH_vectors<Weighted_edge> graph(vertices);

    vector<Weighted_edge> edges;

    //kazda nastepna linijke w pliku, zawierajaca numery wierzcholkow i wagi krawedzi,
    //wczytujemy do stringu "line"...
    for(int vertex = 0 ; !getline(in_str, line, '\n').eof() ; ++vertex)
    {
        //...a nastepnie z tego stringu "line" wydobywamy (za pomoca
        //tzw. strumienia czytajacego ze stringu - istringstream) kolejne dane...
        istringstream in_string_str(line);

        int to_where, weight;
        char temp;

        //...w tej oto petli, az do wyczerpania danej linijki
        while(in_string_str >> to_where >> weight >> temp)
        {
            edges.push_back(Weighted_edge(vertex, to_where-1, weight));
        }
    }

    for(int i = 0 ; i < edges.size() ; ++i) //wstawiamy krawedzie do grafu
        graph.insert(&edges[i]);

    //uruchamiamy algorytm Kruskala
    MST_Kruskal<SparseMultiWeightedGRAPH_vectors<Weighted_edge>, Weighted_edge> kruskal_algorithm(graph);

    kruskal_algorithm.show();   //wyswietlamy krawedzie naszego grafu, ktore wchodza w sklad jego MST

    return EXIT_SUCCESS;
}
