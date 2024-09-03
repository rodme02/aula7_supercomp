#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <numeric>
#include <unordered_map>

using namespace std;
using namespace chrono;

typedef pair<int, int> Resultado;
unordered_map<int, Resultado> memo;

Resultado calcularValorPesoMaximo(int capacidadeRestante, const vector<int>& pesos, const vector<int>& valores, int indiceItem, int& combinacoes) {
    combinacoes++;
    if (indiceItem == 0 || capacidadeRestante == 0) return {0, 0};
    int key = capacidadeRestante * 1000 + indiceItem;  // Uma chave simples para a memoização

    if (memo.find(key) != memo.end()) return memo[key];

    Resultado resultado;
    if (pesos[indiceItem - 1] > capacidadeRestante) {
        resultado = calcularValorPesoMaximo(capacidadeRestante, pesos, valores, indiceItem - 1, combinacoes);
    } else {
        Resultado incluirItem = calcularValorPesoMaximo(capacidadeRestante - pesos[indiceItem - 1], pesos, valores, indiceItem - 1, combinacoes);
        incluirItem.first += valores[indiceItem - 1];
        incluirItem.second += pesos[indiceItem - 1];
        Resultado excluirItem = calcularValorPesoMaximo(capacidadeRestante, pesos, valores, indiceItem - 1, combinacoes);
        resultado = (incluirItem.first > excluirItem.first) ? incluirItem : excluirItem;
    }
    memo[key] = resultado;
    return resultado;
}

void imprimirResultados(const string& metodo, int valorTotal, int pesoTotal, long long duracao, int repeticao = 0) {
    if (repeticao == 0) {
        cout << metodo << ":" << endl;
    } else {
        cout << metodo << " - Execucao " << repeticao << ":" << endl;
    }
    cout << "Valor total: " << valorTotal << endl;
    cout << "Peso total: " << pesoTotal << endl;
    cout << "Tempo de execucao: " << duracao << " ns" << endl << endl;
}

void embaralharEPreencher(int capacidadeDaMochila, vector<int>& pesos, vector<int>& valores, mt19937& gen) {
    int n = pesos.size();
    vector<int> indices(n);
    iota(indices.begin(), indices.end(), 0);

    for (int repeticoes = 0; repeticoes < 5; ++repeticoes) {
        shuffle(indices.begin(), indices.end(), gen);

        int pesoAtual = 0;
        int valorTotal = 0;
        auto inicio = high_resolution_clock::now();

        for (int i : indices) {
            if (pesoAtual + pesos[i] <= capacidadeDaMochila) {
                pesoAtual += pesos[i];
                valorTotal += valores[i];
            }
        }

        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<nanoseconds>(fim - inicio).count();

        imprimirResultados("Metodo Embaralhar e Preencher", valorTotal, pesoAtual, duracao, repeticoes + 1);
    }
}

void selecaoAleatoria(int capacidadeDaMochila, vector<int>& pesos, vector<int>& valores, mt19937& gen) {
    int n = pesos.size();
    uniform_real_distribution<double> distribution(0.0, 1.0);
    double threshold = 0.5;

    for (int repeticoes = 0; repeticoes < 5; ++repeticoes) {
        int pesoAtual = 0;
        int valorTotal = 0;
        auto inicio = high_resolution_clock::now();

        for (int i = 0; i < n; ++i) {
            if (distribution(gen) > threshold && pesoAtual + pesos[i] <= capacidadeDaMochila) {
                pesoAtual += pesos[i];
                valorTotal += valores[i];
            }
        }

        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<nanoseconds>(fim - inicio).count();

        imprimirResultados("Metodo Selecao Aleatoria", valorTotal, pesoAtual, duracao, repeticoes + 1);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Uso: " << argv[0] << " <arquivo de entrada>" << endl;
        return 1;
    }

    ifstream arquivoEntrada(argv[1]);
    if (!arquivoEntrada) {
        cout << "Erro ao abrir o arquivo de entrada!" << endl;
        return 1;
    }

    int numeroDeItens, capacidadeDaMochila;
    arquivoEntrada >> numeroDeItens >> capacidadeDaMochila;

    vector<int> pesos(numeroDeItens), valores(numeroDeItens);
    for (int i = 0; i < numeroDeItens; ++i) {
        arquivoEntrada >> pesos[i] >> valores[i];
    }

    mt19937 gen(random_device{}()); // Gera números aleatórios mais eficientemente
    int combinacoes = 0;
    auto inicio = high_resolution_clock::now();

    auto resultado = calcularValorPesoMaximo(capacidadeDaMochila, pesos, valores, numeroDeItens, combinacoes);

    auto fim = high_resolution_clock::now();
    long long totalTempoExecucao = duration_cast<nanoseconds>(fim - inicio).count();

    imprimirResultados("Metodo Exaustivo", resultado.first, resultado.second, totalTempoExecucao);

    // Executar heurísticas
    embaralharEPreencher(capacidadeDaMochila, pesos, valores, gen);
    selecaoAleatoria(capacidadeDaMochila, pesos, valores, gen);

    return 0;
}