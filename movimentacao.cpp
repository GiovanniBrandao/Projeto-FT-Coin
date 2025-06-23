#include <mariadb/conncpp.hpp>
#include "oraculo.hpp"
#include "movimentacao.hpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <memory>

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

    try
    {
        // Alterado para obter o driver e a conexão corretamente
        sql::Driver *driver = sql::mariadb::get_driver_instance();
         std::shared_ptr<sql::Connection> conn(driver->connect(
          "jdbc:mariadb://*****:3306/*****", //IP e user
          "*******",  // usuário
          "*******")); // senha

        // Verifica se carteira existe
        std::shared_ptr<sql::PreparedStatement> stmntCheckCarteira(conn->prepareStatement("SELECT COUNT(*) AS total FROM CARTEIRA WHERE IdCarteira = ?")); // Usado stmntCheckCarteira para clareza
        stmntCheckCarteira->setInt(0, idCarteira); // Índices em 0

        std::shared_ptr<sql::ResultSet> resCheckCarteira(stmntCheckCarteira->executeQuery()); // Renomeado e envolvido em shared_ptr
        
        if (!resCheckCarteira->next() || resCheckCarteira->getInt("total") == 0) // Usado resCheckCarteira
        {
            cout << "Carteira com ID " << idCarteira << " não encontrada." << endl;
            return;
        }

        // Pega cotação mais recente
        std::shared_ptr<sql::PreparedStatement> stmntCotacao(conn->prepareStatement("SELECT Cotacao FROM ORACULO ORDER BY Data DESC LIMIT 1")); // Usado stmntCotacao
        std::shared_ptr<sql::ResultSet> resCotacao(stmntCotacao->executeQuery()); // Usado resCotacao
        
        double cotacao = 0;
        if (resCotacao->next()) // Usado resCotacao
        {
            cotacao = resCotacao->getDouble("Cotacao"); // Usado resCotacao
        }
        else
        {
            cout << "Erro: Nenhuma cotação disponível no oráculo." << endl;
            return;
        }

        cout << "Cotação atual da FT Coin: R$ " << cotacao << endl;
        cout << "Digite quanto deseja gastar em reais: R$ ";
        cin >> valorReais;

        if (valorReais <= 0)
        {
            cout << "Valor inválido. Compra cancelada." << endl;
            return;
        }

        double quantidade = valorReais / cotacao;

        // Registra a movimentação de compra
        // As chamadas setInt/setDouble são para PreparedStatement, então a variável precisa ser PreparedStatement.
        // A string SQL foi formatada para clareza e correção de espaços.
        std::shared_ptr<sql::PreparedStatement> stmntInsert(conn->prepareStatement(
            "INSERT INTO MOVIMENTACAO (IdCarteira, Data, TipoOperacao, Quantidade) "
            "VALUES (?, CURDATE(), 'C', ?)"
        ));
        stmntInsert->setInt(0, idCarteira);     // Índices em 0 para PreparedStatement
        stmntInsert->setDouble(1, quantidade);
        stmntInsert->executeUpdate(); // executeUpdate() é mais apropriado para INSERT

        cout << "Compra registrada com sucesso!" << endl;
        cout << "Você comprou " << quantidade << " FT Coins por R$ " << valorReais << endl;
    }
    catch (const sql::SQLException &e)
    {
        cerr << "Erro ao registrar compra: " << e.what() << endl;
    }
}

void movimentacao::vendaRemota()
{
    int idCarteira;
    double quantidade;

    cout << "Digite o ID da carteira que deseja vender: ";
    cin >> idCarteira;

    try
    {
        // Alterado para obter o driver e a conexão corretamente
        sql::Driver *driver = sql::mariadb::get_driver_instance();
        std::shared_ptr<sql::Connection> conn(driver->connect(
          "jdbc:mariadb://*****:3306/*****", //IP e user
          "*******",  // usuário
          "*******")); // senha

        // Verifica se carteira existe
        std::shared_ptr<sql::PreparedStatement> stmntCheckCarteira(conn->prepareStatement("SELECT COUNT(*) AS total FROM CARTEIRA WHERE IdCarteira = ?"));
        stmntCheckCarteira->setInt(0, idCarteira); // Índices em 0

        std::shared_ptr<sql::ResultSet> resCheckCarteira(stmntCheckCarteira->executeQuery()); // Renomeado e envolvido em shared_ptr

        if (!resCheckCarteira->next() || resCheckCarteira->getInt("total") == 0) // Usado resCheckCarteira
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

        cout << "Cotação atual da FT Coin: R$ " << cotacao << endl;
        cout << "Digite quantas FT Coins deseja vender: ";
        cin >> quantidade;

        if (quantidade <= 0)
        {
            cout << "Quantidade inválida. Venda cancelada." << endl;
            return;
        }

        // Registra a movimentação de venda
        std::shared_ptr<sql::PreparedStatement> stmntInsert(conn->prepareStatement(
            "INSERT INTO MOVIMENTACAO (IdCarteira, Data, TipoOperacao, Quantidade) "
            "VALUES (?, CURDATE(), 'V', ?)"
        ));
        stmntInsert->setInt(0, idCarteira);
        stmntInsert->setDouble(1, quantidade);
        stmntInsert->executeUpdate();

        cout << "Venda registrada com sucesso!" << endl;
        cout << "Você vendeu " << quantidade << " FT Coins a R$ " << cotacao << " cada." << endl;
    }
    catch (const sql::SQLException &e)
    {
        cerr << "Erro ao registrar venda: " << e.what() << endl;
    }
}