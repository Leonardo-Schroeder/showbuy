#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "user.h"

#define CAMINHO_ARQUIVO_USER "./Database/users.txt"

// Ajeitar essas funções trabalhar para que elas fiquem mais humanizadas além de tornar o código entendível

// Função para remover espaços em branco extras nas bordas da string -- Ajeitar
void trim(char *str) {
    // Remove espaços à direita
    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }

    // Remove espaços à esquerda
    char *start = str;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

int autenticarUsuario() {
    char identificador[tamanhoMaximoCPF];
    char senha[tamanhoMaximoSenha];

    printf("Digite o CPF ou Email: ");
    fgets(identificador, sizeof(identificador), stdin);
    identificador[strcspn(identificador, "\n")] = '\0'; // Remove o caractere de nova linha
    trim(identificador);

    printf("Digite a senha: ");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = '\0'; // Remove o caractere de nova linha
    trim(senha);

    FILE *file = fopen(CAMINHO_ARQUIVO_USER, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo de usuários.");
        return 0;
    }

    char linha[512];
    int autenticado = 0;

    while (fgets(linha, sizeof(linha), file) != NULL) {
        Usuario usuario;

        // Removendo o caractere de nova linha do final da linha lida
        linha[strcspn(linha, "\n")] = '\0';

        // Tentando ler a linha n0o formato especificado
        if (sscanf(linha, "Nome: %[^|]| Idade: %d | CPF: %[^|]| Email: %[^|]| Senha: %[^|]| Role: %d",
                   usuario.nomeCompleto, &usuario.idade, usuario.cpf, usuario.email, usuario.senha, &usuario.role) == 6) {
            
            // Remover espaços extras das strings do usuário
            trim(usuario.cpf);
            trim(usuario.email);
            trim(usuario.senha);

            // Comparando CPF/Email e senha para autenticação
            if ((strcmp(usuario.cpf, identificador) == 0 || strcmp(usuario.email, identificador) == 0) &&
                strcmp(usuario.senha, senha) == 0) {
                printf("Login realizado com sucesso!\n");
                autenticado = 1;
                break;
            }
        } else {
            // Mensagem de erro de debug caso a linha não tenha sido lida corretamente
            printf("Erro ao ler a linha: %s\n", linha);
        }
    }

    fclose(file);

    if (!autenticado) {
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
    trim(cpf);

    printf("Digite a nova senha: ");
    fgets(novaSenha, sizeof(novaSenha), stdin);
    novaSenha[strcspn(novaSenha, "\n")] = '\0';
    trim(novaSenha);

    FILE *file = fopen(CAMINHO_ARQUIVO_USER, "r");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo de usuários.");
        return 0;
    }

    FILE *tempFile = fopen("./Database/temp.txt", "w");
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
            printf("Nome: %s, Idade: %d, CPF: %s, Senha: %s");

            trim(usuario.nomeCompleto);
            trim(usuario.cpf);
            trim(usuario.email);
            trim(usuario.senha);

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
    rename("./Database/temp.txt", CAMINHO_ARQUIVO_USER);

    if (!alterado)
    {
        printf("CPF não encontrado.\n");
    }

    return alterado;
}
