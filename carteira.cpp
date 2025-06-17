#include "carteira.hpp"

#include <fstream>
#include <iostream>
#include <memory>
/*#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>*/

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

Carteira consultarCarteira(int id)
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

void editarCarteira(int id, const std::string &novoTitular)
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

void excluirCarteira(int id)
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

void CarteiraDAO_Remoto::criarCarteira(const Carteira &carteira)
{
   try
   {
      sql::mysql::MySQL_Driver *driver;
      std::unique_ptr<sql::Connection> con;
      std::unique_ptr<sql::PreparedStatement> pstmt;
      std::unique_ptr<sql::Statement> stmt;
      std::unique_ptr<sql::ResultSet> res;

      // Conectar ao MariaDB
      driver = sql::mysql::get_mysql_driver_instance();
      con.reset(driver->connect("tcp://localhost:3306", "seu_usuario", "sua_senha"));
      con->setSchema("PooI_25_Yxx"); // substitua pelo nome do seu banco

      // Inserir dados na tabela CARTEIRA
      pstmt.reset(con->prepareStatement(
          "INSERT INTO CARTEIRA (Titular, Corretora) VALUES (?, ?)"));
      pstmt->setString(1, carteira.getTitular());
      pstmt->setString(2, carteira.getCorretora());
      pstmt->executeUpdate();

      // Recuperar o ID gerado
      stmt.reset(con->createStatement());
      res.reset(stmt->executeQuery("SELECT LAST_INSERT_ID() AS id"));
      if (res->next())
      {
         int idCriado = res->getInt("id");
         std::cout << "\nCarteira criada com sucesso" << std::endl;
         std::cout << "ID da nova carteira: " << idCriado << std::endl;
      }
   }
   catch (sql::SQLException &e)
   {
      std::cerr << "Erro ao criar carteira no banco: " << e.what() << std::endl;
   }
}

Carteira CarteiraDAO_Remoto::consultarCarteira(int id)
{
   try
   {
      sql::mysql::MySQL_Driver *driver;
      unique_ptr<sql::Connection> con;
      unique_ptr<sql::PreparedStatement> pstmt;
      unique_ptr<sql::ResultSet> res;

      // Conectar ao banco
      driver = sql::mysql::get_mysql_driver_instance();
      con.reset(driver->connect("tcp://localhost:3306", "seu_usuario", "sua_senha"));
      con->setSchema("PooI_25_Yxx");

      // Query para buscar a carteira
      pstmt.reset(con->prepareStatement("SELECT * FROM CARTEIRA WHERE IdCarteira = ?"));
      pstmt->setInt(1, id);
      res.reset(pstmt->executeQuery());

      if (res->next())
      {
         // Cria e retorna o objeto Carteira
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
         return Carteira(); // Retorna uma carteira vazia
      }
   }
   catch (sql::SQLException &e)
   {
      cerr << "Erro ao consultar carteira no banco: " << e.what() << endl;
      return Carteira(); // Retorna carteira vazia em caso de erro
   }
}

void CarteiraDAO_Remoto::editarCarteira(int id, const string &novoTitular, const string &novaCorretora)
{
   try
   {
      sql::mysql::MySQL_Driver *driver;
      std::unique_ptr<sql::Connection> con;
      std::unique_ptr<sql::PreparedStatement> pstmt;

      // Conectar ao banco
      driver = sql::mysql::get_mysql_driver_instance();
      con.reset(driver->connect("tcp://localhost:3306", "seu_usuario", "sua_senha"));
      con->setSchema("PooI_25_Yxx");

      // Atualizar os dados
      pstmt.reset(con->prepareStatement(
          "UPDATE CARTEIRA SET Titular = ?, Corretora = ? WHERE IdCarteira = ?"));
      pstmt->setString(1, novoTitular);
      pstmt->setString(2, novaCorretora);
      pstmt->setInt(3, id);

      int linhasAfetadas = pstmt->executeUpdate();

      if (linhasAfetadas > 0)
      {
         std::cout << "Carteira ID " << id << " atualizada com sucesso!" << std::endl;
      }
      else
      {
         std::cout << "Carteira com ID " << id << " não encontrada." << std::endl;
      }
   }
   catch (sql::SQLException &e)
   {
      std::cerr << "Erro ao editar carteira no banco: " << e.what() << std::endl;
   }
}

void CarteiraDAO_Remoto::excluirCarteira(int id)
{
   try
   {
      sql::mysql::MySQL_Driver *driver;
      std::unique_ptr<sql::Connection> con;
      std::unique_ptr<sql::PreparedStatement> pstmt;

      // Conectar ao banco
      driver = sql::mysql::get_mysql_driver_instance();
      con.reset(driver->connect("tcp://localhost:3306", "seu_usuario", "sua_senha"));
      con->setSchema("PooI_25_Yxx");

      // Preparar e executar delete
      pstmt.reset(con->prepareStatement("DELETE FROM CARTEIRA WHERE IdCarteira = ?"));
      pstmt->setInt(1, id);

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
   catch (sql::SQLException &e)
   {
      std::cerr << "Erro ao excluir carteira no banco: " << e.what() << std::endl;
   }
}