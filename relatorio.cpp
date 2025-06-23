#include <mariadb/conncpp.hpp>
#include "relatorio.hpp"
#include "carteira.hpp"
#include "movimentacao.hpp"
#include "oraculo.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <memory>

using namespace std;

struct InfoCarteira
{
    int id;
    string nome;
    string corretora;
};

shared_ptr<sql::Connection> conectarMariaDB()
{

    sql::Driver *driver = sql::mariadb::get_driver_instance();
    return std::shared_ptr<sql::Connection> conn(driver->connect(
          "jdbc:mariadb://*****:3306/*****", //IP e user
          "*******",  // usuário
          "*******")); // senha
}

void RelatorioLocal::listarCarteirasPorId()
{
    ifstream file("carteira.txt");
    if (!file)
    {
        cout << "Arquivo de carteiras não encontrado." << endl;
        return;
    }

    string linha;
    vector<InfoCarteira> carteiras;
    InfoCarteira temp;

    while (getline(file, linha))
    {
        if (linha.find("ID:") != string::npos)
        {
            temp.id = stoi(linha.substr(linha.find(":") + 1));
        }
        else if (linha.find("Nome:") != string::npos)
        {
            temp.nome = linha.substr(linha.find(":") + 2);
        }
        else if (linha.find("Corretora:") != string::npos)
        {
            temp.corretora = linha.substr(linha.find(":") + 2);
            carteiras.push_back(temp);
        }
    }

    sort(carteiras.begin(), carteiras.end(), [](auto &a, auto &b)
         { return a.id < b.id; });

    cout << "\nCarteiras ordenadas por ID:\n";
    for (const auto &c : carteiras)
    {
        cout << "ID: " << c.id << ", Nome: " << c.nome << ", Corretora: " << c.corretora << endl;
    }
}

void RelatorioLocal::listarCarteirasPorNome()
{
    ifstream file("carteira.txt");
    if (!file)
    {
        cout << "Arquivo de carteiras não encontrado." << endl;
        return;
    }

    string linha;
    vector<InfoCarteira> carteiras;
    InfoCarteira temp;

    while (getline(file, linha))
    {
        if (linha.find("ID:") != string::npos)
        {
            temp.id = stoi(linha.substr(linha.find(":") + 1));
        }
        else if (linha.find("Nome:") != string::npos)
        {
            temp.nome = linha.substr(linha.find(":") + 2);
        }
        else if (linha.find("Corretora:") != string::npos)
        {
            temp.corretora = linha.substr(linha.find(":") + 2);
            carteiras.push_back(temp);
        }
    }

    sort(carteiras.begin(), carteiras.end(), [](auto &a, auto &b)
         { return a.nome < b.nome; });

    cout << "\nCarteiras ordenadas por Nome:\n";
    for (const auto &c : carteiras)
    {
        cout << "ID: " << c.id << ", Nome: " << c.nome << ", Corretora: " << c.corretora << endl;
    }
}

void RelatorioLocal::exibirSaldoCarteira(int idCarteira)
{
    ifstream file("movimentacoes.txt");
    if (!file)
    {
        cout << "Arquivo de movimentações não encontrado." << endl;
        return;
    }

    string linha;
    double saldo = 0.0;

    while (getline(file, linha))
    {
        if (linha.find("IDCarteira: " + to_string(idCarteira)) != string::npos)
        {
            size_t tipoPos = linha.find("Tipo: ");
            size_t qtdePos = linha.find("Quantidade: ");
            if (tipoPos != string::npos && qtdePos != string::npos)
            {
                char tipo = linha[tipoPos + 6];
                double qtde = stod(linha.substr(qtdePos + 12));
                saldo += (tipo == 'C') ? qtde : -qtde;
            }
        }
    }

    cout << fixed << setprecision(6);
    cout << "\nSaldo atual da carteira " << idCarteira << ": " << saldo << " FT Coins\n";
}

void RelatorioLocal::exibirHistoricoCarteira(int idCarteira)
{
    ifstream file("movimentacoes.txt");
    if (!file)
    {
        cout << "Arquivo de movimentações não encontrado." << endl;
        return;
    }

    string linha;
    cout << "\nHistórico da carteira " << idCarteira << ":\n";

    while (getline(file, linha))
    {
        if (linha.find("IDCarteira: " + to_string(idCarteira)) != string::npos)
        {
            cout << linha << endl;
        }
    }
}

void RelatorioLocal::exibirGanhoPerdaCarteira(int idCarteira)
{
    ifstream file("movimentacoes.txt");
    if (!file)
    {
        cout << "Arquivo de movimentações não encontrado." << endl;
        return;
    }

    oraculo oracle;
    double totalComprado = 0.0;
    double totalVendido = 0.0;
    double cotacao = oracle.cotacao;

    string linha;
    while (getline(file, linha))
    {
        if (linha.find("IDCarteira: " + to_string(idCarteira)) != string::npos)
        {
            size_t tipoPos = linha.find("Tipo: ");
            size_t qtdePos = linha.find("Quantidade: ");
            if (tipoPos != string::npos && qtdePos != string::npos)
            {
                char tipo = linha[tipoPos + 6];
                double qtde = stod(linha.substr(qtdePos + 12));
                if (tipo == 'C')
                {
                    totalComprado += qtde * cotacao;
                }
                else if (tipo == 'V')
                {
                    totalVendido += qtde * cotacao;
                }
            }
        }
    }

    double lucro = totalVendido - totalComprado;

    cout << fixed << setprecision(2);
    cout << "\nGanho/Perda da carteira " << idCarteira << ": R$ " << lucro << endl;
}

