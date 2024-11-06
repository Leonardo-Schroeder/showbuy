#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "./lib/user.h"

int validarCPF(const char *cpf)
{
    if (strlen(cpf) != 11)
    {
        return 0;
    }

    for (int i = 0; i < 11; i++)
    {
        if (!isdigit(cpf[i]))
        {
            return 0;
        }
    }

    return 1;
}

int validarEmail(const char *email)
{
    return (strchr(email, '@') != NULL);
}

int validarSenha(const char *senha) {
    return (strlen(senha) >= 6); // Exige ao menos 6 caracteres
}

void cadastrarUsuario() {

    Usuario usuario;

    printf("-------------------- Cadastro --------------------\n");
    printf("Seja bem vindo! Vamos começar seu cadastro!\n");

    printf("Digite o nome completo: ");
    fgets(usuario.nomeCompleto, tamanhoMaximoNome, stdin);
    usuario.nomeCompleto[strcspn(usuario.nomeCompleto, "\n")] = 0;

    //Recebendo e Validando *
    //*CPF
    do {

        printf("Agora, digite o CPF (apenas números): ");
        fgets(usuario.cpf, tamanhoMaximoCPF, stdin);
        usuario.cpf[strcspn(usuario.cpf, "\n")] = 0;

        if (!validarCPF(usuario.cpf)) {
            perror("CPF inválido. Deve conter 11 dígitos numéricos.\n");
        }

    } while (!validarCPF(usuario.cpf));

    //*Email
    do {
        
        printf("Para falarmos com você, digite o seu email: ");
        fgets(usuario.email, tamanhoMaximoEmail, stdin);
        usuario.email[strcspn(usuario.email, "\n")] = 0;

        if (!validarEmail(usuario.email)) {
            printf("Email inválido. Certifique-se de incluir '@'.\n");
        }

    } while (!validarEmail(usuario.email));

    //*Senha
    do {
        printf("Por favor, crie uma senha (mínimo de 6 caracteres): ");
        fgets(usuario.senha, tamanhoMaximoSenha, stdin);
        usuario.senha[strcspn(usuario.senha, "\n")] = 0;

        if (!validarSenha(usuario.senha)) {
            perror("Senha inválida. A senha deve ter ao menos 8 caracteres.\n");
        }
    } while (!validarSenha(usuario.senha));

    //*Role
    printf("Você é um Gestor de Eventos ou Cliente?\n");
    printf("1 - Gestor | 2 - Cliente (Digite o número): ");
    while (scanf("%d", &usuario.role) != 1 || usuario.role < 1 || usuario.role > 2) {
        perror("Opção inválida. Digite 1 para Gestor ou 2 para Cliente:\n");
        while (getchar() != '\n'); 
    }

    //*Idade
    printf("Para finalizar, digite a idade: ");
    while (scanf("%d", &usuario.idade) != 1 || usuario.idade <= 0) {
        perror("Idade inválida. Digite um valor positivo:\n");
        while (getchar() != '\n');  
    }

    getchar();  // Limpa o buffer final

    salvarUsuario(usuario);

    printf("-------------------- x --------------------\n");
}

void salvarUsuario(Usuario usuario)
{

    FILE *file = fopen("./Database/users.txt", "a");

    if (file == NULL)
    {
        printf("Ops... Não foi possível realizar seu cadastro.\n");
        perror("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    fprintf(file, "Nome: %s | Idade: %d | CPF: %s | Email: %s | Senha: %s | Role: %d \n", usuario.nomeCompleto, usuario.idade, usuario.cpf, usuario.email, usuario.senha, usuario.role);

    fclose(file);

    printf("Usuário salvo com sucesso!\n");
}
