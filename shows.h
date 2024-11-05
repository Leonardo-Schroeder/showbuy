#ifndef shows
#define shows

#define tamanhoMaximoNome 100

typedef struct
{
    int id;
    char nomeShow[tamanhoMaximoNome];
    float preco;
    int ingressosDisponiveis;
    int ativo;
} Show;

int obterMaiorId();
void cadastrarShow();
void removerShow(int id);
void atualizarShow(int id);

#endif