void RelatorioRemoto::listarCarteirasPorId()
{
    try
    {
        auto conn = conectarMariaDB();
        auto stmt = conn->createStatement();
        auto res = stmt->executeQuery("SELECT IdCarteira, Titular, Corretora FROM CARTEIRA ORDER BY IdCarteira");

        cout << "\nCarteiras ordenadas por ID:\n";
        while (res->next())
        {
            cout << "ID: " << res->getInt("IdCarteira")
                 << ", Nome: " << res->getString("Titular")
                 << ", Corretora: " << res->getString("Corretora") << endl;
        }
    }
    catch (const sql::SQLException &e)
    {
        cerr << "Erro ao listar carteiras por ID: " << e.what() << endl;
    }
}

void RelatorioRemoto::listarCarteirasPorNome()
{
    try
    {
        auto conn = conectarMariaDB();
        auto stmt = conn->createStatement();
        auto res = stmt->executeQuery("SELECT IdCarteira, Titular, Corretora FROM CARTEIRA ORDER BY Titular");

        cout << "\nCarteiras ordenadas por Nome:\n";
        while (res->next())
        {
            cout << "ID: " << res->getInt("IdCarteira")
                 << ", Nome: " << res->getString("Titular")
                 << ", Corretora: " << res->getString("Corretora") << endl;
        }
    }
    catch (const sql::SQLException &e)
    {
        cerr << "Erro ao listar carteiras por Nome: " << e.what() << endl;
    }
}

void RelatorioRemoto::exibirSaldoCarteira(int idCarteira)
{
    try
    {
        auto conn = conectarMariaDB();
        auto stmt = conn->prepareStatement("SELECT TipoOperacao, Quantidade FROM MOVIMENTACAO WHERE IdCarteira = ?");

        std::shared_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement("YOUR_SQL_STRING")); // CORRETO

        stmt->setInt(0, idCarteira);
        auto res = stmt->executeQuery();

        double saldo = 0.0;
        while (res->next())
        {
            char tipo = res->getString("TipoOperacao")[0];
            double qtde = res->getDouble("Quantidade");
            saldo += (tipo == 'C') ? qtde : -qtde;
        }

        cout << fixed << setprecision(6);
        cout << "\nSaldo atual da carteira " << idCarteira << ": " << saldo << " FT Coins\n";
    }
    catch (const sql::SQLException &e)
    {
        cerr << "Erro ao calcular saldo: " << e.what() << endl;
    }
}

void RelatorioRemoto::exibirHistoricoCarteira(int idCarteira)
{
    try
    {
        auto conn = conectarMariaDB();
        auto stmt = conn->prepareStatement("SELECT IdMov, Data, TipoOperacao, Quantidade FROM MOVIMENTACAO WHERE IdCarteira = ? ORDER BY Data");
        stmt->setInt(0, idCarteira);
        auto res = stmt->executeQuery();

        cout << "\nHistórico da carteira " << idCarteira << ":\n";
        while (res->next())
        {
            cout << "IDMov: " << res->getInt("IdMov")
                 << " | Data: " << res->getString("Data")
                 << " | Tipo: " << res->getString("TipoOperacao")
                 << " | Quantidade: " << res->getDouble("Quantidade") << endl;
        }
    }
    catch (const sql::SQLException &e)
    {
        cerr << "Erro ao exibir histórico: " << e.what() << endl;
    }
}

void RelatorioRemoto::exibirGanhoPerdaCarteira(int idCarteira)
{
    try
    {
        auto conn = conectarMariaDB();
        auto stmt = conn->prepareStatement("SELECT M.TipoOperacao, M.Quantidade, O.Cotacao FROM MOVIMENTACAO M JOIN ORACULO O ON M.Data = O.Data WHERE M.IdCarteira = ?");
        stmt->setInt(0, idCarteira);
        auto res = stmt->executeQuery();

        double totalComprado = 0.0;
        double totalVendido = 0.0;

        while (res->next())
        {
            char tipo = res->getString("TipoOperacao")[0];
            double qtde = res->getDouble("Quantidade");
            double cotacao = res->getDouble("Cotacao");

            if (tipo == 'C')
                totalComprado += qtde * cotacao;
            else if (tipo == 'V')
                totalVendido += qtde * cotacao;
        }

        double lucro = totalVendido - totalComprado;
        cout << fixed << setprecision(2);
        cout << "\nGanho/Perda da carteira " << idCarteira << ": R$ " << lucro << endl;
    }
    catch (const sql::SQLException &e)
    {
        cerr << "Erro ao calcular ganho/perda: " << e.what() << endl;
    }
}
