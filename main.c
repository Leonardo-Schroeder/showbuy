#include <stdio.h>
#include "user.h"

int main() {
    int opcao;

    while (1) {
        printf("\n--- Cadastro de Usuários ---\n");
        printf("1. Cadastrar usuário\n");
        printf("2. Login\n");
        printf("3. Alterar senha\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do stdin após o scanf

        switch (opcao) {
            case 1:
                cadastrarUsuario();
                break;

            case 2:
                if (autenticarUsuario()) {
                    printf("Bem-vindo ao sistema!\n");
                } else {
                    printf("Falha no login. Tente novamente.\n");
                }
                break;

            case 3:
                if (alterarSenha()) {
                    printf("Senha alterada com sucesso.\n");
                } else {
                    printf("Erro ao alterar a senha. CPF não encontrado.\n");
                }
                break;

            case 0:
                printf("Programa encerrado.\n");
                return 0;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }
}
