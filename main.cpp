#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

//FIFO
vector<int> fifo(const vector<int>& paginas, int quadros, int &falhas) {
    vector<int> memoria;
    queue<int> ordem;
    falhas = 0;

    for (int pagina : paginas) {
        auto it = find(memoria.begin(), memoria.end(), pagina);

        if (it == memoria.end()) { //PageFault
            falhas++;
            if ((int)memoria.size() < quadros) {
                memoria.push_back(pagina);
                ordem.push(pagina);
            } else {
                int mais_antiga = ordem.front();
                ordem.pop();

                int indice = find(memoria.begin(), memoria.end(), mais_antiga) - memoria.begin();
                memoria[indice] = pagina;
                ordem.push(pagina);
            }
        }
    }
    return memoria;
}

//LRU
vector<int> lru(const vector<int>& paginas, int quadros, int &falhas) {
    vector<int> memoria;
    vector<int> recentemente_usadas;
    falhas = 0;

    for (int pagina : paginas) {
        auto it = find(memoria.begin(), memoria.end(), pagina);

        if (it == memoria.end()) { //PageFault
            falhas++;
            if ((int)memoria.size() < quadros) {
                memoria.push_back(pagina);
                recentemente_usadas.push_back(pagina);
            } else {
                int menos_usada = recentemente_usadas.front();
                recentemente_usadas.erase(recentemente_usadas.begin());

                int indice = find(memoria.begin(), memoria.end(), menos_usada) - memoria.begin();
                memoria[indice] = pagina;
                recentemente_usadas.push_back(pagina);
            }
        } else {
            // move pro final (foi usada recentemente)
            recentemente_usadas.erase(find(recentemente_usadas.begin(), recentemente_usadas.end(), pagina));
            recentemente_usadas.push_back(pagina);
        }
    }
    return memoria;
}

//MRU
vector<int> mru(const vector<int>& paginas, int quadros, int &falhas) {
    vector<int> memoria;
    vector<int> recentemente_usadas;
    falhas = 0;

    for (int pagina : paginas) {
        auto it = find(memoria.begin(), memoria.end(), pagina);

        if (it == memoria.end()) { //PageFault
            falhas++;
            if ((int)memoria.size() < quadros) {
                memoria.push_back(pagina);
                recentemente_usadas.push_back(pagina);
            } else {
                int mais_usada = recentemente_usadas.back();
                recentemente_usadas.pop_back();

                int indice = find(memoria.begin(), memoria.end(), mais_usada) - memoria.begin();
                memoria[indice] = pagina;
                recentemente_usadas.push_back(pagina);
            }
        } else {
            // Move para o final (foi usada recentemente)
            recentemente_usadas.erase(find(recentemente_usadas.begin(), recentemente_usadas.end(), pagina));
            recentemente_usadas.push_back(pagina);
        }
    }
    return memoria;
}

// Imprimir
void imprimirMemoria(const string& nome, const vector<int>& memoria, int falhas) {
    cout << nome << ": ";
    for (int p : memoria) cout << p << " ";
    cout << " | Falhas de página: " << falhas << endl;
}

int main() {
    int total_quadros = 8;

    vector<int> sequenciaA = {4,3,25,8,19,6,25,8,16,35,45,22,8,3,16,25,7};
    vector<int> sequenciaB = {4,5,7,9,46,45,14,4,64,7,65,2,1,6,8,45,14,11};
    vector<int> sequenciaC = {4,6,7,8,1,6,10,15,16,4,2,1,4,6,12,15,16,11};

    int falhas_fifo, falhas_lru, falhas_mru;

    cout << "Teste A" << endl;
    auto fifoA = fifo(sequenciaA, total_quadros, falhas_fifo);
    auto lruA  = lru(sequenciaA, total_quadros, falhas_lru);
    auto mruA  = mru(sequenciaA, total_quadros, falhas_mru);
    imprimirMemoria("FIFO", fifoA, falhas_fifo);
    imprimirMemoria("LRU ", lruA, falhas_lru);
    imprimirMemoria("MRU ", mruA, falhas_mru);
    cout << "Pagina 7 esta no quadro: " << (find(fifoA.begin(), fifoA.end(), 7) - fifoA.begin() + 1) << endl;
    cout << endl;

    cout << "Teste B" << endl;
    auto fifoB = fifo(sequenciaB, total_quadros, falhas_fifo);
    auto lruB  = lru(sequenciaB, total_quadros, falhas_lru);
    auto mruB  = mru(sequenciaB, total_quadros, falhas_mru);
    imprimirMemoria("FIFO", fifoB, falhas_fifo);
    imprimirMemoria("LRU ", lruB, falhas_lru);
    imprimirMemoria("MRU ", mruB, falhas_mru);
    cout << "Pagina 11 esta no quadro: " << (find(fifoB.begin(), fifoB.end(), 11) - fifoB.begin() + 1) << endl;
    cout << endl;

    cout << "Teste C" << endl;
    auto fifoC = fifo(sequenciaC, total_quadros, falhas_fifo);
    auto lruC  = lru(sequenciaC, total_quadros, falhas_lru);
    auto mruC  = mru(sequenciaC, total_quadros, falhas_mru);
    imprimirMemoria("FIFO", fifoC, falhas_fifo);
    imprimirMemoria("LRU ", lruC, falhas_lru);
    imprimirMemoria("MRU ", mruC, falhas_mru);
    cout << "Pagina 11 esta no quadro: " << (find(fifoC.begin(), fifoC.end(), 11) - fifoC.begin() + 1) << endl;
    cout << endl;

    return 0;
}
