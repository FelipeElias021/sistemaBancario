#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <ctype.h>
#include <stdbool.h>
//#include <pthread.h>
#define TAXA_FIXA 1.5
#define TEMPO 60

/*Áreas: pix - depositar - Investimento - TED - Shopping - Configurações - Extrato - Sair*/

void login();
void menu();
void investimento();
float resgatarDinheiro(float investimentoMisto, char tipo);
void bicho();
void deposito();
// void *fixa(void *arg);
// void *variavel(void *arg);
void configuracao();
void pix();
void minhas_chaves();
void transferencia();
void favoritos();
void menu_ted();
void ted();
void informacoes();
void backup();
void pausar();
void pedirSenha4();

struct dados_cliente
{
    char nome[41];
    char senha[21];
    char email[41];
    char cpf[12];
    int conta;
    int senha4;
    float saldo;
    float investimentoFix;
    float investimentoVar;
};

// Variaveis Globais - Felipe
struct dados_cliente cliente[10];
// pthread_t newthread1;
// pthread_t newthread2; // criando o pthread
float taxa, risco; // variaveis para o investimeto renda variavel
int i_atual = 0, nClientes = 0, cicloRendaVar = 0;
bool mostrarRenda = true, flagFixa, flagVariavel;
char nomeMaiusculo[41]; // se deixar a var nome main tudo minusculo é mais fácil de comparar para o login, ent essa é para ficar formatado

// Variaveis - Paulo
struct pixx
{
    char nome[5][40];
    char chave[5][15];
};
struct pixx cartao;
struct validacao
{
    char email[40];
    char cpf[12];
    char meuemail[40];
    char meucpf[12];
};
struct validacao verificacao[4];
void transferencia();
void minhas_chaves();
void favoritos();
bool chave_email = false, chave_cpf = false, chave_telefone = false, chave_aleatoria = false, verificar_favoritos = false; // as 4 primeiras usadas para validar chaves criadas em minnhas chaves, e a ultima para validar se adicionou favoritos
char meutelefone[12];                                                                                                      // usado em minhas chaves para criar minhas chaves
int chavealeatoria = 0, dfavoritos, cfavoritos;                                                                            // a primeira usada em minhas chaves para gerar chave aleatoria, e as outras duas para armazenar dados na criação de favotios e não sobrescrever

FILE *arquivo;

int main()
{
    arquivo = fopen("user.txt", "r");
    for (int i = 0; i < 10; i++)
    {
        fgets(cliente[i].nome, 41, arquivo);
        fscanf(arquivo, "%s", &cliente[i].cpf);
        fscanf(arquivo, "%s", &cliente[i].senha);
        fscanf(arquivo, "%s", &cliente[i].email);
        fscanf(arquivo, "%d", &cliente[i].conta);
        fscanf(arquivo, "%d", &cliente[i].senha4);
        fscanf(arquivo, "%f", &cliente[i].saldo);
        fscanf(arquivo, "%f", &cliente[i].investimentoVar);
        fscanf(arquivo, "%f", &cliente[i].investimentoFix);
        if (cliente[i].conta != 0)
        {
            nClientes++;
        }
    }
    fclose(arquivo);
    // apenas começa o código levando ao login
    login();
    return 0;
}

void pausar()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ; // limpar o buffer do teclado

    printf("\n\nPressione enter para continuar.\n");
    getchar();
}

void backup()
{
    arquivo = fopen("user.txt", "w");
    for (int i = 0; cliente[i].conta != 0; i++)
    {
        fprintf(arquivo, "%s%s\n%s\n%s\n%d\n%d\n%f\n%f\n%f", cliente[i].nome, cliente[i].cpf, cliente[i].senha, cliente[i].email, cliente[i].conta, cliente[i].senha4, cliente[i].saldo, cliente[i].investimentoVar, cliente[i].investimentoFix);
        nClientes = i + 1;
    }
    fclose(arquivo);
}

void pedirSenha4()
{
    int validacao_senha, tentativas;
    bool flagSenha4;

    for (tentativas = 3; tentativas >= 0; tentativas--)
    {

        if (tentativas == 0)
        {
            printf("\n\nMuitas tentativas incorretas!");
            pausar();
            menu();
        }
        flagSenha4 = true;
        do
        {
            printf("\n> Senha de 4 digitos para transacoes: ");
            scanf("%d", &validacao_senha);
            if (validacao_senha / 1000 < 1 || validacao_senha / 1000 > 9)
            {
                flagSenha4 ? printf("\nSenha deve conter apenas 4 digitos!\n") : false;
            }
            else
            {
                flagSenha4 = false;
            }
            /*vai verificar se tem mesmo 4 digitos, se for digitado pro exemplo 444, a divisão
            desse número por 1000 é 0..., como a divisão é entre inteiro, e se for tipo 55555,
            vai ser 55 nesse caso, maior que 9*/
        } while (flagSenha4);
        // os operadores ternários foram usados para imprimir mensagens quando está errado o campo

        if (validacao_senha != cliente[i_atual].senha4)
        {
            printf("Senha incorreta, nao foi possivel continuar\nTente novamente\n%d tentativas restantes", tentativas - 1);
            pausar();
        }
        else
        {
            break;
        }
    }
}

