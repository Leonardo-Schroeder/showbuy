#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lib/tickets.h"
#include "./lib/user.h"
#include "./lib/shows.h"

#define CAMINHO_ARQUIVO_USER "./Database/users.txt"
#define CAMINHO_ARQUIVO_SHOWS "./Database/shows.txt"
#define CAMINHO_ARQUIVO_TEMP "./Database/temp.txt"
#define CAMINHO_ARQUIVO_TEMP_SHOW "./Database/temp_shows.txt"
#define CAMINHO_ARQUIVO_TICKETS "./Database/tickets.txt"

//Terminar de usar as váriavéis nas funções // validar função de transferir e arrumar a função de exibir detalhes do show / mostrar mais informações para quando a lista de shows for exibidas?

int obterMaiorIdIngresso()
{
    FILE *file = fopen(CAMINHO_ARQUIVO_TICKETS, "r");

    if (file == NULL)
    {
        return 0;
    }

    Ingresso ingresso;
    int maiorId = -1; // Mesma lógica do id do show

    while (fscanf(file, "ID: %d | ShowID: %d | CPF: %s\n",
                  &ingresso.id, &ingresso.idShow, ingresso.cpfComprador) != EOF)
    {
        if (ingresso.id > maiorId)
        {
            maiorId = ingresso.id;
        }
    }

    fclose(file);

    return maiorId + 1;
}

int atualizarIngressosDisponiveis(int idShow, int incremento)
{
    FILE *file = fopen(CAMINHO_ARQUIVO_SHOWS, "r");
    FILE *tempFile = fopen(CAMINHO_ARQUIVO_TEMP_SHOW, "w");
    Show show;
    int atualizado = 0;

    if (file == NULL || tempFile == NULL)
    {
        perror("Erro ao abrir o arquivo de shows.\n");
        return 0;
    }

    while (fscanf(file, "ID: %d | Nome: %99[^|] | Data: %99[^|] | Preço: %f | Ingressos: %d | CPF: %14s | Ativo: %d\n",
                  &show.id, show.nomeShow, show.data, &show.preco, &show.ingressosDisponiveis, show.cpfResponsavel, &show.ativo) != EOF)
    {
        // Identifica o show para atualizar os ingressos
        if (show.id == idShow)
        {
            int novosIngressosDisponiveis = show.ingressosDisponiveis + incremento;

            if (novosIngressosDisponiveis < 0)
            {
                printf("Não há ingressos suficientes disponíveis para este show.\n");
                fclose(file);
                fclose(tempFile);
                remove("./Database/temp_shows.txt");
                return 0;
            }

            // Atualiza a quantidade de ingressos disponíveis
            show.ingressosDisponiveis = novosIngressosDisponiveis;
            atualizado = 1;
        }

        trim(show.nomeShow);
        trim(show.data);
        trim(show.cpfResponsavel);

        fprintf(tempFile, "ID: %d | Nome: %s | Data: %s | Preço: %.2f | Ingressos: %d | CPF: %s | Ativo: %d\n",
                show.id, show.nomeShow, show.data, show.preco, show.ingressosDisponiveis, show.cpfResponsavel, show.ativo);
    }

    fclose(file);
    fclose(tempFile);

    // Atualiza o arquivo original apenas se houver mudança
    if (atualizado)
    {
        remove("./Database/shows.txt");
        rename("./Database/temp_shows.txt", "./Database/shows.txt");
    }
    else
    {
        remove("./Database/temp_shows.txt");
        printf("Show com ID %d não encontrado.\n", idShow);
    }

    return atualizado;
}

