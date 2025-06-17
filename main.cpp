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
                CarteiraDAO_Local dao;

                while (opcaoCarteira != 0)
                {
                    opcaoCarteira = MenuCarteira();

                    switch (opcaoCarteira)
                    {
                    case 1:

                        int i;
                        int n;

                        for (i = 0; i < n; ++i)
                        {
                            string nomeTitular, corretora;
                            cout << "Nome do titular: ";
                            cin >> nomeTitular;
                            cout << "Corretora: ";
                            cin >> corretora;

                            Carteira c(0, nomeTitular, corretora);
                            dao.criarCarteira(c);
                        }

                        break;
                    case 2:
                    {
                        int idConsulta;
                        cout << "Digite o ID da carteira para consultar: ";
                        cin >> idConsulta;

                        Carteira cart = dao.consultarCarteira(idConsulta);

                        if (cart.getId() != 0) // Assumindo que ID 0 significa "não encontrado"
                        {
                            cout << "Carteira encontrada:" << endl;
                            cout << "ID: " << cart.getId() << endl;
                            cout << "Titular: " << cart.getTitular() << endl;
                            cout << "Corretora: " << cart.getCorretora() << endl;
                        }
                        else
                        {
                            cout << "Carteira com ID " << idConsulta << " nao encontrada." << endl;
                        }
                    }
                    break;

                    case 3:
                    {
                        int idEditar;
                        string novoTitular;
                        cout << "Digite o ID da carteira que deseja editar: ";
                        cin >> idEditar;

                        cout << "Digite o novo nome do titular: ";
                        cin.ignore(); // Limpa o buffer do teclado
                        getline(cin, novoTitular);

                        dao.editarCarteira(idEditar, novoTitular);
                        cout << "Carteira editada." << endl;
                    }
                    break;

                    case 4:
                    {
                        int idExcluir;
                        cout << "Digite o ID da carteira que deseja excluir: ";
                        cin >> idExcluir;

                        dao.excluirCarteira(idExcluir);
                        cout << "Carteira excluida." << endl;
                    }
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
                        mov.compraLocal();
                        break;
                    case 2:
                        mov.vendaLocal();
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
        int opcaoPrincipal = -1;

        while (opcaoPrincipal != 0)
        {
            opcaoPrincipal = MenuPrincipal();

            switch (opcaoPrincipal)
            {
            case 1: // menu principal
            {
                int opcaoCarteira = -1;
                CarteiraDAO_Remoto daoRemoto;

                while (opcaoCarteira != 0)
                {
                    opcaoCarteira = MenuCarteira();

                    switch (opcaoCarteira)
                    {
                    case 1:
                    {
                        string nomeTitular, corretora;

                        cout << "Nome do titular: ";
                        cin.ignore(); // limpar buffer se necessário
                        getline(cin, nomeTitular);

                        cout << "Corretora: ";
                        getline(cin, corretora);

                        CarteiraDAO_Remoto daoRemoto;
                        Carteira c(0, nomeTitular, corretora);
                        daoRemoto.criarCarteira(c);
                    }
                    break;

                    case 2:
                    {
                        int id;
                        cout << "Digite o ID da carteira que deseja consultar: ";
                        cin >> id;

                        CarteiraDAO_Remoto daoRemoto;
                        Carteira c = daoRemoto.consultarCarteira(id);

                        if (c.getId() != 0)
                        {
                            cout << "ID: " << c.getId() << endl;
                            cout << "Titular: " << c.getTitular() << endl;
                            cout << "Corretora: " << c.getCorretora() << endl;
                        }
                        else
                        {
                            cout << "Carteira não encontrada." << endl;
                        }
                    }
                    break;

                    case 3:
                    {
                        int id;
                        string novoTitular, novaCorretora;

                        cout << "Digite o ID da carteira que deseja editar: ";
                        cin >> id;
                        cin.ignore();

                        cout << "Novo nome do titular: ";
                        getline(cin, novoTitular);

                        cout << "Nova corretora: ";
                        getline(cin, novaCorretora);

                        CarteiraDAO_Remoto daoRemoto;
                        daoRemoto.editarCarteira(id, novoTitular, novaCorretora);
                    }
                    break;

                    case 4:
                    {
                        int id;
                        cout << "Digite o ID da carteira que deseja excluir: ";
                        cin >> id;

                        CarteiraDAO_Remoto daoRemoto;
                        daoRemoto.excluirCarteira(id);
                    }
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
                        mov.compraRemota();
                        break;
                    case 2:
                        mov.vendaRemota();
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

    return 0;
}
