#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) {
    ofstream fout;
    fout.open("dataset.txt");
    int size = atoi(argv[1]);
    srand(time(0));
    for(int i = 0; i < size; ++i) {
        int length = (rand() % 32) + 2;
        char str[length];
        for(int j = 0; j < length - 1; ++j)
            str[j] = ((rand() % 94) + 33);
        str[length - 1] = '\0';
        fout << str << "\n";
    }
    fout.close();
    return 0;
}