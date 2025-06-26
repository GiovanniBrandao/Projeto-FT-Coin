#include <mariadb/conncpp.hpp>
#include "oraculo.hpp"
#include "movimentacao.hpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <memory>
#include <stdexcept>

using namespace std;

int movimentacaoDAO_Local::gerarNovoIdMovimentoLocal_Helper()
{
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
                continue;
            }
        }
    }
    movLeitura.close();
    return novoIdMov;
}

string movimentacaoDAO_Local::obterDataAtualFormatada_Helper()
{
    time_t t = time(nullptr);
    tm *now = localtime(&t);
    char bufferData[11];
    strftime(bufferData, sizeof(bufferData), "%Y-%m-%d", now);
    return string(bufferData);
}

void movimentacaoDAO_Local::compraLocal(const movimentacao &mov) 
{
    oraculo oracle;
    double cotacaoAtual = oracle.cotacao;

    int idCarteira = mov.getIdCarteira();
    double quantidadeFTCoins = mov.getQuantidade();

    if (quantidadeFTCoins <= 0)
    {
        cout << "Quantidade invalida. A compra não pode ser realizada." << endl;
        return;
    }

    double valorTotalReais = quantidadeFTCoins * cotacaoAtual;

    int novoIdMov = movimentacaoDAO_Local::gerarNovoIdMovimentoLocal_Helper();
    string dataAtual = movimentacaoDAO_Local::obterDataAtualFormatada_Helper();

    ofstream movFile("movimentacoes.txt", ios::app);
    if (movFile.is_open())
    {
        movFile << "IDCarteira: " << idCarteira
                << " | IDMov: " << novoIdMov
                << " | Data: " << dataAtual
                << " | Tipo: C"
                << " | Quantidade: " << setprecision(6) << quantidadeFTCoins
                << endl;

        movFile.close();

        cout << "Compra local registrada com sucesso" << endl;
        cout << "Voce comprou " << quantidadeFTCoins << " FT Coins por R$ " << valorTotalReais << endl;
    }
    else
    {
        cerr << "Erro ao registrar movimentacao." << endl;
    }
}

void movimentacaoDAO_Local::vendaLocal(const movimentacao &mov) 
{
    oraculo oracle;
    double cotacaoAtual = oracle.cotacao;

    int idCarteira = mov.getIdCarteira();
    double quantidadeFTCoins = mov.getQuantidade(); 

    if (quantidadeFTCoins <= 0) 
    {
        cout << "Quantidade invalida. A venda não pode ser realizada." << endl;
        return;
    }

    int novoIdMov = movimentacaoDAO_Local::gerarNovoIdMovimentoLocal_Helper();
    string dataAtual = movimentacaoDAO_Local::obterDataAtualFormatada_Helper();

    ofstream movFile("movimentacoes.txt", ios::app);
    if (movFile.is_open())
    {
        movFile << "IDCarteira: " << idCarteira
                << " | IDMov: " << novoIdMov
                << " | Data: " << dataAtual
                << " | Tipo: V"
                << " | Quantidade: " << quantidadeFTCoins
                << endl;

        movFile.close();

        double valorTotalReais = quantidadeFTCoins * cotacaoAtual; 

        cout << "Venda registrada com sucesso" << endl;
        cout << "Voce vendeu " << quantidadeFTCoins << " FT Coins por R$ " << valorTotalReais << "." << std::endl;
    }
    else
    {
        cerr << "Erro ao registrar movimentacao." << endl;
    }
}

