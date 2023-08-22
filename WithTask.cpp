#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include <cstdlib>

using namespace std;


/*Структура Узел:
* key - ключи
* value - значения
 first,second,third - указатели на следующие узлы
 fourth - доп узел
 parent - узел родителя
 n - ко-во ключей
 */
struct TwoThreeNode {

    string key[3];
    string value[3];
    TwoThreeNode* first;
    TwoThreeNode* second;
    TwoThreeNode* third;
    TwoThreeNode* fourth;
    TwoThreeNode* parent;
    int n;


    //конструктор узла с 1 ключом,0 детей
    TwoThreeNode(string key, string value) :key{ key }, value{ value }, first(nullptr), second(nullptr), third(nullptr), fourth(nullptr), parent(nullptr), n(1) {}
    //конструктор узла с 1 ключом,0-2 детей
    TwoThreeNode(string key, string value, TwoThreeNode* first, TwoThreeNode* second, TwoThreeNode* third, TwoThreeNode* fourth, TwoThreeNode* parent, int n) :key{ key }, value{ value }, first(first), second(second), third(third), fourth(fourth), parent(parent), n(n) {}

    bool is_leaf() {
        return (first == nullptr) && (second == nullptr) && (third == nullptr);
    }

    //вставка ключа в узел
    void insert_to_node(string k, string val) {
        key[n] = k;
        value[n] = val;
        n++;
        sort();
    }
    //удаление ключа из узла
    void remove_from_node(string k) {
        if (n >= 1 && key[0] == k) {
            key[0] = key[1];
            value[0] = value[1];
            key[1] = key[2];
            value[1] = value[2];
        }
        else if (n == 2 && key[1] == k) {
            key[1] = key[2];
            value[1] = value[2];
        }
        n--;
    }

    void sort() {
        if (n == 1) return;
        if (n == 2) {
            sort2(key[0], key[1]);
        }
        if (n == 3) {
            sort3(key[0], key[1], key[2]);
        }
    }

    void sort2(string& first, string& second) {
        if (first > second) {
            swap(first, second);
            swap(value[0], value[1]);
        }
    }

    void sort3(string& first, string& second, string& third) {
        if (first > second) {
            swap(first, second);
            swap(value[0], value[1]);
        }
        if (first > third) {
            swap(first, third);
            swap(value[0], value[2]);
        }
        if (second > third) {
            swap(second, third);
            swap(value[1], value[2]);
        }
    }

    void swap(string& first, string& second) {
        string r = first;
        first = second;
        second = r;
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
        if (root != nullptr) {
            destroy(root);
        }
    }
public:
    bool empty() {
        return root == nullptr;
    }
    void print()                        //обход
    {
        print(root);
    }
    void insert(string key, string value) {   //вставка
        root = insert(root, key, value);
    }
    void remove(string key) {               //удаление
        root = remove(root, key);
    }
    bool search(string key)                      //поиск
    {
        if (search(root, key) != nullptr)return true;
        else return false;
    }
    void about(string key) {
        about(root, key);
    }

    void list_words(char key) {
        list_words(find_el(root, key), key);
    }

