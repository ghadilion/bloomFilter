#include <iostream>
#include <fstream>
#include <string>
#include "BPlusTree.h"

using namespace std;

void print_bool(bool cond)
{
    if (cond)
    {
        std::cout << "True\n";
    }
    else
    {
        std::cout << "False\n";
    }
}

uint64_t djb2(char *str)
{
    uint64_t hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

BPlusTree<uint64_t> bpt(15);

void insert(char *fileName)
{
    ifstream fin;
    fin.open(fileName);
    char word[32];
    uint64_t key;
    while (fin >> word)
    {
        // cout << word << " " << key << " " << endl;
        key = djb2(word);
        bpt.insert(key);
    }
    fin.close();
}

int main(int argc, char *argv[])
{

    insert(argv[1]);

    char again = 'y';
    do
    {
        char c[32];
        cout << "\nEnter the Data to be Searched: ";
        cin >> c;
        uint64_t key = djb2(c);
        if (bpt.search(key))
            cout << "Search string found!\n";
        else
            cout << "Search string not found!\n";
        cout << "Again?(y/n): ";
        cin >> again;

    } while (again == 'y');
}