void login()
{
    int escolhaLogin;
    bool flagEmail, flagSenha, flagValidacao, flagCadastro;
    char validacaoUser[41], validacaoSenha[21];
    system("cls");
    backup();
    printf("Bem vindo ao Banco ETI!\n");
    printf("> 1. Fazer login\n");
    printf("> 2. Cadastrar novo cliente\n> ");
    scanf("%d", &escolhaLogin);
    switch (escolhaLogin)
    {
    case 1:
        // pedir cpf/nome e senha
        if (cliente[0].conta == 0)
        { // se o client 0 n tiver conta ainda, vai pedir o cadrasto, pausando e voltando ao login
            printf("\nCadastre primeiro\n");
            pausar();
            login();
        }

        flagValidacao = true; // deixando a flag true dnovo
        do
        {
            setbuf(stdin, NULL); // limpar para o fgets funcionar
            printf("\nNome / CPF: ");
            fgets(validacaoUser, 41, stdin);
            printf("Senha: ");
            scanf("%s", &validacaoSenha);
            for (int j = 0; cliente[j].conta != 0; j++)
            { // fazer o teste em todos os clientes cadastros, por isso quando conta != 0
                if (isalpha(validacaoUser[0]))
                { // verificar se é nome ou cpf (vai verificar se o primeiro char é letra ou n)
                    for (int i = 0; validacaoUser[i] != '\0'; i++)
                    {                                                 // para passar por todos os char da string
                        validacaoUser[i] = tolower(validacaoUser[i]); // deixar tudo minúsculo
                    }
                    if (strcmp(cliente[j].nome, validacaoUser) == 0)
                    { // se o nome for igual, a flag fica false e sai do do...while
                        flagValidacao = false;
                        i_atual = j; // armazena o index para fazer as transações
                        if (strcmp(cliente[j].senha, validacaoSenha) != 0)
                        {
                            flagValidacao = true;
                        } // se a senha for diferente a flag volta a true e repete o processo
                    }
                }
                else
                {
                    int cont = 0;
                    for (int i = 0; i <= 11; i++)
                    {
                        if ((int)cliente[j].cpf[i] == (int)validacaoUser[i])
                        {
                            cont++;
                        }
                    }
                    if (cont == 11)
                    {
                        flagValidacao = false;
                        i_atual = j;
                        if (strcmp(cliente[j].senha, validacaoSenha) != 0)
                        {
                            flagValidacao = true;
                        }
                    }
                    /*Vai passar por cada char, converter em int e comparar se é igual,
                    se for vai incrementar na variável contadora, e se esta for igual a 11
                    q é o número de números de um cpf é pq todos são iguais e roda*/
                }
            }
        } while (flagValidacao);

        for (int i = 0; cliente[i_atual].nome[i] != ' '; i++)
        {                                                // para passar por todos os char da string até ter um espaço
            nomeMaiusculo[i] = cliente[i_atual].nome[i]; // copiar para a variavel nomeMaiusculo
            if (cliente[i_atual].nome[i] == '\0')
            {
                break;
            } // se chegar no final(\0), então vai quebrar o código, pois ia dar conflito, já que se chegasse no final n ia ter espaço e dar problema
        }
        nomeMaiusculo[0] = toupper(nomeMaiusculo[0]); // deixar a primeira letra maiuscula
        if (cliente[i_atual].investimentoVar > 0)
        {
            flagVariavel = true;
            // pthread_create(&newthread1, NULL, variavel, NULL); // continuar o investimento se ele for maior que 0
        }
        if (cliente[i_atual].investimentoFix > 0)
        {
            flagFixa = true;
            // pthread_create(&newthread2, NULL, fixa, NULL); // continuar o investimento se ele for maior que 0
        }

        menu(); // leva ao menu
        break;
    case 2:
        // pedir informações como nome, senha, email e a senha de 4 dígitos
        setbuf(stdin, NULL);
        printf("\n> Nome: ");
        fgets(cliente[nClientes].nome, 41, stdin);
        for (int i = 0; cliente[nClientes].nome[i] != '\0'; i++)
        {
            cliente[nClientes].nome[i] = tolower(cliente[nClientes].nome[i]);
        } // para deixar as  letras minúsculas

        do
        {
            printf("> CPF: ");
            scanf(" %s", &cliente[nClientes].cpf);
            strlen(cliente[nClientes].cpf) != 11 ? printf("\nCPF invalido, digite um valido !(CPF deve ter apenas 11 numeros)\n") : false;
        } while (strlen(cliente[nClientes].cpf) != 11); // comprimento (total de char) tem q ser igual a 11 para continuar

        flagEmail = true;
        do
        {
            printf("> E-mail: ");
            scanf(" %s", &cliente[nClientes].email);
            for (int i = 0; cliente[nClientes].email[i] != '\0'; i++)
            { // poderia ser usado strlen na segunda parte, mas assim para no final da string, que é \0, por isso deve declarar o chat com 1 a mais
                if (cliente[nClientes].email[i] == '@')
                {
                    flagEmail = false;
                }
                cliente[nClientes].email[i] = tolower(cliente[nClientes].email[i]);
            }
            /*vai verificar cada char para ver se tem um @, se n tiver n é email
            e ainda vai deixar tudo minúsculo*/
            flagEmail ? printf("\nE-mail invalido!\n") : false;
        } while (flagEmail);

        printf("> Senha: ");
        scanf(" %s", &cliente[nClientes].senha);

        flagSenha = true;
        do
        {
            printf("> Senha de 4 digitos para transacoes: ");
            scanf("%d", &cliente[nClientes].senha4);
            if (cliente[nClientes].senha4 / 1000 < 1 || cliente[nClientes].senha4 / 1000 > 9)
            {
                flagSenha ? printf("\nSenha deve conter apenas 4 digitos!\n") : false;
            }
            else
            {
                flagSenha = false;
            }
            /*vai verificar se tem mesmo 4 digitos, se for digitado pro exemplo 444, a divisão
            desse número por 1000 é 0..., como a divisão é entre inteiro, e se for tipo 55555,
            vai ser 55 nesse caso, maior que 9*/
        } while (flagSenha);
        // os operadores ternários foram usados para imprimir mensagens quando está errado o campo

        cliente[nClientes].saldo = 0;
        cliente[nClientes].investimentoFix = 0;
        cliente[nClientes].investimentoVar = 0;

        srand(time(NULL));
        // o rand vai pegar um semnte multiplicar por 9999 e depois somar 100.000.000 para ter o mesmo comprimento todos
        cliente[nClientes].conta = 100000000 + rand() * 9999;

        flagCadastro = true; // sempre definir como true
        for (int i = 0; cliente[i].conta != 0; i++)
        { // passa por todos os vetores de clientes
            if (strcmp(cliente[i - 1].nome, cliente[nClientes].nome) == 0)
            {
                printf("\nCadastro invalido");
                printf("\nNome ja cadastrado\n\n");
                pausar();
                flagCadastro = false;
            }
            if (strcmp(cliente[i - 1].email, cliente[nClientes].email) == 0)
            {
                printf("\nCadastro invalido");
                printf("\nEmail ja cadastrado\n\n");
                pausar();
                flagCadastro = false;
            }
            if (strcmp(cliente[i - 1].cpf, cliente[nClientes].cpf) == 0)
            {
                printf("\nCadastro invalido");
                printf("\nCPF ja cadastrado\n\n");
                pausar();
                flagCadastro = false;
            }
            // se o nome,email ou cpf for igual vai invalidar o cadastro, e definir a flag como false
            if (flagCadastro == false)
            {
                strcpy(cliente[nClientes].nome, "");
                strcpy(cliente[nClientes].cpf, "");
                strcpy(cliente[nClientes].senha, "");
                strcpy(cliente[nClientes].email, "");
                cliente[nClientes].conta = 0;
                cliente[nClientes].senha4 = 0;
                login();
            } // se a tag for false é pq algum foi igual então ele vai redefinir todas as variáveis cadastradas e ir para o login para n incrementar no nCliente
            if (cliente[nClientes].conta == cliente[i - 1].conta)
            {
                srand(time(NULL));
                cliente[nClientes].conta = 100000000 + rand() * 9999;
            } // se a conta for igual(difícil de acontecer) apenas vai definir outra
        }

        nClientes++; // Incrementa para o proximo cadastro
        login();     // leva de volta a função login, para se fazer o login
        break;

    case 1024:
        //"Opção de desenvolvedor" - mostrar todos os dados;
        for (int i = 0; cliente[i].conta != 0; i++)
        {
            printf("\nNome: %s", cliente[i].nome);
            printf("Email: %s\n", cliente[i].email);
            printf("Senha: %s\n", cliente[i].senha);
            printf("Cpf: %s\n", cliente[i].cpf);
            printf("Conta: %d\n", cliente[i].conta);
            printf("Senha 4 digitos: %d\n", cliente[i].senha4);
            printf("Saldo: %f\n", cliente[i].saldo);
            printf("Investimento Fixo: %f\n", cliente[i].investimentoFix);
            printf("Investimento Variavel: %f\n", cliente[i].investimentoVar);
        }
        pausar();
        login(); // leva de volta a função login, para se fazer o login
        break;
    default:
        printf("Opcao Invalida\n");
        pausar();
        login();
    }
    return;
}

