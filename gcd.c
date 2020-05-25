//
// Created by Kurt Lamon on 5/25/2020.
//

#include "m_pd.h"

static  t_class *gcd_class;

typedef struct _gcd {
    t_object    x_obj;
    t_float     gcd_val;
    t_float     val_A, val_B;
    t_outlet    *out_gcd;
} t_gcd;

void gcd_setVals(t_gcd *x, t_floatarg f1, t_floatarg f2) {
    x -> val_A = (f1 <= 0) ? 1 : f1;
    x -> val_B = (f2 <= 0) ? 1 : f2;
}

/**
 * Assign and store the incoming float value. Triggered when a float is received.
 *
 * @param x
 */
void gcd_onFloat(t_gcd *x) {
    post("[gcd] Float value received");
    // TODO: Receive and save float value
}

/**
 * Calculate and output the GCD of the stored float values. Triggered when a bang message is received.
 *
 * @param x
 */
void gcd_onBang(t_gcd *x) {
    outlet_float(x -> out_gcd, x -> gcd_val);
}

void *gcd_new(t_floatarg f1, t_floatarg f2) {
    t_gcd *x = (t_gcd *) pd_new(gcd_class);
    gcd_setVals(x, f1, f2);
    x -> out_gcd = outlet_new(&x -> x_obj, &s_float);

    return (void *) x;
}

void gcd_free(t_gcd *x) {
    outlet_free(x -> out_gcd);
}

void gcd_setup(void) {
    gcd_class = class_new(gensym("gcd"),
                          (t_newmethod) gcd_new,
                          (t_method) gcd_free,
                          sizeof(t_gcd),
                          A_DEFFLOAT,
                          A_DEFFLOAT,
                          0);
    class_addfloat(gcd_class, (t_method) gcd_onFloat);
    class_addbang(gcd_class, (t_method) gcd_onBang);
}