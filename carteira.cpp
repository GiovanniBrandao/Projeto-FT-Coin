#include "carteira.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <mariadb/conncpp.hpp>

using namespace std;

void CarteiraDAO_Local::criarCarteira(const Carteira &carteira)
{

   ifstream leitura("carteira.txt");
   int ID_carteira = 0;
   string linha;

   while (getline(leitura, linha))
   {
      if (linha.find("ID:") != string::npos) //"find" retorna "npos" caso a string não seja encontrada
      {
         int idLido = stoi(linha.substr(linha.find(":") + 1)); // encontrar o número do ID e soma 1
         if (idLido > ID_carteira)                             // registra o ID criado caso maior que o já registrado na classe
            ID_carteira = idLido;
      }
   }

   leitura.close();

   // Incrementa o ID para a próxima carteira
   ID_carteira++;

   ofstream escrita("carteira.txt", ios::app);
   if (!escrita)
   {
      cerr << "Erro ao abrir ou criar o arquivo" << endl;
      return;
   }

   escrita << "----------------------------------" << endl;
   escrita << "ID: " << ID_carteira << endl;
   escrita << "Nome: " << carteira.getTitular() << endl;
   escrita << "Corretora: " << carteira.getCorretora() << endl;
   escrita << "Moeda: " << carteira.getMoeda() << endl;

   escrita.close();

   cout << endl;
   cout << "Carteira com ID: " << ID_carteira << " criada" << endl;
   cout << endl;
}

Carteira CarteiraDAO_Local::consultarCarteira(int id)
{

   std::ifstream file("carteira.txt");
   if (!file)
   {
      std::cerr << "Erro ao abrir o arquivo!" << std::endl;
      return Carteira(); // retorno padrão vazio
   }

   std::string linha;
   bool encontrado = false;
   bool lendoBloco = false;
   Carteira resultado;

   while (std::getline(file, linha))
   {
      if (linha.find("ID:") != std::string::npos)
      {
         int idLido = std::stoi(linha.substr(linha.find(":") + 1));
         if (idLido == id)
         {
            encontrado = true;
            lendoBloco = true;
            // Aqui você pode ler as linhas seguintes e popular resultado, ex:
            // resultado.setId(idLido);
            // ... etc
         }
         else
         {
            lendoBloco = false;
         }
      }
      else if (lendoBloco)
      {
         // Parse linhas do bloco para preencher 'resultado'
      }
   }

   if (!encontrado)
   {
      std::cout << "Carteira com ID " << id << " não encontrada." << std::endl;
   }

   file.close();
   return resultado;
}

void CarteiraDAO_Local::editarCarteira(int id, const std::string &novoTitular)
{
   std::ifstream entrada("carteira.txt");
   std::ofstream saida("temp.txt");

   if (!entrada || !saida)
   {
      std::cerr << "Erro ao abrir os arquivos" << std::endl;
      return;
   }

   std::string linha;
   bool dentroDoBloco = false;
   bool editarBloco = false;
   int idLido;

   while (std::getline(entrada, linha))
   {
      if (linha.find("ID: ") != std::string::npos)
      {
         idLido = std::stoi(linha.substr(4)); // pega o número após "ID: "
         if (idLido == id)
         {
            editarBloco = true;
            dentroDoBloco = true;

            saida << linha << std::endl; // escreve a linha do ID
            saida << "Nome: " << novoTitular << std::endl;
            continue; // pula as linhas seguintes do bloco
         }
         else
         {
            editarBloco = false;
            dentroDoBloco = true;
         }
      }

      if (linha == "----------------------------------")
      {
         dentroDoBloco = false;
         saida << linha << std::endl;
         continue;
      }

      if (!editarBloco || !dentroDoBloco)
      {
         saida << linha << std::endl;
      }
   }

   entrada.close();
   saida.close();

   std::remove("carteira.txt");
   std::rename("temp.txt", "carteira.txt");

   if (editarBloco)
      std::cout << "Carteira editada com sucesso!" << std::endl;
   else
      std::cout << "Carteira com ID " << id << " não encontrada." << std::endl;
}

void CarteiraDAO_Local::excluirCarteira(int id)
{
   std::ifstream entrada("carteira.txt");
   std::ofstream saida("temp.txt");

   if (!entrada || !saida)
   {
      std::cerr << "Erro ao abrir os arquivos" << std::endl;
      return;
   }

   std::string linha;
   int idLido;
   bool excluirBloco = false;
   bool encontrado = false;

   while (std::getline(entrada, linha))
   {
      if (linha.find("ID: ") != std::string::npos)
      {
         idLido = std::stoi(linha.substr(4)); // extrai o número após "ID: "
         excluirBloco = (idLido == id);
         if (excluirBloco)
         {
            encontrado = true;
         }
      }

      if (!excluirBloco)
      {
         saida << linha << std::endl;
      }

      if (linha == "----------------------------------")
      {
         excluirBloco = false; // fim do bloco
      }
   }

   entrada.close();
   saida.close();

   std::remove("carteira.txt");
   std::rename("temp.txt", "carteira.txt");

   if (encontrado)
      std::cout << "Carteira com ID " << id << " foi excluída." << std::endl;
   else
      std::cout << "Carteira com ID " << id << " não foi encontrada." << std::endl;
}