void menu()
{
    int escolhaMenu = 0;
    system("cls");
    backup();
    printf("           Banco ETI\n\n");
    printf("Ola %s\n", nomeMaiusculo);
    printf("Conta: %d\n", cliente[i_atual].conta);
    if (mostrarRenda == true)
    {
        printf("Saldo: %.2f ETC\n", cliente[i_atual].saldo);
    }
    printf("--------------------\n");
    printf("        Menu        \n\n");
    if (mostrarRenda == true)
    {
        printf("> 1. Ocultar Saldo\n");
    }
    else if (mostrarRenda == false)
    {
        printf("> 1. Mostrar Saldo\n");
    }
    printf("> 2. Pix\n");
    printf("> 3. TED\n");
    printf("> 4. Depositar\n");
    printf("> 5. Investimento\n");
    printf("> 6. configuracao\n");
    printf("> 7. Sair\n> ");
    scanf("%d", &escolhaMenu);
    switch (escolhaMenu)
    {
    case 1:
        if (mostrarRenda == true)
        {
            mostrarRenda = false;
        }
        else if (mostrarRenda == false)
        {
            mostrarRenda = true;
        }
        menu();
        break;
    case 2:
        pix();
        break;
    case 3:
        menu_ted();
        break;
    case 4:
        deposito();
        break;
    case 5:
        investimento();
        break;
    case 6:
        configuracao();
        break;
    case 7:
        flagFixa = false; // quando for sair da conta o investimento para
        flagVariavel = false;
        login();
        break;
    default:
        printf("Opcao Invalida\n");
        pausar();
        menu();
    }
    return;
}

void pix()
{
    system("cls");
    int n;

    printf("\n -- Pix --\n");
    printf("\n1-Transferir  \n2-Minhas Chaves\n3-Favoritos\n4-Voltar para o menu\n");
    scanf("%d", &n);

    switch (n)
    {
    case 1:
        transferencia();
        break;
    case 2:
        minhas_chaves();
        break;
    case 3:
        favoritos();
        break;
    case 5:
        pausar();
        menu();
        break;
    default:
        pausar();
        menu();
    }
    return;
}

