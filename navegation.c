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
    getchar(); // Limpa o buffer do stdin após o scanf
    printf("-------------------- xxxxxx --------------------\n");
}

void menuGestor(int *opcao)
{
    printf("\n-------------------- Showbuy --------------------\n");
    printf("Ola, %s! Seja Bem-vindo! Por favor, escolha a opçao desejada:\n", userLoggedIn.nomeCompleto);
    printf("0. Consultar Eventos\n");
    printf("1. Cadastrar Evento\n");
    printf("2. Atualizar Evento\n");
    printf("3. Remover Evento\n");
    printf("4. Consultar Eventos\n");
    printf("5. Consultar Eventos\n");
    printf("6. Fechar\n");
    scanf("%d", opcao);
    getchar(); // Limpa o buffer do stdin após o scanf
    printf("-------------------- xxxxxx --------------------\n");
}