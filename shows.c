#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shows.h"
#include "user.h"

// Ajeitar

int obterMaiorId()
{
    FILE *file = fopen("./Database/shows.txt", "r");

    if (file == NULL)
    {
        return 0; // Arquivo inexistente, começará com ID 0
    }

    Show show;
    int maiorId = -1;

    while (fscanf(file, "ID: %d | Nome: %99[^\n] | Preço: %f | Ingressos: %d | CPF: %s | Ativo: %d\n",
                  &show.id, show.nomeShow, &show.preco, &show.ingressosDisponiveis, &show.cpfResponsavel, &show.ativo) != EOF)
    {
        if (show.id > maiorId)
        {
            maiorId = show.id;
        }
    }

    fclose(file);
    return maiorId + 1;
}

void cadastrarShow()
{
    Show novoShow;
    FILE *file = fopen("./Database/shows.txt", "a");

    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    // Define o ID automaticamente com base no maior ID
    novoShow.id = obterMaiorId();

    // Limpa o buffer antes de usar fgets para evitar conflitos com o scanf
    getchar();

    printf("Digite o nome do show: ");
    fgets(novoShow.nomeShow, tamanhoMaximoNome, stdin);
    novoShow.nomeShow[strcspn(novoShow.nomeShow, "\n")] = 0; // Remove a nova linha

    printf("Digite o preço do ingresso: ");
    if (scanf("%f", &novoShow.preco) != 1)
    {
        printf("Entrada inválida para o preço.\n");
        fclose(file);
        return;
    }

    printf("Digite a quantidade de ingressos disponíveis: ");
    if (scanf("%d", &novoShow.ingressosDisponiveis) != 1)
    {
        printf("Entrada inválida para a quantidade de ingressos.\n");
        fclose(file);
        return;
    }

    novoShow.ativo = 1;

    // Copia o CPF do usuário logado para o campo responsável
    strcpy(novoShow.cpfResponsavel, userLoggedIn.cpf);

    // Salva os dados no arquivo
    fprintf(file, "ID: %d | Nome: %s | Preço: %.2f | Ingressos: %d | CPF: %s | Ativo: %d\n",
            novoShow.id, novoShow.nomeShow, novoShow.preco, novoShow.ingressosDisponiveis, novoShow.cpfResponsavel, novoShow.ativo);

    fclose(file);
    printf("\nShow cadastrado com sucesso!\n");
}

void removerShow(int id)
{
    FILE *file = fopen("./Database/shows.txt", "r");
    FILE *tempFile = fopen("./Database/temp.txt", "w");
    Show show;
    char linha[256];
    int encontrado = 0;

    if (file == NULL || tempFile == NULL)
    {
        perror("Erro ao abrir o arquivo.\n");
        return;
    }

    while (fgets(linha, sizeof(linha), file))
    {
        sscanf(linha, "ID: %d | Nome: %99[^|] | Preço: %f | Ingressos: %d | CPF: %11s | Ativo: %d",
               &show.id, show.nomeShow, &show.preco, &show.ingressosDisponiveis, show.cpfResponsavel, &show.ativo);

        // Remove espaços e caracteres extras antes de comparar
        trim(show.cpfResponsavel);
        trim(userLoggedIn.cpf);

        printf("CPF do Show: '%s' - CPF do Usuário Logado: '%s'\n", show.cpfResponsavel, userLoggedIn.cpf);
        
        if (show.id == id && strcmp(show.cpfResponsavel, userLoggedIn.cpf) == 0)
        {
            encontrado = 1;
            continue;  // Skip this line to "remove" it
        }
        
        fprintf(tempFile, "ID: %d | Nome: %s | Preço: %.2f | Ingressos: %d | CPF: %s | Ativo: %d\n",
                show.id, show.nomeShow, show.preco, show.ingressosDisponiveis, show.cpfResponsavel, show.ativo);
    }

    fclose(file);
    fclose(tempFile);

    if (encontrado)
    {
        remove("./Database/shows.txt");
        rename("./Database/temp.txt", "./Database/shows.txt");
        printf("Show removido com sucesso!\n");
    }
    else
    {
        remove("./Database/temp.txt");
        printf("Show com ID %d não encontrado ou você não tem permissão para removê-lo.\n", id);
    }
}

void atualizarShow(int id)
{
    FILE *file = fopen("./Database/shows.txt", "r");
    FILE *tempFile = fopen("./Database/temp.txt", "w");
    Show show;
    char linha[256];
    int encontrado = 0;

    if (file == NULL || tempFile == NULL)
    {
        perror("Erro ao abrir o arquivo.\n");
        return;
    }

    while (fgets(linha, sizeof(linha), file))
    {
        sscanf(linha, "ID: %d | Nome: %99[^|] | Preço: %f | Ingressos: %d | CPF: %11s | Ativo: %d",
               &show.id, show.nomeShow, &show.preco, &show.ingressosDisponiveis, show.cpfResponsavel, &show.ativo);

        trim(show.cpfResponsavel);
        trim(userLoggedIn.cpf);

        if (show.id == id && strcmp(show.cpfResponsavel, userLoggedIn.cpf) == 0)
        {
            encontrado = 1;

            printf("Digite o novo nome do show: ");
            getchar();
            fgets(show.nomeShow, tamanhoMaximoNome, stdin);
            show.nomeShow[strcspn(show.nomeShow, "\n")] = 0;

            printf("Digite o novo preço do ingresso: ");
            scanf("%f", &show.preco);

            printf("Digite a nova quantidade de ingressos disponíveis: ");
            scanf("%d", &show.ingressosDisponiveis);

            printf("Digite 1 para ativo ou 0 para desativo: ");
            int status;
            while (scanf("%d", &status) != 1 || (status != 0 && status != 1))
            {
                printf("Entrada inválida. Digite 1 para ativo ou 0 para desativo: ");
                while (getchar() != '\n');  // Limpa o buffer de entrada
            }
            show.ativo = status;
        }

        fprintf(tempFile, "ID: %d | Nome: %s | Preço: %.2f | Ingressos: %d | CPF: %s | Ativo: %d\n",
                show.id, show.nomeShow, show.preco, show.ingressosDisponiveis, show.cpfResponsavel, show.ativo);
    }

    fclose(file);
    fclose(tempFile);

    if (encontrado)
    {
        remove("./Database/shows.txt");
        rename("./Database/temp.txt", "./Database/shows.txt");
        printf("\nShow atualizado com sucesso!\n");
    }
    else
    {
        remove("./Database/temp.txt");
        printf("\nShow com ID %d não encontrado ou você não tem permissão para atualizá-lo.\n", id);
    }
}