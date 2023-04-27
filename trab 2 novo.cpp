#include <iostream>
#include <string>
#include <time.h>
using namespace std;

/*
Deve ser implementado um pequeno sistema que fará a gestão de uma pequena biblioteca de livros e revistas.
Esse sistema será executado apenas em memória principal,portanto não há necessidade de salvar os materiais
cadastrados para uso futuro. A implementação dos recursos do sistema como livros, revistas, usuários, autores e
outros deve acontecer, necessariamente com o uso de structs (estruturas) da linguagem C/C++ e
devem ficar armazenados em vetores desse tipo de dado.
Os recursos (obrigatórios) necessários a serem oferecidos ao usuário, via um menu de
opções, são os seguintes:
    • Cadastro de livros e revistas, com possiblidade de alteração (não precisa remover);
    • Cada livro pode ter apenas um autor;
    • Pesquisar qualquer item armazenado por título, autor, editora ou assunto;
    • Deve haver um cadastro de autores (dos livros) e editoras (livros e revistas), com a possibilidade de incluir e alterar;
    • Deve haver um cadastro de usuário que podem retirar livros para devolução após 7 dias;
    • Listagens necessárias (relatórios em tela):
        o Livros/Revistas de um autor ou editora
        o Livros/Revistas de um assunto
        o Livros/Revistas em atraso de devolução
        o Histórico de retiradas de um usuário
*/

struct Data {
    int dia;
    int mes;
    int ano;
};

struct Autor {
    string nome;
    string sobrenome;
};

struct Editora {
    string nome;
    string local;
};

struct Usuario {
    string nome;
    bool atraso;
    //relacionar usuario e livro/revista
};

struct Livro {
    string titulo;
    Autor autor;
    Editora editora;
    int publicacao;
    string assunto;
    int identificador;
    bool disponivel = true;
    Data retirada;
};

Data ler_data() {
    Data d;
    cout << "\tDia: ";
    cin >> d.dia;
    cout << "\tMes: ";
    cin >> d.mes;
    cout << "\tAno: ";
    cin >> d.ano;
    return d;
}

Autor ler_autor() {
    Autor autor;
    cout << "Nome do Autor: ";
    getline(cin, autor.nome);

    cout << "Sobrenome do Autor: ";
    getline(cin, autor.sobrenome);

    return autor;
}

void mostrar_autor(Autor a) {
    cout << a.nome << " " << a.sobrenome << endl;
}

//gera cadastros iniciais de autores
void gerar_autores(Autor lista_autores[]) {
    const int tam = 5;
    string nomes[] = { "Maria", "Ana Carolina", "Davi", "Fernando", "Renato", "Gloria", "Erico" };
    string sobrenomes[] = { "Moraes", "da Silva", "Pereira", "dos Santos", "Cruz", "Souza", "Lima" };

    srand(time(NULL));
    for (int i = 0; i < tam; i++) {
        int aux[tam] = { 0 };

        int a = rand() % 7;
        if (aux[i] == 0) {
            lista_autores[i].nome = nomes[a];
            aux[i] = a;
        }
        else {
            i--;
        }
    }

    srand(time(NULL));
    for (int i = 0; i < tam; i++) {
        int aux[tam] = { 0 };
        if (aux[i] == 0) {
            int b = rand() % 7;
            lista_autores[i].sobrenome = sobrenomes[b];
        }
        else {
            i--;
        }
    }
}

Editora ler_editora() {
    Editora editora;
    cout << "Nome da Editora: ";
    getline(cin, editora.nome);

    cout << "Local da Editora: ";
    getline(cin, editora.local);

    return editora;
}

void mostrar_editora(Editora ed) {
    cout << ed.nome << ", " << ed.local << endl;
}

//gera cadastros iniciais de editoras
void gerar_editoras(Editora lista_editoras[]) {
    const int tam = 9;
    string nomes[] = { "Editora Plano", "Liberdade", "Recanto Editora", "Editora Arcanjo", "Hiperlivros", "Cometa", "Monumental", "Ponto&Virgula", "Sabedoria" };
    string locais[] = { "Brasilia", "Porto Alegre", "Sao Paulo", "Rio de Janeiro", "Florianopolis", "Salvador", "Curitiba", "Santos", "Itajai" };

    srand(time(NULL));
    for (int i = 0; i < tam; i++) {
        int aux[tam] = { 0 };

        int a = rand() % tam;
        if (aux[i] == 0) {
            lista_editoras[i].nome = nomes[a];
            aux[i] = a;
        }
        else {
            i--;
        }
    }

    srand(time(NULL));
    for (int i = 0; i < tam; i++) {
        int aux[tam] = { 0 };
        if (aux[i] == 0) {
            int b = rand() % tam;
            lista_editoras[i].local = locais[b];
        }
        else {
            i--;
        }
    }
}