void transferencia()
{
    char nome[35], mensagem[40], chavea[12], celular[12];
    int w1 = 0, escolha1, saldo, vt, cont = 0, escolhasfavoritos, senha2, i, j = 0;
    bool verificar_emailecpf = true;

    // vai ter ligação com a parte do João
    system("cls");
    printf("Saldo disponivel em sua conta R$ %.2f\n", cliente[i_atual].saldo);
    printf("Informe o valor da transferencia R$ ");
    scanf("%d", &vt);
    if (vt < cliente[i_atual].saldo && vt > 0)
    {
        setbuf(stdin, NULL);
        printf("Qual o nome da pessoa que vai receber o pix: ");
        fgets(nome, 40, stdin);
        setbuf(stdin, NULL);
        printf("Escreva uma mensagem: ");
        fgets(mensagem, 40, stdin);
        printf("\n Qual o tipo de Chave? Pix \n1-E-mail \n2-CPF/CNPJ \n3-chave pix \n4-Numero celular\n5-Favoritos \n");
        scanf("%d", &escolha1);

        switch (escolha1)
        {
        case 1:
            setbuf(stdin, NULL);
            do
            {
                printf("Informe o email:\n");
                fgets(verificacao[0].email, 40, stdin);
                for (i = 0; verificacao[0].email[i] != '\0'; i++)
                {
                    if (verificacao[0].email[i] == '@')
                    {
                        verificar_emailecpf = false;
                    }
                }
                if (j == 2)
                {
                    pausar();
                    pix();
                }
                j++;
            } while (verificar_emailecpf);
            pedirSenha4();
            system("cls");
            printf("\n\nTransferencia realizada com sucesso.\n"); // usar todos dessa forma para gerar o comprovante.
            printf("\n -- Comprovante --\n");
            printf("%s", mensagem);
            printf("Enviadado para %s", nome);
            printf("Chave pix %s", verificacao[0].email);
            printf("No valor de R$%d reais.", vt);
            printf("\nObrigado por usar o ETI.");
            cliente[i_atual].saldo = cliente[i_atual].saldo - vt;
            break;
        case 2:
            setbuf(stdin, NULL);
            do
            {
                printf("Informe o cpf:\n");
                scanf(" %s", &verificacao[1].cpf);
                strlen(verificacao[1].cpf) != 11 ? printf("Erro cpf nao tem 11 numeros, digitar novamente.\n") : false;
                if (j == 2)
                {
                    pausar();
                    pix();
                }
                j++;
            } while (strlen(verificacao[1].cpf) != 11);
            pedirSenha4();
            system("cls");
            printf("\n\nTransferencia realizada com sucesso.\n"); // usar todos dessa forma para gerar o comprovante.
            printf("\n -- Comprovante --\n");
            printf("%s", mensagem);
            printf("Enviadado para %s", nome);
            printf("Chave pix %s\n", verificacao[1].cpf);
            printf("No valor de R$%d reais.", vt);
            printf("\nObrigado por usar o ETI.");
            cliente[i_atual].saldo = cliente[i_atual].saldo - vt;
            break;

        case 3:
            setbuf(stdin, NULL);
            printf("Informe a chave pix: ");
            fgets(chavea, 12, stdin);
            pedirSenha4();
            system("cls");
            printf("\n\nTransferencia realizada com sucesso.\n"); // usar todos dessa forma para gerar o comprovante.
            printf("\n -- Comprovante --\n");
            printf("%s", mensagem);
            printf("Enviadado para %s", nome);
            printf("Chave pix %s", chavea);
            printf("No valor de R$%d reais.", vt);
            printf("\nObrigado por usar o ETI.");
            cliente[i_atual].saldo = cliente[i_atual].saldo - vt;
            break;

        case 4:
            setbuf(stdin, NULL);
            printf("Informe O numero do celular com o dd +55 ");
            fgets(celular, 12, stdin);
            pedirSenha4();
            system("cls");
            printf("\n\nTransferencia realizada com sucesso.\n"); // usar todos dessa forma para gerar o comprovante.
            printf("\n -- Comprovante --\n");
            printf("%s", mensagem);
            printf("Enviadado para %s", nome);
            printf("Chave pix %s", celular);
            printf("\nNo valor de R$ %d reais.", vt);
            printf("\nObrigado por usar o ETI.");
            cliente[i_atual].saldo = cliente[i_atual].saldo - vt;
            break;

        case 5:
            if (verificar_favoritos)
            {
                system("cls");
                printf("\n -- Favoritos --\n\n");
                for (cont = 0; cont < dfavoritos; cont++)
                {
                    printf("%d-%s", cont + 1, cartao.nome[cont]);
                    printf("%s\n\n", cartao.chave[cont]);
                }
                printf("Escolha um contato para fazer transferencia: ");
                scanf("%d", &escolhasfavoritos);
                pedirSenha4();
                system("cls");

                switch (escolhasfavoritos)
                {
                case 1:
                    printf("\n\nTransferencia realizada com sucesso.\n");
                    printf("\n -- Comprovante --\n");
                    printf("%s", mensagem);
                    printf("Enviadado para %s", cartao.nome[0]);
                    printf("Chave pix %s", cartao.chave[0]);
                    printf("\nNo valor de R$ %d reais.", vt);
                    printf("\nObrigado por usar o ETI.");
                    cliente[i_atual].saldo = cliente[i_atual].saldo - vt;
                    break;
                case 2:
                    printf("\n\nTransferencia realizada com sucesso.\n");
                    printf("\n -- Comprovante --\n");
                    printf("%s", mensagem);
                    printf("Enviadado para %s", cartao.nome[1]);
                    printf("Chave pix %s", cartao.chave[1]);
                    printf("\nNo valor de R$ %d reais.", vt);
                    printf("\nObrigado por usar o ETI.");
                    cliente[i_atual].saldo = cliente[i_atual].saldo - vt;
                    break;
                case 3:
                    printf("\n\nTransferencia realizada com sucesso.\n");
                    printf("\n -- Comprovante --\n");
                    printf("%s", mensagem);
                    printf("Envidado para %s", cartao.nome[2]);
                    printf("Chave pix %s", cartao.chave[2]);
                    printf("\nNo valor de R$ %d reais.", vt);
                    printf("\nObrigado por usar o ETI.");
                    cliente[i_atual].saldo = cliente[i_atual].saldo - vt;
                    break;
                case 4:
                    printf("\n\nTransferencia realizada com sucesso.\n");
                    printf("\n -- Comprovante --\n");
                    printf("%s", mensagem);
                    printf("Enviadado para %s", cartao.nome[3]);
                    printf("Chave pix %s", cartao.chave[3]);
                    printf("\nNo valor de R$ %d reais.", vt);
                    printf("\nObrigado por usar o ETI.");
                    cliente[i_atual].saldo = cliente[i_atual].saldo - vt;
                    break;
                case 5:
                    printf("\n\nTransferencia realizada com sucesso.\n");
                    printf("\n -- Comprovante --\n");
                    printf("%s", mensagem);
                    printf("Enviadado para %s", cartao.nome[4]);
                    printf("Chave pix %s", cartao.chave[4]);
                    printf("\nNo valor de R$ %d reais.", vt);
                    printf("\nObrigado por usar o ETI.");
                    cliente[i_atual].saldo = cliente[i_atual].saldo - vt;
                    break;
                }
            }
            else
            {
                printf("Nenhum contanto adicionado aos favoritos");
            }
            break;

        default:
            printf("Escolha uma existente.");
            break;
        }
        // colocar a opção de escrever alguma mensagem e também de gerar um comprovante
        // se tiver como enviar esse comprovante para o email escolhido
        pausar();
        pix();
    }
    else
    {
        printf("\nSaldo insuficiente.\n");
        pausar();
        pix();
    }
    return;
}

void minhas_chaves()
{
    system("cls");
    int w2, escolha2, escolha3, i, j = 0;
    bool verificar_meuemailecpf = true;
    printf("\n1-Para cadastrar uma nova chave pix \n2-Chaves Cadrastadas \n");
    scanf("%d", &escolha2);

    if (escolha2 == 1)
    {
        system("cls");
        printf("\nPermitido cadastrar apenas uma chave pix para cada tipo");
        printf("\nQual o tipo de Chave deseja cadastrar ? Pix \n1-E-mail \n2-CPF/CNPJ \n3-Numero telefone \n4-Gerar uma chave automatica \n");
        scanf("%d", &escolha3);

        switch (escolha3)
        {
        case 1:
            do
            {
                setbuf(stdin, NULL);
                printf("Informe o e-mail: ");
                fgets(verificacao[2].meuemail, 40, stdin);
                for (i = 0; verificacao[2].meuemail[i] != '\0'; i++)
                {
                    if (verificacao[2].meuemail[i] == '@')
                    {
                        verificar_meuemailecpf = false;
                    }
                }

                j++;
                if (j == 3)
                {
                    pix();
                }
            } while (verificar_meuemailecpf);
            printf("Email %scadastrado com sucesso.\n", verificacao[2].meuemail);
            chave_email = true;
            break;

        case 2:
            do
            {
                printf("Informe o cpf:\n");
                scanf(" %s", &verificacao[3].meucpf);
                strlen(verificacao[3].meucpf) != 11 ? printf("Erro cpf nao tem 11 numeros, digitar novamente.\n") : false;
                if (j == 2)
                {
                    pausar();
                    pix();
                }
                j++;
            } while (strlen(verificacao[3].meucpf) != 11);
            printf("Chave %s cadastrada com sucesso.\n", verificacao[3].meucpf);
            chave_cpf = true;
            break;

        case 3:
            setbuf(stdin, NULL);
            printf("Informe o numero de telefone com dd +55 ");
            fgets(meutelefone, 12, stdin);
            printf("Chave %scadastrada com sucesso.\n", meutelefone);
            chave_telefone = true;
            break;

        case 4:
            if (chavealeatoria == 0)
            {
                chavealeatoria = 100000 + rand() % 10000000000000;
                printf("Chave %d cadastrada com sucesso.\n", chavealeatoria);
                chave_aleatoria = true;
            }
            break;
        }
        // printf("%s", me);
    }
    if (escolha2 == 2)
    {
        system("CLS");
        printf("\n -- Chaves Cadastradas --\n");
        if (chave_cpf)
        { // alterar a parte de comparação
            printf("\n%s\n", verificacao[3].meucpf);
        }
        if (chave_aleatoria)
        {
            printf("%d\n", chavealeatoria);
        }
        if (chave_email)
        { // não é uma comparação muito eficiente;
            printf("%s\n", verificacao[2].meuemail);
        }
        if (chave_telefone)
        {
            printf("%s\n", meutelefone);
        }
        if (chave_cpf == false && chave_email == false && chave_aleatoria == false && chave_telefone == false)
        {
            printf("Nao tem nenhuma chave cadastrada!!\n");
        }
    }
    pausar();
    pix();

    return;
}

