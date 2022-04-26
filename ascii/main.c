/*
 * Nome: José Pedro Cioni do Carmo
 * N° USP: 12623988
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *read_line();

//lê uma linha de um arquivo
char *read_file(FILE *arquivo);

//preenche os espaços da imagem
void preencher(
  char **imagem,
  char cor_nova,
  int linha,
  int coluna,
  char cor_atual,
  int altura,
  int largura);

//imprime uma as strings de um vetor linha a linha
void imprimir(char **imagem,int altura);

void enquadra_arte(
  char *nome_do_arquivo_da_arte,
  int  altura_do_quadro,
  int  largura_do_quadro
);

int main(){
  char *nome_arquivo = read_line();

  FILE *arquivo = NULL;

  if (!(arquivo = fopen(nome_arquivo, "r"))){
    printf("O arquivo não existe\n");
    free(nome_arquivo);
    return 0;
  }

  char **imagem = NULL;
  int altura = 0;
  int largura;

  //dar valor para a matriz da imagem
  do{
    imagem = (char **) realloc(imagem, (altura + 1) * sizeof(char *));
    imagem[altura] = read_file(arquivo);
    altura++;
  }while (imagem[altura - 1] != NULL);
  altura --;
  largura = strlen(imagem[0]);
  imagem = (char **) realloc(imagem, (altura) * sizeof(char *));
  fclose(arquivo);


  int n_preenchimentos = 0;
  char cor_atual,cor_nova;
  int linha,coluna;
  char pula_linha ='\n';

  scanf("%d\n", &n_preenchimentos);

  printf("Arte inicial:\n");
  imprimir(imagem, altura);
  for (int i = 0; i < n_preenchimentos; i++){
    scanf("%c %d %d\n", &cor_nova, &linha, &coluna);
    cor_atual = imagem[linha][coluna];
    preencher(imagem, cor_nova, linha, coluna, cor_atual, altura, largura);

    printf("\nArte apos a etapa %d:\n", i);
    imprimir(imagem, altura);
  }

  arquivo = fopen(nome_arquivo, "w");
  for(int i = 0; i < altura; i++){
    fwrite(imagem[i], sizeof(char),largura,arquivo);
    if (i + 1 < altura){
      fwrite(&pula_linha, sizeof(char), 1, arquivo);
    }
  }
  fclose(arquivo);

  printf("\nArte enquadrada:\n");

  enquadra_arte(nome_arquivo, altura,largura);



  for(int i = 0; i < altura; i++){
    free(imagem[i]);
  }
  free(imagem);
  free(nome_arquivo);
  return 0;
}


void imprimir(char **imagem, int altura){

  for(int i = 0; i < altura; i++){
    printf("%s\n", imagem[i]);
  }

}

void preencher(char **imagem, char cor_nova, int linha, int coluna, char cor_atual, int altura, int largura){
  if (linha >=0 && linha < altura && coluna >= 0 && coluna < largura){
    if (imagem[linha][coluna] == cor_atual){
      imagem[linha][coluna] = cor_nova;
      preencher(imagem, cor_nova, linha + 1, coluna, cor_atual, altura, largura);
      preencher(imagem, cor_nova, linha - 1, coluna, cor_atual, altura, largura);
      preencher(imagem, cor_nova, linha, coluna + 1, cor_atual, altura, largura);
      preencher(imagem, cor_nova, linha, coluna - 1, cor_atual, altura, largura);
    }
 }

}

void enquadra_arte(
	char *nome_do_arquivo_da_arte,
	int  altura_do_quadro,
	int  largura_do_quadro
) {
	FILE *f_arte_ptr = fopen(nome_do_arquivo_da_arte, "r");
	if (f_arte_ptr == NULL) {
		printf(
			"Erro na abertura do arquivo, "
			"Você esqueceu de fechar o arquivo antes? "
			"Ou deu free na string com o nome sem querer?\n"
		);

		exit(EXIT_FAILURE);
	}

	int qtd_espc_comeco;
	const char *apoio;
	if (largura_do_quadro % 2 == 0) {
		qtd_espc_comeco = largura_do_quadro / 2;
		apoio           = "/\\";
	} else {
		qtd_espc_comeco = largura_do_quadro / 2 + 1;
		apoio           = "Ʌ";
	}

	for (int i = 0; i < qtd_espc_comeco; i++) printf(" ");
	printf("%s\n", apoio);

	printf("╭");
	for (int i = 0; i < largura_do_quadro; i++) printf("—");
	printf("╮\n");

	for (int i = 0; i < altura_do_quadro; i++) {
		printf("|");
		for (int j = 0; j < largura_do_quadro; j++) {
			char pixel_atual = fgetc(f_arte_ptr);
			printf("%c", pixel_atual);
		}
		printf("|");

		char quebra = fgetc(f_arte_ptr);
		if (quebra != EOF) printf("%c", quebra);
	};
	fclose(f_arte_ptr);

	printf("\n╰");
	for (int i = 0; i < largura_do_quadro; i++) printf("—");
	printf("╯\n");
}

char *read_line(){//ler linha
  char *linha = (char *) calloc(70, sizeof(char));
  int count = 0;

  while (scanf("%c",&linha[count]) && linha[count] != '\n'){

    if (count >= 70){
      linha = (char *) realloc(linha, (count + 1) * sizeof(char));
    }

    count++;
  }
    linha[count] = '\0';
    linha = (char *) realloc(linha, (count + 1) * sizeof(char));

    return linha;

}

char *read_file(FILE *arquivo){
  char *linha = (char *) calloc(70, sizeof(char));
  int count = 0;

  do{
    if (count >= 70){
      linha = (char *) realloc(linha, (count + 1) * sizeof(char));
    }
    linha[count] = fgetc(arquivo);


    count++;
  }while(linha[count - 1] != '\n' && linha[count - 1] != EOF);
  count--;
  if(linha[0] == EOF){
    free(linha);
    return NULL;
  }

  linha[count] = '\0';
  linha = (char *) realloc(linha, (count + 1) * sizeof(char));

  return linha;

}
