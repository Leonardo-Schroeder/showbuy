#include <stdio.h>
#include <string.h>
#include "./lib/user.h"
#include "./lib/navegation.h"
#include "./lib/shows.h"
#include "./lib/tickets.h"
#include "locale.h"

int main()
{
    int opcao, opcaoUser;
    setlocale(LC_ALL, "pt_BR.UTF-8");

    // Menu de Login
    while (1)
    {
        menuInicial(&opcao);

        switch (opcao)
        {
        case 0:
            if (autenticarUsuario())
            {
                char nome[strlen(userLoggedIn.nomeCompleto)];
                strcpy(nome, userLoggedIn.nomeCompleto);
                trim(nome);
                printf("\n%s, bem-vindo ao sistema!\n", nome);

                // Separa os menus de Cliente / Gestor
                if (userLoggedIn.role == 1)
                {
                    while (1)
                    {
                        menuGestor(&opcaoUser);
                        switch (opcaoUser)
                        {
                        case 0:
                            mostrarMeuShow();
                            break;
                        case 1:
                            cadastrarShow();
                            break;
                        case 2:
    
                            atualizarShow();
                            break;
                        case 3:
                            removerShow();
                            break;
                        case 4:
                            mostrarTodosShows();
                            break;
                        case 5:
                            printf("\nO Programa foi encerrado, volte sempre!\n");
                            return 0;
                        default:
                            printf("\nOpção inválida. Tente novamente!\n");
                            break;
                        }
                    }
                }
                else if (userLoggedIn.role == 2)
                {
                    while (1)
                    {
                        menuCliente(&opcaoUser);
                        switch (opcaoUser)
                        {
                        case 0:
                            mostrarTodosShows();
                            break;
                        case 1:
                            mostrarIngressoUsuario();
                            break;
                        case 2:
                            comprarIngresso();
                            break;
                        case 3:
                            reembolsarIngresso();
                            break;
                        case 4:
                            transferirIngresso();
                            break;
                        case 5:
                            mostrarDetalhesShow();
                            break;
                        case 6:
                            return 0;
                        default:
                            printf("\nOpção inválida. Tente novamente!\n");
                        }
                    }
                }
            }
            else
            {
                printf("\nFalha no login. Tente novamente!\n");
            }
            break;
        case 1:
            cadastrarUsuario();
            break;
        case 2:
            if (alterarSenha())
            {
                printf("\nSenha alterada com sucesso.\n");
            }
            else
            {
                printf("\nErro ao alterar a senha. O CPF não foi encontrado!\n");
            }
            break;

        case 3:
            printf("\nO Programa foi encerrado.\n");
            return 0;

        default:
            printf("\nOpção inválida. Tente novamente!\n");
        }
    }
}