void favoritos()
{
    system("cls");
    int favoritos, w4, cont;
    system("cls");
    printf("No maximo pode ser adicionados 5 contatos aos favoritos.\n");
    printf("Informe quantos contatos vai adicionar aos favoritos: ");
    scanf("%d", &favoritos);
    if (favoritos <= 5)
    {
        dfavoritos = dfavoritos + favoritos;
        for (cont = cfavoritos; cont < dfavoritos; cont++)
        {
            setbuf(stdin, NULL);
            printf("Qual o nome: ");
            fgets(cartao.nome[cont], 300, stdin);
            printf("Qual a chave pix: ");
            scanf("%s", &cartao.chave[cont]);
            verificar_favoritos = true;
        }
        cfavoritos = cfavoritos + favoritos;
        printf("Contatos adicionados ao favoritos com sucesso.\n");
        pausar();
        pix();
    }
    else
    {
        pix();
    }

    return;
}

void menu_ted()
{
    int ted1;
    system("cls");
    printf("1-Para fazer TED\n2-Para saber sobre TED:\n3-Para voltar ao menu\n");
    scanf("%d", &ted1);

    switch (ted1)
    {
    case 1:
        ted();
        break;
    case 2:
        informacoes();
        break;
    case 3:
        menu();
        break;
    default:
        pausar();
        menu();
    }
    return;
}

void ted()
{
    bool flag1 = false, flag = true;
    int i, n, n2, ct, sair, senha3;
    float valort = 0;
    char nome2[10], nome3[30], conta[6];
    // a parte logo abaixo vai receber o nome e o valor para fazer a transferencia

    printf("Saldo disponivel em sua conta R$ %.2f\n", cliente[i_atual].saldo);
    printf("Informe o valor a ser transferido: ");
    scanf("%f", &valort);
    if (valort < cliente[i_atual].saldo && valort > 0)
    {
        setbuf(stdin, NULL);
        printf("Informe o nome:");
        fgets(nome2, 10, stdin);

        for (i = 0; nome2[i] != '\0'; i++)
        {

            nome2[i] = tolower(nome2[i]);
        }

        // caso necessário colocar um for para passar todo o nome para minuculo para fazer a comparação
        // for de comparação atráves do nome para saber se a pessoa tem conta no banco
        printf("\n");
        for (i = 0; i < 10; i++)
        { // altera isso aqui

            if (strncmp(cliente[i].nome, nome2, 3) == 0)
            {
                // informar todas as pessoas com esse nome que tem conta no banco
                flag1 = true;
                printf("%d-%s", i + 1, cliente[i].nome);
                printf("%d\n", cliente[i].conta);
            }
        }
        if (flag1)
        {
            // usando flag para se o nome informado tiver no cadastro cair nessa opção, e finalizar a transferencia
            printf("Digite o numero da pessoa para quem deseja fazer a transfencia:\n");
            scanf("%d", &n);
            system("cls");
            printf("Cliente %s", cliente[n - 1].nome);
            printf("Conta %d\n", cliente[n - 1].conta);
            // finalizar a transferencia e gerar o comprovante
            pedirSenha4();
            system("cls");
            printf("\nTransferencia realizada com sucesso.\n");
            printf("\n -- Comprovante --\n");
            printf("Enviada para %s", cliente[n - 1].nome);
            printf("Numero da conta %d\n", cliente[n - 1].conta);
            printf("No valor de R$%.2f reais\n", valort);
            printf("Obrigado por usar o ETI.");
            cliente[i_atual].saldo = cliente[i_atual].saldo - valort;
            pausar();
            menu_ted();
        }
        else
        {
            // caso a pessoa que vai receber não tenha conta no banco
            system("cls");
            printf("Essa pessoa nao tem conta no ETI\n");
            // while aqui
            printf("Para fazer a transferencia:\n");
            setbuf(stdin, NULL);
            printf("Informe o nome do cliente:\n");
            fgets(nome3, 30, stdin);
            setbuf(stdin, NULL);
            printf("Informe o numero da conta:\n");
            fgets(conta, 6, stdin);
            printf("\nDados do cliente:\n");
            for (i = 1; i < 2; i++)
            {
                printf("%s", nome3);
                printf("%s\n", conta);
            }

            pedirSenha4();
            system("cls");
            // colocar um while aqui para fazer com que a pessoa digite a senha até ele acertar
            printf("\nTransferencia realizada com sucesso.\n");
            printf("\n -- Comprovante --\n");
            printf("Enviada para %s", nome3);
            printf("Numero da conta %s", conta);
            printf("No valor de R$%.2f reais\n", valort);
            printf("Obrigado por usar o ETI.");
            cliente[i_atual].saldo = cliente[i_atual].saldo - (valort + (valort * 0.025));
        }
    }
    pausar();
    menu_ted();

    return;
}

void informacoes()
{
    printf("Entre contas do banco nao sera cobrado nenhum valor.\n");
    printf("Transferencia para outros bancos sera cobrado a taxa de 0.0025\n");
    printf("De 30 a 60 minutos para cair o valor da transferencia\n");

    pausar();
    menu_ted();
    return;
}