void movimentacaoDAO_Remoto::compraRemota(const movimentacao &mov)
{
    int idCarteira = mov.getIdCarteira();
    double quantidadeFTCoins = mov.getQuantidade();

    try
    {
        sql::Driver *driver = sql::mariadb::get_driver_instance();
        std::shared_ptr<sql::Connection> conn(driver->connect(
            "jdbc:mariadb://*******:3306/*******", // Altere o IP/Porta/Nome do Banco
            "*******",                             // Altere o usuário
            "*******"));                           // Altere a senha

        // Verifica se carteira existe
        std::shared_ptr<sql::PreparedStatement> stmntCheckCarteira(conn->prepareStatement("SELECT COUNT(*) AS total FROM CARTEIRA WHERE IdCarteira = ?"));
        stmntCheckCarteira->setInt(0, idCarteira);
        std::shared_ptr<sql::ResultSet> resCheckCarteira(stmntCheckCarteira->executeQuery());

        if (!resCheckCarteira->next() || resCheckCarteira->getInt("total") == 0)
        {
            cout << "Carteira com ID " << idCarteira << " não encontrada." << endl;
            return;
        }

        // Pega cotação mais recente
        std::shared_ptr<sql::PreparedStatement> stmntCotacao(conn->prepareStatement("SELECT Cotacao FROM ORACULO ORDER BY Data DESC LIMIT 1"));
        std::shared_ptr<sql::ResultSet> resCotacao(stmntCotacao->executeQuery());

        double cotacao = 0;
        if (resCotacao->next())
        {
            cotacao = resCotacao->getDouble("Cotacao");
        }
        else
        {
            cout << "Erro: Nenhuma cotação disponível no oráculo." << endl;
            return;
        }

        if (quantidadeFTCoins <= 0)
        {
            cout << "Quantidade inválida. Compra cancelada." << endl;
            return;
        }

        double valorTotalReais = quantidadeFTCoins * cotacao;

        // Registra a movimentação de compra
        std::shared_ptr<sql::PreparedStatement> stmntInsert(conn->prepareStatement(
            "INSERT INTO MOVIMENTACAO (IdCarteira, Data, TipoOperacao, Quantidade) "
            "VALUES (?, CURDATE(), 'C', ?)"));
        stmntInsert->setInt(0, idCarteira);
        stmntInsert->setDouble(1, quantidadeFTCoins);
        stmntInsert->executeUpdate();

        cout << "Compra registrada com sucesso!" << endl;
        cout << "Você comprou " << quantidadeFTCoins << " FT Coins por R$ " << valorTotalReais << endl;
    }
    catch (const sql::SQLException &e)
    {
        cerr << "Erro ao registrar compra: " << e.what() << endl;
    }
}

void movimentacaoDAO_Remoto::vendaRemota(const movimentacao &mov) 
{
    int idCarteira = mov.getIdCarteira();
    double quantidadeFTCoins = mov.getQuantidade();

    try
    {
        sql::Driver *driver = sql::mariadb::get_driver_instance();
        std::shared_ptr<sql::Connection> conn(driver->connect(
            "jdbc:mariadb://*******:3306/*******", // Altere o IP/Porta/Nome do Banco
            "*******",                             // Altere o usuário
            "*******"));                           // Altere a senha

        // Verifica se carteira existe
        std::shared_ptr<sql::PreparedStatement> stmntCheckCarteira(conn->prepareStatement("SELECT COUNT(*) AS total FROM CARTEIRA WHERE IdCarteira = ?"));
        stmntCheckCarteira->setInt(0, idCarteira);
        std::shared_ptr<sql::ResultSet> resCheckCarteira(stmntCheckCarteira->executeQuery());

        if (!resCheckCarteira->next() || resCheckCarteira->getInt("total") == 0)
        {
            cout << "Carteira com ID " << idCarteira << " não encontrada." << endl;
            return;
        }

        // Obtém a cotação mais recente
        std::shared_ptr<sql::PreparedStatement> stmntCotacao(conn->prepareStatement("SELECT Cotacao FROM ORACULO ORDER BY Data DESC LIMIT 1"));
        std::shared_ptr<sql::ResultSet> resCotacao(stmntCotacao->executeQuery());

        double cotacao = 0;
        if (resCotacao->next())
        {
            cotacao = resCotacao->getDouble("Cotacao");
        }
        else
        {
            cout << "Erro: Nenhuma cotação disponível no oráculo." << endl;
            return;
        }

        if (quantidadeFTCoins <= 0)
        {
            cout << "Quantidade inválida. Venda cancelada." << endl;
            return;
        }

        double valorTotalReais = quantidadeFTCoins * cotacao;

        // Registra a movimentação de venda
        std::shared_ptr<sql::PreparedStatement> stmntInsert(conn->prepareStatement(
            "INSERT INTO MOVIMENTACAO (IdCarteira, Data, TipoOperacao, Quantidade) "
            "VALUES (?, CURDATE(), 'V', ?)"));
        stmntInsert->setInt(0, idCarteira);
        stmntInsert->setDouble(1, quantidadeFTCoins);
        stmntInsert->executeUpdate();

        cout << "Venda registrada com sucesso!" << endl;
        cout << "Você vendeu " << quantidadeFTCoins << " FT Coins a R$ " << valorTotalReais << " cada." << endl;
    }
    catch (const sql::SQLException &e)
    {
        cerr << "Erro ao registrar venda: " << e.what() << endl;
    }
}