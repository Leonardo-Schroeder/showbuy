#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "user.h"

#define CAMINHO_ARQUIVO_USER "./Database/users.txt"

// Função para autenticar o usuário no login usando CPF ou Email
int autenticarUsuario()
{
    char identificador[tamanhoMaximoCPF];
    char senha[tamanhoMaximoSenha];

    printf("Digite o CPF ou Email: ");
    fgets(identificador, sizeof(identificador), stdin);
    identificador[strcspn(identificador, "\n")] = '\0'; // Remove o caractere de nova linha

    printf("Digite a senha: ");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = '\0'; // Remove o caractere de nova linha

    FILE *file = fopen(CAMINHO_ARQUIVO_USER, "r");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo de usuários.");
        return 0;
    }

    char linha[512];
    int autenticado = 0;

    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        Usuario usuario;
        if (sscanf(linha, "Nome: %[^|]| Idade: %d | CPF: %[^|]| Email: %[^|]| Senha: %[^|]| Role: %d",
                   usuario.nomeCompleto, &usuario.idade, usuario.cpf, usuario.email, usuario.senha, &usuario.role) == 6)
        {
            if ((strcmp(usuario.cpf, identificador) == 0 || strcmp(usuario.email, identificador) == 0) &&
                strcmp(usuario.senha, senha) == 0)
            {
                printf("Login realizado com sucesso!\n");
                autenticado = 1;
                break;
            }
        }
    }

    fclose(file);

    if (!autenticado)
    {
        printf("Identificador ou senha incorretos.\n");
    }

    return autenticado;
}

// Função para alterar a senha de um usuário
int alterarSenha()
{
    char cpf[tamanhoMaximoCPF];
    char novaSenha[tamanhoMaximoSenha];

    printf("Digite o CPF para alterar a senha: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = '\0';

    printf("Digite a nova senha: ");
    fgets(novaSenha, sizeof(novaSenha), stdin);
    novaSenha[strcspn(novaSenha, "\n")] = '\0';

    FILE *file = fopen(CAMINHO_ARQUIVO_USER, "r");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo de usuários.");
        return 0;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL)
    {
        perror("Erro ao abrir o arquivo temporário.");
        fclose(file);
        return 0;
    }

    char linha[512];
    int alterado = 0;

    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        Usuario usuario;
        if (sscanf(linha, "Nome: %[^|]| Idade: %d | CPF: %[^|]| Email: %[^|]| Senha: %[^|]| Role: %d",
                   usuario.nomeCompleto, &usuario.idade, usuario.cpf, usuario.email, usuario.senha, &usuario.role) == 6)
        {
            if (strcmp(usuario.cpf, cpf) == 0)
            {
                strcpy(usuario.senha, novaSenha);
                alterado = 1;
                printf("Senha alterada com sucesso.\n");
            }
            fprintf(tempFile, "Nome: %s | Idade: %d | CPF: %s | Email: %s | Senha: %s | Role: %d\n", usuario.nomeCompleto, usuario.idade, usuario.cpf, usuario.email, usuario.senha, usuario.role);
        }
        else
        {
            fputs(linha, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(CAMINHO_ARQUIVO_USER);
    rename("temp.txt", CAMINHO_ARQUIVO_USER);

    if (!alterado)
    {
        printf("CPF não encontrado.\n");
    }

    return alterado;
}
