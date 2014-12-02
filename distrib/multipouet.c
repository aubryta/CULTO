#include multipouet.c
#include <stdio.h>
#include <stdlib.h>
#include "m_pd.h"


/*
 * Q.3 - Comportement en cas de message bang
 */
void            multipouet_bang(t_multipouet *x);

/*
 * Q.2 - Création d'un nouvel objet multipouet
 */
void            *multipouet_new(t_symbol *s, int argc, t_atom *argv){
  t_multipouet *mp;
  mp=(t_multipouet*)pd_new(multipouet_class);
  mp->i_min = argv[0];
  mp->i_max = argv[1];
  mp->i_step = argv[2]; 
  inlet_new(mp,	// inlet principal
  inlet_new (mp , mp.i_step, // inlet du step
  mp->p_out=outlet_new(&mp->x_obj,&s_symbol);
  mp->b_out=outlet_new(&mp->x_obj,&s_symbol);
  
  }
/*
 * Q.1 - Chargement en mémoire des objets de type multipouet
 */
void            multipouet_setup(void){
	multipouet_class = class_new(gensym("multipouet"),(t_newmethod)multipouet_new,0,sizeof(t_multipouet),CLASS_PD,A_GIMME);
	class_addmethod(multipouet_class,
	class_addbang(horloge_class,horloge_bang);
}