#ifndef shows
#define shows

#define tamanhoMaximoNome 100
#define tamanhoMaximoData 100
#define tamanhoMaximoCPF 13

typedef struct
{
    int id;
    char nomeShow[tamanhoMaximoNome];
    char data[tamanhoMaximoData];
    float preco;
    int ingressosDisponiveis;
    //Adicionar endereço
    char cpfResponsavel[tamanhoMaximoCPF];
    int ativo;
} Show;

int obterMaiorId();
void cadastrarShow();
void removerShow();
void atualizarShow();
void mostrarTodosShows();
void mostrarMeuShow();

#endif