void comprarIngresso()
{
    int idShow;
    char confirmacaoPagamento;

    printf("Digite o ID do show para o qual deseja comprar o ingresso: ");
    scanf("%d", &idShow);

    FILE *file = fopen(CAMINHO_ARQUIVO_SHOWS, "r");
    FILE *tempFile = fopen(CAMINHO_ARQUIVO_TEMP, "w");

    if (file == NULL || tempFile == NULL)
    {
        perror("Erro ao abrir o arquivo de shows.\n");
        return;
    }

    Show show;
    int showEncontrado = 0;

    while (fscanf(file, "ID: %d | Nome: %99[^|] | Data: %99[^|] | Preço: %f | Ingressos: %d | CPF: %s | Ativo: %d\n",
                  &show.id, show.nomeShow, show.data, &show.preco, &show.ingressosDisponiveis, show.cpfResponsavel, &show.ativo) != EOF)
    {
        if (show.id == idShow && show.ativo && show.ingressosDisponiveis > 0)
        {
            showEncontrado = 1;

            printf("\nShow: %s\nData: %s\nPreço: %.2f\nIngressos disponíveis: %d\n",
                   show.nomeShow, show.data, show.preco, show.ingressosDisponiveis);

            printf("Confirme o pagamento do valor de R$%.2f (S/N): ", show.preco);
            getchar();
            confirmacaoPagamento = getchar();

            if (confirmacaoPagamento == 'S' || confirmacaoPagamento == 's')
            {

                show.ingressosDisponiveis--;

                Ingresso ingresso;
                ingresso.id = obterMaiorIdIngresso();
                ingresso.idShow = show.id;
                strcpy(ingresso.cpfComprador, userLoggedIn.cpf);

                FILE *ingressosFile = fopen("./Database/tickets.txt", "a");

                if (ingressosFile == NULL)
                {
                    perror("Erro ao abrir o arquivo de ingressos.\n");
                    fclose(file);
                    fclose(tempFile);
                    return;
                }

                fprintf(ingressosFile, "ID: %d | ShowID: %d | CPF: %s\n",
                        ingresso.id, ingresso.idShow, ingresso.cpfComprador);
                fclose(ingressosFile);

                printf("\nIngresso comprado com sucesso!\n");
            }
            else
            {
                printf("Compra cancelada.\n");
            }
        }

        trim(show.nomeShow);
        trim(show.data);
        trim(show.cpfResponsavel);

        fprintf(tempFile, "ID: %d | Nome: %s | Data: %s | Preço: %.2f | Ingressos: %d | CPF: %s | Ativo: %d\n",
                show.id, show.nomeShow, show.data, show.preco, show.ingressosDisponiveis, show.cpfResponsavel, show.ativo);
    }

    fclose(file);
    fclose(tempFile);

    // Salva as informações
    if (showEncontrado)
    {
        remove("./Database/shows.txt");
        rename("./Database/temp.txt", "./Database/shows.txt");
    }
    else
    {
        printf("Show não encontrado ou ingressos esgotados.\n");
        remove("./Database/temp.txt");
    }
}

void reembolsarIngresso()
{
    printf("Digite o ID do ingresso que deseja reembolsar: ");
    int idIngresso;
    scanf("%d", &idIngresso);

    FILE *file = fopen(CAMINHO_ARQUIVO_TICKETS, "r");
    FILE *tempFile = fopen(CAMINHO_ARQUIVO_TEMP, "w");
    Ingresso ingresso;
    int encontrado = 0;

    if (file == NULL || tempFile == NULL)
    {
        perror("Erro ao abrir o arquivo.\n");
        return;
    }

    while (fscanf(file, "ID: %d | ShowID: %d | CPF: %s\n",
                  &ingresso.id, &ingresso.idShow, ingresso.cpfComprador) != EOF)
    {
        trim(ingresso.cpfComprador);
        trim(userLoggedIn.cpf);

        if (ingresso.id == idIngresso && strcmp(ingresso.cpfComprador, userLoggedIn.cpf) == 0)
        {
            encontrado = 1;
            atualizarIngressosDisponiveis(ingresso.idShow, 1);
            continue;
        }

        fprintf(tempFile, "ID: %d | ShowID: %d | CPF: %s\n",
                ingresso.id, ingresso.idShow, ingresso.cpfComprador);
    }

    fclose(file);
    fclose(tempFile);

    if (encontrado)
    {
        remove("./Database/tickets.txt");
        rename("./Database/temp.txt", "./Database/tickets.txt");
        printf("Ingresso reembolsado com sucesso!\n");
    }
    else
    {
        remove("./Database/temp.txt");
        printf("Ingresso com ID %d não encontrado ou você não tem permissão para reembolsá-lo.\n", idIngresso);
    }
}

