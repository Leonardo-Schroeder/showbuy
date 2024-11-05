#include <stdio.h>
#include <string.h>
#include "user.h"
#include "navegation.h"
#include "shows.h"

int main()
{
    int opcao, opcaoUser;

    // Modularizar - Menu (Navegação Login, Navegação Promoter, Navegação Cliente)

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

                if (userLoggedIn.role == 1)
                {
                    while (1)
                    {
                        menuGestor(&opcaoUser);
                        switch (opcaoUser)
                        {
                        case 0:
                            
                            break;
                        case 1:
                            cadastrarShow(); // Função para cadastrar um evento
                            break;
                        case 2:
                            printf("Digite o ID do evento para atualizar: ");
                            int idAtualizar;
                            scanf("%d", &idAtualizar);
                            atualizarShow(idAtualizar); // Função para atualizar um evento
                            break;
                        case 3:
                            printf("Digite o ID do evento para remover: ");
                            int idRemover;
                            scanf("%d", &idRemover);
                            removerShow(idRemover); // Função para remover um evento
                            break;
                        case 4:
                            break;
                        case 5:
                            break;
                        case 6:
                            printf("\nO Programa foi encerrado, volte sempre!\n");
                            return 0;
                        default:
                            printf("\nOpção inválida. Tente novamente.\n");
                            break;
                        }
                    }
                }
                else if (userLoggedIn.role == 2)
                {
                    /* code */
                }
            }
            else
            {
                printf("\nFalha no login. Tente novamente.\n");
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
                printf("\nErro ao alterar a senha. CPF não encontrado.\n");
            }
            break;

        case 3:
            printf("\nO Programa foi encerrado.\n");
            return 0;

        default:
            printf("\nOpção inválida. Tente novamente.\n");
        }
    }
}
