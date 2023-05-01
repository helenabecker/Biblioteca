#include <iostream>
#include <string>
#include <time.h>
#include <ctime>
#include <Windows.h>

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

using namespace std;

const int MAX_RETIRADOS = 100;

#pragma region Structs

struct Data {
	int dia, mes, ano;
};

struct Autor {
	string nome, sobrenome;
};

struct Editora {
	string nome, local;
};

struct Retirado {
	int id = 0;
	Data retirada, devolucao;
	bool atraso = false;
};

struct Usuario {
	string nome;
	int id = 0;
	Retirado retirados[100];
};

struct Livro {
	string titulo, assunto;
	Autor autor;
	Editora editora;
	int publicacao, id = 0;
	bool disponivel = true;
	Data retirada;
};

struct Revista {
	string titulo, assunto;
	Editora editora;
	Data publicacao;
	int id = 0;
	bool disponivel = true;
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

Editora ler_editora() {
	Editora editora;
	cout << "Nome da Editora: ";
	getline(cin, editora.nome);

	cout << "Local da Editora: ";
	getline(cin, editora.local);

	return editora;
}

Usuario cadastrar_usuario(int id) {
	Usuario u;
	cout << "\nPreencha os dados abaixo: " << endl;

	cin.ignore();
	cout << "\nNome Completo: ";
	getline(cin, u.nome);

	u.id = id;
	cout << "Seu identificador de usuario: " << u.id;

	return u;
}

Livro cadastrar_livro() {
	Livro l;
	cout << "\nPreencha os dados abaixo: " << endl;

	cin.ignore();
	cout << "\nTitulo: ";
	getline(cin, l.titulo);

	l.autor = ler_autor();
	l.editora = ler_editora();

	cout << "Ano de publicacao: ";
	cin >> l.publicacao;

	while (l.publicacao < 0)
	{
		cout << "Digite um ano valido: ";
		cin >> l.publicacao;
	}

	cin.ignore();
	cout << "Assunto: ";
	getline(cin, l.assunto);

	return l;
}

Revista cadastrar_revista() {
	Revista r;
	cout << "\nPreencha os dados abaixo: " << endl;

	cin.ignore();
	cout << "\nTitulo: ";
	getline(cin, r.titulo);

	r.editora = ler_editora();

	do {
		cout << "Mes de publicacao: ";
		cin >> r.publicacao.mes;
	} while (r.publicacao.mes < 1 || r.publicacao.mes > 12);
	do {
		cout << "Ano de publicacao: ";
		cin >> r.publicacao.ano;
	} while (r.publicacao.ano < 0);

	cin.ignore();
	cout << "Assunto: ";
	getline(cin, r.assunto);

	return r;
}

#pragma endregion Structs

#pragma region Funcoes
//Usado para posicionar o cursor em uma determinada posição da tela
void set_cursor(int x = 0, int y = 0) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

//system("color 1"); também funciona vai de 1 a 15
void set_color(int color) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COLORREF cor;
	SetConsoleTextAttribute(handle, color);
}

//funcao para verificar se a escolha do usuario eh um numero valido
bool opcao_invalida(int opcao, int min_opcao, int max_opcao) {
	if (opcao < min_opcao || opcao > max_opcao) { //Define as opções mínimas e máximas de escolha para cada menu e submenu
		set_color(4);
		cout << "\nOpcao invalida. Tente novamente." << endl;
		Sleep(500);
		set_color(7);
		cout << endl;
		return true;
	}
	else {
		return false;
	}
}

//quantidade de itens retirados por um usuario
int qtd_retiradas(Usuario usuario) {
	int soma = 0;
	for (int j = 0; j < MAX_RETIRADOS; j++) {
		if (usuario.retirados[j].id != 0) { //verifica a quantidade de itens já retirados pelo usuario
			soma++;
		}
	}
	return soma;
}

//verifica se ha atraso 
bool atraso(Usuario usuario) {
	for (int j = 0; j < MAX_RETIRADOS; j++) {
		if (usuario.retirados[j].atraso) { //nao permite retirar mais livros enquanto tiver um com atraso de devolucao
			cout << "\nVoce esta com um livro em atraso" << endl
				<< "Nao podera realizar mais retiradas enquanto nao regularizar as devolucoes" << endl;
			return true;
		}
	}
	return false;
}

void mostrar_data(int dia, int mes, int ano) {
	cout << dia << "/" << mes << "/" << ano;
}

void mostrar_autor(Autor a) {
	cout << a.nome << " " << a.sobrenome << endl;
}

void mostrar_editora(Editora ed) {
	cout << ed.nome << ", " << ed.local << endl;
}

void mostrar_livro(Livro a) {
	cout << "\nDados do livro:" << endl

		<< "\tID do livro: " << a.id << endl

		<< "\tTitulo: " << a.titulo << endl

		<< "\tAutor: ";
	mostrar_autor(a.autor);

	cout << "\tEditora: ";
	mostrar_editora(a.editora);

	cout << "\tAno de Publicacao: " << a.publicacao << endl

		<< "\tAssunto: " << a.assunto << endl;

	if (a.disponivel == true)
		cout << "\tDisponivel\n";
	else
		cout << "\tIndisponivel\n";
}

