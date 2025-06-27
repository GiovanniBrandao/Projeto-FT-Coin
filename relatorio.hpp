#ifndef RELATORIO_HPP
#define RELATORIO_HPP

class RelatorioDAO_Local
{
public:
    void listarCarteirasPorId();
    void listarCarteirasPorNome();
    void exibirSaldoCarteira(int idCarteira);
    void exibirHistoricoCarteira(int idCarteira);
    void exibirGanhoPerdaCarteira(int idCarteira);
};

class RelatorioDAO_Remoto
{
public:
    void listarCarteirasPorId();
    void listarCarteirasPorNome();
    void exibirSaldoCarteira(int idCarteira);
    void exibirHistoricoCarteira(int idCarteira);
    void exibirGanhoPerdaCarteira(int idCarteira);
};

#endif