//gera cadastros iniciais de assuntos
void gerar_assuntos(string lista_assuntos[]) {
    const int tam = 16;
    string assuntos[] = { "Comedia", "Terror", "Romance", "Drama", "Ficcao Cientifica", "Suspense", "Horror", "Romance de Epoca", "Historia Real", "Historia", "Comedia Romantica", "Autoajuda", "Terror Psicologico", "Fantasia", "Religiao", "Poesia" };

    srand(time(NULL));
    for (int i = 0; i < tam; i++) {
        int aux[tam] = { 0 };

        int a = rand() % tam;
        if (aux[i] == 0) {
            lista_assuntos[i] = assuntos[a];
            aux[i] = a;
        }
        else {
            i--;
        }
    }
}

//gera cadastros iniciais de titulos
void gerar_titulos(string lista_titulos[]) {
    const int tam = 5, tam2 = 10;
    string titulos[] = { "Tudo ou Nada", "Amanhã é outro dia", "Felizes para Sempre", "A Ultima Batalha", "Guerra dos Reis", "A Voz da Natureza", "Canto da Tempestade", "Maldicao Azul", "Harmonia em Vermelho", "Deuses em Caos" };

    srand(time(NULL));
    for (int i = 0; i < tam; i++) {
        int aux[tam] = { 0 };

        int a = rand() % tam2;
        if (aux[i] == 0) {
            lista_titulos[i] = titulos[a];
            aux[i] = a;
        }
        else {
            i--;
        }
    }
}

void gerar_livros(Livro& a, Editora b, Autor c, string assunto, string titulo) {
    a.titulo = titulo;
    a.autor = c;
    a.editora = b;

    srand(time(NULL));
    int aux = rand() % 150;
    a.publicacao = 2023 - aux; //sorteia ano de publicação aleatorio

    a.assunto = assunto;
    //a.identificador;
}

void mostrar_livro(Livro a) {
    cout << "\nDados do livro:" << endl;
    cout << "\tTitulo: " << a.titulo << endl;

    cout << "\tAutor: ";
    mostrar_autor(a.autor);
    
    cout << "\tEditora: ";
    mostrar_editora(a.editora);

    cout << "\tAno de Publicacao: " << a.publicacao << endl;
    cout << "\tAssunto: " << a.assunto << endl;
    if (a.disponivel == true) {
        cout << "\tDisponivel\n";
    }
    else {
        cout << "\tIndisponivel\n";
    }
    //id livro
}

Livro cadastrar_livro() {
    Livro a;
    cout << "\nPreencha os dados abaixo: " << endl;

    cin.ignore();
    cout << "\nTitulo: ";
    getline(cin, a.titulo);

    a.autor = ler_autor();
    a.editora = ler_editora();

    cout << "Ano de publicacao: ";
    cin >> a.publicacao;

    cin.ignore();
    cout << "Assunto: ";
    getline(cin, a.assunto);

    //add identificador
    return a;
}

