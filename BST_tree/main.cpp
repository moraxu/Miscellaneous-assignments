#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>

using namespace std;

//szczegoly dzialania BST tree:
//http://en.wikipedia.org/wiki/Binary_search_tree

//--------------------------------------------------------------------------------------------
class BST   //klasa reprezentujaca drzewo BST
{
    struct node //klasa reprezentujaca wezel w drzewie BST
    {
        int key;
        node* left, *right;

        node()
        {
            left = right = 0;
        }

        node(int k, node* l = 0, node* r = 0)
        {
            key = k;
            left = l;
            right = r;
        }
    };

    node* root; //korzen drzewa BST

    //nasz posredni destruktor zwalniajacy pamiec przyznana wezlom drzewa
    void clear(node* start)
    {
        if(start != 0)
        {
            clear(start->left);
            clear(start->right);
            delete start;
        }
    }

    void deleteByMerging(node*& n)  //tzw. usuwanie przez scalanie wybranego wezla "n"
    {
        node* tmp = n;

        if(n != 0)
        {
            if(n->right == 0)   //jesli wezel nie ma prawego syna - jego lewy syn (jesli istnieje)
            {                   //jest dolaczany do jego rodzica
                n = n->left;
            }
            else if(n->left == 0)   //jesli wezel nie ma lewego syna - jego prawy syn (jesli istnieje)
            {                       //jest dolaczany do jego rodzica
                n = n->right;
            }
            else    //scalanie poddrzew
            {
                tmp = n->left;  //przesuwamy sie na lewo

                while(tmp->right != 0)  //a ponizej jak najbardziej na prawo
                    tmp = tmp->right;

                tmp->right = n->right;  //laczymy najbardziej wysuniety na prawo wezel lewego poddrzewa
                                        //z prawym poddrzewem
                tmp = n;
                n = n->left;
            }

            delete tmp;
        }
    }

public:

    BST()
    {
        root = 0;
    }

    ~BST()
    {
        clear();
    }

    void clear()
    {
        clear(root);
        root = 0;
    }

    //funkcja tworzaca drzewo BST z elementow wektora
    void Create(const vector<int>& vec_of_ints)
    {
        for(int i = 0 ; i < vec_of_ints.size() ; ++i)
            Insert(vec_of_ints[i]);
    }

    void Insert(int pValue)
    {
        node* current = root, *prev = 0;

        while(current != 0) //znajdujemy miejsce na wstawienie nowego wezla
        {
            prev = current;

            if(pValue < current->key)
                current = current->left;
            else
                current = current->right;
        }

        if(root == 0)   //jesli drzewo jest puste
            root = new node(pValue);
        else if(pValue < prev->key)
            prev->left = new node(pValue);
        else
            prev->right = new node(pValue);
    }

    bool Find(int pValue) const
    {
        node* current = root;

        while(current != 0)
        {
            if(pValue == current->key)
                return true;
            else if(pValue < current->key)
                current = current->left;
            else
                current = current->right;
        }

        return false;
    }

    bool Remove(int pValue)
    {
        node* current = root, *prev = 0;

        while(current != 0)
        {
            if(current->key == pValue)
                break;

            prev = current;

            if(pValue < current->key)
                current = current->left;
            else
                current = current->right;
        }

        if(current != 0 && current->key == pValue)
        {
            if(current == root)
                deleteByMerging(root);
            else if(prev->left == current)
                deleteByMerging(prev->left);
            else
                deleteByMerging(prev->right);

            return true;
        }
        else
        {
            return false;   //elementu nie ma w drzewie, lub jest ono po prostu puste
        }
    }
};
//--------------------------------------------------------------------------------------------

int main()
{
    ifstream in_str("data.txt");

    if(!in_str.is_open())
    {
        cout << "Wystapil blad podczas otwierania pliku!\n";
        return EXIT_FAILURE;
    }

    vector<int> numbers;
    int temp;

    while(in_str >> temp)   //wczytujemy liczby z pliku do wektora
    {
        numbers.push_back(temp);
    }

    BST bst_tree;
    bst_tree.Create(numbers);   //przekazujemy wektor z liczbami do naszego drzewa BST

    cout << "(Legenda: 0-falsz, 1-prawda)\n";
    cout << "Istnieje element 12: " << bst_tree.Find(12) << "\n"
         << "Istnieje element 234: " << bst_tree.Find(234) << "\n"
         << "Istnieje element 600: " << bst_tree.Find(600) << "\n";

    bst_tree.Insert(600);
    bst_tree.Remove(234);

    cout << "Po modyfikacjach...\n";

    cout << "Istnieje element 600: " << bst_tree.Find(600) << "\n"
         << "Istnieje element 234: " << bst_tree.Find(234) << "\n";

    return EXIT_SUCCESS;
}
