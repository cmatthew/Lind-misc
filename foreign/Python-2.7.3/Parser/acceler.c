
/* Parser accelerator module */

/* The parser as originally conceived had disappointing performance.
   This module does some precomputation that speeds up the selection
   of a DFA based upon a token, turning a search through an array
   into a simple indexing operation.  The parser now cannot work
   without the accelerators installed.  Note that the accelerators
   are installed dynamically when the parser is initialized, they
   are not part of the static data structure written on graminit.[ch]
   by the parser generator. */

#include "pgenheaders.h"
#include "grammar.h"
#include "node.h"
#include "token.h"
#include "parser.h"


#define DBP(...) printf(">>> DBP - %s:%d - ", __FILE__,__LINE__); printf(__VA_ARGS__); fflush(stdout);


/* Forward references */
static void fixdfa(grammar *, dfa *);
static void fixstate(grammar *, state *);

void
PyGrammar_AddAccelerators(grammar *g)
{
    dfa *d;
    int i;

    DBP("Adding accelerators\n");
    DBP("grammar:%p, g_gdfa:%p, g_ndfas:%d\n", (void*) g, (void*)g->g_dfa, g->g_ndfas);

    d = g->g_dfa;
    for (i = g->g_ndfas; --i >= 0; d++) {
        DBP("fixing_dfa %d %p\n",g->g_ndfas - i, (void*)d);
        fixdfa(g, d);
    }
    g->g_accel = 1;
}

void
PyGrammar_RemoveAccelerators(grammar *g)
{
    dfa *d;
    int i;
    g->g_accel = 0;
    d = g->g_dfa;
    for (i = g->g_ndfas; --i >= 0; d++) {
        state *s;
        int j;
        s = d->d_state;
        for (j = 0; j < d->d_nstates; j++, s++) {
            if (s->s_accel)
                PyObject_FREE(s->s_accel);
            s->s_accel = NULL;
        }
    }
}

static void
fixdfa(grammar *g, dfa *d)
{
    state *s;
    int j;

    s = d->d_state;
    for (j = 0; j < d->d_nstates; j++, s++) {
        DBP("fixing_state %d %p\n", j, (void*)s);
	//(void) usleep(100000); // sleep .1 secs... didn't work
        fixstate(g, s);
    }
}

static void
fixstate(grammar *g, state *s)
{
    arc *a;
    int k;
    int *accel;
    int nl = g->g_ll.ll_nlabels;

    DBP("fix it\n");

    s->s_accept = 0;
    accel = (int *) PyObject_MALLOC(nl * sizeof(int));
    if (accel == NULL) {
        fprintf(stderr, "no mem to build parser accelerators\n");
        exit(1);
    }
    DBP("fix for k, ln = %d\n", nl);
    for (k = 0; k < nl; k++) {
      //  DBP("k: %d\n", k);
        accel[k] = -1;
    }
    DBP("fixed for k %d\n", k);
    a = s->s_arc;

    DBP("narcs!\n");
    for (k = s->s_narcs; --k >= 0; a++) {
      //DBP("narcs: %d\n",s->s_narcs - k);
        int lbl = a->a_lbl;
        label *l = &g->g_ll.ll_label[lbl];
        int type = l->lb_type;
        if (a->a_arrow >= (1 << 7)) {
            printf("XXX too many states!\n");
            continue;
        }
        if (ISNONTERMINAL(type)) {
            dfa *d1 = PyGrammar_FindDFA(g, type);
            int ibit;
            if (type - NT_OFFSET >= (1 << 7)) {
                printf("XXX too high nonterminal number!\n");
                continue;
            }
            for (ibit = 0; ibit < g->g_ll.ll_nlabels; ibit++) {
	      //DBP("testing ibit: %d with %d\n", ibit, d1->d_first);
	      //DBP("result %d\n", testbit(d1->d_first, ibit));
                if (testbit(d1->d_first, ibit)) {
		  //DBP("accel -1?\n");
                    if (accel[ibit] != -1) {
                        printf("XXX ambiguity!\n");
		    }
		    //DBP("bit operations\n");
                    accel[ibit] = a->a_arrow | (1 << 7) |
                        ((type - NT_OFFSET) << 8);
                }
		//DBP("Next ibit using %p %d!!\n", g->g_ll, g->g_ll.ll_nlabels);
            }
	    DBP("Done ibits!\n");
        }
        else if (lbl == EMPTY)
            s->s_accept = 1;
        else if (lbl >= 0 && lbl < nl)
            accel[lbl] = a->a_arrow;
    }
    DBP("accels\n");
    while (nl > 0 && accel[nl-1] == -1) {
      //DBP("nl: %d\n", nl);
        nl--;
    }
    for (k = 0; k < nl && accel[k] == -1;) {
      //DBP("k: %d\n",k);
        k++;
    }
    if (k < nl) {
        DBP("k < nl\n");
        int i;
        s->s_accel = (int *) PyObject_MALLOC((nl-k) * sizeof(int));
        if (s->s_accel == NULL) {
            fprintf(stderr, "no mem to add parser accelerators\n");
            exit(1);
        }
        s->s_lower = k;
        s->s_upper = nl;
        for (i = 0; k < nl; i++, k++) {
	  //DBP("i: %d\n", i);
	    s->s_accel[i] = accel[k];
	}
    }
    DBP("accel free\n");
    PyObject_FREE(accel);
}