void deposito()
{
    float depositoo = 0;
    int senha;

    printf("\nInforme o valor que deseja depositar: ");
    scanf("%f", &depositoo);

    printf("Informe a senha de 4 Digitos: ");
    scanf("%d", &senha);

    if (senha == cliente[i_atual].senha4)
    {
        cliente[i_atual].saldo += depositoo;
        printf("Deposito concluido com sucesso!\n");
    }
    else
    {
        printf("Senha incorreta, nao foi possivel concluir o deposito!\n");
        pausar();
        menu();
    }

    pausar();
    menu();
    return;
}

void investimento()
{
    int escolhaInvestimento;
    char escolhaResgate[16];
    float valor_investido;
    system("cls");
    backup();
    printf("Bem vindo ao IETi %s\n", nomeMaiusculo);
    printf("Aqui voce faz investimentos em ETC como seguranca\n\n");
    if (mostrarRenda == true)
    { // se o mostrar renda for true vai mostrar a renda
        printf("Renda variavel: %.2f ETC\n", cliente[i_atual].investimentoVar);
        printf("Renda fixa: %.2f ETC\n", cliente[i_atual].investimentoFix);
    }
    printf("--------------------------------------------------\n");
    printf("> 1. Investir em renda Variavel\n");
    printf("> 2. Investir em renda Fixa\n");
    printf("> 3. Resgatar dinheiro\n");
    if (mostrarRenda == true)
    { // se for true vai mostrar o saldo e colocar opcção para ocultar
        printf("> 4. Ocultar saldo\n");
    }
    else
    {
        printf("> 4. Mostrar saldo\n");
    }
    printf("> 5. Atualizar\n");
    printf("> 6. Voltar ao menu\n> ");
    scanf("%d", &escolhaInvestimento);
    switch (escolhaInvestimento)
    {
    case 1:
        printf("\nQual o valor do investimento?\n");
        scanf("%f", &valor_investido);
        if (valor_investido > cliente[i_atual].saldo)
        { // se o valor investido for maior que o disponivel n funciona
            printf("\nValor maior de resgate maior do que disponivel em saldo\nSaldo: %.2f\n", cliente[i_atual].saldo);
            pausar();
            investimento();
        }

        printf("\nQuanto maior o valor da taxa, maior o risco\n");
        printf("Se voce ja estiver investindo, o valor da taxa e atualizado\n");
        printf("Taxa (%): ");
        scanf("%f", &taxa);
        if (taxa <= 0)
        { // se o valor da taxa for menor que 0 é inválido
            printf("\nValor de taxa invalida\n");
            pausar();
            investimento();
        }

        pedirSenha4();

        if (taxa < 30)
        { // se a taxa for menor que 20%, o risco vai ser de 30 de qualquer jeito
            risco = 30;
        }
        else if (taxa > 90)
        { // se a taxa for maior que 90%, o risco vai ser de 90 de qualquer jeito
            risco = 90;
        }
        else
        {
            risco = taxa;
        }

        cliente[i_atual].saldo -= valor_investido;
        cliente[i_atual].investimentoVar += valor_investido;

        cicloRendaVar = 0; // Como está investindo denovo, a contagem para diminuição de impostos é zerada

        flagVariavel = true;
        // pthread_create(&newthread1, NULL, variavel, NULL); // chama a pthread fixa
        break;
    case 2:
        printf("\nQual o valor do investimento?\n");
        scanf("%f", &valor_investido);
        if (valor_investido > cliente[i_atual].saldo)
        { // se o valor investido for maior que o disponivel n funciona
            printf("\nValor maior de resgate maior do que disponivel em saldo\nSaldo: %.2f\n", cliente[i_atual].saldo);
            pausar();
            investimento();
        }

        pedirSenha4();

        cliente[i_atual].saldo -= valor_investido;
        cliente[i_atual].investimentoFix += valor_investido; // adicionar o valor na variavel investimentoFix
        flagFixa = true;                                     // tornar a flag true
        // pthread_create(&newthread2, NULL, fixa, NULL);       // chama a pthread fixa
        break;
    case 3:
        // resgatarDinheiro();

        pedirSenha4();

        flagFixa = false; // parar o investimento para o resgate
        flagVariavel = false;
        setbuf(stdin, NULL); // limpar para o fgets funcionar
        printf("\nDeseja resgatar da renda variavel ou fixa?\n");
        fgets(escolhaResgate, 16, stdin);
        for (int i = 0; escolhaResgate[i] != '\0'; i++)
        {                                                   // para passar por todos os char da string
            escolhaResgate[i] = tolower(escolhaResgate[i]); // deixar tudo minúsculo
        }

        if (strncmp(escolhaResgate, "variavel", 3) == 0) // se pelo menos as 3 primeiras letras fore var vai vir aqui
        {
            printf("Renda variavel: %2f ETC\n", cliente[i_atual].investimentoVar);
            cliente[i_atual].investimentoVar = resgatarDinheiro(cliente[i_atual].investimentoVar, escolhaResgate[0]); // chama a funcao e armazena o return no valor do investimento
        }
        else if (strncmp(escolhaResgate, "fixa", 3) == 0)
        {
            printf("Renda fixa: %2f ETC\n", cliente[i_atual].investimentoFix);
            cliente[i_atual].investimentoFix = resgatarDinheiro(cliente[i_atual].investimentoFix, escolhaResgate[0]);
        }
        else
        {
            printf("\n\nDigite variavel ou fixa.\n");
            pausar();
        }

        if (cliente[i_atual].investimentoFix > 0)
        {
            flagFixa = true;
            // pthread_create(&newthread2, NULL, fixa, NULL); // se o valor investido ainda for maior que 0, ele continua
        }
        if (cliente[i_atual].investimentoVar > 0)
        {
            flagVariavel = true;
            // pthread_create(&newthread1, NULL, variavel, NULL); // continuar o investimento se ele for maior que 0
        }
        else if (cliente[i_atual].investimentoVar == 0)
        { // se o investimento estiver zerado, seu ciclo é reiniciado
            cicloRendaVar = 0;
        }

        investimento();
        break;
    case 4:
        // trocar a visualização
        if (mostrarRenda == false)
        {
            mostrarRenda = true;
        }
        else
        {
            mostrarRenda = false;
        }
        investimento();
        break;
    case 5:
        investimento();
        break;
    case 6:
        menu();
        break;
    case 25:
        bicho();
        break;
    default:
        printf("Opcao Invalida\n");
        pausar();
    }
    investimento();
}

