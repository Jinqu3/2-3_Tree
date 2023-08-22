#include <iostream>
#include <windows.h>
#include <fstream>
using namespace std;
/*Структура Узел:
* key - ключи
 first,second,fird - указатели на следующие узлы
 fourth - доп узел
 parent - узел родителя
 n - ко-во ключей
 */
struct TwoThreeNode {
    int key[3];
    //поле для даты
    TwoThreeNode* first;
    TwoThreeNode* second;
    TwoThreeNode* third;
    TwoThreeNode* fourth;
    TwoThreeNode* parent;
    int n;

    //node constructor with 1 key,0 child
    TwoThreeNode(int k) :key{ k, 0, 0 }, first(nullptr), second(nullptr), third(nullptr), fourth(nullptr), parent(nullptr), n(1) {}
    //node constructor with 1 key,0-2 child
    TwoThreeNode(int k, TwoThreeNode* first, TwoThreeNode* second, TwoThreeNode* third, TwoThreeNode* fourth, TwoThreeNode* parent, int n) :key{ k, 0, 0 }, first(first), second(second), third(third), fourth(fourth), parent(parent), n(n) {}


    bool is_leaf() {
        return (first == nullptr) && (second == nullptr) && (third == nullptr);
    }

    //insert to node(not in the tree)
    void insert_to_node(int k) {  
        key[n] = k;
        n++;
        sort();
    }

    void remove_from_node(int k) {
        if (n >= 1 && key[0] == k) {
            key[0] = key[1];
            key[1] = key[2];
        }
        else if (n == 2 && key[1] == k) {
            key[1] = key[2];
        }
        n--;
    }
    void sort() {
        if (n == 1) return;
        if (n == 2) sort2(key[0], key[1]);
        if (n == 3) sort3(key[0], key[1], key[2]);
    }

    void sort2(int& x, int& y) {
        if (x > y) swap(x, y);
    }

    void sort3(int& x, int& y, int& z) {
        if (x > y) swap(x, y);
        if (x > z) swap(x, z);
        if (y > z) swap(y, z);
    }

    void swap(int& x, int& y) {
        int r = x;
        x = y;
        y = r;
    }
    
};


