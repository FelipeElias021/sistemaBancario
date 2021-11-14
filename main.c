#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#define TAXA_FIXA 1.5
#define TEMPO 60

/*Áreas: pix - depositar - Investimento - TED - Shopping - Configurações - Extrato - Sair*/

void login();
void menu();
void investimento();
float resgatarDinheiro(float investimentoMisto, char tipo);
void bicho();
void deposito();
void* fixa(void * arg);
void* variavel(void * arg);

int main()
{
    // apenas começa o código levando ao login
    login();
    return 0;
}

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

//Variaveis Globais - Felipe
struct dados_cliente cliente[10];
pthread_t newthread1;
pthread_t newthread2; //criando o pthread
float taxa, risco; // variaveis para o investimeto renda variavel
int i_atual = 0, nClientes = 0, cicloRendaVar = 0;
bool mostrarRenda = true, flagFixa, flagVariavel;
char nomeMaiusculo[41]; // se deixar a var nome main tudo minusculo é mais fácil de comparar para o login, ent essa é para ficar formatado

void login()
{
    int escolhaLogin;
    bool flagEmail, flagSenha, flagValidacao, flagCadastro;
    char validacaoUser[41], validacaoSenha[21];
    system("cls");
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
            system("PAUSE");
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
        if (cliente[i_atual].investimentoVar > 0) {
            flagVariavel = true;
            pthread_create(&newthread1, NULL, variavel, NULL); //continuar o investimento se ele for maior que 0
        }
        if (cliente[i_atual].investimentoFix > 0) {
            flagFixa = true;
            pthread_create(&newthread2, NULL, fixa, NULL); //continuar o investimento se ele for maior que 0
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
                system("PAUSE");
                flagCadastro = false;
            }
            if (strcmp(cliente[i - 1].email, cliente[nClientes].email) == 0)
            {
                printf("\nCadastro invalido");
                printf("\nEmail ja cadastrado\n\n");
                system("PAUSE");
                flagCadastro = false;
            }
            if (strcmp(cliente[i - 1].cpf, cliente[nClientes].cpf) == 0)
            {
                printf("\nCadastro invalido");
                printf("\nCPF ja cadastrado\n\n");
                system("PAUSE");
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
            printf("\nNome: %s\n", cliente[i].nome);
            printf("Email: %s\n", cliente[i].email);
            printf("Senha: %s\n", cliente[i].senha);
            printf("Cpf: %s\n", cliente[i].cpf);
            printf("Conta: %d\n", cliente[i].conta);
            printf("Senha 4 digitos: %d\n", cliente[i].senha4);
            printf("Saldo: %f\n", cliente[i].saldo);
            printf("Investimento Fixo: %f\n", cliente[i].investimentoFix);
            printf("Investimento Variavel: %f\n", cliente[i].investimentoVar);
        }
        system("PAUSE");
        login(); // leva de volta a função login, para se fazer o login
        break;
    case 3:
        // criar clientes para testes
        strcpy(cliente[nClientes].nome, "felipe\n");
        strcpy(cliente[nClientes].email, "felipeelias@gmail.com");
        strcpy(cliente[nClientes].senha, "Ff12345");
        strcpy(cliente[nClientes].cpf, "12312312344");
        nClientes++;
        strcpy(cliente[nClientes].nome, "paulo\n");
        strcpy(cliente[nClientes].email, "paulosergio@gmail.com");
        strcpy(cliente[nClientes].senha, "Pp12345");
        strcpy(cliente[nClientes].cpf, "12345678900");
        nClientes++;
        strcpy(cliente[nClientes].nome, "joao\n");
        strcpy(cliente[nClientes].email, "joaomanjabosco@gmail.com");
        strcpy(cliente[nClientes].senha, "Jj12345");
        strcpy(cliente[nClientes].cpf, "98765432100");
        nClientes++;
        for (int i = 0; i < 3; i++) {
            cliente[i].conta = 100000000 + rand() * 9999;
            cliente[i].senha4 = 1234;
            cliente[i].saldo = 0;
            cliente[i].investimentoFix = 0;
            cliente[i].investimentoVar = 0;
        }

        login();
        break;
    default:
        printf("Opcao Invalida\n");
        system("PAUSE");
        login();
    }
    return;
}

void menu()
{
    int escolhaMenu = 0;
    system("cls");
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
    printf("> 3. Depositar\n");
    printf("> 4. Investimento\n");
    printf("> 5. Sair\n> ");
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
        // pix();
        break;
    case 3:
        deposito();
        break;
    case 4:
        investimento();
        break;
    case 5:
        flagFixa = false; //quando for sair da conta o investimento para
        flagVariavel = false;
        login();
        break;
    default:
        printf("Opcao Invalida\n");
        system("PAUSE");
        menu();
    }
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
        system("PAUSE");
        menu();
    }

    system("PAUSE");
    menu();
    return;
}

