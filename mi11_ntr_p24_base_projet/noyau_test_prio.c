/*----------------------------------------------------------------------------*
 * fichier : noyau_test.c                                                     *
 * programme de test du noyaut                                                *
 *----------------------------------------------------------------------------*/

#include <stdint.h>
#include <stdlib.h>

#include "stm32h7xx.h"
#include "serialio.h"
#include "noyau_prio.h"
#include "noyau_file_prio.h"
#include "delay.h"
#include "TERMINAL.h"


#define MAX_CARA_LIGNE 80

/*
 * structure du contexte d'une tache
 */
typedef struct {
    // adresse de debut de la tache
    uint16_t Nb_tour;
    // etat courant de la tache
    uint16_t wait_time;
} NOYAU_TCB_ADD;

/*----------------------------------------------------------------------------*
 * declaration des variables du noyau comme extern pour pouvoir les
 * utiliser dans d'autres partie du code
 * *--------------------------------------------------------------------------*/
#define POS_CHRONO 10
/*
 * tableau stockant le contexte de chaque tache
 */
NOYAU_TCB_ADD _noyau_tcb_add[MAX_TACHES_NOYAU];

uint16_t pos_x = 1;
uint16_t pos_y = 10;


void tacheap(void** args, int nb_args){
	uint16_t id_parent = args[0];
	for(int i = 0; i< 20; i++){
		printf("-----p%d - %d\n",id_parent,i);
		for(int j=0; j < 300000 ; j++){

		}
	}
	printf("tache ap de %d finis !!!!!!!!!!!!!!!!\n",id_parent);
}

void tache_p(){
	volatile NOYAU_TCB* p_tcb = NULL;
	volatile uint16_t id_tache;
	uint16_t i, j=1;


	id_tache = noyau_get_tc();
	p_tcb = noyau_get_p_tcb(id_tache);

	volatile uint16_t Nb_tour = ((NOYAU_TCB_ADD*)(p_tcb->tcb_add))->Nb_tour;
	volatile uint16_t wait_time = ((NOYAU_TCB_ADD*)(p_tcb->tcb_add))->wait_time;


	// on laisse du temps à la tâche de fond de démarrer toutes les tâches
	delay_n_ticks(20);
	while(1){
		  _lock_();
		  printf("ap %d -> %d;\n", id_tache,j);
		  _unlock_();
		  delay_n_ticks(100);
		  if (j >= Nb_tour){
				 j = 1;
				 delay_n_ticks(wait_time);
				 _lock_();
				 printf("%d a finis !!!!!!\n", id_tache);
				 _unlock_();
				 ajoute_tache_aperiodique(tacheap, (void**)&id_tache, 1);
		  } else {
				 j++;
		  }
	  }
}

void lol(){
	printf("lol !!");
	int nbs[10];
	for(int i = 0; i < 10; i++){
			nbs[i] = i;
	}
	//delay_n_ticks(200);
	int id;
	id = 3;
		_noyau_tcb_add[id].Nb_tour = 5;
		_noyau_tcb_add[id].wait_time = 0;
		active(cree(tache_p, id, (void*)&_noyau_tcb_add[id]));
	id = 8;
			_noyau_tcb_add[id].Nb_tour = 10;
			_noyau_tcb_add[id].wait_time = 0;
			active(cree(tache_p, id, (void*)&_noyau_tcb_add[id]));
	id = 16;
			_noyau_tcb_add[id].Nb_tour = 15;
			_noyau_tcb_add[id].wait_time = 0;
			active(cree(tache_p, id, (void*)&_noyau_tcb_add[id]));
	fin_tache();
	return;
}

int main()
{
	usart_init(115200);
	CLEAR_SCREEN(1);
    SET_CURSOR_POSITION(1,1);
    test_colors();
    CLEAR_SCREEN(1);
    SET_CURSOR_POSITION(1,1);
    puts("Test noyau");
    puts("Noyau preemptif");
    SET_CURSOR_POSITION(5,1);
    SAVE_CURSOR_POSITION();
	start(lol);
	return(0);
}
