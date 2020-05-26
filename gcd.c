#include "m_pd.h"

static  t_class *gcd_class;

typedef struct _gcd {
    t_object    x_obj;
    t_float     gcd_val;
    t_float     val_A, val_B;
    t_inlet     *in_val_A, *in_val_B;
    t_outlet    *out_gcd;
} t_gcd;

int gcd(int a, int b) {
    if (a == 0) {
        return b;
    }
    if (b == 0) {
        return a;
    }
    if (a == b) {
        return a;
    }
    if (a > b) {
        return gcd(a - b, b);
    }
    return gcd(a, b - a);
}

void gcd_setVals(t_gcd *x, t_floatarg f1, t_floatarg f2) {
    x->val_A = (f1 <= 0) ? 1 : f1;
    x->val_B = (f2 <= 0) ? 1 : f2;
}

void gcd_resetVal(t_gcd *x) {
    x->gcd_val = 0;
}

/**
 * Calculate and output the GCD of the stored float values. Triggered when a bang message is received.
 *
 * @param x
 */
void gcd_onBang(t_gcd *x) {
    t_int val_A = x->val_A;
    t_int val_B = x->val_B;
    x->gcd_val = gcd(val_A, val_B);
    outlet_float(x->out_gcd, x->gcd_val);
}

void gcd_onReset(t_gcd *x) {
    gcd_resetVal(x);
}

void gcd_onList(t_gcd *x, t_symbol *s, t_int argc, t_atom *argv) {
    switch(argc) {
        case 2:
            gcd_setVals(x, atom_getfloat(argv), atom_getfloat(argv + 1));
            gcd_resetVal(x);
            break;
        default:
            error("[gcd]: received incorrect number of arguments to calculate new GCD");
    }
}

void gcd_onSet_A(t_gcd *x, t_floatarg f) {
    gcd_setVals(x, f, x->val_B);
}

void gcd_onSet_B(t_gcd *x, t_floatarg f) {
    gcd_setVals(x, x->val_A, f);
}

void *gcd_new(t_floatarg f1, t_floatarg f2) {
    t_gcd *x = (t_gcd *) pd_new(gcd_class);
    gcd_setVals(x, f1, f2);
    x->out_gcd = outlet_new(&x->x_obj, &s_float);

    x->in_val_A = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("inVal_A"));
    x->in_val_B = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("inVal_B"));

    return (void *) x;
}

void gcd_free(t_gcd *x) {
    outlet_free(x->out_gcd);
}

void gcd_setup(void) {
    gcd_class = class_new(gensym("gcd"),
                          (t_newmethod) gcd_new,
                          (t_method) gcd_free,
                          sizeof(t_gcd),
                          A_DEFFLOAT,
                          A_DEFFLOAT,
                          0);

    class_addbang(gcd_class, (t_method) gcd_onBang);
    class_addlist(gcd_class, (t_method) gcd_onList);
    class_addmethod(gcd_class, (t_method) gcd_onReset, gensym("reset"), 0);
    class_addmethod(gcd_class, (t_method) gcd_onSet_A, gensym("inVal_A"), A_DEFFLOAT, 0);
    class_addmethod(gcd_class, (t_method) gcd_onSet_B, gensym("inVal_B"), A_DEFFLOAT, 0);
}