/*void *variavel(void *arg)
{
    int aleatorio;
    srand(time(NULL));
    while (1)
    {
        sleep(TEMPO); // tempo entre as ações
        if (flagVariavel == false)
        {
            pthread_exit(NULL);
        } // se a flag for false ela para a pthread
        sleep(TEMPO);
        aleatorio = 1 + rand() % 100;
        if (aleatorio > risco)
        { // se o numero aleatório for maior que o risco o valor é investido, caso o contrário perde o valor
            cliente[i_atual].investimentoVar += (cliente[i_atual].investimentoVar * (taxa / 100.0));
        }
        else
        {
            cliente[i_atual].investimentoVar -= (cliente[i_atual].investimentoVar * (taxa / 100.0));
        }
        /*Ex:
        Se a taxa for 30, tem 70% do numero aleatorio for maior que o risco
        Se a taxa for 60, tem 40% do numero aleatorio for maior que o risco


        if (cliente[i_atual].investimentoVar < 0.001)
        { // se o valor for 0.001, a função para
            cliente[i_atual].investimentoVar == 0;
            printf("\n\n--- Voce perdeu tudo no investimento ---\n\n");
            cicloRendaVar = 0; // se o investimento estiver zerado, seu ciclo é reiniciado
            pthread_exit(NULL);
        }
        cicloRendaVar++;
    }

    return NULL;
}

void *fixa(void *arg)
{
    while (1)
    {
        sleep(TEMPO); // tempo entre as ações
        if (flagFixa == false)
        {
            pthread_exit(NULL);
        } // se a flag for false ela para a pthread
        cliente[i_atual].investimentoFix += cliente[i_atual].investimentoFix * (TAXA_FIXA / 100);
    }

    return NULL;
}*/

float resgatarDinheiro(float investimentoMisto, char tipo)
{
    float resgate, imposto;
    printf("\n\nValor do resgate: ");
    scanf("%f", &resgate);
    if (resgate > investimentoMisto)
    { // se o resgate for maior que o disponivel n funciona
        printf("\nValor maior de resgate maior do que disponivel\n");
        pausar();
        investimento();
    }
    investimentoMisto -= resgate; // o investimento que pode ser variavel e  fixo é diminuido do valor do resgate

    if (cicloRendaVar <= 17 && tipo == 'v')
    { // se o investimento é do tipo variavel, e se ciclo for menor ou igual a 17,  gerando assim o imposto, se o ciclo for 0 vai ser de 20%, 1 19%...17 3% a depois 2% fixo
        imposto = resgate * ((20 - cicloRendaVar) / 100.0);
    }
    else
    { // se o ciclo for 18 ou mais, os impostos vão ser fixos de 2%
        imposto = resgate * 0.02;
    }
    if (tipo == 'f')
    {
        imposto = resgate * 0.07;
    }
    resgate -= imposto;
    cliente[i_atual].saldo += resgate; // o saldo é acrescido do valor do resgate

    printf("\nTranferencia realizada com sucesso!\n"); // FAZER UMA "NOTA FICAL" E QUEM SABE ENVIAR UM EMAIL
    printf("Nota fiscal:\n\n");
    printf("n 12345678 -  Resgate\n");
    printf("*********************\n\n");
    printf("     Data: 06/11/2021\n");
    printf("       Horario: 13:13\n\n");
    printf("Imposto: %.2f ETC\n", imposto);
    printf("*********************\n");
    printf("   Valor: %.2f ETC   \n", resgate);

    pausar();
    return investimentoMisto; // retornar o tipo que pode ser variavel ou fixo
}

