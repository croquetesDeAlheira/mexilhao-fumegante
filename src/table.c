#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "table-private.h"

//constants
const int OK = 0;
const int ERROR = -1;
const int NOTHING_CHANGED = 1;
const int EQUALS = 0;






int key_hash(char *key, int l){

  /* Verificar se key é NULL */
	if(key == NULL){return NULL;}
  /* l tem valor válido? */
	if(l < 0){return NULL;}
	int soma, keySize, ind;
	if((keySize = strlen(key)) < 6){
		for(ind = 0; ind < keySize; ind++)
			soma+=key[ind];
	}else{
		for(ind = 0; ind < 3; ind++)
			soma+=key[ind];
		soma = soma + key[keySize-2] + key[keySize-1];
	}

  return soma % l;
}

/* Função para criar/inicializar uma nova tabela hash, com n  
 * linhas(n = módulo da função hash)
 */
struct table_t *table_create(int n) {


  /* n tem valor válido? */
	if(n <= 0){ return NULL; }

  /* Alocar memória para struct table_t */
	struct table_t *new_table = (struct table_t *) malloc(sizeof(struct table_t));
	if(new_table == NULL){ return NULL; } //verifica se not null
  /* Alocar memória para array de listas com n entradas 
     que ficará referenciado na struct table_t alocada.*/ 
	struct table_t *tabelaAux[n];
	if(tabelaAux == NULL){ return NULL; }
  	
  /*   Inicializar listas.

     Inicializar atributos da tabela.
  */
	for (int i = 0; i < n; ++i){
		struct list_t *list = list_create();
		if(list == NULL){return NULL;} //nao ha memoria
		tabelaAux[i] = list;
	}
	new_table->tabela = tabelaAux;
	new_table->size = 0;
	new_table->maxSize = n;
	return new_table;
}


/* Libertar toda a memória ocupada por uma tabela.
 */
void table_destroy(struct table_t *table) {

  	/* table é NULL? */
	if(table == NULL){return; /*do nothing*/ }
	/*Libertar memória das listas.
	  Libertar memória da tabela.
	*/
	for (int i = 0; i < table->maxSize; ++i){
		list_destroy(table->tabela[i]);
	} //destroy todas as listas de cada entrada da tabela...
	free(table);  
}


/* Função para adicionar um par chave-valor na tabela. 
 * Os dados de entrada desta função deverão ser copiados.
 * Devolve 0 (ok) ou -1 (out of memory, outros erros)
 */
int table_put(struct table_t *table, char *key, struct data_t *value) {

  /* Verificar valores de entrada */
	if(table == NULL || key == NULL || value == NULL){ return ERROR;}

  /* Criar entry com par chave/valor */
	struct entry_t *entry = entry_create(key, value);
	//Verifica just in case , O CREATE JA O FAZ
	if(entry == NULL){return ERROR; }
  /* Executar hash para determinar onde inserir a entry na tabela */
	int index = key_hash(key, table->maxSize); 
  /* Inserir entry na tabela */
	/*cria uma var que guarda o size da list no index.*/
	int listSize = list_size(table->tabela[index]);
	int result = list_add(table->tabela[index], entry); //OK se tudo correu bem
	if(result == OK){
		if(listSize == list_size(table->tabela[index])){
			//fez update e nao add new
			return OK;
		}else{
			//incrementa size
			table->size++;
			return OK;
		}
	}else{
		return ERROR;
	}
}


/* Função para substituir na tabela, o valor associado à chave key. 
 * Os dados de entrada desta função deverão ser copiados.
 * Devolve 0 (OK) ou -1 (out of memory, outros erros)
 */
int table_update(struct table_t *table, char * key, struct data_t *value) {
	//chama o put? o list_add ja faz o update se for repetido
	return table_put(table, key, value);
}


/* Função para obter da tabela o valor associado à chave key.
 * A função deve devolver uma cópia dos dados que terão de ser libertados
 * no contexto da função que chamou table_get.
 * Devolve NULL em caso de erro.
 */
struct data_t *table_get(struct table_t *table, char * key){
	 /* Verificar valores de entrada */
	if(table == NULL || key == NULL){ return NULL; }
	 /* Verifica o hash index */
	int index = key_hash(key, table->maxSize);
	/*  buscar o entry na lista  */
	struct entry_t entry = list_get(table->tabela[index], key);
	/*  verificar se entry no null */ 
	if(entry == NULL){return NULL; }
	/*  return o data do entry duplicado, pois é uma copia  */
	return data_dup(entry->value);

}

/* Função para remover um par chave valor da tabela, especificado 
 * pela chave key, libertando a memória associada a esse par.
 * Devolve: 0 (OK), -1 (nenhum tuplo encontrado; outros erros)
 */
int table_del(struct table_t *table, char *key){
	/*  verificar valores de entrada */
	if(table == NULL || key == NULL){return ERROR; }

	/* encontrar o hash index*/
	int index = key_hash(key, table->maxSize);

	/* ja tenho a lista, remove esta key */
	int result = list_remove(table->tabela[index], key);
	if(result == OK){
		table->size--;
		return OK;
	}else{
		return ERROR;
	}

}

/* Esta é dada! Ao estilo C! */
/* Devolve o número de elementos na tabela.
 */
int table_size(struct table_t *table) {
	return table == NULL ? -1 : table->size;
}

/* Devolve um array de char * com a cópia de todas as keys da tabela,
 * e um último elemento a NULL.
 */
char **table_get_keys(struct table_t *table) {
	

}


/* Liberta a memória alocada por table_get_keys().
 */
void table_free_keys(char **keys) {

}

