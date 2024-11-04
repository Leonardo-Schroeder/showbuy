#include <stdio.h>
#include "user.h"

int main() {
    int opcao;
    
    while (1) {
        printf("\n--- Cadastro de Usuários ---\n");
        printf("1. Cadastrar usuário\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do stdin após o scanf

        if (opcao == 1) {
            cadastrarUsuario();
        } else if (opcao == 0) {
            break;
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }

    printf("Programa encerrado.\n");
    return 0;
}
