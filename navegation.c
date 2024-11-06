#include <stdio.h>
#include <string.h>
#include "user.h"

void menuInicial(int *opcao)
{
    printf("\n-------------------- Showbuy --------------------\n");
    printf("Ola! Seja Bem-vindo! Por favor, escolha a opçao desejada:\n");
    printf("0. Login\n");
    printf("1. Cadastro\n");
    printf("2. Alterar senha\n");
    printf("3. Sair\n");
    scanf("%d", opcao);
    getchar();
    printf("-------------------- xxxxxx --------------------\n");
}

void menuGestor(int *opcao)
{
    trim(userLoggedIn.nomeCompleto);
    printf("\n-------------------- Showbuy --------------------\n");
    printf("Ola, %s! Seja Bem-vindo! Por favor, escolha a opçao desejada:\n", userLoggedIn.nomeCompleto);
    printf("0. Meus Shows\n");
    printf("1. Cadastrar Show\n");
    printf("2. Atualizar Show\n");
    printf("3. Remover Show\n");
    printf("4. Todos os Shows\n");
    printf("5. Fechar\n");
    scanf("%d", opcao);
    //getchar();
    printf("-------------------- xxxxxx --------------------\n");
}

void menuCliente(int *opcao)
{
    trim(userLoggedIn.nomeCompleto);
    printf("\n-------------------- Showbuy --------------------\n");
    printf("Ola, %s! Seja Bem-vindo! Por favor, escolha a opçao desejada:\n", userLoggedIn.nomeCompleto);
    printf("0. Todos os Shows\n");
    printf("1. Meus Ingressos\n");
    printf("2. Comprar Ingresso\n");
    printf("3. Reembolsar Ingresso\n");
    printf("4. Transferir Ingresso\n");
    printf("5. Fechar\n");
    scanf("%d", opcao);
    printf("-------------------- xxxxxx --------------------\n");
}
