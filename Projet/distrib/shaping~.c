#include "shaping~.h"
#include "myfft_fft.c"
#include "m_pd.h"
#include <stdlib.h>
#include <math.h>
/*
static t_class      *shaping_tilde_class;
typedef struct      _shaping_tilde
{
    t_object        x_obj;
    t_sample		f_spec;
    t_sample		f;
    t_outlet		*x_out;
	t_inlet			*x_in_harm; // type signal
	t_inlet         *x_in_mod;	// type signal	
	t_inlet 		*x_in_shapeWidth // type float
	t_inlet         *x_in_mes;	// type flottant
	
    int             *bitshuffle;
    float           *weighting;
    float           *window;
	float			*buffer;
	int	 			tailleBuff;
}
t_shaping_tilde;*/

/*
 * Q.5 - Fonction centrale effectuant le calcul
 */
 float distance_euclidienne(float x, float y)
{
 return sqrtf(x * x + y * y);
}

void shaping_tilde_messages(t_shaping_tilde *x, t_floatarg n, t_floatarg p)
{
 x->autonorm = n;
 x->bypass   = p;
}


t_int           *shaping_tilde_perform(t_int *w){
	/*
	w[1] Pointeur vers l’objet shaping
	w[2] type t_sample* - Pointeur vers le buffer entrant dans le premier inlet harmonique
	w[3] type t_sample* - Pointeur vers le buffer entrant dans le deuxieme inlet mod
	w[4] type t_sample* - Pointeur vers le buffer sortant
	w[5] type int - Taille des buffers (tous sont alloués par PureData à la même taille)
	*/
  t_shaping_tilde *x =(t_shaping_tilde*) w[1];
  t_sample *buff_in_harmonie = (t_sample*) w[2];
  t_sample *buff_in_mod = (t_sample*) w[3]; 
  float shapeWidth = (float)x->f;
  t_sample *buff_out = (t_sample*) w[4];
  int taille = (int) w[5];
  t_sample*dup_harmonie = malloc (taille * sizeof (long int));
  t_sample*dup_mod = malloc (taille * sizeof (long int));
  int i =0;
  int j =0;
  if (x->bypass==1){
    for ( i = 0 ; i < taille ; i++)
		  buff_out[i] = buff_in_harmonie[i];
	  return w+6;
  }
  init_rdft(taille, x->bitshuffle, x->weighting);  
  for ( i = 0 ; i < taille ; i++){
	dup_harmonie[i]=buff_in_harmonie[i];
	dup_mod[i]=buff_in_mod[i];
  }
  // /*** OK JUSQUE ICI 
  
  
	for( i=0 ; i <taille;i+=2){
		  buff_out[i]= dup_harmonie[i]*cos(dup_harmonie[i+1]);
		  buff_out[i+1]=-dup_harmonie[i]*sin(dup_harmonie[i+1]);
	  }
	  rdft(taille,-1,buff_out,x->bitshuffle,x->weighting);
	return w+6;
	
  /*
  


  NOPE
  for (i = 0; i<taille; i++) {
  x->window[i] = (float) (0.54-0.46*(cos (2*PI * i/taille)));
  dup_harmonie[i] = (x->window[i])*(dup_harmonie[i]);
  dup_mod[i]=(x->window[i])*(dup_mod[i]);
 }
  
  
  rdft(taille, 1, dup_harmonie, x->bitshuffle, x->weighting);
  rdft(taille,1,dup_mod,x->bitshuffle,x->weighting);
  //tailleFFT a definir
  float a1,b1,a2,b2;
  for ( i=0 ; i<taille;i+=2){
	  a1= dup_harmonie[i]; b1 =dup_harmonie[i+1];
	  a2=dup_mod[i]; b2=dup_mod[i+1];
	  dup_harmonie[i]=distance_euclidienne(a1,b1);
	  dup_harmonie[i+1]= -atan2(b1,a1);
	  dup_mod[i]=distance_euclidienne(a2,b2);
	  dup_mod[i+1]= -atan2(b2,a2);
  }
   float ampSum,freqSum,factor;
  for ( i = 0 ; i<taille ; i+= shapeWidth*2){
	  ampSum=0;freqSum=0;
	  for(j=0 ; j<shapeWidth*2;j+=2){
		  ampSum+=dup_mod[i+j];
		  freqSum+=dup_harmonie[i+j];
	  }
	  factor = ampSum/freqSum;
	  for( j =0 ; j<shapeWidth*2;j+=2){
		  dup_harmonie[i+j]*=factor;
	  }
	  
	  OK
	  for( i=0 ; i <taille;i+=2){
		  buff_out[i]= dup_harmonie[i]*cos(dup_harmonie[i+1]);
		  buff_out[i+1]=-dup_harmonie[i]*sin(dup_harmonie[i+1]);
	  }
	  rdft(taille,-1,buff_out,x->bitshuffle,x->weighting);
	  //Normalisation Later
	 
  }
   return w+6;
*/
	
}

/*
 * Q.4 - Ajout de l'objet shaping~ à l'arbre de traitement DSP
 */
void            shaping_tilde_dsp(t_shaping_tilde *x, t_signal **sp){
	dsp_add(shaping_tilde_perform, 5, x,  
          sp[0]->s_vec, sp[1]->s_vec,sp[2]->s_vec, sp[0]->s_n);
}

/*
 * Q.3 - Libération de la mémoire de l'objet shaping~
 */
void            shaping_tilde_free(t_shaping_tilde *x){
	outlet_free(x->x_out);
	inlet_free(x->x_in_mod);
	free(x->bitshuffle);
	free(x->buffer);
	free(x->weighting);
}

/*
 * Q.2 - Création d'un nouvel objet shaping~
 */
void            *shaping_tilde_new(void){
	t_shaping_tilde  *d;
	d = (t_shaping_tilde *)pd_new(shaping_tilde_class);
	// allocation du buffer de taille 4096 pour le future traitement
	d->buffer=(float*) malloc (4096 * sizeof(float));
	d->bitshuffle = (int*) malloc (4096 * sizeof( int));
	d->weighting = (float*) malloc (4096 * sizeof (long int));
	d->tailleFFT = 0;

	// inlet principal = x_in_harm ? pas besoin de le créé si oui	
	d->x_in_mod =inlet_new(&d->x_obj, &d->x_obj.ob_pd,
             &s_signal,  &s_signal);
    floatinlet_new(&d->x_obj,&d->f);
	d->x_in_mess=inlet_new(&d->x_obj, &d->x_obj.ob_pd,
             gensym("list"), gensym("messages"));// pas sur de la creation des inlet de type message
    d->x_out = outlet_new(&d->x_obj, &s_signal);
	
	return (void*) d;
	
}

/*
 * Q.1 - Chargement en mémoire des objets de type shaping~
 */
void            shaping_tilde_setup(void){
	shaping_tilde_class = class_new(gensym("shaping~"),(t_newmethod)shaping_tilde_new,(t_method)shaping_tilde_free,sizeof(t_shaping_tilde),CLASS_DEFAULT,A_GIMME,0);
  class_addmethod(shaping_tilde_class,(t_method)shaping_tilde_dsp,gensym("dsp"), 0); // mettre l'argument signal
  class_addmethod(shaping_tilde_class, (t_method)shaping_tilde_messages,
     gensym("messages"), A_DEFFLOAT, A_DEFFLOAT, 0);
  CLASS_MAINSIGNALIN(shaping_tilde_class,t_shaping_tilde,f);
	
}
