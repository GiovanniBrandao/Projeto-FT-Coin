#include "interface.hpp"
#include <iostream>
#include <limits>

using namespace std;

int readIntegerInput() {
    int choice;
    while (!(cin >> choice)) { 
        cout << "Entrada invalida. Por favor, digite um numero: ";
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}
int Interface::MenuGravacao()
{

    cout << "----------------------------------" << endl;
    cout << "=== BEM VINDO AO SISTEMA FT COIN ===" << endl;
    cout << "----------------------------------" << endl;
    cout << "escolha o tipo de gravacao de arquivo" << endl;
    cout << "----------------------------------" << endl;
    cout << "[1] - Gravacao Local" << endl;
    cout << "[2] - Gravacao Remota" << endl;
    cout << "----------------------------------" << endl;
    int escolha = readIntegerInput();
    return escolha;
 
}

int Interface::MenuPrincipal()
{
    cout << "\n--- MENU PRINCIPAL ---" << endl;
    cout << "[1] - Carteira" << endl;
    cout << "[2] - Movimentacao" << endl;
    cout << "[3] - Relatorios" << endl;
    cout << "[4] - Ajuda" << endl;
    cout << "[0] - Sair" << endl;
    cout << "----------------------------------" << endl;
    int escolha = readIntegerInput();

    return escolha;
}

int Interface::MenuCarteira()
{

    cout << "----------------------------------" << endl;
    cout << "[1] - Novo Registro" << endl;
    cout << "[2] - Verificar Registro" << endl;
    cout << "[3] - Editar Registro" << endl;
    cout << "[4] - Excluir Registro" << endl;
    cout << "[0] - Voltar" << endl;
    cout << "----------------------------------" << endl;
    int escolha = readIntegerInput();

    return escolha;
}

int Interface::MenuMovimentacao()
{

    cout << "----------------------------------" << endl;
    cout << "[1] - Comprar Moeda" << endl;
    cout << "[2] - Vender Moeda" << endl;
    cout << "[0] - Voltar" << endl;
    cout << "----------------------------------" << endl;
    int escolha = readIntegerInput();

    return escolha;
}

int Interface::MenuRelatorios()
{

    cout << "----------------------------------" << endl;
    cout << "[1] - Ordem de Carteiras por Identificador" << endl;
    cout << "[2] - Ordem de Carteiras por Nome" << endl;
    cout << "[3] - Saldo Atual da Carteira" << endl;
    cout << "[4] - Historico de Movimentacao" << endl;
    cout << "[5] - Valor de Lucro" << endl;
    cout << "[0] - Voltar" << endl;
    cout << "----------------------------------" << endl;
    int escolha = readIntegerInput();

    return escolha;
}

int Interface::MenuAjuda()
{

    cout << "----------------------------------" << endl;
    cout << "[1] - Instrucoes" << endl;
    cout << "[2] - Creditos" << endl;
    cout << "[0] - Voltar" << endl;
    cout << "----------------------------------" << endl;
    int escolha = readIntegerInput();

    return escolha;
}
