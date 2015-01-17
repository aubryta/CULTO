

#ifndef _shaping_TILDE_H_
# define _shaping_TILDE_H_

# include "m_pd.h"

#ifndef PI
#define PI 3.141592653589793115997963468544185161590576171875
#endif

#ifndef TWOPI
#define TWOPI 6.28318530717958623199592693708837032318115234375
#endif

static t_class      *shaping_tilde_class;
typedef struct      _shaping_tilde
{
    t_object        x_obj;
    t_sample		f_spec;
    t_sample		f;
    t_outlet		*x_out;
	t_inlet         *x_in_mod;	// type signal
	t_inlet         *x_in_mess;
	int 			autonorm;
	int 			bypass;
	int             *bitshuffle;
    float           *weighting;
    float           *window;
	float			*buffer;
	int	 			tailleFFT;	// type message
	/*
    int             *bitshuffle;
    float           *weighting;
    float           *window;
	float			*buffer;
	int	 			tailleBuff;*/
}
t_shaping_tilde;

void shaping_tilde_messages(t_shaping_tilde *x, t_floatarg n, t_floatarg p);
/*
 * Q.5 - Fonction centrale effectuant le calcul
 */
t_int           *shaping_tilde_perform(t_int *w);

/*
 * Q.4 - Ajout de l'objet shaping~ à l'arbre de traitement DSP
 */
void            shaping_tilde_dsp(t_shaping_tilde *x, t_signal **sp);

/*
 * Q.3 - Libération de la mémoire de l'objet shaping~
 */
void            shaping_tilde_free(t_shaping_tilde *x);

/*
 * Q.2 - Création d'un nouvel objet shaping~
 */
void            *shaping_tilde_new(void);

/*
 * Q.1 - Chargement en mémoire des objets de type shaping~
 */
void            shaping_tilde_setup(void);

#endif