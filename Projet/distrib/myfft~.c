#include "myfft~.h"
#include "myfft_fft.h"
#include "m_pd.h"
#include <math.h>
/*
static t_class      *myfft_tilde_class;
typedef struct      _myfft_tilde
{
    t_object        x_obj;
    t_sample		f_pan;
    t_sample		f;
    t_outlet		*x_out;
    int             *bitshuffle;
    float           *weighting;
    float           *window;
	int	
}
t_myfft_tilde;*/

/*
 * Q.5 - Fonction centrale effectuant le calcul
 */
t_int           *myfft_tilde_perform(t_int *w){
	/*
	w[1] Pointeur vers l’objet fft
w[2] type t_sample* - Pointeur vers le buffer entrant dans le premier inlet
w[3] type t_sample* - Pointeur vers le buffer de sortie (celui qui contiendra le résultat de notre calcul)
w[4] type int - Taille des buffers (tous sont alloués par PureData à la même taille)
*/
int taille = (int) w[4];
  t_myfft_tilde *fft =(t_myfft_tilde*) w[1];
  float *buff = fft->buffer;
  int tailleBuffer= fft->tailleBuff;
  t_sample *buff_in1 = (t_sample*) w[2];
  t_sample *buff_out = (t_sample*) w[3];
  
  while (taille > 0){
	  *buff= *buff_in1;
	  buff++;
	  buff_in1++;
	  tailleBuffer++;
	  taille --;
  }
  
  if (tailleBuffer>=4096) {
	 float* window = fft->window;
	  // creation fenetre
	  int i = 0;
	  while (i<tailleBuffer){
		  *window = 0.54-0.46*(cos (2*PI* i/tailleBuffer));
		  window++;
	  }
	  init_rdft(4096, fft->bitshuffle, fft->weighting);	 
	  rdft (4096,1,buff,fft->bitshuffle,fft->weighting);
	  *buff_out = *buff;
	  fft->tailleBuff=0;
	  free(buff);
  }
	/* *  @param      n       : Size of the vector to transform
 *  @param      isgn    : Flag for signed (1) or unsigned (0) data
 *  @param      a       : Buffer of data to transform
 *  @param      ip      : Bitshuffling vector of size n*2 (to be inited with init_rdft)
 *  @param      ip      : Weighting vector of size n*2 (to be inited with init_rdft)*/
	 // rdft(int n, int isgn, float *a, int *ip, float *w) signature rdft
	 
	 
	 return w+5;
}

/*
 * Q.4 - Ajout de l'objet myfft~ à l'arbre de traitement DSP
 */
void            myfft_tilde_dsp(t_myfft_tilde *x, t_signal **sp){
	dsp_add(myfft_tilde_perform, 4, x,  
          sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

/*
 * Q.3 - Libération de la mémoire de l'objet myfft~
 */
void            myfft_tilde_free(t_myfft_tilde *x){
	outlet_free(x->x_out);   
	
}

/*
 * Q.2 - Création d'un nouvel objet myfft~
 */
 /*
 *
 *  La meilleure solution consiste à gérer les buffers soit-même. L’idée est donc d’allouer un buffer de la taille
voulue (4096 ou 8192 points) à la création de l’objet (*_new) puis de gérer l’arrivée de nouvelle données (lors
de l’appel à la méthode *_perform) sous forme de buffers circulaires. Ainsi l’arrivée d’un nouveau vecteur de
données décale l’ensemble des données existantes dans le buffer de 64 points et ajoute les nouvelles données
dans les 64 dernières positions du buffer. Si assez de données sont présentes, on lance alors le calcul de FFT.
Attention dans ce cas il faudra également gérer la libération de la mémoire associée au buffer.
*
*
*/
void            *myfft_tilde_new(void){
	t_myfft_tilde  *d;
	d = (t_myfft_tilde *)pd_new(myfft_tilde_class);
	// allocation du buffer de taille 4096 pour le future traitement
	d->buffer=(float*) malloc (4096 * sizeof(float));
	d->tailleBuff = 0;
    d->x_out = outlet_new(&d->x_obj, &s_signal);
	return (void*) d;
}

/*
 * Q.1 - Chargement en mémoire des objets de type myfft~
 */
 
 
void            myfft_tilde_setup(void){
	myfft_tilde_class = class_new(gensym("myfft~"),(t_newmethod)myfft_tilde_new,(t_method)myfft_tilde_free,sizeof(t_myfft_tilde),CLASS_DEFAULT,A_GIMME,0);
  class_addmethod(myfft_tilde_class,(t_method)myfft_tilde_dsp,gensym("dsp"), 0); // mettre l'argument signal
  CLASS_MAINSIGNALIN(myfft_tilde_class,t_myfft_tilde,f);
}

