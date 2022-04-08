//TODO: generate bitset dump file option

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <ctime>

#include "MurmurHash3.h"

using namespace std;

#define LOG_TWO_SQUARED  0.480453013918201388143813800 
#define LOG_TWO 0.693147180559945286226764000 

uint32_t djb2(char *str) {
    uint32_t hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

class bloomFilter {

    // n = number of items in filter
    // m = number of bits in filter
    // k = number of hash functions
    // p = probability of false positives

    uint32_t n, m, k;
    double p;
    vector<bool> *bitset;
    vector<uint32_t> seeds;
public:
    bloomFilter(uint32_t, double);
    void insert(char*);
    bool find();
    void printPrivates();
};

void bloomFilter::printPrivates() {
    cout << "\nNumber of items in bloom filter: " << n << "\nProbability of false positive: " << p;
    cout << "\nNumber of bits in bloom filter: " << m << "\nNumber of hash functions: " << k << endl;  

}

bloomFilter::bloomFilter(uint32_t n, double p) {
    this->n = n;  // set number of items
    this->p = p;  // set probability of false positives
    
    // calculate optimal number of bits in filter and optimal number of hash functions
    m = ceil((n * log(p)) / log(1 / pow(2, log(2))));
    k = round(((float)m / (float)n) * log(2));
    
    // initialize all bits to false
    bitset = new vector<bool>(m, false);
    
    //generate seeds for murmur hash function
    srand(time(0));
    for(int i = 0; i < k; ++i)
        seeds.push_back(rand());
}

void bloomFilter::insert(char* fileName) {
    ifstream  fin;
    fin.open(fileName);
    char word[32];
    uint32_t key, out;
    while(fin >> word) {
        key = djb2(word);
        for(int i = 0; i < k; ++i) {
            MurmurHash3_x86_32(&key, 4, seeds[i], &out);
            out %= m;
            (*bitset)[out] = true;
        }
    }
    fin.close();
}

bool bloomFilter::find() {
    cout << "\nEnter search string: ";
    char search_string[32];
    cin >> search_string;
    uint32_t search_key = djb2(search_string), out;
    for(int i = 0; i < k; ++i) {
        MurmurHash3_x86_32(&search_key, 4, seeds[i], &out);
        out %= m;
        if((*bitset)[out] == 0)
            return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    // argv[1]: dataset fileName
    // argv[2]: size of dataset
    // argv[3]: probability of false positive

    bloomFilter bf(atoi(argv[2]), atof(argv[3]));
    bf.insert(argv[1]);
    
    bf.printPrivates();

    char again = 'y';
    do {
        if(bf.find())
            cout << "Search string found!\n";
        else
            cout << "Search string not found!\n";
        cout << "Again?(y/n): ";
        cin >> again;
    } while(again == 'y');

    return 0;
}