void CarteiraDAO_Remoto::NovaCarteiraRemoto(const string &nome_titular, const string &corretora)
{
   try
   {
      // Criação do cliente MariaDB
      mariadb::Client *client = mariadb::Client::create();

      // Conexão com o banco de dados
      shared_ptr<mariadb::Connection> conn = client->connect(
          "jdbc:mariadb://localhost:3306/PooI_25_Yxx", // conexão completa
          "seu_usuario",
          "sua_senha");

      // Preparar a inserção
      shared_ptr<mariadb::PreparedStatement> pstmt =
          conn->prepare("INSERT INTO CARTEIRA (Titular, Corretora) VALUES (?, ?)");

      pstmt->setString(0, nome_titular);
      pstmt->setString(1, corretora);
      pstmt->execute();

      // Buscar o ID gerado
      shared_ptr<mariadb::Statement> stmt = conn->createStatement();
      shared_ptr<mariadb::ResultSet> res = stmt->executeQuery("SELECT LAST_INSERT_ID() AS id");

      if (res->next())
      {
         int idCriado = res->getInt("id");
         cout << "\nCarteira criada com sucesso" << endl;
         cout << "ID da nova carteira: " << idCriado << endl;
      }
   }
   catch (const mariadb::SQLException &e)
   {
      cerr << "Erro ao criar carteira no banco: " << e.what() << endl;
   }
}

Carteira CarteiraDAO_Remoto::CarteiraDAO_Remoto::consultarCarteira(int id)

{
   try
   {
      // Criação do cliente MariaDB
      mariadb::Client *client = mariadb::Client::create();

      // Conexão com o banco de dados
      shared_ptr<mariadb::Connection> conn = client->connect(
          "jdbc:mariadb://localhost:3306/PooI_25_Yxx", // substitua com seu banco
          "seu_usuario", "sua_senha");

      // Preparar a consulta
      shared_ptr<mariadb::PreparedStatement> pstmt =
          conn->prepare("SELECT * FROM CARTEIRA WHERE IdCarteira = ?");
      pstmt->setInt(0, id); // índices começam em 0 no MariaDB Connector

      // Executar e obter resultado
      shared_ptr<mariadb::ResultSet> res = pstmt->executeQuery();

      if (res->next())
      {
         Carteira carteira;
         carteira.setId(res->getInt("IdCarteira"));
         carteira.setTitular(res->getString("Titular"));
         carteira.setCorretora(res->getString("Corretora"));

         // Opcional: imprimir
         cout << "----------------------------------" << endl;
         cout << "ID: " << carteira.getId() << endl;
         cout << "Nome: " << carteira.getTitular() << endl;
         cout << "Corretora: " << carteira.getCorretora() << endl;

         return carteira;
      }
      else
      {
         cout << "Carteira com ID " << id << " não encontrada." << endl;
         return Carteira(); // carteira vazia
      }
   }
   catch (const mariadb::SQLException &e)
   {
      cerr << "Erro ao consultar carteira no banco: " << e.what() << endl;
      return Carteira(); // em caso de erro
   }
}

void CarteiraDAO_Remoto::editarCarteira(int id, const std::string &novoTitular, const std::string &novaCorretora)
{
   try {
      // Criar cliente e conexão
      mariadb::Client client = mariadb::Client::create();
      std::shared_ptr<mariadb::Connection> conn = client->connect(
         "jdbc:mariadb://localhost:3306/PooI_25_Yxx", // URL com nome do banco
         "seu_usuario", "sua_senha"
      );

      // Preparar e executar comando SQL
      std::shared_ptr<mariadb::Statement> pstmt = conn->prepare(
         "UPDATE CARTEIRA SET Titular = ?, Corretora = ? WHERE IdCarteira = ?"
      );
      pstmt->setString(0, novoTitular);
      pstmt->setString(1, novaCorretora);
      pstmt->setInt(2, id);

      int linhasAfetadas = pstmt->executeUpdate();

      if (linhasAfetadas > 0)
         std::cout << "Carteira ID " << id << " atualizada com sucesso!" << std::endl;
      else
         std::cout << "Carteira com ID " << id << " não encontrada." << std::endl;

   } catch (const mariadb::SQLException &e) {
      std::cerr << "Erro ao editar carteira no banco: " << e.what() << std::endl;
   }
}

void CarteiraDAO_Remoto::excluirCarteira(int id)
{
   try
   {
      // Criar cliente e conexão com banco
      mariadb::Client client = mariadb::Client::create();
      std::shared_ptr<mariadb::Connection> conn = client->connect(
          "jdbc:mariadb://localhost:3306/PooI_25_Yxx", // URL completa com banco
          "seu_usuario", "sua_senha");

      // Preparar e executar DELETE
      std::shared_ptr<mariadb::Statement> pstmt = conn->prepare(
          "DELETE FROM CARTEIRA WHERE IdCarteira = ?");
      pstmt->setInt(0, id); // Índices começam em 0

      int linhasAfetadas = pstmt->executeUpdate();

      if (linhasAfetadas > 0)
      {
         std::cout << "Carteira ID " << id << " excluída com sucesso." << std::endl;
      }
      else
      {
         std::cout << "Carteira com ID " << id << " não encontrada." << std::endl;
      }
   }
   catch (const mariadb::SQLException &e)
   {
      std::cerr << "Erro ao excluir carteira no banco: " << e.what() << std::endl;
   }
}