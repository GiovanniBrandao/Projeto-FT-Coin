#ifndef MOVIMENTACAO_H
#define MOVIMENTACAO_H

#include <string>

class movimentacao
{
private:
    int idMovimento;
    int idCarteira;
    std::string dataOperacao;
    char tipoOperacao; // 'C' ou 'V'
    double quantidade;

public:
    movimentacao() : idMovimento(0), idCarteira(0), dataOperacao(""), tipoOperacao(' '), quantidade(0.0) {}

    void setIdCarteira(int id) { idCarteira = id; }
    void setDataOperacao(const std::string &data) { dataOperacao = data; }
    void setTipoOperacao(char tipo) { tipoOperacao = tipo; }
    void setQuantidade(double qtde) { quantidade = qtde; }

    int getIdCarteira() const { return idCarteira; }
    std::string getDataOperacao() const { return dataOperacao; }
    char getTipoOperacao() const { return tipoOperacao; }
    double getQuantidade() const { return quantidade; }
};

class movimentacaoDAO_Local
{
    public:
    void compraLocal(const movimentacao& mov);
    void vendaLocal(const movimentacao& mov);
    
    private: 
    static int gerarNovoIdMovimentoLocal_Helper(); 
    static std::string obterDataAtualFormatada_Helper();
};

class movimentacaoDAO_Remoto
{
    public:
    void compraRemota(const movimentacao& mov);
    void vendaRemota(const movimentacao& mov);
};

#endif