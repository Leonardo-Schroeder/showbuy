#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shows.h"

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

    while (fscanf(file, "ID: %d | Nome: %99[^\n] | Preço: %f | Ingressos: %d | Ativo: %d\n",
                  &show.id, show.nomeShow, &show.preco, &show.ingressosDisponiveis, &show.ativo) != EOF)
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

    printf("Digite o nome do show: ");
    fgets(novoShow.nomeShow, tamanhoMaximoNome, stdin);
    novoShow.nomeShow[strcspn(novoShow.nomeShow, "\n")] = 0; // Remove a nova linha

    printf("Digite o preço do show: ");
    scanf("%f", &novoShow.preco);

    printf("Digite a quantidade de ingressos disponíveis: ");
    scanf("%d", &novoShow.ingressosDisponiveis);

    novoShow.ativo = 1;

    // Salva os dados no arquivo
    fprintf(file, "ID: %d | Nome: %s | Preço: %.2f | Ingressos: %d | Ativo: %d\n",
            novoShow.id, novoShow.nomeShow, novoShow.preco, novoShow.ingressosDisponiveis, novoShow.ativo);
    fclose(file);

    printf("Show cadastrado com sucesso!\n");
}

void removerShow(int id)
{
    FILE *file = fopen("./Database/shows.txt", "r");
    FILE *tempFile = fopen("./Database/temp.txt", "w");
    Show show;
    int encontrado = 0;

    if (file == NULL || tempFile == NULL)
    {
        perror("Erro ao abrir o arquivo.\n");
        return;
    }

    // Lê os shows e escreve no temp.txt, exceto o show com o ID fornecido
    while (fscanf(file, "ID: %d | Nome: %99[^\n] | Preço: %f | Ingressos: %d | Ativo: %d\n",
                  &show.id, show.nomeShow, &show.preco, &show.ingressosDisponiveis, &show.ativo) != EOF)
    {
        if (show.id == id && show.ativo)
        {
            encontrado = 1;
            show.ativo = 0;
        }
        fprintf(tempFile, "ID: %d | Nome: %s | Preço: %.2f | Ingressos: %d | Ativo: %d\n",
                show.id, show.nomeShow, show.preco, show.ingressosDisponiveis, show.ativo);
    }

    fclose(file);
    fclose(tempFile);

    // Substitui o arquivo original pelo arquivo temporário
    remove("./Database/shows.txt");
    rename("./Database/temp.txt", "./Database/shows.txt");

    if (encontrado)
    {
        printf("Show removido com sucesso!\n");
    }
    else
    {
        printf("Show com ID %d não encontrado.\n", id);
    }
}

void atualizarShow(int id)
{
    FILE *file = fopen("./Database/shows.txt", "r");
    FILE *tempFile = fopen("./Database/temp.txt", "w");
    Show show;
    int encontrado = 0;

    if (file == NULL || tempFile == NULL)
    {
        perror("Erro ao abrir o arquivo.\n");
        return;
    }

    // Lê e atualiza o show com o ID fornecido
    while (fscanf(file, "ID: %d | Nome: %99[^\n] | Preço: %f | Ingressos: %d | Ativo: %d\n",
                  &show.id, show.nomeShow, &show.preco, &show.ingressosDisponiveis, &show.ativo) != EOF)
    {
        if (show.id == id && show.ativo)
        {
            encontrado = 1;
            printf("Digite o novo nome do show: ");
            getchar(); // Limpa o buffer
            fgets(show.nomeShow, tamanhoMaximoNome, stdin);
            show.nomeShow[strcspn(show.nomeShow, "\n")] = 0;

            printf("Digite o novo preço do show: ");
            scanf("%f", &show.preco);

            printf("Digite a nova quantidade de ingressos disponíveis: ");
            scanf("%d", &show.ingressosDisponiveis);
        }
        fprintf(tempFile, "ID: %d | Nome: %s | Preço: %.2f | Ingressos: %d | Ativo: %d\n",
                show.id, show.nomeShow, show.preco, show.ingressosDisponiveis, show.ativo);
    }

    fclose(file);
    fclose(tempFile);

    remove("./Database/shows.txt");
    rename("./Database/temp.txt", "./Database/shows.txt");

    if (encontrado)
    {
        printf("Show atualizado com sucesso!\n");
    }
    else
    {
        printf("Show com ID %d não encontrado.\n", id);
    }
}