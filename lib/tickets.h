#ifndef ingressos
#define shingressosows

#define tamanhoMaximoCPF 15

typedef struct
{
    int id;
    int idShow;
    char cpfComprador[tamanhoMaximoCPF];
} Ingresso;

int obterMaiorIdIngresso();
int atualizarIngressosDisponiveis(int idShow, int incremento);
void comprarIngresso();
int cpfExisteRecursivo(FILE *file, const char *cpf);
void transferirIngresso(); 
void reembolsarIngresso();
void mostrarIngressoUsuario();
void mostrarDetalhesShow();
void excluirTodosTicketsPorShow(int idShow);
void excluirTicketsPorShowRecursivo(FILE *file, FILE *tempFile, int idShow);

#endif
