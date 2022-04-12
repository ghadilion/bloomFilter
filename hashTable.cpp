#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

#define LOADFACTORTHRESHOLD 0.75

uint32_t djb2(string str) {
    uint32_t hash = 5381;
    for (string::iterator it = str.begin(); it != str.end(); ++it)
        hash = ((hash << 5) + hash) + *it; /* hash * 33 + *it */
    return hash;
}

class node {
public:
    string data;
    node *next;
    node(string);
};

node::node(string data) {
    this->data = data;
    next = NULL;
}

class linkedList {
    node *head;
public:
    linkedList();
    void insert(string);
    bool search(string);
    void print();
};

linkedList::linkedList() {
    head = NULL;
}

void linkedList::insert(string data) {
    node *newNode = new node(data);
    if(head) {
        node *temp = head;
        while(temp->next)
            temp = temp->next;
        temp->next = newNode;
    }
    else
        head = newNode;
}

bool linkedList::search(string data) {
    node *temp = head;
    while(temp)
        if(temp->data == data)
            return true;
    return false;
}

void linkedList::print() {
    node *temp = head;
    while(temp) {
        cout << temp->data << "\t";
        temp = temp->next;
    }
}

class hashTable {
    linkedList *arr;
    int hashTableSize;
public:
    hashTable(int);
    void insert(char*);
    void find();
    void print();
};

hashTable::hashTable(int inputSize) {
    hashTableSize = (float) inputSize / LOADFACTORTHRESHOLD;
    arr = new linkedList[hashTableSize];
}

void hashTable::insert(char* fileName) {
    ifstream  fin;
    fin.open(fileName);
    string value;
    uint32_t key;
    while(fin >> value) {
        key = djb2(value) % hashTableSize;
        arr[key].insert(value);
        // cout << "\n" << key << " " << value;
    }
    fin.close();
}

void hashTable::find() {
    cout << "\nEnter search string: ";
    string search_string;
    cin >> search_string;
    uint32_t search_key = djb2(search_string) % hashTableSize;
    if(arr[search_key].search(search_string))
        cout << search_string << " found at position " << search_key << endl;
    else
        cout << search_string << " not found!" << endl;
}

void hashTable::print() {
    for(int i = 0; i < hashTableSize; ++i) {
        cout << i << "\t";
        arr[i].print();
        cout << endl;
    }
}

int main(int argc, char* argv[]) {
    int inputSize = atoi(argv[1]);
    char* fileName = argv[2];
    hashTable H(inputSize);
    H.insert(fileName);
    char c = 'y';
    while(c == 'y') {
        H.find();
        cout << "\nSearch again? (y/n): ";
        cin >> c;
    }
    H.print();
}