#include "interface.hpp"
#include "carteira.hpp"

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
            carteira c;

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
                        cin >> c.nome_titular;

                        c.NovaCarteiraLocal();
                    }
                    break;
                case 2:
                    c.ChecarCarteiraLocal();
                    break;
                case 3:
                    c.EditarCarteiraLocal();
                    break;
                case 4:
                    c.ExcluirCarteiraLocal();
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
            while (opcaoMov != 0)
            {
                opcaoMov = MenuMovimentacao();
                // Adicione as chamadas de funções aqui
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

} else if (MenuGravacao() == 2)
{
    //gravação remota
}


    return 0;
}