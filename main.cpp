#include "interface.hpp"
#include "carteira.hpp"
#include "oraculo.hpp"
#include "movimentacao.hpp"

#include <iostream>

using namespace std;

int main()
{

    if (MenuGravacao() == 1)
    {

        int opcaoPrincipal = -1;

        while (opcaoPrincipal != 0)
        {
            opcaoPrincipal = MenuPrincipal();

            switch (opcaoPrincipal)
            {
            case 1:
            {
                int opcaoCarteira = -1;
                carteira cart;

                while (opcaoCarteira != 0)
                {
                    opcaoCarteira = MenuCarteira();

                    switch (opcaoCarteira)
                    {
                    case 1:
                        int n;
                        cout << "Quantas carteiras deseja registrar? ";
                        cin >> n;

                        for (int i = 0; i < n; ++i)
                        {

                            cout << "Nome do titular: ";
                            cin >> cart.nome_titular;

                            cart.NovaCarteiraLocal();
                        }
                        break;
                    case 2:
                        cart.ChecarCarteiraLocal();
                        break;
                    case 3:
                        cart.EditarCarteiraLocal();
                        break;
                    case 4:
                        cart.ExcluirCarteiraLocal();
                        break;
                    case 0:
                        break;
                    default:
                        cout << "Opcao invalida." << endl;
                    }
                }
                break;
            }

            case 2:
            {
                int opcaoMov = -1;
                movimentacao mov;

                while (opcaoMov != 0)
                {

                    opcaoMov = MenuMovimentacao();

                    switch (opcaoMov)
                    {
                    case 1:
                        mov.compra();
                        break;
                    case 2:
                        mov.venda();
                        break;
                    case 0:
                        break;
                    default:
                        cout << "Opcao invalida." << endl;
                    }
                }
                break;
            }

            case 3:
            {
                int opcaoRel = -1;
                while (opcaoRel != 0)
                {
                    opcaoRel = MenuRelatorios();
                    // Adicione as chamadas de funções aqui
                }
                break;
            }

            case 4:
            {
                int opcaoAjuda = -1;
                while (opcaoAjuda != 0)
                {
                    opcaoAjuda = MenuAjuda();
                    // Adicione ações aqui se quiser
                }
                break;
            }

            case 0:
                cout << "Saindo..." << endl;
                break;

            default:
                cout << "Opcao invalida, tente novamente." << endl;
            }
        }
    }
    else if (MenuGravacao() == 2)
    {
        // gravação remota
    }

    return 0;
}