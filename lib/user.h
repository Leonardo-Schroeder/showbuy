#ifndef users
#define users

#define tamanhoMaximoNome 100
#define tamanhoMaximoEmail 120
#define tamanhoMaximoSenha 40
#define tamanhoMaximoCPF 13

typedef struct
{
    char nomeCompleto[tamanhoMaximoNome];
    char cpf[tamanhoMaximoCPF];
    char email[tamanhoMaximoEmail];
    char senha[tamanhoMaximoSenha];
    int idade;
    int role;
} Usuario;

Usuario userLoggedIn;

int validarCPF(const char *cpf);
int validarEmail(const char *email);
int validarSenha(const char *senha);
int autenticarUsuario();
int alterarSenha();
Usuario* buscarUsuarioPorCPF(const char *cpf);
void cadastrarUsuario();
void salvarUsuario(Usuario usuario);
void trim(char *str);

#endif
