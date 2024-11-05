#ifndef shows
#define shows

#define tamanhoMaximoNome 100
#define tamanhoMaximoCPF 15

typedef struct
{
    int id;
    char nomeShow[tamanhoMaximoNome];
    float preco;
    int ingressosDisponiveis;
    char cpfResponsavel[tamanhoMaximoCPF];
    int ativo;
} Show;

int obterMaiorId();
void cadastrarShow();
void removerShow(int id);
void atualizarShow(int id);

#endif
