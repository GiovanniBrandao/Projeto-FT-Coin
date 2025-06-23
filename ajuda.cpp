#include <iostream>
#include "ajuda.hpp"

void mostrarAjuda()
{
    std::cout << R"(
====================================
        AJUDA - FT Coin
====================================
Este programa permite o gerenciamento de carteiras virtuais com operações de
compra e venda da criptomoeda fictícia FT Coin.

COMANDOS DISPONÍVEIS:
1. Cadastrar Carteira
2. Editar Carteira
3. Comprar Moedas
4. Vender Moedas
5. Listar Movimentações
6. Sair

- A cotação da FT Coin é obtida da tabela ORACULO.
- As movimentações são salvas no MariaDB.
- Requisitos: conexão ativa com MariaDB.

Para mais informações, consulte a documentação.
)";
}

void mostrarCreditos()
{
    std::cout << R"(
====================================
     FT COIN - CRÉDITOS
====================================
Autores:
- Giovanni da Silva Virginio Brandão
- Murillo Martins Proveza
- Nathan Damico Cardoso
- Thomaz Henrique Pedro Miranda
- Victor Hugo Silva

Disciplina:
Programação Orientada a Objetos I

Instituição:
Universidade XYZ

Versão: 1.0.0 - Junho/2025
Copyright (c) 2025
Todos os direitos reservados.
)";
}
