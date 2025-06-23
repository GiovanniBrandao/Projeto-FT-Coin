#ifndef CARTEIRA_H
#define CARTEIRA_H

#include <string>

class Carteira
{
private:
    int ID_carteira;
    std::string nome_titular;
    std::string corretora;
    const std::string moeda = "FT Coin";

public:
    Carteira() = default;
    Carteira(int id, std::string titular, std::string corretora) : ID_carteira(id), nome_titular(titular), corretora(corretora) {}

    int getId() const { return ID_carteira; }
    void setId(int id) { ID_carteira = id; }

    std::string getTitular() const { return nome_titular; }
    void setTitular(std::string t) { nome_titular = t; }

    std::string getCorretora() const { return corretora; }
    void setCorretora(std::string c) { corretora = c; }

    std::string getMoeda() const { return moeda; }
};

class CarteiraDAO_Local
{
public:
    void criarCarteira(const Carteira &carteira);
    Carteira consultarCarteira(int id);
    void editarCarteira(int id, const std::string &novoTitular);
    void excluirCarteira(int id);
};

class CarteiraDAO_Remoto
{
public:
    void criarCarteira(const Carteira &carteira);
    Carteira consultarCarteira(int id);
    void editarCarteira(int id, const std::string &novoTitular, const std::string &novaCorretora);
    void excluirCarteira(int id);
};

#endif
