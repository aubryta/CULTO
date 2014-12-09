#include "multipouet.h"
#include "m_pd.h"
/*
 * Q.4 - Comportement de l'objet en cas de messages reset, set ou bound
 */
void            multipouet_reset(t_multipouet *x){
  x->i_count = x->i_min;
}
void            multipouet_set(t_multipouet *x, t_floatarg f){
  x->step = f;
}
void            multipouet_bound(t_multipouet *x, t_floatarg min, t_floatarg max){
  x->i_min =min;
  x->i_max = max;
}

/*
 * Q.3 - Comportement en cas de message bang
 */
void            multipouet_bang(t_multipouet *x){
x->i_count++;
}

/*
 * Q.2 - Création d'un nouvel objet multipouet
 */
void            *multipouet_new(t_symbol *s, int argc, t_atom *argv){
  t_multipouet *mp;
 mp=(t_multipouet*)pd_new(multipouet_class);
 switch (argc) {

case 1 :
 mp->i_min = (int)argv[0].a_w.w_float;

 break;

case 2 :
 mp->i_min = (int)argv[0].a_w.w_float;
 mp->step = argv[1].a_w.w_float;
 break;

case 3 :
 mp->i_min = (int)argv[0].a_w.w_float;
  mp->i_max=(int)argv[1].a_w.w_float;
 mp->step = argv[2].a_w.w_float;

 break;

default: 
	break;

}
  
  mp->b_out=outlet_new(&mp->x_obj,&s_symbol);
  inlet_new(&mp->x_obj,
                &mp->x_obj.ob_pd,
                gensym("list"),
                gensym("bound"));
  floatinlet_new(&mp->x_obj, &mp->step);
  return (void*)(mp);
}


/*
 * Q.1 - Chargement en mémoire des objets de type multipouet
 */
void            multipouet_setup(void){
  multipouet_class = class_new(gensym("multipouet"),(t_newmethod)multipouet_new,0,sizeof(t_multipouet),CLASS_PD,A_GIMME,0);
  class_addbang(multipouet_class,multipouet_bang);
  class_addmethod(multipouet_class,(t_method)multipouet_reset,gensym("reset"),0);
  class_addmethod(multipouet_class,(t_method)multipouet_set,gensym("set"),A_DEFFLOAT,0);
  class_addmethod(multipouet_class,(t_method)multipouet_bound,gensym("bound"),A_DEFFLOAT,A_DEFFLOAT,0);
}
