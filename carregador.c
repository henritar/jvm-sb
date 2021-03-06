/**
 *@file
 *@section DESCRIPTION
 *Universidade de Brasilia
 *
 *Gabriel Mesquita de Araujo 13/0009121\n
 *Gabriel Ferreira Silva 14/0140131\n
 *Renato 13/0009121\n
 *Leandro 13/0009121\n
 *Carlos 13/0009121\n\n
 *
 * Software Basico - 1/2016\n
 * Professor: Marcelo Ladeira\n\n
 *
 * Arquivo que contém funções para carregar classes nas estruturas adequadas.
 * Além de funções auxiliares que retornam nome de classe, nome em um utf8 e
 * a partir da posição de uma classe no array de classes dentro da area de métodos
 * retorna uma referencia a essa classe.
 */

#include "./includes/carregador.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/** 
 *Area de métodos que engloba o array de classes e seus respectivos metodos.
 */
area_metodos area_met; 

/** 
 *booleano para verificar a primeira classe a ser carregada.
 */
bool ja_carregou = false;

/**
 * É passado o path para o object.class a ser carregado.\n
 * 
 * Se ainda não foi carregado carrega no array de classes.
 * @param path do "object.class" a ser carregado no array de classes.
 * @return posição da classe alocada no array de classes.
 */
int32_t carregaObjectClasse(char* ObjectClass){

	int aux;

	// se eh a primeira classe a ser carregada
    if (ja_carregou == false)
    {
        area_met.num_classes = 0;
        ja_carregou = true; 
    }

    //Se ja esta carregado retorna posição no array de classes.
	for (int32_t i = 0; i < area_met.num_classes; i++) {
		if (strcmp(ObjectClass, retornaNomeClasse(area_met.array_classes[i])) == 0){
			return i;
		}
	}

	//Se não está carregado carrega e salva no array de classes.

	//uma classe nova vai entrar na lista de classes carregadas.
	area_met.num_classes++;
	aux = area_met.num_classes;

	classFile** arrayClassesTemp = NULL;

	//Realoca o tamanho do vetor para adicionar a nova classe carregada.
	//Classes previamente carregas não são perdidas com o realloc.
	arrayClassesTemp = (classFile**) realloc(area_met.array_classes, (aux*sizeof(classFile *)));
	//printf("ponteiro tempo: %d\n",*arrayClassesTemp);

    area_met.array_classes = (classFile**) calloc(1, sizeof(classFile*));
    area_met.array_classes = arrayClassesTemp;

    //Atualiza o nome do path para adicionar .class a um path
    //Aloca espaço do tamanho do nome da classe mais espaço para .class /0 e ./
    char* destino = malloc(strlen(ObjectClass) + 10);
    if (strstr(ObjectClass,".class") != NULL) {
		sprintf(destino, "%s", ObjectClass);
	} else {
		sprintf(destino, "./%s.class",ObjectClass);
	}

	//printf("destino: %s\n",destino);

    area_met.array_classes[area_met.num_classes - 1] = leitorClasse(destino);  
    
	if(area_met.array_classes[area_met.num_classes -1] == NULL){
		printf("Erro ao carregar classe!\n");
		exit(0);
	}

	return aux - 1;


}

/**
 * É passado o nome do arquivo .class a ser carregado.\n
 * 
 * Se ainda não foi carregado carrega no array de classes.
 * @param nome do arquivo ".class" a ser carregado no array de classes.
 * @return posição da classe alocada no array de classes.
 */
