#include <stdio.h>
#include "user.h"

int main()
{
    int opcao;

    // Modularizar - Menu (Navegação Login, Navegação Promoter, Navegação Cliente)

    while (1)
    {
        printf("\n-------------------- Showbuy --------------------\n");
        printf("Ola! Seja Bem-vindo! Por favor, escolha a opçao desejada:\n");
        printf("0. Login\n");
        printf("1. Cadastro\n");
        printf("2. Alterar senha\n");
        printf("3. Sair\n");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do stdin após o scanf
        printf("-------------------- xxxxxx --------------------\n");

        switch (opcao)
        {

        case 0:
            if (autenticarUsuario())
            {
                printf("\nBem-vindo ao sistema!\n");
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