void mostrar_revista(Revista r) {
	cout << "\nDados da Revista:" << endl

		<< "\tID da revista: " << r.id << endl

		<< "\tTitulo: " << r.titulo << endl

		<< "\tEditora: ";
	mostrar_editora(r.editora);

	cout << "\tData da Publicacao " << r.publicacao.mes << "/" << r.publicacao.ano << endl;

	cout << "\tAssunto: " << r.assunto << endl;

	if (r.disponivel == true)
		cout << "\tDisponivel\n";
	else
		cout << "\tIndisponivel\n";
}

void mostrar_usuario(Usuario u) {
	cout << "\nDados do Usuario: " << endl
		<< "\tID do usuario: " << u.id << endl
		<< "\tNome Completo:" << u.nome << endl;
}

void mostrar_retirado(Retirado a) {
	cout << "ID do item retirado: " << a.id << endl
		<< "Data de retirada: ";
	mostrar_data(a.retirada.dia, a.retirada.mes, a.retirada.ano);
	cout << endl;

	cout << "Data limite de devolucao: ";
	mostrar_data(a.devolucao.dia, a.devolucao.mes, a.devolucao.ano);
	cout << endl;
}

int gerar_ID(int num) {
	int id = 0;
	srand(time(NULL));
	if (num < 1100) {
		id = num + rand() % 900 + 1; //id pra livros
		return id;
	}
	if (num > 1100 && num < 2100) {
		id = num + rand() % 900 + 1; //id pra revistas
		return id;
	}
	if (num > 2100 && num < 3100) {
		id = num + rand() % 900 + 1; //id pra usuarios
		return id;
	}
}

//gera cadastros iniciais de titulos
void gerar_titulos(string lista_titulos[], string titulos_revistas[]) {
	const int tam = 9, tam2 = 10;
	string titulos[] = { "Tudo ou Nada", "Amanha eh outro dia", "Felizes para Sempre", "A Ultima Batalha", "Guerra dos Reis", "A Voz da Natureza", "Canto da Tempestade", "Maldicao Azul", "Harmonia em Vermelho", "Deuses em Caos" };

	int aux[tam] = { 0 }, a = 0;
	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		a = rand() % tam2;
		if (i == 0) {
			lista_titulos[i] = titulos[a];
			aux[i] = a;
		}
		else if (i > 0) {
			if (a != aux[i - 1]) {
				lista_titulos[i] = titulos[a];
				aux[i] = a;
			}
			else {
				i--;
			}
		}
	}

	const int tam_r = 4, tam2_r = 10;
	int aux2[tam_r] = { 0 }, b = 0;
	string titulos_r[] = { "Vogue", "ELLE", "V Magazine", "Capricho", "Caras", "Veja SP", "A Sentinela", "Despertai", "Crusoé Revista Digital Semanal", "Manequim" };

	srand(time(NULL));
	for (int i = 0; i < tam_r; i++) {
		b = rand() % tam2_r;
		if (i == 0) {
			titulos_revistas[i] = titulos_r[b];
			aux2[i] = b;
		}
		else if (i > 0) {
			if (b != aux2[i - 1]) {
				titulos_revistas[i] = titulos_r[b];
				aux2[i] = b;
			}
			else {
				i--;
			}
		}
	}
}

//gera cadastros iniciais de autores
void gerar_autores(Autor lista_autores[]) {
	const int tam = 5;
	string nomes[] = { "Maria", "Ana Carolina", "Davi", "Fernando", "Renato", "Gloria", "Erico", "Lais", "Juliano", "Rodrigo" };
	string sobrenomes[] = { "Moraes", "da Silva", "Pereira", "dos Santos", "Cruz", "Souza", "Lima", "Soares", "Zacarias", "Oliveira" };

	int aux[tam] = { 0 }, a = 0;
	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		a = rand() % 10;
		if (i == 0) {
			lista_autores[i].nome = nomes[a];
			aux[i] = a;
		}
		else if (i > 0) {
			if (a != aux[i - 1]) {
				lista_autores[i].nome = nomes[a];
				aux[i] = a;
			}
			else {
				i--;
			}
		}
	}

	int aux2[tam] = { 0 }, b = 0;
	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		b = rand() % 10;
		if (i == 0) {
			lista_autores[i].sobrenome = sobrenomes[b];
			aux2[i] = b;
		}
		else if (i > 0) {
			if (b != aux2[i - 1]) {
				lista_autores[i].sobrenome = sobrenomes[b];
				aux2[i] = b;
			}
			else {
				i--;
			}
		}
	}
}