int32_t carregaMemClasse(char* nomeClass){
	int aux;

    // se eh a primeira classe a ser carregada
    if (ja_carregou == false)
    {
        area_met.num_classes = 0;
        ja_carregou = true; 
    }

	//Se ja esta carregado retorna posição no array de classes.
	for (int32_t i = 0; i < area_met.num_classes; i++) {
		if (strcmp(nomeClass, retornaNomeClasse(area_met.array_classes[i])) == 0){
			return i;
		}
	}

	//Se não está carregado carrega e salva no array de classes.

	//uma classe nova vai entrar na lista de classes carregadas.
	area_met.num_classes++;
	aux = area_met.num_classes;

	classFile** arrayClassesTemp = NULL;

	//Realoca o tamanho do vetor para adicionar a nova classe carregada.
	//Classes previamente carregas não são perdidas com o realloc.
	arrayClassesTemp = (classFile**) realloc(area_met.array_classes, (aux*sizeof(classFile *)));
	//printf("ponteiro tempo: %d\n",*arrayClassesTemp);

    area_met.array_classes = (classFile**) calloc(1, sizeof(classFile*));
    area_met.array_classes = arrayClassesTemp;

    //Atualiza o nome do path para adicionar .class a um path
    //Aloca espaço do tamanho do nome da classe mais espaço para .class /0 e ./
    char* destino = malloc(strlen(nomeClass) + 10);
    if (strstr(nomeClass,".class") != NULL) {
		sprintf(destino, "%s", nomeClass);
	} else {
		sprintf(destino, "./%s.class",nomeClass);
	}

	//printf("destino: %s\n",destino);

    area_met.array_classes[area_met.num_classes - 1] = leitorClasse(destino);  
    
	if(area_met.array_classes[area_met.num_classes -1] == NULL){
		printf("Erro ao carregar classe!\n");
		exit(0);
	}

	return aux - 1;

}


/**
 *Acessa a constant pool no campo bytes a partir do index da classe e 
 *retorna o nome da classe.\n
 *Necessaria para saber se classe ja foi carregada ou não.
 * @param Estrutura classFile de que se deseja obter o nome.
 * @return nome da classe fornecida.
 */
char* retornaNomeClasse(classFile* classe){
	uint16_t thisClass = classe->this_class;
	uint16_t nameIndex = (classe->constant_pool[thisClass - 1]).info.Class.name_index;

	int i;
	char* retorno = (char*) malloc((classe->constant_pool[nameIndex - 1]).info.Utf8.length + 1);
	uint16_t indice = classe->constant_pool[nameIndex - 1].info.Utf8.length;

	// Percorre o bytes na constant pool e salva no retorno.
	for (i = 0; i < indice; i++) {
		retorno[i] = (char) (classe->constant_pool[nameIndex - 1]).info.Utf8.bytes[i];
	}

	// \0 no final do nome da classe a ser retornada
	retorno[i] = '\0';

	//Retorna nome da classe.
	return retorno;
}

/**
 * Função que retorna uma referencia a uma classFile de acordo com o indice.\n
 * Util para encontrar a classe da main no inicio do processamento.
 * @param Posicao a ser acessada no array de classes.
 * @return Referencia a um classFile ou NULL caso nao exista a referencia 
 */
classFile* buscaClasseIndice(int indice){
	return indice >= area_met.num_classes ? NULL : area_met.array_classes[indice];
}


/**
 * Função que retorna um nome de um utf8 a partir de um indice.
 * Muito usada como função de auxílio para implementar as instruções.
 * @param Referência a um classFile para acesso a constant pool.
 * @param indice para acesso a constant pool.
 * @return retorna uma string com o nome.
 */
char * retornaNome(classFile* cf, uint16_t indiceNome) {
	int i;

	//Aloca espaço.
	char* retorno = malloc((cf->constant_pool[indiceNome - 1]).info.Utf8.length + 1);
	uint16_t indice = cf->constant_pool[indiceNome - 1].info.Utf8.length;
	//Percorre o campo bytes de um utf8 para um auxiliar.
	for (i = 0; i < indice; i++) {
		retorno[i] = (char) (cf->constant_pool[indiceNome - 1]).info.Utf8.bytes[i];
	}

	//Adiciona \0 ao final do nome.
	retorno[i] = '\0';

	return retorno;
}
