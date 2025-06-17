#ifndef CARTEIRA_H
#define CARTEIRA_H

#include <string>

class Carteira
{
private:
    int ID_carteira;
    string nome_titular;
    string corretora;
    const string moeda = "FT Coin";

public:
    Carteira() = default;
    Carteira(int id, string titular, string corretora) : ID_carteira(id), nome_titular(titular), corretora(corretora) {}

    int getId() const { return ID_carteira; }
    void setId(int id) { ID_carteira = id; }

    string getTitular() const { return nome_titular; }
    void setTitular(string t) { nome_titular = t; }

    string getCorretora() const { return corretora; }
    void setCorretora(string c) { corretora = c; }

    string getMoeda() const { return moeda; }
};

class CarteiraDAO_Local
{
public:
    void criarCarteira(const Carteira &carteira);
    Carteira consultarCarteira(int id);
    void editarCarteira(int id, const string &novoTitular);
    void excluirCarteira(int id);
};

class CarteiraDAO_Remoto
{
public:
    void criarCarteira(const Carteira &carteira);
    Carteira consultarCarteira(int id);
    void editarCarteira(int id, const string &novoTitular, const string &novaCorretora);
    void excluirCarteira(int id);
};

#endif