//gera cadastros iniciais de editoras
void gerar_editoras(Editora lista_editoras[]) {
	const int tam = 9;
	string nomes[] = { "Editora Plano", "Liberdade", "Recanto Editora", "Editora Arcanjo", "Hiperlivros", "Cometa", "Monumental", "Ponto&Virgula", "Sabedoria" };
	string locais[] = { "Brasilia", "Porto Alegre", "Sao Paulo", "Rio de Janeiro", "Florianopolis", "Salvador", "Curitiba", "Santos", "Itajai" };

	int aux[tam] = { 0 }, a = 0, aux2[tam] = { 0 }, b = 0;
	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		a = rand() % tam;
		if (i == 0) {
			lista_editoras[i].nome = nomes[a];
			aux[i] = a;
		}
		else if (i > 0) {
			if (a != aux[i - 1]) {
				lista_editoras[i].nome = nomes[a];
				aux[i] = a;
			}
			else {
				i--;
			}
		}
	}

	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		b = rand() % tam;
		if (i == 0) {
			lista_editoras[i].local = locais[b];
			aux2[i] = b;
		}
		else if (i > 0) {
			if (b != aux2[i - 1]) {
				lista_editoras[i].local = locais[b];
				aux2[i] = b;
			}
			else {
				i--;
			}
		}
	}
}

//gera cadastros iniciais de assuntos
void gerar_assuntos(string lista_assuntos[]) {
	const int tam = 9;
	string assuntos[] = { "Comedia", "Terror", "Romance", "Drama", "Ficcao Cientifica", "Suspense", "Horror", "Romance de Epoca", "Historia Real", "Historia", "Comedia Romantica", "Autoajuda", "Terror Psicologico", "Fantasia", "Religiao", "Poesia" };

	int aux[tam] = { 0 }, a = 0;
	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		a = rand() % 16;
		if (i == 0) {
			lista_assuntos[i] = assuntos[a];
			aux[i] = a;
		}
		else if (i > 0) {
			if (a != aux[i - 1]) {
				lista_assuntos[i] = assuntos[a];
				aux[i] = a;
			}
			else {
				i--;
			}
		}
	}
}

//gera cadastros iniciais de anos de publicacao
void gerar_ano(int lista_ano[]) {
	int aux = 0;

	srand(time(NULL));
	for (int i = 0; i < 10; i++) {
		aux = rand() % 150;
		lista_ano[i] = 2023 - aux; //sorteia ano de publicação aleatorio
	}
}

//gera cadastros iniciais de mes de publicacao
void gerar_mes(int lista_mes[]) {
	int v[10] = { 0 }, aux = 0;

	srand(time(NULL));
	for (int i = 0; i < 10; i++) {
		aux = rand() % 12 + 1;
		if (i == 0) {
			lista_mes[i] = aux;
			v[i] = aux;
		}
		else if (i > 0) {
			if (aux != v[i - 1]) {
				lista_mes[i] = aux;
				v[i] = aux;
			}
			else {
				i--;
			}
		}
	}
}

// gera cadastros iniciais de livros
void gerar_livros(Livro& a, Editora b, Autor c, string assunto, string titulo, int ano, int id) {
	a.titulo = titulo;
	a.autor = c;
	a.editora = b;
	a.publicacao = ano;
	a.assunto = assunto;
	a.id = id;
}

void gerar_revistas(Revista& r, Editora b, string assunto, int ano, int mes, string titulo, int id) {
	r.titulo = titulo;
	r.editora = b;
	r.publicacao.mes = mes;
	r.publicacao.ano = ano;
	r.assunto = assunto;
	r.id = id;
}

void gerar_nomes(string lista_nomes[]) {
	const int tam = 3;
	string nomes[] = { "Pablo Jeremias", "Bernardo Junior", "Livia Luz", "Suzana Cruz", "Renata Silva", "Osvaldo Moreira", "Poliana Silva", "Julia Fernandes", "Cesar Neto" };

	int aux[tam] = { 0 }, a = 0;
	srand(time(NULL));
	for (int i = 0; i < tam; i++) {
		a = rand() % 9;
		if (i == 0) {
			lista_nomes[i] = nomes[a];
			aux[i] = a;
		}
		else if (i > 0) {
			if (a != aux[i - 1]) {
				lista_nomes[i] = nomes[a];
				aux[i] = a;
			}
			else {
				i--;
			}
		}
	}
}

void gerar_usuarios(Usuario& u, int id, string nome) {
	u.nome = nome;
	u.id = id;
}

// Função para verificar se um livro está disponível para empréstimo
bool livro_disponivel(Livro livro) {
	return !livro.disponivel;
}
#pragma endregion Funcoes

#pragma region Menu
void exibir_menu_principal() {
	set_color(3);
	cout << "\n - - - - - - - - BIBLIOTECA - - - - - - - - \n" << endl;
	set_color(7);
	cout << "[1] - Cadastro" << endl
		<< "[2] - Alteracao" << endl
		<< "[3] - Retirada" << endl
		<< "[4] - Consultar Acervo" << endl
		<< "[5] - Central do Usuario" << endl
		<< "[0] - Sair" << endl
		<< endl
		<< "Digite a opcao desejada: ";

}

