#include "interface.hpp"
#include "carteira.hpp"
#include "oraculo.hpp"
#include "movimentacao.hpp"
#include "ajuda.hpp"
#include "relatorio.hpp"

#include <iostream>
#include <string>

int main()
{
    if (Interface::MenuGravacao() == 1)
    {
        int opcaoPrincipal = -1;

        while (opcaoPrincipal != 0)
        {
            opcaoPrincipal = Interface::MenuPrincipal();

            switch (opcaoPrincipal)
            {
            case 1:
            {
                int opcaoCarteira = -1;
                CarteiraDAO_Local dao;

                while (opcaoCarteira != 0)
                {
                    opcaoCarteira = Interface::MenuCarteira();

                    switch (opcaoCarteira)
                    {
                    case 1:
                    {
                        std::string nomeTitular, corretora;
                        std::cout << "----------------------------------" << std::endl;
                        std::cout << "Nome do titular: ";
                        std::cin >> nomeTitular;
                        std::cout << "----------------------------------" << std::endl;
                        std::cout << "Corretora: ";
                        std::cin >> corretora;

                        Carteira c(0, nomeTitular, corretora);
                        dao.criarCarteira(c);
                        break;
                    }
                    case 2:
                    {
                        int idConsulta;
                        std::cout << "Digite o ID da carteira para consultar: ";
                        std::cin >> idConsulta;

                        Carteira cart = dao.consultarCarteira(idConsulta);

                        if (cart.getId() != 0) // Assumindo que ID 0 significa "não encontrado"
                        {
                            std::cout << "----------------------------------" << std::endl;
                            std::cout << "ID: " << cart.getId() << std::endl;
                            std::cout << "Titular: " << cart.getTitular() << std::endl;
                            std::cout << "Corretora: " << cart.getCorretora() << std::endl;
                        }
                        else
                        {
                            std::cout << "Carteira com ID " << idConsulta << " nao encontrada." << std::endl;
                        }
                        break;
                    }
                    case 3:
                    {
                        int idEditar;
                        std::string novoTitular;
                        std::cout << "Digite o ID da carteira que deseja editar: ";
                        std::cin >> idEditar;
                        std::cout << "----------------------------------" << std::endl;
                        std::cout << "Digite o novo nome do titular: ";
                        std::cin.ignore(); // Limpa o buffer do teclado
                        std::getline(std::cin, novoTitular);
                        std::cout << "----------------------------------" << std::endl;
                        dao.editarCarteira(idEditar, novoTitular);
                        break;
                    }
                    case 4:
                    {
                        int idExcluir;
                        std::cout << "Digite o ID da carteira que deseja excluir: ";
                        std::cin >> idExcluir;

                        dao.excluirCarteira(idExcluir);
                        break;
                    }
                    case 0:
                        break;
                    default:
                        std::cout << "Opcao invalida." << std::endl;
                        break;
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
                    opcaoMov = Interface::MenuMovimentacao();

                    switch (opcaoMov)
                    {
                    case 1:
                    {
                        int idCarteira;
                        double quantidade;

                        std::cout << "Digite o ID da carteira: ";
                        std::cin >> idCarteira;
                        std::cout << "Digite a quantidade de FT Coins para comprar: ";
                        std::cin >> quantidade;

                        movimentacao novaMov;
                        novaMov.setIdCarteira(idCarteira);
                        novaMov.setQuantidade(quantidade);

                        novaMov.setTipoOperacao('C');
                        movimentacaoDAO_Local movLocal;
                        movLocal.compraLocal(novaMov);
                    }
                    break;
                    case 2:
                    {
                        int idCarteira;
                        double quantidade;

                        std::cout << "Digite o ID da carteira: ";
                        std::cin >> idCarteira;
                        std::cout << "Digite a quantidade de FT Coins para vender: ";
                        std::cin >> quantidade;

                        movimentacao novaMov;
                        novaMov.setIdCarteira(idCarteira);
                        novaMov.setQuantidade(quantidade);

                        novaMov.setTipoOperacao('V');
                        movimentacaoDAO_Local movLocal;
                        movLocal.vendaLocal(novaMov);

                        break;
                    }
                    case 0:
                        break;
                    default:
                        std::cout << "Opcao invalida." << std::endl;
                    }
                }
                break;
            }

            case 3:
            {
                int opcaoRel = -1;
                while (opcaoRel != 0)
                {
                    opcaoRel = Interface::MenuRelatorios();
                    RelatorioDAO_Local rel;

                    switch (opcaoRel)
                    {
                    case 1:
                        rel.listarCarteirasPorId();
                        break;
                    case 2:
                        rel.listarCarteirasPorNome();
                        break;
                    case 3:
                        int id;
                        std::cout << "Digite o ID da carteira: ";
                        std::cin >> id;
                        rel.exibirSaldoCarteira(id);
                        break;
                    case 4:
                        std::cout << "Digite o ID da carteira: ";
                        std::cin >> id;
                        rel.exibirHistoricoCarteira(id);
                        break;
                    case 5:
                        std::cout << "Digite o ID da carteira: ";
                        std::cin >> id;
                        rel.exibirGanhoPerdaCarteira(id);
                        break;
                    case 0:
                        break;
                    default:
                        std::cout << "Opcao invalida." << std::endl;
                    }
                }
                break;
            }

            case 4:
            {
                int opcaoAjuda = -1;
                while (opcaoAjuda != 0)
                {
                    opcaoAjuda = Interface::MenuAjuda();

                    extras ext;

                    switch (opcaoAjuda)
                    {
                    case 1:
                        ext.mostrarAjuda();
                        break;
                    case 2:
                        ext.mostrarCreditos();
                        break;
                    case 0:
                        break;
                    default:
                        std::cout << "Opcao invalida." << std::endl;
                    }
                }
                break;
            }

            case 0:
                std::cout << "Saindo..." << std::endl;
                break;

            default:
                std::cout << "Opcao invalida, tente novamente." << std::endl;
            }
        }
    }

    else if (Interface::MenuGravacao() == 2)
    {
        int opcaoPrincipal = -1;

        while (opcaoPrincipal != 0)
        {
            opcaoPrincipal = Interface::MenuPrincipal();

            switch (opcaoPrincipal)
            {
            case 1:
            {
                int opcaoCarteira = -1;

                while (opcaoCarteira != 0)
                {
                    opcaoCarteira = Interface::MenuCarteira();

                    switch (opcaoCarteira)
                    {
                    case 1:
                    {
                        std::string nomeTitular, corretora;

                        std::cout << "Nome do titular: ";
                        std::cin.ignore(); // limpar buffer se necessário
                        getline(std::cin, nomeTitular);

                        std::cout << "Corretora: ";
                        getline(std::cin, corretora);

                        CarteiraDAO_Remoto daoRemoto;
                        Carteira c(0, nomeTitular, corretora);
                        daoRemoto.criarCarteira(c);
                    }
                    break;

                    case 2:
                    {
                        int id;
                        std::cout << "Digite o ID da carteira que deseja consultar: ";
                        std::cin >> id;

                        CarteiraDAO_Remoto daoRemoto;
                        Carteira c = daoRemoto.consultarCarteira(id);

                        if (c.getId() != 0)
                        {
                            std::cout << "ID: " << c.getId() << std::endl;
                            std::cout << "Titular: " << c.getTitular() << std::endl;
                            std::cout << "Corretora: " << c.getCorretora() << std::endl;
                        }
                        else
                        {
                            std::cout << "Carteira não encontrada." << std::endl;
                        }
                    }
                    break;

                    case 3:
                    {
                        int id;
                        std::string novoTitular, novaCorretora;

                        std::cout << "Digite o ID da carteira que deseja editar: ";
                        std::cin >> id;
                        std::cin.ignore();

                        std::cout << "Novo nome do titular: ";
                        getline(std::cin, novoTitular);

                        std::cout << "Nova corretora: ";
                        getline(std::cin, novaCorretora);

                        CarteiraDAO_Remoto daoRemoto;
                        daoRemoto.editarCarteira(id, novoTitular, novaCorretora);
                    }
                    break;

                    case 4:
                    {
                        int id;
                        std::cout << "Digite o ID da carteira que deseja excluir: ";
                        std::cin >> id;

                        CarteiraDAO_Remoto daoRemoto;
                        daoRemoto.excluirCarteira(id);
                    }
                    break;

                    case 0:
                        break;

                    default:
                        std::cout << "Opcao invalida." << std::endl;
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

                    opcaoMov = Interface::MenuMovimentacao();

                    switch (opcaoMov)
                    {
                    case 1:
                    {
                        int idCarteira;
                        double quantidade;

                        std::cout << "Digite o ID da carteira: ";
                        std::cin >> idCarteira;
                        std::cout << "Digite a quantidade de FT Coins para comprar: ";
                        std::cin >> quantidade;

                        movimentacao novaMov;
                        novaMov.setIdCarteira(idCarteira);
                        novaMov.setQuantidade(quantidade);
                        novaMov.setTipoOperacao('C');
                        movimentacaoDAO_Remoto movRem;

                        movRem.compraRemota(novaMov);
                    }

                    break;
                    case 2:
                    {
                        int idCarteira;
                        double quantidade;

                        std::cout << "Digite o ID da carteira: ";
                        std::cin >> idCarteira;
                        std::cout << "Digite a quantidade de FT Coins para vender: ";
                        std::cin >> quantidade;

                        movimentacao novaMov;
                        novaMov.setIdCarteira(idCarteira);
                        novaMov.setQuantidade(quantidade);
                        novaMov.setTipoOperacao('C');
                        movimentacaoDAO_Remoto movRem;

                        movRem.vendaRemota(novaMov);
                    }

                    break;
                    case 0:
                        break;
                    default:
                        std::cout << "Opcao invalida." << std::endl;
                    }
                }
                break;
            }

            case 3:
            {
                int opcaoRel = -1;
                RelatorioDAO_Remoto rel;

                while (opcaoRel != 0)
                {
                    opcaoRel = Interface::MenuRelatorios();

                    switch (opcaoRel)
                    {
                    case 1:
                        rel.listarCarteirasPorId();
                        break;
                    case 2:
                        rel.listarCarteirasPorNome();
                        break;
                    case 3:
                    {
                        int id;
                        std::cout << "Digite o ID da carteira: ";
                        std::cin >> id;
                        rel.exibirSaldoCarteira(id);
                        break;
                    }
                    case 4:
                    {
                        int id;
                        std::cout << "Digite o ID da carteira: ";
                        std::cin >> id;
                        rel.exibirHistoricoCarteira(id);
                        break;
                    }
                    case 5:
                    {
                        int id;
                        std::cout << "Digite o ID da carteira: ";
                        std::cin >> id;
                        rel.exibirGanhoPerdaCarteira(id);
                        break;
                    }
                    case 0:
                        break;
                    default:
                        std::cout << "Opcao invalida." << std::endl;
                    }
                }
                break;
            }
            case 4:
            {
                int opcaoAjuda = -1;
                while (opcaoAjuda != 0)
                {
                    opcaoAjuda = Interface::MenuAjuda();

                    extras ext;

                    switch (opcaoAjuda)
                    {
                    case 1:
                        ext.mostrarAjuda();
                        break;
                    case 2:
                        ext.mostrarCreditos();
                        break;
                    case 0:
                        break;
                    default:
                        std::cout << "Opcao invalida." << std::endl;
                    }
                }
                break;
            }

            case 0:
                std::cout << "Saindo..." << std::endl;
                break;

            default:
                std::cout << "Opcao invalida, tente novamente." << std::endl;
            }
        }
    }

    return 0;
}