void investimento()
{
    int escolhaInvestimento;
    char escolhaResgate[16];
    float valor_investido;
    system("cls");
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
        scanf("%f",&valor_investido);
        if (valor_investido > cliente[i_atual].saldo)
        { // se o valor investido for maior que o disponivel n funciona
            printf("\nValor maior de resgate maior do que disponivel em saldo\nSaldo: %.2f\n",cliente[i_atual].saldo);
            system("PAUSE");
            investimento();
        }

        printf("\nQuanto maior o valor da taxa, maior o risco\n");
        printf("Se voce ja estiver investindo, o valor da taxa e atualizado\n");
        printf("Taxa (%): ");
        scanf("%f",&taxa);
        if (taxa <= 0) { //se o valor da taxa for menor que 0 é inválido
            printf("\nValor de taxa invalida\n");
            system("PAUSE");
            investimento();
        }

        if (taxa < 30) { //se a taxa for menor que 20%, o risco vai ser de 30 de qualquer jeito
            risco = 30;
        } else if(taxa > 90) { //se a taxa for maior que 90%, o risco vai ser de 90 de qualquer jeito
            risco = 90;
        } else {
            risco = taxa;
        }

        cliente[i_atual].saldo -= valor_investido;
        cliente[i_atual].investimentoVar += valor_investido;

        cicloRendaVar = 0; //Como está investindo denovo, a contagem para diminuição de impostos é zerada

        flagVariavel = true;
        pthread_create(&newthread1, NULL, variavel, NULL); //chama a pthread fixa
        break;
    case 2:
        printf("\nQual o valor do investimento?\n");
        scanf("%f",&valor_investido);
        if (valor_investido > cliente[i_atual].saldo)
        { // se o valor investido for maior que o disponivel n funciona
            printf("\nValor maior de resgate maior do que disponivel em saldo\nSaldo: %.2f\n",cliente[i_atual].saldo);
            system("PAUSE");
            investimento();
        }
        cliente[i_atual].saldo -= valor_investido;
        cliente[i_atual].investimentoFix += valor_investido; //adicionar o valor na variavel investimentoFix
        flagFixa = true; //tornar a flag true
        pthread_create(&newthread2, NULL, fixa, NULL); //chama a pthread fixa
        break;
    case 3:
        // resgatarDinheiro();
        flagFixa = false; //parar o investimento para o resgate
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
            system("PAUSE");
        }
        
        if (cliente[i_atual].investimentoFix > 0) {
            flagFixa = true;
            pthread_create(&newthread2, NULL, fixa, NULL); //se o valor investido ainda for maior que 0, ele continua
        } 
        if (cliente[i_atual].investimentoVar > 0) {
            flagVariavel = true;
            pthread_create(&newthread1, NULL, variavel, NULL); //continuar o investimento se ele for maior que 0
        } else if (cliente[i_atual].investimentoVar == 0) { //se o investimento estiver zerado, seu ciclo é reiniciado
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
        system("PAUSE");
    }
    investimento();
}

void* variavel(void * arg) {
    int aleatorio;
    srand(time(NULL));
    while (1) {
        sleep(TEMPO); //tempo entre as ações
        if (flagVariavel == false) {
            pthread_exit(NULL);
        } //se a flag for false ela para a pthread
        sleep(TEMPO);
        aleatorio = 1 + rand() % 100;
        if (aleatorio > risco) { // se o numero aleatório for maior que o risco o valor é investido, caso o contrário perde o valor
            cliente[i_atual].investimentoVar += (cliente[i_atual].investimentoVar * (taxa/100.0)); 
        } else {
            cliente[i_atual].investimentoVar -= (cliente[i_atual].investimentoVar * (taxa/100.0));
        }
    /*Ex:
    Se a taxa for 30, tem 70% do numero aleatorio for maior que o risco 
    Se a taxa for 60, tem 40% do numero aleatorio for maior que o risco 
    */

        if (cliente[i_atual].investimentoVar < 0.001) { //se o valor for 0.001, a função para
            cliente[i_atual].investimentoVar == 0;
            printf("\n\n--- Voce perdeu tudo no investimento ---\n\n");
            cicloRendaVar = 0; //se o investimento estiver zerado, seu ciclo é reiniciado
            pthread_exit(NULL);
        }
        cicloRendaVar++;
    }

    return NULL;
}

void* fixa(void * arg) {
    while (1) {
        sleep(TEMPO); //tempo entre as ações
        if (flagFixa == false) {
            pthread_exit(NULL);
        } //se a flag for false ela para a pthread
        cliente[i_atual].investimentoFix += cliente[i_atual].investimentoFix * (TAXA_FIXA/100);

    }

    return NULL;
}

float resgatarDinheiro(float investimentoMisto, char tipo)
{
    float resgate, imposto;
    printf("\n\nValor do resgate: ");
    scanf("%f", &resgate);
    if (resgate > investimentoMisto)
    { // se o resgate for maior que o disponivel n funciona
        printf("\nValor maior de resgate maior do que disponivel\n");
        system("PAUSE");
        investimento();
    }
    investimentoMisto -= resgate; // o investimento que pode ser variavel e  fixo é diminuido do valor do resgate
    
    if (cicloRendaVar <= 17 && tipo == 'v') { // se o investimento é do tipo variavel, e se ciclo for menor ou igual a 17,  gerando assim o imposto, se o ciclo for 0 vai ser de 20%, 1 19%...17 3% a depois 2% fixo
        imposto = resgate * ((20 - cicloRendaVar)/100.0);
    } else { //se o ciclo for 18 ou mais, os impostos vão ser fixos de 2%
        imposto = resgate * 0.02;
    }
    if (tipo == 'f') {
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
    printf("Imposto: %.2f ETC\n",imposto);
    printf("*********************\n");
    printf("   Valor: %.2f ETC   \n", resgate);

    system("PAUSE");
    return investimentoMisto; // retornar o tipo que pode ser variavel ou fixo
}

void bicho()
{
    int escolhaBicho, tipoAposta, numeros_sorteados[5], num, contador_acerto;
    float aposta;
    bool validacaoAnimal[3];
    char animal[3][16] = {};
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
        system("PAUSE");
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
            if (strncmp(animal[i], animais[j],3) == 0)
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
        system("PAUSE");
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
        system("PAUSE");
        menu();
    } // se o saldo do cliente atual for menor que o valor da aposta entra no if

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
        system("PAUSE");
        bicho();
    }

    system("PAUSE");
    investimento();
}