void submenu_cadastro() {
	set_color(3);
	cout << "\n - - - - - CADASTRO - - - - -\n" << endl;
	set_color(7);
	cout << "[1] - Livro" << endl
		<< "[2] - Revista" << endl
		<< "[3] - Autor" << endl
		<< "[4] - Editora" << endl
		<< "[5] - Usuario" << endl
		<< "[0] - Voltar" << endl
		<< endl
		<< "Digite a opcao desejada: ";
}

void submenu_alteracao() {
	set_color(3);
	cout << "\n - - - - - ALTERACAO - - - - -\n" << endl;
	set_color(7);
	cout << "[1] - Livro" << endl
		<< "[2] - Revista" << endl
		<< "[3] - Autor" << endl
		<< "[4] - Editora" << endl
		<< "[0] - Voltar" << endl
		<< endl
		<< "Digite a opcao desejada: ";
}

void submenu_retirada() {
	set_color(3);
	cout << "\n - - - - - RETIRADA - - - - -\n" << endl; //FALTA MUDAR O STATUS DE DISPONIVEL DO LIVRO OU REVISTA QUANDO FOR RETIRADO
	set_color(7);
	cout << "[1] - Livro" << endl
		<< "[2] - Revista" << endl
		<< "[0] - Voltar" << endl
		<< endl
		<< "Digite a opcao desejada: ";
}

void submenu_acervo() {
	set_color(3);
	cout << "\n - - - - - CONSULTAR ACERVO - - - - -\n" << endl;
	set_color(7);
	cout << "[1] - Pesquisa Filtrada" << endl
		<< "[2] - Listagem Completa" << endl
		<< "[0] - Voltar" << endl
		<< endl
		<< "Digite a opcao desejada: ";
}

void submenu_central_usuario() {
	set_color(3);
	cout << "\n - - - - - CENTRAL DO USUARIO - - - - -\n" << endl;
	set_color(7);
	cout << "[1] - Cadastro" << endl
		<< "[2] - Usuarios com atraso de devolucao" << endl
		<< "[3] - Historico de retiradas" << endl
		<< "[4] - Listagem de Usuarios" << endl
		<< "[0] - Voltar" << endl
		<< endl
		<< "Digite a opcao desejada: ";
}
#pragma endregion  Menu

