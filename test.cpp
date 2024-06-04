#include "btb.hpp"
#include <iostream>
#include <cstdlib>

int main(){
    srand(1);
    double accuracy;
    long long int br, lat, hit, miss, total;
    uint64_t add, c;
    btb_t *btb = new btb_t;

    std::cout << "Inicializando BTB...\n";
    btb->allocate();
    std::cout << "BTB Inicializada.\n";

    hit = 0;
    lat = 0;
    miss = 0;
    total = 0;
    for(int i = 0; i < 1666666; i++){
        for(int j = 0; j < INPUT; ++j){
            add = rand() % 12288;
            br = rand() % 6;
            c = (i+1) * 10;
            //std::cout << "Inserindo: Add = " << add << ", Br = " << br << ", Cycle = " << j << "\n";
            if (btb->btb_search_update(add, c)) {
                ++lat;
                ++hit;
            } else {
                btb->btb_insert(add, br, c);
                lat += 12;
                ++miss;
            }
            ++total;
        }
    }
    
    btb->imprime();

    accuracy = ((double)hit / (double)total) * 100;

    std::cout << "Ciclos Totais: " << lat << '\n';
    std::cout << "Hit: " << hit << '\n';
    std::cout << "Miss: " << miss << '\n';
    std::cout << "Precisão: " << accuracy << '\n';

    delete btb;

    return 0;
}
