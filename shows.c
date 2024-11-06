#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shows.h"
#include "user.h"
#include "tickets.h"

int obterMaiorId()
{
    FILE *file = fopen("./Database/shows.txt", "r");

    if (file == NULL)
    {
        return 0;
    }

    Show show;
    int maiorId = -1;

    while (fscanf(file, "ID: %d | Nome: %99[^|] | Data: %99[^|] | Preço: %f | Ingressos: %d | CPF: %s | Ativo: %d\n",
                  &show.id, show.nomeShow, show.data, &show.preco, &show.ingressosDisponiveis, show.cpfResponsavel, &show.ativo) != EOF)
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

    novoShow.id = obterMaiorId();
    getchar();

    printf("Digite o nome do show: ");
    fgets(novoShow.nomeShow, tamanhoMaximoNome, stdin);
    novoShow.nomeShow[strcspn(novoShow.nomeShow, "\n")] = 0;

    printf("Digite a data do show (DD/MM/AAAA): ");
    fgets(novoShow.data, tamanhoMaximoData, stdin);
    novoShow.data[strcspn(novoShow.data, "\n")] = 0;

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
    strcpy(novoShow.cpfResponsavel, userLoggedIn.cpf);

    trim(novoShow.nomeShow);
    trim(novoShow.cpfResponsavel);
    trim(novoShow.data);

    fprintf(file, "ID: %d | Nome: %s | Data: %s | Preço: %.2f | Ingressos: %d | CPF: %s | Ativo: %d\n",
            novoShow.id, novoShow.nomeShow, novoShow.data, novoShow.preco, novoShow.ingressosDisponiveis, novoShow.cpfResponsavel, novoShow.ativo);

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
        sscanf(linha, "ID: %d | Nome: %99[^|] | Data: %99[^|] | Preço: %f | Ingressos: %d | CPF: %s | Ativo: %d",
               &show.id, show.nomeShow, show.data, &show.preco, &show.ingressosDisponiveis, show.cpfResponsavel, &show.ativo);

        trim(show.cpfResponsavel);
        trim(userLoggedIn.cpf);

        if (show.id == id && strcmp(show.cpfResponsavel, userLoggedIn.cpf) == 0)
        {
            encontrado = 1;
            continue;
        }

        trim(show.nomeShow);
        trim(show.data);

        fprintf(tempFile, "ID: %d | Nome: %s | Data: %s | Preço: %.2f | Ingressos: %d | CPF: %s | Ativo: %d\n",
                show.id, show.nomeShow, show.data, show.preco, show.ingressosDisponiveis, show.cpfResponsavel, show.ativo);
    }

    fclose(file);
    fclose(tempFile);

    if (encontrado)
    {
        remove("./Database/shows.txt");
        rename("./Database/temp.txt", "./Database/shows.txt");
        printf("Show removido com sucesso!\n");

        excluirTodosTicketsPorShow(id);
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
        sscanf(linha, "ID: %d | Nome: %99[^|] | Data: %99[^|] | Preço: %f | Ingressos: %d | CPF: %s | Ativo: %d",
               &show.id, show.nomeShow, show.data, &show.preco, &show.ingressosDisponiveis, show.cpfResponsavel, &show.ativo);

        trim(show.cpfResponsavel);
        trim(userLoggedIn.cpf);

        if (show.id == id && strcmp(show.cpfResponsavel, userLoggedIn.cpf) == 0)
        {
            encontrado = 1;

            printf("Digite o novo nome do show: ");
            getchar();
            fgets(show.nomeShow, tamanhoMaximoNome, stdin);
            show.nomeShow[strcspn(show.nomeShow, "\n")] = 0;

            printf("Digite a nova data do show (DD/MM/AAAA): ");
            fgets(show.data, tamanhoMaximoData, stdin);
            show.data[strcspn(show.data, "\n")] = 0;

            printf("Digite o novo preço do ingresso: ");
            scanf("%f", &show.preco);

            printf("Digite a nova quantidade de ingressos disponíveis: ");
            scanf("%d", &show.ingressosDisponiveis);

            printf("Digite o CPF do Responsavél: ");
            scanf("%s", &show.cpfResponsavel);

            printf("Digite 1 para ativo ou 0 para desativo: ");
            scanf("%d", &show.ativo);
        }

        trim(show.nomeShow);
        trim(show.data);

        fprintf(tempFile, "ID: %d | Nome: %s | Data: %s | Preço: %.2f | Ingressos: %d | CPF: %s | Ativo: %d\n",
                show.id, show.nomeShow, show.data, show.preco, show.ingressosDisponiveis, show.cpfResponsavel, show.ativo);
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

void mostrarTodosShows()
{
    FILE *file = fopen("./Database/shows.txt", "r");
    Show show;

    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo de shows.\n");
        return;
    }

    printf("\nTodos os Shows:\n");

    while (1) // Loop infinito, sairá quando não conseguir mais ler
    {
        int result = fscanf(file, "ID: %d | Nome: %99[^|] | Data: %99[^|] | Preço: %f | Ingressos: %d | CPF: %s | Ativo: %d\n",
                            &show.id, show.nomeShow, show.data, &show.preco, &show.ingressosDisponiveis, show.cpfResponsavel, &show.ativo);
        if (result == EOF) // Verifica se EOF foi alcançado
            break;

        if (result != 7) // Se não leu todos os 7 campos, houve um erro
        {
            printf("Erro ao ler o arquivo. Verifique o formato das entradas.\n");
            break;
        }

        trim(show.nomeShow);
        trim(show.data);

        if (show.ativo == 1)
        {
            printf("ID: %d | Nome: %s | Data: %s | Preço: %.2f | Ingressos: %d\n",
                   show.id, show.nomeShow, show.data, show.preco, show.ingressosDisponiveis);
        }
    }

    fclose(file);
}

void mostrarMeuShow()
{
    FILE *file = fopen("./Database/shows.txt", "r");
    Show show;
    int encontrado = 0;

    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo de shows.\n");
        return;
    }

    printf("\nMeus Shows:\n");
    while (fscanf(file, "ID: %d | Nome: %99[^|] | Data: %99[^|] | Preço: %f | Ingressos: %d | CPF: %s | Ativo: %d\n",
                  &show.id, show.nomeShow, show.data, &show.preco, &show.ingressosDisponiveis, show.cpfResponsavel, &show.ativo) != EOF)
    {
        // Verifica se o CPF do responsável corresponde ao CPF do usuário logado
        trim(show.cpfResponsavel);
        trim(userLoggedIn.cpf);
        trim(show.nomeShow);
        trim(show.data);
        
        if (strcmp(show.cpfResponsavel, userLoggedIn.cpf) == 0)
        {
            printf("ID: %d | Nome: %s | Data: %s | Preço: %.2f | Ingressos: %d | Ativo: %d\n",
                   show.id, show.nomeShow, show.data, show.preco, show.ingressosDisponiveis, show.ativo);
            encontrado = 1;
        }
    }

    if (!encontrado)
    {
        printf("Nenhum show encontrado para o CPF do usuário logado.\n");
    }

    fclose(file);
}
