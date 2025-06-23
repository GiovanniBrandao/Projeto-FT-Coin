#ifndef RELATORIO_HPP
#define RELATORIO_HPP

class RelatorioLocal
{
public:
    void listarCarteirasPorId();
    void listarCarteirasPorNome();
    void exibirSaldoCarteira(int idCarteira);
    void exibirHistoricoCarteira(int idCarteira);
    void exibirGanhoPerdaCarteira(int idCarteira);
};

class RelatorioRemoto
{
public:
    void listarCarteirasPorId();
    void listarCarteirasPorNome();
    void exibirSaldoCarteira(int idCarteira);
    void exibirHistoricoCarteira(int idCarteira);
    void exibirGanhoPerdaCarteira(int idCarteira);
};

#endif
