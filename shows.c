#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lib/shows.h"
#include "./lib/user.h"
#include "./lib/tickets.h"

#define CAMINHO_ARQUIVO_SHOWS "./Database/shows.txt"
#define CAMINHO_ARQUIVO_TEMP "./Database/temp.txt"
#define CAMINHO_ARQUIVO_TICKETS "./Database/tickets.txt"

int obterMaiorId()
{
    FILE *file = fopen(CAMINHO_ARQUIVO_SHOWS, "r");

    if (file == NULL)
    {
        return 0;
    }

    Show show;
    int maiorId = -1; // Começamos com -1 pois se nn houver nenhum elemento ele inicializa em 0

    while (fscanf(file, "ID: %d | Nome: %99[^|] | Data: %99[^|] | Preço: %f | Ingressos: %d | CPF: %s | Ativo: %d\n",
                  &show.id, show.nomeShow, show.data, &show.preco, &show.ingressosDisponiveis, show.cpfResponsavel, &show.ativo) != EOF)
    {
        // Lógica para achar o maior ID
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
    FILE *file = fopen(CAMINHO_ARQUIVO_SHOWS, "a");

    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    novoShow.id = obterMaiorId();
    getchar(); // Buffer

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

void removerShow()
{
    FILE *file = fopen(CAMINHO_ARQUIVO_SHOWS, "r");
    FILE *tempFile = fopen(CAMINHO_ARQUIVO_TEMP, "w");
    Show show;
    char linha[256];
    int encontrado = 0;

    if (file == NULL || tempFile == NULL)
    {
        perror("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("Digite o ID do evento para remover: ");
    int id;
    scanf("%d", &id);

    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        sscanf(linha, "ID: %d | Nome: %99[^|] | Data: %99[^|] | Preço: %f | Ingressos: %d | CPF: %s | Ativo: %d",
               &show.id, show.nomeShow, show.data, &show.preco, &show.ingressosDisponiveis, show.cpfResponsavel, &show.ativo);

        // Retirando possíveis espaços para garantir que a comparação vai ser feita corretamente
        trim(show.cpfResponsavel);
        trim(userLoggedIn.cpf);

        if (show.id == id && strcmp(show.cpfResponsavel, userLoggedIn.cpf) == 0)
        {
            encontrado = 1; // Show Encontrado & com autorização para excluir
            continue;
        }

        trim(show.nomeShow);
        trim(show.data);

        fprintf(tempFile, "ID: %d | Nome: %s | Data: %s | Preço: %.2f | Ingressos: %d | CPF: %s | Ativo: %d\n",
                show.id, show.nomeShow, show.data, show.preco, show.ingressosDisponiveis, show.cpfResponsavel, show.ativo);
    }

    fclose(file);
    fclose(tempFile);

    // Transforma o arquivo temporário no original
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

void atualizarShow()
{
    FILE *file = fopen(CAMINHO_ARQUIVO_SHOWS, "r");
    FILE *tempFile = fopen(CAMINHO_ARQUIVO_TEMP, "w");
    Show show;
    char linha[256];
    int encontrado = 0;

    if (file == NULL || tempFile == NULL)
    {
        perror("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("Digite o ID do evento para atualizar: ");
    int id;
    scanf("%d", &id);

    while (fgets(linha, sizeof(linha), file))
    {
        sscanf(linha, "ID: %d | Nome: %99[^|] | Data: %99[^|] | Preço: %f | Ingressos: %d | CPF: %s | Ativo: %d",
               &show.id, show.nomeShow, show.data, &show.preco, &show.ingressosDisponiveis, show.cpfResponsavel, &show.ativo);

        trim(show.cpfResponsavel);
        trim(userLoggedIn.cpf);

        // Verifica se o usuário tem autorização para atualizar o show
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

    // Transforma o arquivo temporário no original
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
    FILE *file = fopen(CAMINHO_ARQUIVO_SHOWS, "r");
    Show show;

    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo de shows.\n");
        return;
    }

    printf("\nTodos os Shows:\n");

    while (1) // Sai quando ele não conseguir mais ler
    {
        int result = fscanf(file, "ID: %d | Nome: %99[^|] | Data: %99[^|] | Preço: %f | Ingressos: %d | CPF: %s | Ativo: %d\n",
                            &show.id, show.nomeShow, show.data, &show.preco, &show.ingressosDisponiveis, show.cpfResponsavel, &show.ativo);
        if (result == EOF)
            break;

        if (result != 7) // Leu algo incorretamente
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
    FILE *file = fopen(CAMINHO_ARQUIVO_SHOWS, "r");
    Show show;
    int encontrado = 0;

    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo de shows.\n");
        return;
    }

    printf("\nMeus Shows:\n");
    while (fscanf(file, "ID: %d | Nome: %99[^|] | Data: %99[^|] | Preço: %f | Ingressos: %d | CPF: %s | Ativo: %d\n",
                  &show.id, show.nomeShow, show.data, &show.preco, &show.ingressosDisponiveis, show.cpfResponsavel, &show.ativo) != EOF)
    {
        trim(show.cpfResponsavel);
        trim(userLoggedIn.cpf);
        trim(show.nomeShow);
        trim(show.data);

        // Validação para verificar se o user é responsavél pelo show
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