int main()
{
    Autor lista_autores[100];
    Editora lista_editoras[100];
    string lista_assuntos[100];
    string lista_titulos[100];
    Usuario usuarios[100];
    Livro lista_livros[100];
    //Revista lista_revistas[100];
    
    int escolha, cont_livros = 4, cont_autor = 4, cont_editora = 8;
    char resposta;
    bool flag = false;
    Autor autor;
    Editora ed;
    string aux;

    gerar_autores(lista_autores); //cadastro de 7 autores aleatorios
    gerar_editoras(lista_editoras); //cadastro de 9 editoras aleatorios
    gerar_assuntos(lista_assuntos); //cadastro de 15 assuntos aleatorios
    gerar_titulos(lista_titulos); //cadastro de 10 assuntos aleatorios

    for (int i = 0; i < 5; i++) { //cadastro de 5 livros aleatorios
        gerar_livros(lista_livros[i], lista_editoras[i], lista_autores[i], lista_assuntos[i], lista_titulos[i]);
    }

    /* REVISTA
    for (int i = 0; i < 4; i++) { //cadastro de 4 revistas aleatorias
        gerar_livros(lista_livros[i], lista_editoras[i+5]);
    }
    */
    while (true) {
        //menu principal da biblioteca
        cout << "\n - - - - - - - - BIBLIOTECA - - - - - - - -" << endl
            << "[1] - Cadastro" << endl
            << "[2] - Alteracao" << endl
            << "[3] - Retirada" << endl
            << "[4] - Consultar Acervo" << endl
            << "[5] - Central do Usuario" << endl
            << "[0] - Sair" << endl;
        cin >> escolha;
        system("cls");

        switch (escolha) {
        case 0: //fecha o programa
            system("cls");
            return 0;
            break;

        case 1: //cadastro de material
            cout << "\n - - - - - CADASTRO - - - - -" << endl
                << "[1] - Livro" << endl
                << "[2] - Revista" << endl
                << "[3] - Autor" << endl
                << "[4] - Editora" << endl
                << "[0] - Voltar" << endl;
            cin >> escolha;
            system("cls");

            switch (escolha) {
            case 0:
                system("cls");
                break;

            case 1: //cadastrar livro
                do {
                    cont_livros++;
                    lista_livros[cont_livros] = cadastrar_livro();
                    cout << "Deseja cadastrar mais um livro (s/n)? ";
                    cin >> resposta;
                } while (resposta == 's' || resposta == 'S');

                system("cls");
                break;

            case 3: //cadastrar autor
                do {
                    cout << "\nLista atual de autores cadastrados: " << endl;
                    for (int i = 0; i < cont_autor; i++) {
                        cout << "\t[" << i + 1 << "] ";
                        mostrar_autor(lista_autores[i]);
                        cout << endl;
                    }

                    cont_autor++;
                    cin.ignore();
                    lista_autores[cont_autor] = ler_autor();
                    cout << "\nAutor cadastrado com sucesso!" << endl;
                    cout << "\nDeseja cadastrar mais um autor (s/n)? ";
                    cin >> resposta;

                } while (resposta == 's' || resposta == 'S');

                system("cls");
                break;
            case 4: //cadastrar editora
                do { 
                    cout << "\nLista atual de editoras cadastrados: " << endl; // PAREI NESSA PARTE
                    for (int i = 0; i < cont_autor; i++) {
                        cout << "\t[" << i + 1 << "] ";
                        mostrar_autor(lista_autores[i]);
                        cout << endl;
                    }

                    cont_autor++;
                    cin.ignore();
                    lista_autores[cont_autor] = ler_autor();
                    cout << "\nAutor cadastrado com sucesso!" << endl;
                    cout << "\nDeseja cadastrar mais um autor (s/n)? ";
                    cin >> resposta;

                } while (resposta == 's' || resposta == 'S');

                system("cls");
                break;
            }
            break;
        case 2: //alteracao de material
            cout << "\n - - - - - ALTERACAO - - - - -" << endl
                << "[1] - Livro" << endl
                << "[2] - Revista" << endl
                << "[3] - Autor" << endl
                << "[4] - Editora" << endl
                << "[0] - Voltar" << endl;
            cin >> escolha;
            system("cls");

            switch (escolha) {
            case 0: //voltar para o menu
                system("cls");
                break;

            case 1://alterar livro
                cout << "\nLivros cadastrados atualmente (id e titulo): " << endl;
                for (int i = 0; i < cont_livros + 1; i++) {
                    cout << "\t[" << i + 1 << "] ";
                    cout << lista_livros[i].titulo; //ADICIONAR ID
                    cout << endl;
                }
                cout << "\nEscolha a opcao que deseja alterar: ";
                cin >> escolha;

                cin.ignore();
                lista_livros[escolha - 1] = cadastrar_livro();
                cout << "\nLivro alterado com sucesso!" << endl;

                //atualiza lista de autores e editoras tambem
                lista_autores[escolha - 1].nome = lista_livros[escolha - 1].autor.nome;
                lista_autores[escolha - 1].sobrenome = lista_livros[escolha - 1].autor.sobrenome;
                lista_editoras[escolha - 1].nome = lista_livros[escolha - 1].editora.nome;
                lista_editoras[escolha - 1].local = lista_livros[escolha - 1].editora.local;
                break;

            case 3: //alterar autor
                cout << "\nLista atual de autores cadastrados: " << endl;
                for (int i = 0; i < cont_autor + 1; i++) {
                    cout << "\t[" << i + 1 << "] ";
                    mostrar_autor(lista_autores[i]);
                }
                cout << "\nEscolha a opcao que deseja alterar: ";
                cin >> escolha;

                cin.ignore();
                lista_autores[escolha - 1] = ler_autor();
                cout << "\nAutor alterado com sucesso!" << endl; 

                //atualiza lista de livros tambem
                lista_livros[escolha - 1].autor.nome = lista_autores[escolha - 1].nome;
                lista_livros[escolha - 1].autor.sobrenome = lista_autores[escolha - 1].sobrenome;
                break;
            
            case 4: //alterar editora
                cout << "\nLista atual de editoras cadastradas: " << endl;
                for (int i = 0; i < cont_editora + 1; i++) {
                    cout << "\t[" << i + 1 << "] ";
                    mostrar_editora(lista_editoras[i]);
                }
                cout << "\nEscolha a opcao que deseja alterar: ";
                cin >> escolha;

                cin.ignore();
                lista_editoras[escolha - 1] = ler_editora();
                cout << "\nAEditora alterada com sucesso!" << endl;

                //atualiza lista de livros tambem
                lista_livros[escolha - 1].editora.nome = lista_editoras[escolha - 1].nome;
                lista_livros[escolha - 1].editora.local = lista_editoras[escolha - 1].local;
                break;
            }
            break;

        case 3: // retirada
            break;

        case 4: // consultar acervo
            cout << "\n - - - - - CONSULTAR ACERVO - - - - -" << endl
                << "[1] - Pesquisa Filtrada" << endl
                << "[2] - Listagem Completa" << endl
                << "[0] - Voltar" << endl;
            cin >> escolha;
            system("cls");

            if (escolha == 1) {
                cout << "\nDeseja filtrar sua pesquisa por: " << endl
                    << "[1] - Livros" << endl
                    << "[2] - Revistas" << endl
                    << "[3] - Autor" << endl
                    << "[4] - Editora" << endl
                    << "[5] - Assunto" << endl;
                cin >> escolha;
                system("cls");

                switch (escolha) {
                case 1: //mostrar todos os livros
                    for (int i = 0; i < cont_livros + 1; i++) {
                        mostrar_livro(lista_livros[i]);
                    }
                    break;
                    //case 2: //mostrar todas as revistas
                        //mostrar_revistas(lista_revistas[i]);
                       // break;
                case 3: //mostrar livros de um autor especifico
                    cout << "\nLista atual de autores cadastrados: " << endl;
                    for (int i = 0; i < cont_autor + 1; i++) {
                        cout << "\t[" << i + 1 << "] ";
                        mostrar_autor(lista_autores[i]);
                    }
                    cout << endl;
                    cin.ignore();
                    autor = ler_autor();
                    
                    flag = false;
                    for (int i = 0; i < cont_livros + 1; i++) {
                        if (lista_livros[i].autor.nome == autor.nome && lista_livros[i].autor.sobrenome == autor.sobrenome) {
                            mostrar_livro(lista_livros[i]);
                            flag = true;
                        }
                    }
                    if (flag == false) {
                        cout << "\nNao existem livros cadastrados com este autor" << endl;
                    }
                    break;

                case 4: //mostrar livros e revistas de uma editora
                    cout << "\nLista atual de editoras cadastradas: " << endl;
                    for (int i = 0; i < cont_editora + 1; i++) {
                        cout << "\t[" << i + 1 << "] ";
                        mostrar_editora(lista_editoras[i]);
                    }
                    cout << endl;
                    cin.ignore();
                    ed = ler_editora();
                    
                    flag = false;
                    for (int i = 0; i < cont_livros + 1; i++) {
                        if (lista_livros[i].editora.nome == ed.nome && lista_livros[i].editora.local == ed.local) {
                            mostrar_livro(lista_livros[i]);
                            flag = true;
                        }
                        //ADD PARTE DAS REVISTAS
                    }
                    if (flag == false) {
                        cout << "\nNao existem livros ou revistas cadastrados com esta Editora" << endl;
                    }
                    break;

                case 5: //mostrar livros e revistas de um assunto
                    cin.ignore();
                    cout << "Qual assunto deseja encontrar? "; //SO FUNCIONA QUANDO A FRASE SO TEM UMA PALAVRA
                    cin >> aux;
                    flag = false;
                    for (int i = 0; i < cont_livros + 1; i++) {
                        if (lista_livros[i].assunto == aux) {
                            mostrar_livro(lista_livros[i]);
                            flag = true;
                        }
                    }
                    if (flag == false) {
                        cout << "\nNao existem livros ou revistas cadastrados com este assunto" << endl;
                    }
                    break;
                }
            }
            else if (escolha == 2) { //lista de todos os livros e revistas
                cout << "\n - - - - - LISTAGEM COMPLETA - - - - -" << endl;
                for (int i = 0; i < cont_livros + 1; i++) {
                    mostrar_livro(lista_livros[i]);
                    //mostrar_revista(lista_revistas[i]);
                }
            }
            system("pause");
            system("cls");
            break;

        case 5: //informacoes relacionadas ao usuario
            cout << "\n - - - - - CENTRAL DO USUARIO - - - - -" << endl
                << "[1] - Cadastro" << endl
                << "[2] - Usuarios com atraso de devolucao" << endl
                << "[3] - Historico de retiradas" << endl
                << "[4] - Listagem de Usuarios" << endl
                << "[0] - Voltar" << endl;
            cin >> escolha;
            break;
        }
    }
    return 0;
}