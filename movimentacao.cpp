#include "oraculo.hpp"
#include "movimentacao.hpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>


using namespace std;

void movimentacao::compraLocal()
{
    oraculo oraculo;
    int idAlvo;
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

    // Gerar ID para movimentação
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
                // Ignorar linha mal formatada
                continue;
            }
        }
    }
    movLeitura.close();

    // Pegar data atual formatada YYYY-MM-DD
    time_t t = time(nullptr);
    tm *now = localtime(&t);
    char bufferData[11];
    strftime(bufferData, sizeof(bufferData), "%Y-%m-%d", now);

    // Registrar movimentação
    ofstream movFile("movimentacoes.txt", ios::app);
    if (movFile.is_open())
    {
        movFile << "IDCarteira: " << idAlvo
                << " | IDMov: " << novoIdMov
                << " | Data: " << bufferData
                << " | Tipo: C"
                << " | Quantidade: " << setprecision(6) << moedasCompradas
                << endl;

        movFile.close();

        cout << "Compra realizada com sucesso" << endl;
        cout << "Você comprou " << moedasCompradas << " FT Coins por R$ " << valorReais << endl;
    }
    else
    {
        cerr << "Erro ao registrar movimentação." << endl;
    }
}

void movimentacao::vendaLocal()
{
    oraculo oraculo;
    int idAlvo;
    double quantidadeVender;
    double cotacaoAtual = oraculo.cotacao;

    cout << "Digite o ID da carteira que deseja vender: ";
    cin >> idAlvo;

    cout << "Cotacao atual da FT Coin: " << cotacaoAtual << endl;
    cout << "Digite quantas FT Coins deseja vender: ";
    cin >> quantidadeVender;

    if (quantidadeVender <= 0)
    {
        cout << "Quantidade inválida. A venda não pode ser realizada." << endl;
        return;
    }

    // Gerar novo ID de movimentação
    int novoIdMov = 1;
    ifstream movLeitura("movimentacoes.txt");
    string linhaMov;
    while (getline(movLeitura, linhaMov))
    {
        size_t posIdMov = linhaMov.find("IDMov: ");
        if (posIdMov != string::npos)
        {
            int idMovTmp = stoi(linhaMov.substr(posIdMov + 7));
            if (idMovTmp >= novoIdMov)
                novoIdMov = idMovTmp + 1;
        }
    }
    movLeitura.close();

    // Registrar movimentação no arquivo
    ofstream movFile("movimentacoes.txt", ios::app);
    if (movFile.is_open())
    {
        string data = "2025"; // ou gere a data atual com ctime

        movFile << "IDCarteira: " << idAlvo
                << " | IDMov: " << novoIdMov
                << " | Data: " << data
                << " | Tipo: V"
                << " | Quantidade: " << quantidadeVender
                << endl;

        movFile.close();

        cout << "Venda registrada com sucesso" << endl;
        cout << "Voce vendeu " << quantidadeVender << " FT Coins." << endl;
    }
    else
    {
        cerr << "Erro ao registrar movimentacao." << endl;
    }
}

void movimentacao::compraRemota()
{
int idCarteira;
    double valorReais;

    cout << "Digite o ID da carteira que deseja comprar: ";
    cin >> idCarteira;

    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://localhost:3306", "seu_usuario", "sua_senha"));
        con->setSchema("PooI_25_Yxx");

        // Verifica se carteira existe
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(
            "SELECT COUNT(*) AS total FROM CARTEIRA WHERE IdCarteira = ?"));
        pstmt->setInt(1, idCarteira);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        res->next();
        if (res->getInt("total") == 0) {
            cout << "Carteira com ID " << idCarteira << " não encontrada." << endl;
            return;
        }

        // Pega cotação mais recente
        pstmt.reset(con->prepareStatement("SELECT Cotacao FROM ORACULO ORDER BY Data DESC LIMIT 1"));
        res.reset(pstmt->executeQuery());

        double cotacao = 0;
        if (res->next()) {
            cotacao = res->getDouble("Cotacao");
        } else {
            cout << "Erro: Nenhuma cotação disponível no oráculo." << endl;
            return;
        }

        cout << "Cotação atual da FT Coin: R$ " << cotacao << endl;
        cout << "Digite quanto deseja gastar em reais: R$ ";
        cin >> valorReais;

        if (valorReais <= 0) {
            cout << "Valor inválido. Compra cancelada." << endl;
            return;
        }

        double quantidade = valorReais / cotacao;

        // Registra a movimentação de compra
        pstmt.reset(con->prepareStatement(
            "INSERT INTO MOVIMENTACAO (IdCarteira, Data, TipoOperacao, Quantidade) VALUES (?, CURDATE(), 'C', ?)"));
        pstmt->setInt(1, idCarteira);
        pstmt->setDouble(2, quantidade);
        pstmt->executeUpdate();

        cout << "Compra registrada com sucesso!" << endl;
        cout << "Você comprou " << quantidade << " FT Coins por R$ " << valorReais << endl;

    } catch (sql::SQLException& e) {
        cerr << "Erro ao registrar compra: " << e.what() << endl;
    }
}

void movimentacao::vendaRemota()
{
int idCarteira;
    double quantidade;

    cout << "Digite o ID da carteira que deseja vender: ";
    cin >> idCarteira;

    // Conecta ao banco
    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://localhost:3306", "seu_usuario", "sua_senha"));
        con->setSchema("PooI_25_Yxx");

        // Verifica se carteira existe
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(
            "SELECT COUNT(*) AS total FROM CARTEIRA WHERE IdCarteira = ?"));
        pstmt->setInt(1, idCarteira);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        res->next();
        if (res->getInt("total") == 0) {
            cout << "Carteira com ID " << idCarteira << " não encontrada." << endl;
            return;
        }

        // Obtém a cotação mais recente
        pstmt.reset(con->prepareStatement("SELECT Cotacao FROM ORACULO ORDER BY Data DESC LIMIT 1"));
        res.reset(pstmt->executeQuery());

        double cotacao = 0;
        if (res->next()) {
            cotacao = res->getDouble("Cotacao");
        } else {
            cout << "Erro: Nenhuma cotação disponível no oráculo." << endl;
            return;
        }

        cout << "Cotação atual da FT Coin: R$ " << cotacao << endl;
        cout << "Digite quantas FT Coins deseja vender: ";
        cin >> quantidade;

        if (quantidade <= 0) {
            cout << "Quantidade inválida. Venda cancelada." << endl;
            return;
        }

        // Registra a movimentação de venda
        pstmt.reset(con->prepareStatement(
            "INSERT INTO MOVIMENTACAO (IdCarteira, Data, TipoOperacao, Quantidade) VALUES (?, CURDATE(), 'V', ?)"));
        pstmt->setInt(1, idCarteira);
        pstmt->setDouble(2, quantidade);
        pstmt->executeUpdate();

        cout << "Venda registrada com sucesso!" << endl;
        cout << "Você vendeu " << quantidade << " FT Coins a R$ " << cotacao << " cada." << endl;

    } catch (sql::SQLException& e) {
        cerr << "Erro ao registrar venda: " << e.what() << endl;
    }
}
