#include <mariadb/conncpp.hpp>
#include "carteira.hpp"
#include <fstream>
#include <iostream>
#include <memory>

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
      std::cerr << "Erro ao abrir o arquivo" << std::endl;
      return Carteira();
   }

   std::string linha;
   bool encontrado = false;
   Carteira resultado;

   while (std::getline(file, linha))
   {
      if (linha.find("ID:") != std::string::npos)
      {
         int idLido = std::stoi(linha.substr(linha.find(":") + 1));
         if (idLido == id)
         {
            encontrado = true;
            resultado.setId(idLido); // Define o ID da carteira encontrada

            if (std::getline(file, linha) && linha.find("Nome:") != std::string::npos)
            {
               resultado.setTitular(linha.substr(linha.find(":") + 2));
            }
            if (std::getline(file, linha) && linha.find("Corretora:") != std::string::npos)
            {
               resultado.setCorretora(linha.substr(linha.find(":") + 2));
            }
            if (std::getline(file, linha) && linha.find("Moeda:") != std::string::npos)
            {
            }

            break;
         }
      }
   }

   file.close();

   if (!encontrado)
   {
      std::cout << "Carteira com ID " << id << " nao encontrada." << std::endl;
      return Carteira();
   }
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
   bool editadoComSucesso = false; // Flag para indicar se a carteira foi encontrada e editada

   while (std::getline(entrada, linha))
   {
      if (linha.find("ID: ") != std::string::npos)
      {
         int idLido = std::stoi(linha.substr(linha.find(":") + 1));
         if (idLido == id)
         {
            editadoComSucesso = true;

            saida << linha << std::endl;

            std::getline(entrada, linha);
            saida << "Nome: " << novoTitular << std::endl;

            if (std::getline(entrada, linha))
            {
               saida << linha << std::endl;
            }

            if (std::getline(entrada, linha))
            {
               saida << linha << std::endl;
            }
         }
         else
         {
            saida << linha << std::endl;
         }
      }
      else
      {
         if (!editadoComSucesso || linha.find("----------------------------------") == std::string::npos)
         {
            saida << linha << std::endl;
         }
         else
         {
            saida << linha << std::endl;
            editadoComSucesso = false;
         }
      }
   }

   entrada.close();
   saida.close();

   std::remove("carteira.txt");
   std::rename("temp.txt", "carteira.txt");

   if (editadoComSucesso) // Verifica se a carteira foi encontrada e editada
      std::cout << "Carteira editada com sucesso" << std::endl;
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
      std::cout << "Carteira com ID " << id << " foi excluida." << std::endl;
   else
      std::cout << "Carteira com ID " << id << " não foi encontrada." << std::endl;
}

void CarteiraDAO_Remoto::criarCarteira(const Carteira &carteira)
{
   try
   {
      sql::Driver *driver = sql::mariadb::get_driver_instance();
      std::shared_ptr<sql::Connection> conn(driver->connect(
          "jdbc:mariadb://*******:3306/*******", // Altere o IP/Porta/Nome do Banco
          "*******",                                    // Altere o usuário
          "*******"));                                   // Altere a senha

      // Altere a senha

      // Preparar a inserção
      std::shared_ptr<sql::PreparedStatement> stmntInsert(conn->prepareStatement("INSERT INTO CARTEIRA (Titular, Corretora) VALUES (?, ?)"));

      stmntInsert->setString(0, carteira.getTitular());
      stmntInsert->setString(1, carteira.getCorretora());
      stmntInsert->execute();

      // Renomeado para evitar conflito de nome com o stmnt anterior
      std::shared_ptr<sql::Statement> stmntLastId(conn->createStatement());
      std::shared_ptr<sql::ResultSet> resLastId(stmntLastId->executeQuery("SELECT LAST_INSERT_ID() AS id"));

      if (resLastId->next())
      {
         int idCriado = resLastId->getInt("id");
         std::cout << "\nCarteira criada com sucesso" << std::endl;
         std::cout << "ID da nova carteira: " << idCriado << std::endl;
      }
   }
   catch (const sql::SQLException &e)
   {
      std::cerr << "Erro ao criar carteira no banco: " << e.what() << std::endl;
   }
}