int cpfExisteRecursivo(FILE *file, const char *cpf)
{
    char linha[tamanhoMaximoCPF];

    if (fgets(linha, sizeof(linha), file) == NULL)
        return 0; // CPF não encontrado

    trim(linha);

    if (strcmp(linha, cpf) == 0)
        return 1; // CPF encontrado

    return cpfExisteRecursivo(file, cpf); // Chamada recursiva para próxima linha
}

void transferirIngresso()
{
    printf("Digite o ID do ingresso que deseja transferir: ");
    int idIngresso;
    scanf("%d", &idIngresso);

    printf("Digite o CPF do novo comprador: ");
    char novoCpf[tamanhoMaximoCPF];
    scanf("%s", novoCpf);

    // Abrir o arquivo de usuários para verificar se o novo CPF existe
    FILE *usuariosFile = fopen("./Database/users.txt", "r");
    if (usuariosFile == NULL)
    {
        perror("Erro ao abrir o arquivo de usuários.\n");
        return;
    }

    // Validar se o CPF do novo comprador existe
    if (!cpfExisteRecursivo(usuariosFile, novoCpf))
    {
        fclose(usuariosFile);
        printf("CPF %s não está registrado. Transferência cancelada.\n", novoCpf);
        return;
    }

    fclose(usuariosFile);

    FILE *file = fopen("./Database/tickets.txt", "r");
    FILE *tempFile = fopen("./Database/temp.txt", "w");
    Ingresso ingresso;
    int encontrado = 0;

    if (file == NULL || tempFile == NULL)
    {
        perror("Erro ao abrir o arquivo de ingressos.\n");
        return;
    }

    while (fscanf(file, "ID: %d | ShowID: %d | CPF: %s\n",
                  &ingresso.id, &ingresso.idShow, ingresso.cpfComprador) != EOF)
    {
        if (ingresso.id == idIngresso && strcmp(ingresso.cpfComprador, userLoggedIn.cpf) == 0)
        {
            encontrado = 1;
            strcpy(ingresso.cpfComprador, novoCpf);
        }

        trim(ingresso.cpfComprador);
        fprintf(tempFile, "ID: %d | ShowID: %d | CPF: %s\n",
                ingresso.id, ingresso.idShow, ingresso.cpfComprador);
    }

    fclose(file);
    fclose(tempFile);

    if (encontrado)
    {
        remove("./Database/tickets.txt");
        rename("./Database/temp.txt", "./Database/tickets.txt");
        printf("Ingresso transferido com sucesso!\n");
    }
    else
    {
        remove("./Database/temp.txt");
        printf("Ingresso com ID %d não encontrado ou você não tem permissão para transferi-lo.\n", idIngresso);
    }
}

void mostrarIngressoUsuario()
{
    FILE *file = fopen("./Database/tickets.txt", "r");
    Ingresso ingresso;
    int encontrouIngresso = 0;

    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo de tickets.\n");
        return;
    }

    printf("\nMeus tickets:\n");

    while (fscanf(file, "ID: %d | ShowID: %d | CPF: %s\n",
                  &ingresso.id, &ingresso.idShow, ingresso.cpfComprador) != EOF)
    {

        trim(ingresso.cpfComprador);
        trim(userLoggedIn.cpf);

        if (strcmp(ingresso.cpfComprador, userLoggedIn.cpf) == 0)
        {
            encontrouIngresso = 1;
            printf("Ingresso ID: %d | ShowID: %d\n", ingresso.id, ingresso.idShow);
        }
    }

    if (!encontrouIngresso)
    {
        printf("Você não possui tickets comprados.\n");
    }

    fclose(file);
}