void bicho()
{
    int escolhaBicho, tipoAposta, numeros_sorteados[5], num, contador_acerto;
    float aposta;
    bool validacaoAnimal[3];
    char animal[3][16];
    char animais[25][16] = {"avestruz", "aguia", "burro", "borboleta", "cachorro", "cabra", "carneiro", "camelo", "cobra", "coelho", "cavalo", "elefante", "galo", "gato", "jacare", "leao", "macaco", "porco", "pavao", "peru", "touro", "tigre", "urso", "veado", "vaca"};
    // adicionei todos os aniamis e infelizmente os acentos não funcionam
    system("cls");
    printf("Qual aposta quer fazer?\n\n");
    printf("> 1. Grupo\n");
    printf("> 2. Duque de grupo\n");
    printf("> 3. Terno de grupo\n");
    printf("> 4. Voltar ao menu\n> ");
    scanf("%d", &escolhaBicho);
    switch (escolhaBicho)
    {
    case 1:
        // perguntar o tipo de aposta e pergunta apenas um animal
        printf("\n> 1. Apostar na cabeca\n");
        printf("> 2. Apostar do 1 ao 5\n> ");
        scanf("%d", &tipoAposta);
        printf("\nQual animal quer apostar?\n");
        printf("> ");
        scanf("%s", &animal[0]);
        break;
    case 2:
    case 3:
        // vai definir o tipo de aposta como 2 (1 ao 5), e perguntar a quantidade de animais referente a escolha ser 2 e 3
        tipoAposta = 2;
        printf("\nQuais os %d animais que quer apostar?\n", escolhaBicho);
        for (int i = 0; i < escolhaBicho; i++)
        {
            printf("> ");
            scanf("%s", &animal[i]);
        }
        break;
    case 4:
        // voltar ao menu
        menu();
        break;
    default:
        printf("Opcao Invalida\n");
        pausar();
        bicho();
    }

    // deixar todas a letras minusculas, vai passar por todas a letras do primeiro animal e assim em diante
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            animal[i][j] = tolower(animal[i][j]);
        }
    }

    for (int i = 0; i < 3; i++)
    { // definir todas as validações com false
        validacaoAnimal[i] = false;
    }
    for (int i = 0; i < 3; i++)
    { // comparar o animal[0] digitado com todos os 25, e assim por diante
        for (int j = 0; j < 25; j++)
        {
            if (strncmp(animal[i], animais[j], 3) == 0)
            { // se achar o animal[0] com algum vai entrar, depois o [1] com outro vai entrar e o [2]
                switch (escolhaBicho)
                {
                case 1: // se a escolha bicho for 1, vai ter apenas 1 animal, então seria apenas um validação animal true, então com esse for eu coloco todos como true
                    for (int i = 0; i < 3; i++)
                    {
                        validacaoAnimal[i] = true;
                    }
                    break;
                case 2: // para colocar todos como true se for certo, se o animal[1] for certo ent como true o [2] tbm, assim garantido todos com true
                    validacaoAnimal[i] = true;
                    i == 1 ? validacaoAnimal[2] = true : false;
                case 3: // simples, todos tem que passar para ficar true, como são 2 animais
                    validacaoAnimal[i] = true;
                }
            }
        }
    }
    if (validacaoAnimal[0] == false || validacaoAnimal[1] == false || validacaoAnimal[2] == false)
    {
        printf("\nNome(s) invalido!\nTente novamente\n");
        pausar();
        bicho();
    } // se algum for false, com o operador OU ||, ele torna tudo verdadeiro e entra, já se for verdade vai ser falso o if e não entra

    do
    { // o do..while garante que os valores sejam maiores que 0
        printf("\nQuantidade que deseja apostar: ");
        scanf("%f", &aposta);
    } while (aposta < 0);
    if (aposta > cliente[i_atual].saldo)
    {
        printf("Valor apostado maior que o saldo\nSaldo: %.2f ETC\n", cliente[i_atual].saldo);
        pausar();
        bicho();
    } // se o saldo do cliente atual for menor que o valor da aposta entra no if

    pedirSenha4();

    // define os numeros sorteados, 5 colocando no vetor
    printf("\nNumeros sorteados:\n");
    srand(time(NULL));
    for (int i = 0; i < 5; i++)
    {
        numeros_sorteados[i] = 1000 + rand() % (9999 - 1000);
        ;
        printf("%d. %d\n", i + 1, numeros_sorteados[i]);
    }

    switch (tipoAposta)
    {
    case 1:
        // se a aposta for na cabeça
        contador_acerto = 0;
        for (int i = 0; i < 25; i++)
        {
            num = (i + 1) * 4;
            if (numeros_sorteados[0] % 100 >= (num - 3) && numeros_sorteados[0] % 100 <= (num % 100) && strcmp(animal[0], animais[i]) == 0)
            {
                contador_acerto++;
            }
        }
        /*Vai passar por todos os animais no vetor.
        Cada animal recebe 4 números em ordem que compara com as 2 ultimas casas do numero sorteado
        01,02,03,04 - 05,06,07,08 .... 97,98,99,00.
        assim pega o resto da divisão de 100 pelo numero sorteado, e vê se é menor que o (index  +1) * 4 e maior que esse calculo - 3
        para ver se o numero é maior pega o resto da divisão por 100, para o 100 virar 00
        Ex:
        indice = 0 - num = (0 + 1) *4 - Intervalo: 4-3= 1 && 4
        indice = 1 - num = (1 + 1) *4 - Intervalo: 8-3= 5 && 8
        ...
        indice = 24 - num = (24 + 1) *4 - Intervalo: 100-3= 97 && 100 (100%100 = 00)
        E também o animal 0 tem quer ser igual ao indice que representa os animais no vetores*/
        if (contador_acerto == 1)
        {                                            // se a condição acima for realizada, vai incrementar na contadora e se for 1 mostrar se ganhou ou n
            cliente[i_atual].saldo += (aposta * 18); // vai adicionar ao saldo o valor da aposta * 18
            printf("\nVoce ganhou %.2f ETC apostando no(a) %s\n", aposta * 18, animal[0]);
        }
        else
        {
            cliente[i_atual].saldo -= aposta; // o saldo vai diminuir do valor da aposta
            printf("\nVoce perdeu, mais sorte na proxima\n");
        }
        break;
    case 2:
        // se a aposta for do 1 ao 5
        contador_acerto = 0;
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 25; k++)
                {
                    num = (k + 1) * 4;
                    if (numeros_sorteados[i] % 100 >= (num - 3) && numeros_sorteados[i] % 100 <= (num % 100) && strcmp(animal[j], animais[k]) == 0)
                    {
                        contador_acerto++;
                    }
                }
            }
        }
        /*mesma ideia do caso 1, mas primeiro passa pelo primeiro numero sorteado e animal escolhido pelo todos os animais no vetor
        apos conferir os primeiros, passa pelos 3 animais escolhidos e depois por todos os numeros sorteados*/
        if (contador_acerto == 1 && escolhaBicho == 1)
        { // se o contador tiver apenas 1 e na escolha tiver escolhido para apostar em 1 bixo
            cliente[i_atual].saldo += (aposta * 3.6);
            printf("\nVoce ganhou %.2f ETC apostando no(a) %s\n", aposta * 3.6, animal[0]);
        }
        else if (contador_acerto == 2 && escolhaBicho == 2)
        { // se o contador tiver apenas 2 e na escolha tiver escolhido para apostar em 2 bixo
            cliente[i_atual].saldo += (aposta * 18.5);
            printf("\nVoce ganhou %.2f ETC apostando no(a) %s e %s\n", aposta * 18.5, animal[0], animal[1]);
        }
        else if (contador_acerto == 3 && escolhaBicho == 3)
        { // se o contador tiver apenas 3 e na escolha tiver escolhido para apostar em 3 bixo
            cliente[i_atual].saldo += (aposta * 130);
            printf("\nVoce ganhou %.2f ETC apostando no(a) %s, %s e %s\n", aposta * 130, animal[0], animal[1]);
        }
        else
        {
            cliente[i_atual].saldo -= aposta;
            printf("\nVoce perdeu, mais sorte na proxima\n");
        }
        break;
    default:
        printf("Aposta Invalida\nTente remover os acentos\n");
        pausar();
        bicho();
    }

    pausar();
    investimento();
}

void configuracao()
{
    int alterar;
    printf("Alterar dados!\n");
    printf(" 1-Nome\n 2-Email\n 3-cpf\n 4-Senha\n");
    scanf("%d", &alterar);
    switch (alterar)
    {
    case 1:
        printf("Digite novo nome:");
        setbuf(stdin, NULL); // limpar para o fgets funcionar
        fgets(cliente[i_atual].nome, 41, stdin);
        printf("Nome alterado com sucesso...\n");

        for (int i = 0; cliente[i_atual].nome[i] != '\0'; i++)
        {
            cliente[i_atual].nome[i] = tolower(cliente[i_atual].nome[i]);
        } // para deixar as  letras minúsculas
        break;
    case 2:
        printf("Digite o novo Email:");
        scanf(" %s", cliente[i_atual].email);
        printf("Email cadastrado com sucesso...");
        break;
    case 3:
        printf("Digite o novo cpf:");
        scanf(" %s", cliente[i_atual].cpf);
        printf("CPF cadastrado com sucesso...\n");
        break;
    case 4:
        // Senha antiga
        printf("Digite sua senha atual:");
        scanf("%s", cliente[i_atual].senha);
        if (cliente[i_atual].senha)
        {
            // Senha Nova
            printf("Digite uma nova senha:");
            scanf(" %s", cliente[i_atual].senha);
            printf("Senha alterada com sucesso...\n");
        }
        else
        {
            printf("Nova senha nao pode ser igual a antetior!\n");
        }
        break;
    default:
        printf("Opcao invalida....\n");
        break;
    }
    backup();
    menu();
    return;
}