int main()
{
#pragma region listas
	//listas
	Autor lista_autores[100];
	Editora lista_editoras[100];
	string lista_assuntos[100], lista_titulos[100], titulos_revistas[100], lista_nomes[3];
	Livro lista_livros[100];
	Revista lista_revistas[100];
	Usuario lista_usuarios[100];
	int lista_ano[10], lista_mes[10];
#pragma endregion listas

#pragma region variaveis
	//outras variaveis
	int opcao, min_opcao, max_opcao, escolha;
	int cont_livros = 4, cont_revistas = 3, cont_autor = 4, cont_editora = 8, cont_assunto = 8, cont_usuario = 2, soma = 0;;
	char resposta;
	bool flag = false;
	Livro livro;
	Autor autor;
	Editora ed;
	string aux;
	Usuario usuario;
	Data data;
#pragma endregion variaveis

#pragma region gerar
	//gerar IDs iniciais
	int id_livro = gerar_ID(1001);
	int id_revista = gerar_ID(2001);
	int id_usuario = gerar_ID(3001);

	gerar_autores(lista_autores); //cadastro de 5 autores 
	gerar_editoras(lista_editoras); //cadastro de 9 editoras 
	gerar_assuntos(lista_assuntos); //cadastro de 15 assuntos 
	gerar_titulos(lista_titulos, titulos_revistas); //cadastro de 9 titulos
	gerar_ano(lista_ano); //cadastro de 10 anos aleatorios
	gerar_mes(lista_mes); //cadastro de 10 meses aleatorios
	gerar_nomes(lista_nomes); //cadastro de 10 meses aleatorios
#pragma endregion gerar

	// obter a data atual
	time_t data_atual = time(nullptr);

	for (int i = 0; i < 5; i++) { //cadastro de 5 livros aleatorios
		gerar_livros(lista_livros[i], lista_editoras[i], lista_autores[i], lista_assuntos[i], lista_titulos[i], lista_ano[i], id_livro);
		id_livro++;
	}

	for (int i = 0; i < 4; i++) { //cadastro de 4 revistas aleatorias
		gerar_revistas(lista_revistas[i], lista_editoras[i + 5], lista_assuntos[i + 5], lista_ano[i + 5], lista_mes[i], titulos_revistas[i], id_revista);
		id_revista++;
	}

	for (int i = 0; i < 3; i++) { //cadastro de 3 usuarios aleatorios
		gerar_usuarios(lista_usuarios[i], id_usuario, lista_nomes[i]);
		id_usuario++;
	}

	int escolha_menu_principal = 0;
	int escolha_submenu = 0;

	while (true)
	{
		do {
			exibir_menu_principal();
			cin >> escolha_menu_principal;
			system("cls");

		} while (opcao_invalida(escolha_menu_principal, 0, 5));

		// continua com a opção que o usuario escolheu
		switch (escolha_menu_principal)
		{
		case 0: // fecha o programa
			system("cls");
			cout << "Volte sempre! " << endl;
			return 0;
			break;

		case 1: // cadastro de material
			do {
				submenu_cadastro();
				cin >> escolha_submenu;

				switch (escolha_submenu) {
					// Casos para cada opção do submenu 1
				case 0:
					system("cls");
					break;

				case 1: //cadastrar livro
					do {
						cont_livros++;
						lista_livros[cont_livros] = cadastrar_livro();
						lista_livros[cont_livros].id = id_livro;
						id_livro++;
						cont_autor++;
						cont_editora++;

						//atualiza lista de autores e editoras tambem
						lista_autores[cont_autor].nome = lista_livros[cont_livros].autor.nome;
						lista_autores[cont_autor].sobrenome = lista_livros[cont_livros].autor.sobrenome;
						lista_editoras[cont_editora].nome = lista_livros[cont_livros].editora.nome;
						lista_editoras[cont_editora].local = lista_livros[cont_livros].editora.local;

						flag = false;
						for (int i = 0; i < cont_assunto + 1; i++) {
							if (lista_assuntos[i] == lista_livros[cont_livros].assunto) { //verifica se o assunto ja esta cadastrado
								flag = true;
							}
						}
						if (flag == false) { //se não estiver cadastrado, eh adicionado a lista
							cont_assunto++;
							lista_assuntos[cont_assunto] = lista_livros[cont_livros].assunto;
						}

						cout << "Deseja cadastrar mais um livro (s/n)? ";
						cin >> resposta;
					} while (resposta == 's' || resposta == 'S'); //loop continua enquanto o usuario desejar cadastrar mais livros

					system("cls");
					break;

				case 2: //cadastrar revistas
					do {
						cont_revistas++;
						lista_revistas[cont_revistas] = cadastrar_revista();
						lista_revistas[cont_revistas].id = id_revista;
						id_revista++;
						cont_editora++;

						//atualiza lista de autores e editoras tambem
						lista_editoras[cont_editora].nome = lista_revistas[cont_revistas].editora.nome;
						lista_editoras[cont_editora].local = lista_revistas[cont_revistas].editora.local;

						flag = false;
						for (int i = 0; i < cont_assunto + 1; i++) {
							if (lista_assuntos[i] == lista_revistas[cont_revistas].assunto) { //verifica se o assunto ja esta cadastrado
								flag = true;
							}
						}
						if (flag == false) { //se não estiver cadastrado, eh adicionado a lista
							cont_assunto++;
							lista_assuntos[cont_assunto] = lista_revistas[cont_revistas].assunto;
						}

						cout << "Deseja cadastrar mais um livro (s/n)? ";
						cin >> resposta;
					} while (resposta == 's' || resposta == 'S');  //continua enquanto for desejado cadastrar mais revistas

					system("cls");
					break;

				case 3: //cadastrar autor
					do {
						cout << "\nLista atual de autores cadastrados: " << endl;
						for (int i = 0; i < cont_autor; i++) {
							cout << "\t[" << i + 1 << "] ";
							mostrar_autor(lista_autores[i]); //mostra todos os autores cadastrados ate o momento
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
						cout << "\nLista atual de editoras cadastrados: " << endl;
						for (int i = 0; i < cont_autor; i++) {
							cout << "\t[" << i + 1 << "] ";
							mostrar_autor(lista_autores[i]);  //mostra todas as editoras cadastradas ate o momento
							cout << endl;
						}

						cont_editora++;
						cin.ignore();
						lista_editoras[cont_editora] = ler_editora();
						cout << "\nEditora cadastrada com sucesso!" << endl;
						cout << "\nDeseja cadastrar mais uma Editora (s/n)? ";
						cin >> resposta;

					} while (resposta == 's' || resposta == 'S');
					system("cls");
					break;

				case 5: // cadastrar usuario
					do {
						cont_usuario++;
						id_usuario++;
						Usuario novo_usuario = cadastrar_usuario(id_usuario);
						flag = false;

						for (int i = 1; i <= cont_usuario; i++) {
							if (lista_usuarios[i].nome == novo_usuario.nome) { // verifica se o usuário já está cadastrado
								cout << "\nUsuario ja cadastrado." << endl;
								cont_usuario--;
								flag = true;
								break;
							}
						}
						if (!flag) {
							lista_usuarios[cont_usuario] = novo_usuario;
						}

						cout << "\nDeseja cadastrar mais um usuario (s/n)? ";
						cin >> resposta;
					} while (resposta == 's' || resposta == 'S'); //loop continua enquanto o usuario desejar cadastrar mais usuarios
					system("cls");
					break;
				}
				system("cls");
			} while (opcao_invalida(escolha_submenu, 0, 5));
			break;

		case 2: // alteracao de material
			do {
				submenu_alteracao();
				cin >> escolha_submenu;

				switch (escolha_submenu) {
				case 0: //voltar para o menu
					system("cls");
					break;

				case 1://alterar livro
					cout << "\nLivros cadastrados atualmente (id e titulo): " << endl;
					for (int i = 0; i < cont_livros + 1; i++) {
						cout << "\t[" << i + 1 << "] ";
						cout << lista_livros[i].titulo << " (" << lista_livros[i].id << ")";
						cout << endl;

					}

					cout << "\nEscolha a opcao que deseja alterar: ";
					cin >> escolha;

					lista_livros[escolha - 1] = cadastrar_livro();
					cout << "\nLivro alterado com sucesso!" << endl;

					//atualiza lista de autores e editoras tambem
					lista_autores[escolha - 1].nome = lista_livros[escolha - 1].autor.nome;
					lista_autores[escolha - 1].sobrenome = lista_livros[escolha - 1].autor.sobrenome;
					lista_editoras[escolha - 1].nome = lista_livros[escolha - 1].editora.nome;
					lista_editoras[escolha - 1].local = lista_livros[escolha - 1].editora.local;

					flag = false;
					for (int i = 0; i < cont_assunto + 1; i++) {
						if (lista_assuntos[i] == lista_livros[cont_livros].assunto) { //verifica se o assunto ja esta cadastrado
							flag = true;
						}
					}
					if (flag == false) { //se não estiver cadastrado, eh adicionado a lista
						cont_assunto++;
						lista_assuntos[cont_assunto] = lista_livros[cont_livros].assunto;
					}
					break;

				case 2:
					cout << "\nRevistas cadastrados atualmente (id e titulo): " << endl;
					for (int i = 0; i < cont_revistas + 1; i++) {
						cout << "\t[" << i + 1 << "] ";
						cout << lista_revistas[i].titulo << " (" << lista_revistas[i].id << ")";
						cout << endl;

					}

					cout << "\nEscolha a opcao que deseja alterar: ";
					cin >> escolha;

					lista_revistas[escolha - 1] = cadastrar_revista();
					cout << "\nLivro alterado com sucesso!" << endl;

					//atualiza lista de autores e editoras tambem
					lista_editoras[escolha - 1].nome = lista_revistas[escolha - 1].editora.nome;
					lista_editoras[escolha - 1].local = lista_revistas[escolha - 1].editora.local;

					flag = false;
					for (int i = 0; i < cont_assunto + 1; i++) {
						if (lista_assuntos[i] == lista_revistas[cont_revistas].assunto) { //verifica se o assunto ja esta cadastrado
							flag = true;
						}
					}
					if (flag == false) { //se não estiver cadastrado, eh adicionado a lista
						cont_assunto++;
						lista_assuntos[cont_assunto] = lista_revistas[cont_revistas].assunto;
					}
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
				system("cls");
			} while (opcao_invalida(escolha_submenu, 0, 4));
			system("cls");
			break;

		case 3: // retirada
			do {
				submenu_retirada();
				cin >> escolha_submenu;
				system("cls");
			} while (opcao_invalida(escolha_submenu, 0, 2));

			switch (escolha_submenu)
			{
			case 0:
				system("cls");
				break;

			case 1:
				cout << "\nLista de Livros (Id - Titulo)" << endl;

				for (int i = 0; i < cont_livros + 1; i++) {
					cout << "\t[" << lista_livros[i].id << "] - ";
					cout << lista_livros[i].titulo << endl;
				}

				do {
					cout << "Informe o ID do livro: ";
					cin >> escolha;

					flag = false;
					for (int i = 0; i < cont_livros + 1; i++) {
						if (lista_livros[i].id == escolha) {
							flag = true;
						}
					}
					if (flag == false) {
						cout << "\nId de Livro nao foi encontrada, verifique se digitou o ID corretamente!" << endl;
					}
				} while (flag == false);

				cout << "\nInforme seu ID de usuario: ";
				cin >> usuario.id;

				cout << "\nData de retirada: \n";
				data = ler_data();

				soma = 0;
				flag = false;
				for (int i = 0; i < cont_usuario + 1; i++) {
					if (lista_usuarios[i].id == usuario.id) {
						flag = true;

						if (atraso(lista_usuarios[i]) == false) {
							soma = qtd_retiradas(lista_usuarios[i]);
							lista_usuarios[i].retirados[soma].id = livro.id;
							
							lista_usuarios[i].retirados[soma].retirada.dia = data.dia;
							lista_usuarios[i].retirados[soma].retirada.mes = data.mes;
							lista_usuarios[i].retirados[soma].retirada.ano = data.ano;
							
							for (int i = 0; i < cont_livros + 1; i++) {
								if (lista_livros[i].id == escolha) {
									lista_livros[i].disponivel = false; //passa a colocar o livro como indisponivel
								}
							}
							cout << "\nItem retirado com sucesso!" << endl;
							cout << "Voce tem 7 dias para realizar a leitura do material" << endl;
							cout << "\tData de devolucao: "; // << data_devolucao() << endl;
						}
						else if (atraso(lista_usuarios[i])) {
							break;
						}
					}
				}
				if (flag == false) {
					cout << "\nID de Usuario nao foi encontrado, verifique se ja realizou cadastro ou se digitou o ID corretamente!" << endl;
				}
				system("pause");
				break;

			case 2:
				cout << "\nLista de Revistas (id e titulo): " << endl;
				for (int i = 0; i < cont_revistas + 1; i++) {
					cout << "\t[" << lista_revistas[i].id << "] ";
					cout << lista_revistas[i].titulo << endl;
				}
				do {
					cout << "\nInforme o ID da revista que deseja retirar: ";
					cin >> escolha;

					flag = false;
					for (int i = 0; i < cont_revistas + 1; i++) {
						if (lista_revistas[i].id == escolha) {
							flag = true;
						}
					}
					if (flag == false) {
						cout << "\nID de Revista nao foi encontrado, verifique se digitou o ID corretamente!" << endl;
					}
				} while (flag == false);

				cout << "\nInforme seu ID de usuario: ";
				cin >> usuario.id;

				cout << "Informe a data de retirada: " << endl;
				data = ler_data();

				soma = 0;
				flag = false;
				for (int i = 0; i < cont_usuario + 1; i++) {
					if (lista_usuarios[i].id == usuario.id) { //encontra na lista de usuarios cadastrados o usuario correspondente ao id informado
						flag = true;

						if (atraso(lista_usuarios[i]) == false) {
							soma = qtd_retiradas(lista_usuarios[i]);
							lista_usuarios[i].retirados[soma].id = escolha; //atualiza lista de retirados do usuario

							lista_usuarios[i].retirados[soma].retirada.dia = data.dia;
							lista_usuarios[i].retirados[soma].retirada.mes = data.mes;
							lista_usuarios[i].retirados[soma].retirada.ano = data.ano;

							for (int i = 0; i < cont_revistas + 1; i++) {
								if (lista_revistas[i].id == escolha) {
									lista_revistas[i].disponivel = false; //passa a colocar o livro como indisponivel
								}
							}
							cout << "\nItem retirado com sucesso!" << endl;
							cout << "Voce tem 7 dias para realizar a leitura do material" << endl;
							cout << "\tData de devolucao: " << endl; //data_devolucao(); FAZER FUNCAO PRA CALCULAR DATA DE DEVOLUCAO
						}
						else if (atraso(lista_usuarios[i])) {
							break;
						}
					}
				}
				if (flag == false) {
					cout << "\nID de Usuario nao foi encontrado, verifique se ja realizou cadastro ou se digitou a ID corretamente!" << endl;
				}
				break;
			}
			system("pause");
			break;

		case 4: // consultar acervo
			do {
				submenu_acervo();
				cin >> escolha_submenu;
				system("cls");
			} while (opcao_invalida(escolha_submenu, 0, 2));

			if (escolha_submenu == 0) {
				system("cls");
				break;
			}

			if (escolha_submenu == 1) {
				do {
					cout << "\nDeseja filtrar sua pesquisa por: " << endl
						<< "\n[1] - Livros" << endl
						<< "[2] - Disponibilidade" << endl
						<< "[3] - Revistas" << endl
						<< "[4] - Autor" << endl
						<< "[5] - Editora" << endl
						<< "[6] - Assunto" << endl
						<< "[0] - Voltar" << endl
						<< "\nEscolha uma opcao: ";
					cin >> escolha_submenu;

					switch (escolha_submenu) {
					case 0: //voltar para o menu
						system("cls");
						break;

					case 1: //mostrar todos os livros
						cout << "\n - - - - - LISTAGEM DE LIVROS - - - - -" << endl;
						for (int i = 0; i < cont_livros; i++) {
							mostrar_livro(lista_livros[i]);
						}
						system("pause");
						break;

					case 2: // mostrar todos os livros e revistas disponiveis
						cout << "\nLista de livros disponiveis:\n" << endl;
						for (int i = 0; i < cont_livros + 1; i++) {
							if (lista_livros[i].disponivel) {
								mostrar_livro(lista_livros[i]);

							}
						}
						for (int i = 0; i < cont_livros + 1; i++) {
							if (lista_revistas[i].disponivel) {
								mostrar_revista(lista_revistas[i]);
							}
						}
						system("pause");
						break;

					case 3: //mostrar todas as revistas
						cout << "\n - - - - - LISTAGEM DE REVISTAS - - - - -" << endl;
						for (int i = 0; i < cont_livros + 1; i++) {
							mostrar_revista(lista_revistas[i]);
						}
						system("pause");
						break;

					case 4: //mostrar livros de um autor especifico
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
							if (lista_livros[i].autor.nome == autor.nome && lista_livros[i].autor.sobrenome == autor.sobrenome) { //verifica livros com o autor procurado
								mostrar_livro(lista_livros[i]);
								flag = true;
							}
						}
						if (flag == false) {
							cout << "\nNao existem livros cadastrados com este autor" << endl;
						}
						system("pause");
						break;

					case 5: //mostrar livros e revistas de uma editora
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
							if (lista_livros[i].editora.nome == ed.nome && lista_livros[i].editora.local == ed.local) { //verifica se existem livros da editora procurada
								mostrar_livro(lista_livros[i]);
								flag = true;
							}
							else if (lista_revistas[i].editora.nome == ed.nome && lista_revistas[i].editora.local == ed.local) { //verifica as revistas
								mostrar_revista(lista_revistas[i]);
								flag = true;
							}
						}
						if (flag == false) {
							cout << "\nNao existem livros ou revistas cadastrados com esta Editora" << endl;
						}
						system("pause");
						break;

					case 6: //mostrar livros e revistas de um assunto
						cin.ignore();
						cout << "Qual assunto deseja encontrar? ";
						getline(cin, aux);

						flag = false;
						for (int i = 0; i < cont_livros + 1; i++) {
							if (lista_livros[i].assunto == aux) { //verifica se existem livros relacionados ao assunto procurado
								mostrar_livro(lista_livros[i]);
								flag = true;
							}
							else if (lista_revistas[i].assunto == aux) { //verifica as revistas
								mostrar_revista(lista_revistas[i]);
								flag = true;
							}
						}
						if (flag == false) {
							cout << "\nNao existem livros ou revistas cadastrados com este assunto" << endl;
						}
						system("pause");
						break;
					}
					system("cls");
					break;
				} while (opcao_invalida(escolha_submenu, 0, 5));
			}

			else if (escolha_submenu == 2) { //lista de todos os livros e revistas
				cout << "\n - - - - - LISTAGEM COMPLETA - - - - -" << endl;
				for (int i = 0; i < cont_livros + 1; i++) {
					mostrar_livro(lista_livros[i]);
				}
				for (int i = 0; i < cont_revistas + 1; i++) {
					mostrar_revista(lista_revistas[i]);
				}
			}
			cout << endl;
			system("pause");
			system("cls");
			break;

		case 5: //informacoes relacionadas ao usuario
			do {
				submenu_central_usuario();
				cin >> escolha_submenu;
			} while (opcao_invalida(escolha_submenu, 0, 4));

			switch (escolha_submenu)
			{
			case 0:
				system("cls");
				break;

			case 1:
				cout << "\n - - - - - CADASTRE-SE - - - - -" << endl;
				cont_usuario++;
				lista_usuarios[cont_usuario] = cadastrar_usuario(id_usuario);
				cout << "\nCadastro realizado com sucesso!" << endl;
				id_usuario++;
				system("pause");
				system("cls");
				break;

			case 2:
				system("cls");
				cout << "- - - - - Usuarios com Atraso de Devolucao - - - - -\n";
				soma = 0;
				for (int i = 0; i < cont_usuario + 1; i++) {
					soma = qtd_retiradas(lista_usuarios[i]);

					for (int j = 0; j < soma; j++) {
						if (lista_usuarios[i].retirados[j].atraso == true) {
							mostrar_usuario(lista_usuarios[i]);
						}
					}
				}
				system("cls");
				break;

			case 3:
				system("cls");
				cout << "- - - - - HISTORICO DE RETIRADA - - - - -" << endl;
				cout << "\nInforme o ID do usuario: ";
				cin >> usuario.id;

				soma = 0;
				flag = false;
				for (int i = 0; i < cont_usuario + 1; i++) {
					if (lista_usuarios[i].id == usuario.id) { //encontra na lista de usuarios cadastrados o usuario correspondente ao id informado
						flag = true;
						soma = qtd_retiradas(lista_usuarios[i]);

						for (int j = 0; j < soma; j++) {
							for (int k = 0; k < cont_livros; k++) {
								if (lista_usuarios[i].retirados[j].id == lista_livros[k].id) { //mostra livros retirados
									mostrar_livro(lista_livros[k]);
									//cout << "\tData de retirada: " << lista_usuarios[i].retirados[j].retirada;
								}
							}
						}
						for (int j = 0; j < soma; j++) {
							for (int k = 0; k < cont_revistas; k++) {
								if (lista_usuarios[i].retirados[j].id == lista_revistas[k].id) { //mostra revistas retiradas
									mostrar_revista(lista_revistas[k]);
									//cout << "\tData de retirada: "
								}
							}
						}
					}
				}
				if (flag == false) {
					cout << "\nID de Usuario não foi encontrado, verifique existe cadastro ou se digitou o ID corretamente" << endl;
				}
				system("pause");
				system("cls");
				break;

			case 4: // Listagem de Usuarios
				system("cls");
				cout << "- - - - - LISTAGEM DE USUARIOS - - - - -" << endl;
				for (int i = 0; i < cont_usuario + 1; i++) {
					mostrar_usuario(lista_usuarios[i]);
				}
				system("pause");
				system("cls");
				break;
			}
			break;
		}
	}
	return 0;
}
