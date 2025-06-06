#include "interface.hpp"
#include <iostream>

using namespace std;


int MenuGravacao()
{

    int escolha;
    cout << "[1] - Gravacao Local" << endl;
    cout << "[2] - Gravacao Remota" << endl;
    cin >> escolha;

    return escolha;
}

int MenuPrincipal()
{
    int escolha;
    cout << "[1] - Carteira" << endl;
    cout << "[2] - Movimentacao" << endl;
    cout << "[3] - Relatorios" << endl;
    cout << "[4] - Ajuda" << endl;
    cout << "[0] - Sair" << endl;
    cin >> escolha;

    return escolha;
}

int MenuCarteira()
{

    int escolha;
    cout << "[1] - Novo Registro" << endl;
    cout << "[2] - Verificar Registro" << endl;
    cout << "[3] - Editar Registro" << endl;
    cout << "[4] - Excluir Registro" << endl;
    cout << "[0] - Voltar" << endl;
    cin >> escolha;

    return escolha;
}

int MenuMovimentacao()
{

    int escolha;
    cout << "[1] - Comprar Moeda" << endl;
    cout << "[2] - Vender Moeda" << endl;
    cout << "[0] - Voltar" << endl;
    cin >> escolha;

    return escolha;
}

int MenuRelatorios()
{

    int escolha;
    cout << "[1] - Ordem de Carteiras por Identificador" << endl;
    cout << "[2] - Ordem de Carteiras por Nome" << endl;
    cout << "[3] - Saldo Atual da Carteira" << endl;
    cout << "[4] - Historico de Movimentacao" << endl;
    cout << "[5] - Valor de Lucro" << endl;
    cout << "[0] - Voltar" << endl;
    cin >> escolha;

    return escolha;
}

int MenuAjuda()
{

    int escolha;
    cout << "[1] - Instrucoes" << endl;
    cout << "[2] - Creditos" << endl;
    cout << "[0] - Voltar" << endl;
    cin >> escolha;

    return escolha;
}

/*void MostrarMenu()
{

    int opcao1 = -1;
    int opcao2 = -1;

    while (opcao1 != 0)
    {

        MenuPrincipal();
        cin >> opcao1;

        if (opcao1 != 0)
        {
            opcao2 = -1;
            switch (opcao1)
            {
            case 1:

                while (opcao2 != 0)
                {
                    MenuCarteira();
                    cin >> opcao2;
                }
                break;

            case 2:
                while (opcao2 != 0)
                {
                    MenuMovimentacao();
                    cin >> opcao2;
                }
                break;

            case 3:
                while (opcao2 != 0)
                {
                    MenuRelatorios();
                    cin >> opcao2;
                }
                break;

            case 4:
                while (opcao2 != 0)
                {
                    MenuAjuda();
                    cin >> opcao2;
                }
                break;

            case 0:
                break;

            default:
                cout << "Opcao invalida, tente novamente.\n";
            }
        }
    }
}*/
