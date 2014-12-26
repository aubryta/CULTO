#include "duck~.h"
#include "m_pd.h"

/*t_object        x_obj;
    t_sample		f_pan;
    t_sample		f;
    t_inlet 		*x_in2;
    t_outlet		*x_out;
*/
/*
 * Q.5 - Fonction centrale effectuant le calcul
 */
t_int *duck_tilde_perform(t_int *w){
  // w=  tableau de donnée à caster
  // w[0] = obhet duck
  //w[1] pointeur vers le buffer entrant dans le premier inlet
  //w[2] pointeur vers le buffer entrant dans le deuxieme inlet
  //w[3] pointeur vers le buffer de sortie (résultat)
  // w[4] taille des buffers, tous a la meme taille
  int taille = (int) w[5];
  t_duck_tilde µduck =(t_duck_tilde) w[1];
  t_sample *buff_in1 = (t_sample) w[2];
  t_sample *buff_in2 = (t_sample) w[3];
  t_sample *buff_out = (t_sample) w[4];
  int moyenne= 0;
  while (taille > 0){
	if (buf_in2 >0)
		moyenne += *buff_in2;
	else
		moyenne += - *buff_in2;
	buff_in2++;
	taille --;
   }
moyenne = moyenne / w[4];
taille = w[4];

while (taille > 0){
*buff_out =  (*buff_in1)*(1-moyenne);
*buff_out++;
*buff_in1++;
taille--;
}
  
  return w + 6;
  
}

/*
 * Q.4 - Ajout de l'objet duck~ à l'arbre de traitement DSP
 */
void duck_tilde_dsp(t_duck_tilde *x, t_signal **sp){
  dsp_add(t_perfroutine f, int n, ...);
  //Arguments ?
  dsp_add(duck_tilde_perform),5,x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n); ); // pas sûre
  //"on devra également indiquer les buffers necessaires"
}

/*
 * Q.3 - Libération de la mémoire de l'objet duck~
 */
void duck_tilde_free(t_duck_tilde *x){
	inlet_free(x->x_in2);
	outlet_free(x->x_out);   
}

/*
 * Q.2 - Création d'un nouvel objet duck~
 */
void *duck_tilde_new(void){
   t_duck_tilde  *d;
  d = (t_duck_tilde *)pd_new(duck_tilde_class);
  
 
   
    d->x_in2 =inlet_new(&d->x_obj, &d->x_obj.ob_pd,
            gensym("signal"), gensym("bound"));
    d->x_out = outlet_new(&d->x_obj, &s_signal);
}

/*
 * Q.1 - Chargement en mémoire des objets de type duck~
 */
void duck_tilde_setup(void){
  duck_tilde_class = class_new(gensym("duck_tilde"),(t_newmethod)duck_tilde_new,(t_method)duck_tilde_free,sizeof(t_duck_tilde),CLASS_MAINSIGNAL,A_GIMME,0);
  class_addmethod(duck_tilde_class,(t_method)duck_tilde_dsp,gensym("dsp"), 0); // mettre l'argument signal
  CLASS_MAINSIGNALIN(duck_tilde_class,t_duck_tilde,f);
}