    void print_info(string key, string value) {
        cout << "=====================\n";
        cout << key << " - " << value << endl;
        cout << "=====================\n";
    }

private:
    void about(TwoThreeNode* r, string key) { //вывод информации о слове
        TwoThreeNode* node = search(r, key);
        if (node->key[0] == key) {
            print_info(key, node->value[0]);
        }
        else {
            print_info(key, node->value[1]);
        }
    }
    TwoThreeNode* find_el(TwoThreeNode* r, char key) {//ф-ия поиск поддерева с заданным символом
        if (r != nullptr) {
            if (r->n == 1)            //2-х узловая вершина
            {
                if (key == (r->key[0])[0])return r;
                else {
                    find_el(r->first, key);

                    find_el(r->second, key);
                }
            }

            else                        //3-х узловая вершина
            {
                if (key == (r->key[0])[0]) return r;
                else if (key == (r->key[1])[0]) return r;
                else {
                    find_el(r->first, key);

                    find_el(r->second, key);

                    find_el(r->third, key);
                }
            }
            return r;
        }
        return nullptr;
    }
    void list_words(TwoThreeNode* r, char key) { //ф-я вывода список слов
        if (r != nullptr)
        {
            if (r->n == 1)            //2-х узловая вершина
            {
                if (key == (r->key[0])[0])print_info(r->key[0], r->value[0]);

                list_words(r->first, key);
                list_words(r->second, key);
            }

            else                        //3-х узловая вершина
            {
                if (key == (r->key[0])[0])print_info(r->key[0], r->value[0]);
                if (key == (r->key[1])[0])print_info(r->key[1], r->value[1]);

                list_words(r->first, key);

                list_words(r->second, key);

                list_words(r->third, key);
            }
        }
        else
            return;
    }
    void print(TwoThreeNode* r)
    {
        if (r != nullptr)
        {
            if (r->n == 1)        //2-х узловая вершина
            {
                cout << "(" << r->key[0] << " - " << r->value[0] << ")" << endl;
                print(r->first);
                print(r->second);
            }

            else if (r->n == 2)                    //3-х узловая вершина
            {
                cout << "(" << r->key[0] << " - " << r->value[0] << ")";
                cout << "(" << r->key[1] << " - " << r->value[1] << ")" << endl;
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
    TwoThreeNode* insert(TwoThreeNode* r, string k, string v) { // вставка ключа k в дерево с корнем r; всегда возвращаем корень дерева, т.к. он может меняться
        if (!r) return new TwoThreeNode(k, v); // если дерево пусто, то создаем первую 2-3-вершину (корень)

        if (r->is_leaf()) r->insert_to_node(k, v);
        else if (k <= r->key[0]) insert(r->first, k, v);
        else if ((r->n == 1) || ((r->n == 2) && k <= r->key[1])) insert(r->second, k, v);
        else insert(r->third, k, v);

        return split(r);
    }
    TwoThreeNode* split(TwoThreeNode* r) {
        // если узел не переполнен
        if (r->n < 3)return r;

        TwoThreeNode* n_1 = new TwoThreeNode(r->key[0], r->value[0], r->first, r->second, nullptr, nullptr, r->parent, 1); //создаём двух детей 
        TwoThreeNode* n_2 = new TwoThreeNode(r->key[2], r->value[2], r->third, r->fourth, nullptr, nullptr, r->parent, 1);

        if (n_1->first)n_1->first->parent = n_1;
        if (n_1->second)n_1->second->parent = n_1;
        if (n_2->first)n_2->first->parent = n_2;
        if (n_2->second)n_2->second->parent = n_2;

        if (r->parent) {          //если есть родитель
            r->parent->insert_to_node(r->key[1], r->value[1]);
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
        else {              //если нет родителя
            n_1->parent = r;
            n_2->parent = r;
            //узел преобразуется в 2-х узловую вершину
            r->key[0] = r->key[1];
            r->value[0] = r->value[1];
            r->first = n_1;
            r->second = n_2;
            r->third = nullptr;
            r->fourth = nullptr;
            r->parent = nullptr;
            r->n = 1;
            return r;
        }

    }
    TwoThreeNode* remove(TwoThreeNode* r, string k) { // Удаление ключа k в 2-3-дереве с корнем p.
        TwoThreeNode* item = search(r, k); // Ищем узел, где находится ключ k

        if (!item) return r;

        TwoThreeNode* min = nullptr;
        if (item->key[0] == k) min = search_min(item->second); // Ищем эквивалентный ключ
        else min = search_min(item->third);

        if (min) {
            //меняем значения заменяемого и удаляемого ключей
            if (k == item->key[0]) {
                string& s = item->key[0];
                string& in = item->value[0];
                item->swap(s, min->key[0]);
                item->swap(in, min->value[0]);
            }
            else {
                string& s = item->key[1];
                string& in = item->value[1];
                item->swap(s, min->key[1]);
                item->swap(in, min->value[1]);
            }

            item = min;// Перемещаем указатель на лист
        }

        item->remove_from_node(k); // И удаляем требуемый ключ из листа
        return fix(item); // Вызываем функцию для восстановления свойств дерева.
    }
    TwoThreeNode* search_min(TwoThreeNode* r) { // Поиск узла с минимальным ключом
        if (!r) return r;
        if (!(r->first)) return r;
        else return search_min(r->first);
    }
    TwoThreeNode* fix(TwoThreeNode* leaf) {
        if (leaf->n == 0 && leaf->parent == nullptr) { // Случай 1, когда удаляем единственный ключ в дереве
            delete leaf;
            return nullptr;
        }
        if (leaf->n != 0) { // Случай 2, когда вершина, в которой удалили ключ, имела два ключа
            if (leaf->parent) return fix(leaf->parent);
            else return leaf;
        }

        TwoThreeNode* parent = leaf->parent;
        if (parent->first->n == 2 || parent->second->n == 2 || parent->n == 2) leaf = redistribute(leaf); // Случай 3, когда достаточно перераспределить ключи в дереве
        else if (parent->n == 2 && parent->third->n == 2) leaf = redistribute(leaf); // Аналогично
        else leaf = merge(leaf); // Случай 4, когда нужно произвести склеивание и пройтись вверх по дереву как минимум на еще одну вершину

        return fix(leaf);
    }
    TwoThreeNode* redistribute(TwoThreeNode* leaf) {
        TwoThreeNode* parent = leaf->parent;
        TwoThreeNode* first = parent->first;
        TwoThreeNode* second = parent->second;
        TwoThreeNode* third = parent->third;

        if ((parent->n == 2) && (first->n < 2) && (second->n < 2) && (third->n < 2)) {
            if (first == leaf) {
                parent->first = parent->second;
                parent->second = parent->third;
                parent->third = nullptr;
                parent->first->insert_to_node(parent->key[0], parent->value[0]);
                parent->first->third = parent->first->second;
                parent->first->second = parent->first->first;

                if (leaf->first != nullptr) parent->first->first = leaf->first;
                else if (leaf->second != nullptr) parent->first->first = leaf->second;

                if (parent->first->first != nullptr) parent->first->first->parent = parent->first;

                parent->remove_from_node(parent->key[0]);
                delete first;
            }
            else if (second == leaf) {
                first->insert_to_node(parent->key[0], parent->value[0]);
                parent->remove_from_node(parent->key[0]);
                if (leaf->first != nullptr) first->third = leaf->first;
                else if (leaf->second != nullptr) first->third = leaf->second;

                if (first->third != nullptr) first->third->parent = first;

                parent->second = parent->third;
                parent->third = nullptr;

                delete second;
            }
            else if (third == leaf) {
                second->insert_to_node(parent->key[1], parent->value[1]);
                parent->third = nullptr;
                parent->remove_from_node(parent->key[1]);
                if (leaf->first != nullptr) second->third = leaf->first;
                else if (leaf->second != nullptr) second->third = leaf->second;

                if (second->third != nullptr)  second->third->parent = second;

                delete third;
            }
        }
        else if ((parent->n == 2) && ((first->n == 2) || (second->n == 2) || (third->n == 2))) {
            if (third == leaf) {
                if (leaf->first != nullptr) {
                    leaf->second = leaf->first;
                    leaf->first = nullptr;
                }

                leaf->insert_to_node(parent->key[1], parent->value[1]);
                if (second->n == 2) {
                    parent->key[1] = second->key[1];
                    parent->value[1] = second->value[1];
                    second->remove_from_node(second->key[1]);
                    leaf->first = second->third;
                    second->third = nullptr;
                    if (leaf->first != nullptr) leaf->first->parent = leaf;
                }
                else if (first->n == 2) {
                    parent->key[1] = second->key[0];
                    parent->value[1] = second->value[0];
                    leaf->first = second->second;
                    second->second = second->first;
                    if (leaf->first != nullptr) leaf->first->parent = leaf;

                    second->key[0] = parent->key[0];
                    second->value[0] = parent->key[0];
                    parent->key[0] = first->key[1];
                    parent->value[0] = first->value[1];
                    first->remove_from_node(first->key[1]);
                    second->first = first->third;
                    if (second->first != nullptr) second->first->parent = second;
                    first->third = nullptr;
                }
            }
            else if (second == leaf) {
                if (third->n == 2) {
                    if (leaf->first == nullptr) {
                        leaf->first = leaf->second;
                        leaf->second = nullptr;
                    }
                    second->insert_to_node(parent->key[1], parent->value[1]);
                    parent->key[1] = third->key[0];
                    parent->value[1] = third->value[0];
                    third->remove_from_node(third->key[0]);
                    second->second = third->first;
                    if (second->second != nullptr) second->second->parent = second;
                    third->first = third->second;
                    third->second = third->third;
                    third->third = nullptr;
                }
                else if (first->n == 2) {
                    if (leaf->second == nullptr) {
                        leaf->second = leaf->first;
                        leaf->first = nullptr;
                    }
                    second->insert_to_node(parent->key[0], parent->value[0]);
                    parent->key[0] = first->key[1];
                    parent->value[0] = first->value[1];
                    first->remove_from_node(first->key[1]);
                    second->first = first->third;
                    if (second->first != nullptr) second->first->parent = second;
                    first->third = nullptr;
                }
            }
            else if (first == leaf) {
                if (leaf->first == nullptr) {
                    leaf->first = leaf->second;
                    leaf->second = nullptr;
                }
                first->insert_to_node(parent->key[0], parent->value[0]);
                if (second->n == 2) {
                    parent->key[0] = second->key[0];
                    parent->value[0] = second->value[0];
                    second->remove_from_node(second->key[0]);
                    first->second = second->first;
                    if (first->second != nullptr) first->second->parent = first;
                    second->first = second->second;
                    second->second = second->third;
                    second->third = nullptr;
                }
                else if (third->n == 2) {
                    parent->key[0] = second->key[0];
                    parent->value[0] = second->value[0];
                    second->key[0] = parent->key[1];
                    second->value[0] = parent->value[1];
                    parent->key[1] = third->key[0];
                    parent->value[1] = third->value[0];
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
            leaf->insert_to_node(parent->key[0], parent->value[0]);

            if (first == leaf && second->n == 2) {
                parent->key[0] = second->key[0];
                parent->value[0] = second->value[0];
                second->remove_from_node(second->key[0]);

                if (leaf->first == nullptr) leaf->first = leaf->second;

                leaf->second = second->first;
                second->first = second->second;
                second->second = second->third;
                second->third = nullptr;
                if (leaf->second != nullptr) leaf->second->parent = leaf;
            }
            else if (second == leaf && first->n == 2) {
                parent->key[0] = first->key[1];
                parent->value[0] = first->value[1];
                first->remove_from_node(first->key[1]);

                if (leaf->second == nullptr) leaf->second = leaf->first;

                leaf->first = first->third;
                first->third = nullptr;
                if (leaf->first != nullptr) leaf->first->parent = leaf;
            }
        }
        return parent;
    }
    TwoThreeNode* merge(TwoThreeNode* leaf) {
        TwoThreeNode* parent = leaf->parent;

        if (parent->first == leaf) {
            parent->second->insert_to_node(parent->key[0], parent->value[0]);
            parent->second->third = parent->second->second;
            parent->second->second = parent->second->first;

            if (leaf->first != nullptr) parent->second->first = leaf->first;
            else if (leaf->second != nullptr) parent->second->first = leaf->second;

            if (parent->second->first != nullptr) parent->second->first->parent = parent->second;

            parent->remove_from_node(parent->key[0]);
            delete parent->first;
            parent->first = nullptr;
        }
        else if (parent->second == leaf) {
            parent->first->insert_to_node(parent->key[0], parent->value[0]);

            if (leaf->first != nullptr) parent->first->third = leaf->first;
            else if (leaf->second != nullptr) parent->first->third = leaf->second;

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
    TwoThreeNode* search(TwoThreeNode* r, string key) { // Поиск ключа k в 2-3 дереве с корнем r.
        if (r != nullptr) {
            if (r->n == 1) {        //2-х узловая вершина
                if (key == r->key[0])
                    return r;

                else if (key < r->key[0]) {
                    return search(r->first, key);
                }

                else {
                    return search(r->second, key);
                }
            }

            else {                        //3-х узловая вершина
                if (key == r->key[0] || key == r->key[1])
                    return r;

                else if (key < r->key[0]) {
                    return search(r->first, key);
                }

                else if (key < r->key[1]) {
                    return search(r->second, key);
                }

                else
                    return search(r->third, key);
            }
        }
        else return nullptr;
    }
};

int main()
{
    system("color F0");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    TwoThreeTree tree1;
    int userInput;
    string key, value;
    cout << "============ Англо - Русский словарь ================";
    cout << "\nДоступные команды : ";
    cout << "\n1 - Вставка";
    cout << "\n2 - Удаление";
    cout << "\n3 - Поиск";
    cout << "\n4 - Вывод cловаря";
    cout << "\n5 - Вывод слов на букву";
    cout << "\n6 - Список доступных команд";
    cout << "\n0 - Выход";
    ifstream in("Dict.txt");
    if (!in.is_open()) {
        cout << "Файл не был открыт";
    }
    else {
        while (!in.eof()) {
            getline(in, key);
            getline(in, value);
            tree1.insert(key, value);
        }

    }
    while (true)
    {
        cout << "\nВведите команду : ";
        cin >> userInput;
        switch (userInput) {
            case 1: {
                cout << "\nВведите английское слово и его перевод на русский язык : ";
                cin >> key >> value;

                if (!tree1.search(key)) {
                    tree1.insert(key, value);
                    cout << "\nСлово добавлено";
                }
                else cout << "\nДанное слово уже добавлено в словарь";

                break;
            }
            case 2: {
                cout << "\nВведите слово,которое хотите удалить из словаря : ";
                cin >> key;

                tree1.remove(key);
                cout << "\nСлово удалено";

                break;
            }
            case 3: {
                cout << "\nВведите слово,которое хотите найти : ";
                cin >> key;

                if (tree1.search(key))
                    tree1.about(key);
                else
                    cout << "\nСлово не найдено. ";
                break;
            }
            case 4: {
                tree1.print();
                break;
            }
            case 5: {
                cout << "\nВведите символ : ";
                string c;
                cin >> c;
                if (c.length() == 1)tree1.list_words(c[0]);
                else cout << "\nНекорректный ввод символа";
                break;
            }
            case 6: {
                cout << "\nДоступные команды : ";
                cout << "\n1 - Вставка";
                cout << "\n2 - Удаление";
                cout << "\n3 - Поиск";
                cout << "\n4 - Вывод словаря";
                cout << "\n5 - Вывод слов на заданную букву";
                cout << "\n6 - Список доступных команд";
                cout << "\n0 - Выход";
                break;
            }
            case 0: {
                return 0;
                break;
            }
            default: {
                cout << "\nНеправильно введённая команда ";
                break;
            }
        }
        
    }
};