void mostrarDetalhesShow()
{
    int idShow;
    printf("Digite o ID do show para visualizar os detalhes: ");
    scanf("%d", &idShow);

    FILE *file = fopen("./Database/shows.txt", "r");
    Show show;
    char linha[256];
    int encontrado = 0;

    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo.\n");
        return;  // Retorna da função sem encerrar o programa
    }

    while (fgets(linha, sizeof(linha), file))
    {
        // Lê as informações do show a partir da linha do arquivo
        if (sscanf(linha, "ID: %d | Nome: %99[^|] | Data: %99[^|] | Preço: %f | Ingressos: %d | CPF: %s | Ativo: %d",
                   &show.id, show.nomeShow, show.data, &show.preco, &show.ingressosDisponiveis, show.cpfResponsavel, &show.ativo) != 7)
        {
            fprintf(stderr, "Erro ao ler dados do show. A linha pode estar mal formatada.\n");
            continue;  // Pula para a próxima linha, caso a leitura falhe
        }

        // Verifica se o ID do show corresponde ao fornecido
        if (show.id == idShow)
        {
            encontrado = 1;
            break; // Encontrou o show, não precisa continuar a busca
        }
    }

    fclose(file);

    if (encontrado)
    {
        // Exibe as informações do show
        printf("\nDetalhes do Show:\n");
        printf("ID: %d\n", show.id);
        printf("Nome: %s\n", show.nomeShow);
        printf("Data: %s\n", show.data);
        printf("Preço: %.2f\n", show.preco);
        printf("Ingressos Disponíveis: %d\n", show.ingressosDisponiveis);
        printf("CPF do Responsável: %s\n", show.cpfResponsavel);
        printf("Ativo: %s\n", show.ativo ? "Sim" : "Não");
    }
    else
    {
        printf("Show com ID %d não encontrado.\n", idShow);
    }
}

void excluirTicketsPorShowRecursivo(FILE *file, FILE *tempFile, int idShow)
{
    Ingresso *ingresso = (Ingresso *)malloc(sizeof(Ingresso));

    if (ingresso == NULL)
    {
        perror("Erro ao alocar memória para o ingresso.\n");
        return;
    }

    // Lê um ingresso do arquivo
    if (fscanf(file, "ID: %d | ShowID: %d | CPF: %s\n", &ingresso->id, &ingresso->idShow, ingresso->cpfComprador) == EOF)
    {
        // Fim do arquivo
        free(ingresso);
        return;
    }

    // Se o ingresso não pertence ao show excluído, grava no arquivo temporário
    if (ingresso->idShow != idShow)
    {
        fprintf(tempFile, "ID: %d | ShowID: %d | CPF: %s\n",
                ingresso->id, ingresso->idShow, ingresso->cpfComprador);
    }

    // Libera a memória do ingresso atual
    free(ingresso);

    // Chamada recursiva para o próximo ingresso
    excluirTicketsPorShowRecursivo(file, tempFile, idShow);
}

void excluirTodosTicketsPorShow(int idShow)
{
    FILE *file = fopen("./Database/tickets.txt", "r");
    FILE *tempFile = fopen("./Database/temp_tickets.txt", "w");

    if (file == NULL || tempFile == NULL)
    {
        perror("Erro ao abrir o arquivo de tickets.\n");
        return;
    }

    // Chamada inicial da função recursiva
    excluirTicketsPorShowRecursivo(file, tempFile, idShow);

    fclose(file);
    fclose(tempFile);

    // Substitui o arquivo original pelo temporário sem os ingressos do show excluído
    remove("./Database/tickets.txt");
    rename("./Database/temp_tickets.txt", "./Database/tickets.txt");

    printf("Todos os tickets associados ao show com ID %d foram excluídos.\n", idShow);
}