Carteira CarteiraDAO_Remoto::consultarCarteira(int id)
{
   try
   {
      sql::Driver *driver = sql::mariadb::get_driver_instance();
      std::shared_ptr<sql::Connection> conn(driver->connect(
          "jdbc:mariadb://*******:3306/*******", // Altere o IP/Porta/Nome do Banco
          "*******",                                    // Altere o usuário
          "*******"));                                   // Altere a senha

      // Preparar a consulta
      std::shared_ptr<sql::PreparedStatement> stmntSelect(conn->prepareStatement("SELECT * FROM CARTEIRA WHERE IdCarteira = ?"));
      stmntSelect->setInt(0, id);

      // Executar e obter resultado
      std::shared_ptr<sql::ResultSet> resSelect(stmntSelect->executeQuery()); // CORREÇÃO: Inicialização correta de shared_ptr

      if (resSelect->next())
      {
         Carteira carteira;
         carteira.setId(resSelect->getInt("IdCarteira"));
         carteira.setTitular(std::string(resSelect->getString("Titular").c_str()));
         carteira.setCorretora(std::string(resSelect->getString("Corretora").c_str()));
      }
      else
      {
         cout << "Carteira com ID " << id << " não encontrada." << endl;
         return Carteira(); // carteira vazia
      }
   }
   catch (const sql::SQLException &e)
   {
      cerr << "Erro ao consultar carteira no banco: " << e.what() << endl;
      return Carteira(); // em caso de erro
   }

   return Carteira(); // Retorno padrão caso nenhum if/else retorne
}

void CarteiraDAO_Remoto::editarCarteira(int id, const std::string &novoTitular, const std::string &novaCorretora)
{
   try
   {
      sql::Driver *driver = sql::mariadb::get_driver_instance();
      std::shared_ptr<sql::Connection> conn(driver->connect(
          "jdbc:mariadb://*******:3306/*******", // Altere o IP/Porta/Nome do Banco
          "*******",                                    // Altere o usuário
          "*******"));                                   // Altere a senha

      // Preparar e executar comando SQL
      std::shared_ptr<sql::PreparedStatement> stmntUpdate(conn->prepareStatement("UPDATE CARTEIRA SET Titular = ?, Corretora = ? WHERE IdCarteira = ?"));

      stmntUpdate->setString(0, novoTitular);
      stmntUpdate->setString(1, novaCorretora);
      stmntUpdate->setInt(2, id);

      int linhasAfetadas = stmntUpdate->executeUpdate();

      if (linhasAfetadas > 0)
         std::cout << "Carteira ID " << id << " atualizada com sucesso!" << std::endl;
      else
         std::cout << "Carteira com ID " << id << " não encontrada." << std::endl;
   }
   catch (const sql::SQLException &e)
   {
      std::cerr << "Erro ao editar carteira no banco: " << e.what() << std::endl;
   }
}

void CarteiraDAO_Remoto::excluirCarteira(int id)
{
   try
   {
      sql::Driver *driver = sql::mariadb::get_driver_instance();
      std::shared_ptr<sql::Connection> conn(driver->connect(
          "jdbc:mariadb://*******:3306/*******", // Altere o IP/Porta/Nome do Banco
          "*******",                                    // Altere o usuário
          "*******"));                                   // Altere a senha

      std::shared_ptr<sql::PreparedStatement> stmntDelete(conn->prepareStatement("DELETE FROM CARTEIRA WHERE IdCarteira = ?"));
      stmntDelete->setInt(0, id);

      int linhasAfetadas = stmntDelete->executeUpdate();

      if (linhasAfetadas > 0)
      {
         std::cout << "Carteira ID " << id << " excluída com sucesso." << std::endl;
      }
      else
      {
         std::cout << "Carteira com ID " << id << " não encontrada." << std::endl;
      }
   }
   catch (const sql::SQLException &e)
   {
      std::cerr << "Erro ao excluir carteira no banco: " << e.what() << std::endl;
   }
}