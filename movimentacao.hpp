#ifndef MOVIMENTO_H
#define MOVIMENTO_H

#include <string>

using namespace std;

class movimentacao
{
private:
    int idMovimento;            
    int idCarteira;
    std::string dataOperacao; 
    char tipoOperacao;       // 'C' ou 'V'
    double quantidade;

public:

    movimentacao() : idMovimento(0), idCarteira(0), dataOperacao(""), tipoOperacao(' '), quantidade(0.0) {}


    void compraLocal();
    void vendaLocal();
    void compraRemota();
    void vendaRemota();

    void setIdCarteira(int id) { idCarteira = id; }
    void setDataOperacao(const std::string& data) { dataOperacao = data; }
    void setTipoOperacao(char tipo) { tipoOperacao = tipo; }
    void setQuantidade(double qtde) { quantidade = qtde; }

    int getIdCarteira() const { return idCarteira; }
    std::string getDataOperacao() const { return dataOperacao; }
    char getTipoOperacao() const { return tipoOperacao; }
    double getQuantidade() const { return quantidade; }
};



#endif