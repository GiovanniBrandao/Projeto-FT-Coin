#include "carteira.hpp"

#include <fstream>
#include <iostream>

void carteira::NovaCarteiraLocal()
{

   // abre em modo leitura para descobrir o último ID
   ifstream leitura("carteira.txt");
   int ID_carteira = 0;
   string linha;

   while (getline(leitura, linha))
   {
      if (linha.find("ID:") != string::npos) //"find" retorna "npos" caso a string não seja encontrada
      {
         int idLido = stoi(linha.substr(linha.find(":") + 1)); //encontrar o número do ID e soma 1
         if (idLido > ID_carteira) //registra o ID criado caso maior que o já registrado na classe
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
   escrita << "Nome: " << nome_titular << endl;
   escrita << "Fundos: " << saldo << endl;
   escrita << "Moeda: " << moeda << endl;

   escrita.close();
}

void carteira::ChecarCarteiraLocal()
{

   ifstream file("carteira.txt");

   if (!file)
   {
      cerr << "Erro ao abrir o arquivo!" << endl;
      return;
   }

   int idProcurado;
   cout << "Digite o ID da carteira que deseja consultar: ";
   cin >> idProcurado;

   string linha;
   bool encontrado = false;
   bool lendoBloco = false;

   while (getline(file, linha))
   {
      if (linha.find("ID:") != string::npos)
      {
         int idLido = stoi(linha.substr(linha.find(":") + 1));
         if (idLido == idProcurado)
         {
            encontrado = true;
            lendoBloco = true;
            cout << "----------------------------------" << endl;
            cout << linha << endl;
         }
         else
         {
            lendoBloco = false;
         }
      }
      else if (lendoBloco)
      {
         cout << linha << endl;
      }
   }

   if (!encontrado)
   {
      cout << "Carteira com ID " << idProcurado << " não encontrada." << endl;
   }

   file.close();
}

void carteira::EditarCarteiraLocal() {}

void carteira::ExcluirCarteiraLocal()
{

   int escolha;

   cout << "Escolha um ID para excluir: " << endl;
   cin >> escolha;

   ifstream entrada("carteira.txt");
   ofstream saida("temp.txt");

   if (!entrada || !saida)
   {
      cerr << "Erro ao abrir os arquivos" << endl;
      return;
   }

   string linha;
   int idLido;
   bool excluirBloco = false;

   // le apenas os IDs até encontrar o escolhido
   while (getline(entrada, linha))
   {
      if (linha.find("ID: ") != string::npos) //"find" retorna "npos" caso a string não seja encontrada
      {
         idLido = stoi(linha.substr(4)); // pega o número após o "ID: "
         excluirBloco = (idLido == escolha);
      }

      if (!excluirBloco) // escreve a mesma linha caso não seja a do ID escolhido
      {
         saida << linha << endl;
      }

      if (linha == "----------------------------------")
      {
         excluirBloco = false; // exclui bloco até o fim de um registro
      }
   }

   entrada.close();
   saida.close();

   remove("carteira.txt");
   rename("temp.txt", "carteira.txt");

   cout << "Carteira com ID " << escolha << " foi excluida." << endl;
}


void NovaCarteiraRemoto() {}

void ChecarCarteiraRemoto() {}

void EditarCarteiraRemoto() {}

void ExcluirCarteiraRemoto() {}