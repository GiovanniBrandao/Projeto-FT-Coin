#include "oraculo.hpp"
#include "movimentacao.hpp"
#include <iostream>
#include <fstream>

using namespace std;

void movimentacao::compra()
{
    oraculo oraculo;
    int idAlvo, idLido;
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

    ifstream entrada("carteira.txt");
    ofstream saida("temp.txt");

    if (!entrada || !saida)
    {
        cerr << "Erro ao abrir os arquivos." << endl;
        return;
    }

    string linha;
    bool dentroDoBloco = false;
    bool editarBloco = false;
    double fundosReais = 0.0;
    double saldoCripto = 0.0;
    bool fundosAtualizados = false;
    bool criptoAtualizado = false;

    while (getline(entrada, linha))
    {
        if (linha.find("ID: ") != string::npos)
        {
            idLido = stoi(linha.substr(4));
            if (idLido == idAlvo)
            {
                editarBloco = true;
                dentroDoBloco = true;
                saida << linha << endl;
                continue;
            }
            else
            {
                editarBloco = false;
                dentroDoBloco = true;
            }
        }

        if (linha.find("Dinheiro real R$: ") != string::npos && editarBloco)
        {
            try
            {
                string valorStr = linha.substr(18);
                fundosReais = stod(valorStr);

                if (fundosReais < valorReais)
                {
                    cout << "Saldo insuficiente em reais para realizar a compra." << endl;
                    entrada.close();
                    saida.close();
                    remove("temp.txt");
                    return;
                }

                double novoFundo = fundosReais - valorReais;
                saida << "Dinheiro real R$: " << novoFundo << endl;
                fundosAtualizados = true;
            }
            catch (...)
            {
                cerr << "Erro ao interpretar o valor de 'dinheiro real R$:'." << endl;
                entrada.close();
                saida.close();
                remove("temp.txt");
                return;
            }
            continue;
        }

        if (linha.find("Criptomoeda: ") != string::npos && editarBloco)
        {
            try
            {
                string valorStr = linha.substr(13); // "Criptomoeda: " = 14 caracteres
                saldoCripto = stod(valorStr);

                double novoSaldo = saldoCripto + moedasCompradas;
                saida << "Criptomoeda: " << novoSaldo << endl;
                criptoAtualizado = true;
            }
            catch (...)
            {
                cerr << "Erro ao interpretar o valor de 'Criptomoeda:'." << endl;
                entrada.close();
                saida.close();
                remove("temp.txt");
                return;
            }
            continue;
        }

        if (linha == "----------------------------------")
        {
            dentroDoBloco = false;
        }

        // Escreve a linha normalmente se não for a linha modificada
        if (!editarBloco ||
            (linha.find("dinheiro real R$: ") == string::npos &&
             linha.find("Criptomoeda: ") == string::npos))
        {
            saida << linha << endl;
        }
    }

    entrada.close();
    saida.close();

    remove("carteira.txt");
    rename("temp.txt", "carteira.txt");

    if (editarBloco && fundosAtualizados && criptoAtualizado)
    {
        cout << "Compra realizada com sucesso!" << endl;
        cout << "Você comprou " << moedasCompradas << " FT Coins por R$ " << valorReais << endl;

        ofstream movFile("movimentacoes.txt", ios::app);
        if (movFile.is_open())
        {
            string data = "2025"; // ou gere a data atual com <ctime>
            movFile << "ID: " << idAlvo
                    << " | Tipo: Compra"
                    << " | Quantidade: " << moedasCompradas
                    << " | Cotação: " << cotacaoAtual
                    << " | Valor total: R$" << valorReais
                    << " | Data: " << data << endl;
            movFile.close();
        }
        else
        {
            cerr << "Erro ao registrar movimentação." << endl;
        }
    }
    else if (!editarBloco)
    {
        cout << "Carteira com ID " << idAlvo << " não encontrada." << endl;
    }
}