/*Класс 2-3 дерева:
 root - корень дерева
*/
class TwoThreeTree {
private:
    TwoThreeNode* root;
public:
    TwoThreeTree() {
        root = nullptr;
    }
    ~TwoThreeTree() {
        if (root != nullptr){
            destroy(root);
        }
    }
public:
    void print()                        //print
    {
        print(root);
    }  
    void insert(int key) {                    //insert
        if (!search(root, key))root = insert(root, key);
        else cout << "can't insert,the node with this key already exist\n";
    }
    void remove(int key) {               //delete
        root = remove(root, key);
    }
    bool searchFor(int key)                      //search
    {
        if (search(root,key) != nullptr)return true;
        else return false;
    }
private:
    void print(TwoThreeNode* r)
    {
        if (r != nullptr)
        {
            if (r->n == 1)        //2-х узловая вершина
            {
                cout << " " << r->key[0] << endl;
                print(r->first);
                print(r->second);
            }

            else if(r->n ==2)                    //3-х узловая вершина
            {
                cout << " " << r->key[0];
                cout << " " << r->key[1] << endl;
                print(r->first);
                print(r->second);
                print(r->third);
            }
        }
    }
    void destroy(TwoThreeNode* r) {

        if (r != nullptr) {

            destroy(r->first);
            destroy(r->second);
            destroy(r->third);
            delete r;

        }
    }
    TwoThreeNode * insert(TwoThreeNode* r,int key) {
        if (r == nullptr)return new TwoThreeNode(key);
        if (r->is_leaf())r->insert_to_node(key);
        else if (key < r->key[0])insert(r->first, key);//goes left
        else if (r->n == 1 || r->n == 2 && key < r->key[1])insert(r->second, key);//goes middle
        else insert(r->third, key);//goes right

        return split(r);
    }
    TwoThreeNode* split(TwoThreeNode* r) {
        // owerflow
        if (r->n < 3)return r;

        TwoThreeNode* n_1 = new TwoThreeNode(r->key[0],r->first,r->second,nullptr,nullptr,r->parent,1);
        TwoThreeNode* n_2 = new TwoThreeNode(r->key[2], r->third, r->fourth, nullptr, nullptr, r->parent, 1);
        
        if (n_1->first)n_1->first->parent = n_1;
        if (n_1->second)n_1->second->parent = n_1;
        if (n_2->first)n_2->first->parent = n_2;
        if (n_2->second)n_2->second->parent = n_2;
        //если есть родитель
        if (r->parent) {
            r->parent->insert_to_node(r->key[1]);
            if (r->parent->first == r) {
                r->parent->fourth = r->parent->third;
                r->parent->third = r->parent->second;
                r->parent->first = n_1;
                r->parent->second = n_2;
            }
            else if (r->parent->second == r) {
                r->parent->fourth = r->parent->third;
                r->parent->second = n_1;
                r->parent->third = n_2;
            }
            else if (r->parent->third == r) {
                r->parent->third = n_1;
                r->parent->fourth = n_2;
            }

            TwoThreeNode* parent = r->parent;
            delete r;
            return parent;
        }
        else {
            n_1->parent = r;
            n_2->parent = r;
            //узел преобразуется в 2-х узловую вершину
            r->key[0] = r->key[1];
            r->first = n_1;
            r->second = n_2;
            r->third = nullptr;
            r->fourth = nullptr;
            r->parent = nullptr;
            r->n = 1;
            return r;
        }

    }
    TwoThreeNode * remove(TwoThreeNode* r, int key) {
        TwoThreeNode* node = search(r, key);

        if (!node)return r;

        TwoThreeNode* min_node;
        if (r->key[0] == key) min_node = min_in_first(r->second);
        else min_node = min_in_first(r->third);

        
        if (min_node) {                                                     //меняем значения заменяемого и удаляемого ключей
            int& s = key == node->key[0] ? node->key[0] : node->key[1];
            node->swap(s, min_node->key[0]);
            node = min_node;
        }

        node->remove_from_node(key);
        return fix(node); //для заменяющего узла вызываем вызываем ф-ю для восстановления баланса
    }
    TwoThreeNode* fix(TwoThreeNode* r) {
        if (r->n == 0 && r->parent == nullptr) {        //Случай,когда удаляем единственный ключ в дереве
            delete r;
            return nullptr;
        }
        if (r->n != 0) {                                  //Случай,когда остался один ключ
            if (r->parent) return fix(r->parent);
            else return r;
        }

        TwoThreeNode* parent = r->parent;
        if (parent->first->n == 2 || parent->second->n == 2 || parent->n == 2) r = redistribute(r);         // Случай когда достаточно перераспределить ключи в дереве
        else if (parent->n == 2 && parent->third->n == 2) r = redistribute(r);                              //Случай когда достаточно перераспределить ключи в дереве
        else r = merge(r);                                                          // Случай , когда нужно произвести склеивание и пройтись вверх по дереву как минимум на еще одну вершину
    }
    TwoThreeNode* redistribute(TwoThreeNode* r) {
        TwoThreeNode* parent = r->parent;
        TwoThreeNode* first = parent->first;
        TwoThreeNode* second = parent->second;
        TwoThreeNode* third = parent->third;

        if ((parent->n == 2) && (first->n < 2) && (second->n < 2) && (third->n < 2)) {
            if (first == r) {
                parent->first = parent->second;
                parent->second = parent->third;
                parent->third = nullptr;
                parent->first->insert_to_node(parent->key[0]);
                parent->first->third = parent->first->second;
                parent->first->second = parent->first->first;

                if (r->first != nullptr) parent->first->first = r->first;
                else if (r->second != nullptr) parent->first->first = r->second;

                if (parent->first->first != nullptr) parent->first->first->parent = parent->first;

                parent->remove_from_node(parent->key[0]);
                delete first;
            }
            else if (second == r) {
                first->insert_to_node(parent->key[0]);
                parent->remove_from_node(parent->key[0]);
                if (r->first != nullptr) first->third = r->first;
                else if (r->second != nullptr) first->third = r->second;

                if (first->third != nullptr) first->third->parent = first;

                parent->second = parent->third;
                parent->third = nullptr;

                delete second;
            }
            else if (third == r) {
                second->insert_to_node(parent->key[1]);
                parent->third = nullptr;
                parent->remove_from_node(parent->key[1]);
                if (r->first != nullptr) second->third = r->first;
                else if (r->second != nullptr) second->third = r->second;

                if (second->third != nullptr)  second->third->parent = second;

                delete third;
            }
        }
        else if ((parent->n == 2) && ((first->n == 2) || (second->n == 2) || (third->n == 2))) {
            if (third == r) {
                if (r->first != nullptr) {
                    r->second = r->first;
                    r->first = nullptr;
                }

                r->insert_to_node(parent->key[1]);
                if (second->n == 2) {
                    parent->key[1] = second->key[1];
                    second->remove_from_node(second->key[1]);
                    r->first = second->third;
                    second->third = nullptr;
                    if (r->first != nullptr) r->first->parent = r;
                }
                else if (first->n == 2) {
                    parent->key[1] = second->key[0];
                    r->first = second->second;
                    second->second = second->first;
                    if (r->first != nullptr) r->first->parent = r;

                    second->key[0] = parent->key[0];
                    parent->key[0] = first->key[1];
                    first->remove_from_node(first->key[1]);
                    second->first = first->third;
                    if (second->first != nullptr) second->first->parent = second;
                    first->third = nullptr;
                }
            }
            else if (second == r) {
                if (third->n == 2) {
                    if (r->first == nullptr) {
                        r->first = r->second;
                        r->second = nullptr;
                    }
                    second->insert_to_node(parent->key[1]);
                    parent->key[1] = third->key[0];
                    third->remove_from_node(third->key[0]);
                    second->second = third->first;
                    if (second->second != nullptr) second->second->parent = second;
                    third->first = third->second;
                    third->second = third->third;
                    third->third = nullptr;
                }
                else if (first->n == 2) {
                    if (r->second == nullptr) {
                        r->second = r->first;
                        r->first = nullptr;
                    }
                    second->insert_to_node(parent->key[0]);
                    parent->key[0] = first->key[1];
                    first->remove_from_node(first->key[1]);
                    second->first = first->third;
                    if (second->first != nullptr) second->first->parent = second;
                    first->third = nullptr;
                }
            }
            else if (first == r) {
                if (r->first == nullptr) {
                    r->first = r->second;
                    r->second = nullptr;
                }
                first->insert_to_node(parent->key[0]);
                if (second->n == 2) {
                    parent->key[0] = second->key[0];
                    second->remove_from_node(second->key[0]);
                    first->second = second->first;
                    if (first->second != nullptr) first->second->parent = first;
                    second->first = second->second;
                    second->second = second->third;
                    second->third = nullptr;
                }
                else if (third->n == 2) {
                    parent->key[0] = second->key[0];
                    second->key[0] = parent->key[1];
                    parent->key[1] = third->key[0];
                    third->remove_from_node(third->key[0]);
                    first->second = second->first;
                    if (first->second != nullptr) first->second->parent = first;
                    second->first = second->second;
                    second->second = third->first;
                    if (second->second != nullptr) second->second->parent = second;
                    third->first = third->second;
                    third->second = third->third;
                    third->third = nullptr;
                }
            }
        }
        else if (parent->n == 1) {
            r->insert_to_node(parent->key[0]);

            if (first == r && second->n == 2) {
                parent->key[0] = second->key[0];
                second->remove_from_node(second->key[0]);

                if (r->first == nullptr) r->first = r->second;

                r->second = second->first;
                second->first = second->second;
                second->second = second->third;
                second->third = nullptr;
                if (r->second != nullptr) r->second->parent = r;
            }
            else if (second == r && first->n == 2) {
                parent->key[0] = first->key[1];
                first->remove_from_node(first->key[1]);

                if (r->second == nullptr) r->second = r->first;

                r->first = first->third;
                first->third = nullptr;
                if (r->first != nullptr) r->first->parent = r;
            }
        }
        return parent;
    }
    TwoThreeNode* merge(TwoThreeNode* r) {
        TwoThreeNode* parent = r->parent;

        if (parent->first == r) {
            parent->second->insert_to_node(parent->key[0]);
            parent->second->third = parent->second->second;
            parent->second->second = parent->second->first;

            if (r->first != nullptr) parent->second->first = r->first;
            else if (r->second != nullptr) parent->second->first = r->second;

            if (parent->second->first != nullptr) parent->second->first->parent = parent->second;

            parent->remove_from_node(parent->key[0]);
            delete parent->first;
            parent->first = nullptr;
        }
        else if (parent->second == r) {
            parent->first->insert_to_node(parent->key[0]);

            if (r->first != nullptr) parent->first->third = r->first;
            else if (r->second != nullptr) parent->first->third = r->second;

            if (parent->first->third != nullptr) parent->first->third->parent = parent->first;

            parent->remove_from_node(parent->key[0]);
            delete parent->second;
            parent->second = nullptr;
        }

        if (parent->parent == nullptr) {
            TwoThreeNode* tmp = nullptr;
            if (parent->first != nullptr) tmp = parent->first;
            else tmp = parent->second;
            tmp->parent = nullptr;
            delete parent;
            return tmp;
        }
        return parent;
    }
    TwoThreeNode * search(TwoThreeNode* r, int key)
    {
        if (r != nullptr)
        {
            if (r->n == 1)            //Two Node
            {
                if (key == r->key[0])
                    return r;

                else if (key < r->key[0])
                {
                    return search(r->first, key);
                }

                else
                {
                    return search(r->second, key);
                }
            }

            else                        //Three Node
            {
                if (key == r->key[0] || key == r->key[1])
                    return r;

                else if (key < r->key[0])
                {
                    return search(r->first, key);
                }

                else if (key < r->key[1])
                {
                    return search(r->second, key);
                }

                else
                    return search(r->third, key);
            }
        }

        else
            return nullptr;
    }
    TwoThreeNode* min_in_first(TwoThreeNode* r) {               //здесь я выбрал минимальный из средней ветви(в 2 - х узловом родителе) или из правой ветви(из 3 - х узлового узла)
        if (r == nullptr || r->first == nullptr)return r;       //потому что в ином случае приходится проверять на тип узла(кол-во ключей), чтобы понять где искать максимальный (в среднем узле или правом)
        else return min_in_first(r);                            //в этом случае всё тривиально и мы постоянно ищем по левой ветви
    }
};
void driver()
{
    TwoThreeTree tree1;

    int userInput = 0;
    int data;
    ifstream in("Dict.txt");
    while (userInput != -1)
    {
        cout << "\nВведите команду : ";
        cin >> userInput;

        if (userInput == 1) {
            if (in.is_open()) {
                while (in >> data) {

                    tree1.insert(data);
                }
            }
        }

        else if (userInput == 2)
        {
            cout << "\nEnter the data to be deleted from the Tree : ";
            cin >> data;

            tree1.remove(data);

            cout << "\nAfter deletion :";
            tree1.print();
        }

        else if (userInput == 3)
        {
            cout << "\nEnter the data element which is to be searched : ";
            cin >> data;

            if (tree1.searchFor(data))
                cout << "\nThe element is found. ";
            else
                cout << "\nThe element is not found. ";
        }

        else if (userInput == 4)
        {
            tree1.print();
        }
    }
}
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    driver();
    return 0;
};
