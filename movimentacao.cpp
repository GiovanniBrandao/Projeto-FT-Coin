#include "oraculo.hpp"
#include "movimentacao.hpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>

using namespace std;

void movimentacao::compraLocal()
{
    oraculo oraculo;
    int idAlvo;
    double valorReais;
    double cotacaoAtual = oraculo.cotacao;

    cout << "Digite o ID da carteira que deseja comprar: ";
    cin >> idAlvo;

    cout << "Cotacao atual da FT Coin: " << cotacaoAtual << endl;
    cout << "Digite quanto deseja gastar em reais: R$ ";
    cin >> valorReais;

    if (valorReais <= 0)
    {
        cout << "Valor inválido. A compra não pode ser realizada." << endl;
        return;
    }

    double moedasCompradas = valorReais / cotacaoAtual;

    // Gerar ID para movimentação
    int novoIdMov = 1;
    ifstream movLeitura("movimentacoes.txt");
    string linhaMov;
    while (getline(movLeitura, linhaMov))
    {
        size_t posIdMov = linhaMov.find("IDMov: ");
        if (posIdMov != string::npos)
        {
            try
            {
                int idMovTmp = stoi(linhaMov.substr(posIdMov + 7));
                if (idMovTmp >= novoIdMov)
                    novoIdMov = idMovTmp + 1;
            }
            catch (...)
            {
                // Ignorar linha mal formatada
                continue;
            }
        }
    }
    movLeitura.close();

    // Pegar data atual formatada YYYY-MM-DD
    time_t t = time(nullptr);
    tm *now = localtime(&t);
    char bufferData[11];
    strftime(bufferData, sizeof(bufferData), "%Y-%m-%d", now);

    // Registrar movimentação
    ofstream movFile("movimentacoes.txt", ios::app);
    if (movFile.is_open())
    {
        movFile << "IDCarteira: " << idAlvo
                << " | IDMov: " << novoIdMov
                << " | Data: " << bufferData
                << " | Tipo: C"
                << " | Quantidade: " << setprecision(6) << moedasCompradas
                << endl;

        movFile.close();

        cout << "Compra realizada com sucesso" << endl;
        cout << "Você comprou " << moedasCompradas << " FT Coins por R$ " << valorReais << endl;
    }
    else
    {
        cerr << "Erro ao registrar movimentação." << endl;
    }
}

void movimentacao::vendaLocal()
{
    oraculo oraculo;
    int idAlvo;
    double quantidadeVender;
    double cotacaoAtual = oraculo.cotacao;

    cout << "Digite o ID da carteira que deseja vender: ";
    cin >> idAlvo;

    cout << "Cotacao atual da FT Coin: " << cotacaoAtual << endl;
    cout << "Digite quantas FT Coins deseja vender: ";
    cin >> quantidadeVender;

    if (quantidadeVender <= 0)
    {
        cout << "Quantidade inválida. A venda não pode ser realizada." << endl;
        return;
    }

    // Gerar novo ID de movimentação
    int novoIdMov = 1;
    ifstream movLeitura("movimentacoes.txt");
    string linhaMov;
    while (getline(movLeitura, linhaMov))
    {
        size_t posIdMov = linhaMov.find("IDMov: ");
        if (posIdMov != string::npos)
        {
            int idMovTmp = stoi(linhaMov.substr(posIdMov + 7));
            if (idMovTmp >= novoIdMov)
                novoIdMov = idMovTmp + 1;
        }
    }
    movLeitura.close();

    // Registrar movimentação no arquivo
    ofstream movFile("movimentacoes.txt", ios::app);
    if (movFile.is_open())
    {
        string data = "2025"; // ou gere a data atual com ctime

        movFile << "IDCarteira: " << idAlvo
                << " | IDMov: " << novoIdMov
                << " | Data: " << data
                << " | Tipo: V"
                << " | Quantidade: " << quantidadeVender
                << endl;

        movFile.close();

        cout << "Venda registrada com sucesso" << endl;
        cout << "Voce vendeu " << quantidadeVender << " FT Coins." << endl;
    }
    else
    {
        cerr << "Erro ao registrar movimentacao." << endl;
    }
}

void movimentacao::compraRemota()
{
}

void movimentacao::vendaRemota()
{
}