void movimentacao::venda()
{
    oraculo oraculo;
    int idAlvo, idLido;
    double quantidadeVenda;
    double cotacaoAtual = oraculo.cotacao;

    cout << "Digite o ID da carteira que deseja vender: ";
    cin >> idAlvo;

    cout << "Cotacao atual da FT Coin: " << cotacaoAtual << endl;
    cout << "Digite a quantidade de moedas que deseja vender: ";
    cin >> quantidadeVenda;

    if (quantidadeVenda <= 0)
    {
        cout << "Quantidade inválida. A venda não pode ser realizada." << endl;
        return;
    }

    ifstream entrada("carteira.txt");
    ofstream saida("temp.txt");

    if (!entrada || !saida)
    {
        cerr << "Erro ao abrir os arquivos." << endl;
        return;
    }

    string linha;
    bool dentroDoBloco = false;
    bool editarBloco = false;
    double fundosReais = 0.0;
    double saldoCripto = 0.0;
    bool fundosAtualizados = false;
    bool criptoAtualizado = false;

    while (getline(entrada, linha))
    {
        if (linha.find("ID: ") != string::npos)
        {
            idLido = stoi(linha.substr(4));
            if (idLido == idAlvo)
            {
                editarBloco = true;
                dentroDoBloco = true;
                saida << linha << endl;
                continue;
            }
            else
            {
                editarBloco = false;
                dentroDoBloco = true;
            }
        }

        if (linha.find("Criptomoeda: ") != string::npos && editarBloco)
        {
            try
            {
                string valorStr = linha.substr(13);
                saldoCripto = stod(valorStr);

                if (saldoCripto < quantidadeVenda)
                {
                    cout << "Saldo insuficiente de criptomoedas para realizar a venda." << endl;
                    entrada.close();
                    saida.close();
                    remove("temp.txt");
                    return;
                }

                double novoSaldo = saldoCripto - quantidadeVenda;
                saida << "Criptomoeda: " << novoSaldo << endl;
                criptoAtualizado = true;
            }
            catch (...)
            {
                cerr << "Erro ao interpretar o valor de 'Criptomoeda:'." << endl;
                entrada.close();
                saida.close();
                remove("temp.txt");
                return;
            }
            continue;
        }

        if (linha.find("Dinheiro real R$: ") != string::npos && editarBloco)
        {
            try
            {
                string valorStr = linha.substr(18);
                fundosReais = stod(valorStr);

                double valorRecebido = quantidadeVenda * cotacaoAtual;
                double novoFundo = fundosReais + valorRecebido;
                saida << "Dinheiro real R$: " << novoFundo << endl;
                fundosAtualizados = true;
            }
            catch (...)
            {
                cerr << "Erro ao interpretar o valor de 'Dinheiro real R$:'." << endl;
                entrada.close();
                saida.close();
                remove("temp.txt");
                return;
            }
            continue;
        }

        if (linha == "----------------------------------")
        {
            dentroDoBloco = false;
        }

        if (!editarBloco ||
            (linha.find("Dinheiro real R$: ") == string::npos &&
             linha.find("Criptomoeda: ") == string::npos))
        {
            saida << linha << endl;
        }
    }

    entrada.close();
    saida.close();

    remove("carteira.txt");
    rename("temp.txt", "carteira.txt");

    if (editarBloco && fundosAtualizados && criptoAtualizado)
    {
        cout << "Venda realizada com sucesso" << endl;
        cout << "Voce vendeu " << quantidadeVenda << " FT Coins e recebeu R$ " << quantidadeVenda * cotacaoAtual << endl;

        ofstream movFile("movimentacoes.txt", ios::app);
        if (movFile.is_open())
        {
            string data = "2025"; // ou gere data atual
            movFile << "ID: " << idAlvo;
            movFile << " | Tipo: Venda";
            movFile << " | Quantidade: " << quantidadeVenda;
            movFile << " | Cotação: " << cotacaoAtual;
            movFile << " | Valor total: R$" << quantidadeVenda * cotacaoAtual;
            movFile << " | Data: " << data << endl;
            movFile.close();
        }
        else
        {
            cerr << "Erro ao registrar movimentacao." << endl;
        }
    }
    else if (!editarBloco)
    {
        cout << "Carteira com ID " << idAlvo << " nao encontrada." << endl;
    }
}
