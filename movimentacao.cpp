#include "oraculo.hpp"
#include "movimentacao.hpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <mariadb/conncpp.hpp>


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

    std::cout << "Digite o ID da carteira que deseja comprar: ";
    std::cin >> idCarteira;

    try
    {
        mariadb::Client client = mariadb::Client::create();
        std::shared_ptr<mariadb::Connection> con = client->connect(
            "jdbc:mariadb://localhost:3306/PooI_25_Yxx",
            "seu_usuario",
            "sua_senha");

        // Verifica se carteira existe
        std::shared_ptr<mariadb::Statement> pstmt = con->prepare(
            "SELECT COUNT(*) AS total FROM CARTEIRA WHERE IdCarteira = ?");
        pstmt->setInt(0, idCarteira);
        std::shared_ptr<mariadb::ResultSet> res = pstmt->executeQuery();

        if (!res->next() || res->getInt("total") == 0)
        {
            std::cout << "Carteira com ID " << idCarteira << " não encontrada." << std::endl;
            return;
        }

        // Pega cotação mais recente
        pstmt = con->prepare("SELECT Cotacao FROM ORACULO ORDER BY Data DESC LIMIT 1");
        res = pstmt->executeQuery();

        double cotacao = 0;
        if (res->next())
        {
            cotacao = res->getDouble("Cotacao");
        }
        else
        {
            std::cout << "Erro: Nenhuma cotação disponível no oráculo." << std::endl;
            return;
        }

        std::cout << "Cotação atual da FT Coin: R$ " << cotacao << std::endl;
        std::cout << "Digite quanto deseja gastar em reais: R$ ";
        std::cin >> valorReais;

        if (valorReais <= 0)
        {
            std::cout << "Valor inválido. Compra cancelada." << std::endl;
            return;
        }

        double quantidade = valorReais / cotacao;

        // Registra a movimentação de compra
        pstmt = con->prepare(
            "INSERT INTO MOVIMENTACAO (IdCarteira, Data, TipoOperacao, Quantidade) VALUES (?, CURDATE(), 'C', ?)");
        pstmt->setInt(0, idCarteira);
        pstmt->setDouble(1, quantidade);
        pstmt->executeUpdate();

        std::cout << "Compra registrada com sucesso!" << std::endl;
        std::cout << "Você comprou " << quantidade << " FT Coins por R$ " << valorReais << std::endl;
    }
    catch (const mariadb::SQLException &e)
    {
        std::cerr << "Erro ao registrar compra: " << e.what() << std::endl;
    }
}

void movimentacao::vendaRemota()
{
    int idCarteira;
    double quantidade;

    std::cout << "Digite o ID da carteira que deseja vender: ";
    std::cin >> idCarteira;

    try
    {
        mariadb::Client client = mariadb::Client::create();
        std::shared_ptr<mariadb::Connection> con = client->connect(
            "jdbc:mariadb://localhost:3306/PooI_25_Yxx",
            "seu_usuario",
            "sua_senha");

        // Verifica se carteira existe
        std::shared_ptr<mariadb::Statement> pstmt = con->prepare(
            "SELECT COUNT(*) AS total FROM CARTEIRA WHERE IdCarteira = ?");
        pstmt->setInt(0, idCarteira);
        std::shared_ptr<mariadb::ResultSet> res = pstmt->executeQuery();

        if (!res->next() || res->getInt("total") == 0)
        {
            std::cout << "Carteira com ID " << idCarteira << " não encontrada." << std::endl;
            return;
        }

        // Obtém a cotação mais recente
        pstmt = con->prepare("SELECT Cotacao FROM ORACULO ORDER BY Data DESC LIMIT 1");
        res = pstmt->executeQuery();

        double cotacao = 0;
        if (res->next())
        {
            cotacao = res->getDouble("Cotacao");
        }
        else
        {
            std::cout << "Erro: Nenhuma cotação disponível no oráculo." << std::endl;
            return;
        }

        std::cout << "Cotação atual da FT Coin: R$ " << cotacao << std::endl;
        std::cout << "Digite quantas FT Coins deseja vender: ";
        std::cin >> quantidade;

        if (quantidade <= 0)
        {
            std::cout << "Quantidade inválida. Venda cancelada." << std::endl;
            return;
        }

        // Registra a movimentação de venda
        pstmt = con->prepare(
            "INSERT INTO MOVIMENTACAO (IdCarteira, Data, TipoOperacao, Quantidade) VALUES (?, CURDATE(), 'V', ?)");
        pstmt->setInt(0, idCarteira);
        pstmt->setDouble(1, quantidade);
        pstmt->executeUpdate();

        std::cout << "Venda registrada com sucesso!" << std::endl;
        std::cout << "Você vendeu " << quantidade << " FT Coins a R$ " << cotacao << " cada." << std::endl;
    }
    catch (const mariadb::SQLException &e)
    {
        std::cerr << "Erro ao registrar venda: " << e.what() << std::endl;
    }
}