#ifndef MOVIMENTO_H
#define MOVIMENTO_H

#include <string>

using namespace std;

class movimentacao
{

private:
    int idMovimento;
    int idCarteira;
    string dataOperacao;
    char tipoOperacao; // 'C' ou 'V'
    double quantidade;

public:

    void compraLocal();

    void vendaLocal();

    void compraRemota();

    void vendaRemota();
};




#endif