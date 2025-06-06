#ifndef CARTERIRA_H
#define CARTEIRA_H

#include <string>

using namespace std;

class carteira {

public:

int ID_carteira;
float saldo = 0.00;
string nome_titular;
string moeda = "FT Coin";

void NovaCarteiraLocal();

void ChecarCarteiraLocal();

void EditarCarteiraLocal();

void ExcluirCarteiraLocal();

void NovaCarteiraRemoto();

void ChecarCarteiraRemoto();

void EditarCarteiraRemoto();

void ExcluirCarteiraRemoto();

};


#endif