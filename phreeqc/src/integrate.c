#define EXTERNAL extern
#include "global.h"
#include "phqalloc.h"
#include "output.h"
#include "phrqproto.h"

/*     $Date: 2004/10/06 16:20:47 $ */
static char const rcsid[] = "$RCSfile: integrate.c,v $  $Revision: 2.8 $";

#define MAX_QUAD 20
#define K_POLY 5
static LDBLE g_function(LDBLE x_value);
static LDBLE midpnt (LDBLE x1, LDBLE x2, int n);
static void polint(LDBLE *xa, LDBLE *ya, int n, LDBLE xv, LDBLE *yv, LDBLE *dy);
static LDBLE qromb_midpnt (LDBLE x1, LDBLE x2);
static LDBLE z, xd, alpha, G_TOL;
static struct surface_charge *surface_charge_ptr;

/* ---------------------------------------------------------------------- */
int calc_all_g( void )
/* ---------------------------------------------------------------------- */
{
	int i, j, k;
	int converge, converge1;
	int count_g, count_charge;
	LDBLE new_g, xd1;
	LDBLE epsilon;

	if (rcsid == NULL) fprintf(stderr," ");
	if (use.surface_ptr == NULL) return(OK);
/*
 *   calculate g for each surface
 */
#ifdef SKIP
	if (punch.high_precision == FALSE) {
		epsilon = 1e-8;
		G_TOL = 1e-9;
	} else {
		epsilon = 1.e-12;
		G_TOL = 1e-10;
	}
#endif
	epsilon = convergence_tolerance;
	if (convergence_tolerance >= 1e-8) {
		G_TOL = 1e-9;
	} else {
		G_TOL = 1e-10;
	}

	converge = TRUE;
	count_charge = 0;
	for (j = 0; j < count_unknowns; j++) {
		if (x[j]->type != SURFACE_CB) continue;
		if (debug_diffuse_layer == TRUE) output_msg(OUTPUT_MESSAGE, "Calc_all_g, X[%d]\n", j);
		surface_charge_ptr = x[j]->surface_charge;
		count_g = 1;
		x[j]->surface_charge->g[0].charge = 0.0;
		x[j]->surface_charge->g[0].g = 0.0;
		x[j]->surface_charge->g[0].dg = 0.0;
		xd = exp(-2 * x[j]->master[0]->s->la * LOG_10);
		/* alpha = 0.02935 @ 25;        (ee0RT/2)**1/2, (L/mol)**1/2 C / m**2 */
		/* 1000 J/kJ and 1000 L/m**3 */
		alpha = sqrt( EPSILON * EPSILON_ZERO * (R_KJ_DEG_MOL * 1000.0) * 1000.0 * tk_x * 0.5);
/*
 *   calculate g for given surface for each species
 */
		for (i = 0; i < count_s_x; i++) {
			if (s_x[i]->type > HPLUS) continue;
			for (k=0; k < count_g; k++) {
				if (equal(x[j]->surface_charge->g[k].charge, s_x[i]->z, G_TOL) == TRUE) {
					s_x[i]->diff_layer[count_charge].charge = x[j]->surface_charge;
					s_x[i]->diff_layer[count_charge].count_g = k;
					break;
				}
			} 
			if (k < count_g) continue;

			if (x[j]->surface_charge->grams > 0.0) {
				z = s_x[i]->z;
				if ((use.surface_ptr->only_counter_ions == FALSE) ||
				    (((x[j]->master[0]->s->la > 0)  && (z < 0)) || ((x[j]->master[0]->s->la < 0) && (z > 0)))) {
					if (xd > 0.1) {
						new_g = qromb_midpnt( 1.0, xd);
					} else if (xd > 0.01) {
						new_g = qromb_midpnt( 1.0, 0.1);
						new_g += qromb_midpnt( 0.1, xd);
					} else if (xd > 0.001) {
						new_g = qromb_midpnt( 1.0, 0.1);
						new_g += qromb_midpnt( 0.1, 0.01);
						new_g += qromb_midpnt( 0.01, xd);
					} else if (xd > 0.0001) {
						new_g = qromb_midpnt( 1.0, 0.1);
						new_g += qromb_midpnt( 0.1, 0.01);
						new_g += qromb_midpnt( 0.01, .001);
						new_g += qromb_midpnt( 0.001, xd);
					} else if (xd > 0.00001) {
						new_g = qromb_midpnt( 1.0, 0.1);
						new_g += qromb_midpnt( 0.1, 0.01);
						new_g += qromb_midpnt( 0.01, .001);
						new_g += qromb_midpnt( 0.001, .0001);
						new_g += qromb_midpnt( 0.0001, xd);
					} else if (xd > 0.000001) {
						new_g = qromb_midpnt( 1.0, 0.1);
						new_g += qromb_midpnt( 0.1, 0.01);
						new_g += qromb_midpnt( 0.01, .001);
						new_g += qromb_midpnt( 0.001, .0001);
						new_g += qromb_midpnt( 0.0001, .00001);
						new_g += qromb_midpnt( 0.00001, xd);
					} else if (xd > 0.0000001) {
						new_g = qromb_midpnt( 1.0, 0.1);
						new_g += qromb_midpnt( 0.1, 0.01);
						new_g += qromb_midpnt( 0.01, .001);
						new_g += qromb_midpnt( 0.001, .0001);
						new_g += qromb_midpnt( 0.0001, .00001);
						new_g += qromb_midpnt( 0.00001, .000001);
						new_g += qromb_midpnt( 0.000001, xd);
					} else if (xd > 0.00000001) {
						new_g = qromb_midpnt( 1.0, 0.1);
						new_g += qromb_midpnt( 0.1, 0.01);
						new_g += qromb_midpnt( 0.01, .001);
						new_g += qromb_midpnt( 0.001, .0001);
						new_g += qromb_midpnt( 0.0001, .00001);
						new_g += qromb_midpnt( 0.00001, .000001);
						new_g += qromb_midpnt( 0.000001, .0000001);
						new_g += qromb_midpnt( 0.0000001, xd);
					} else {
						new_g = qromb_midpnt( 1.0, 0.1);
						new_g += qromb_midpnt( 0.1, 0.01);
						new_g += qromb_midpnt( 0.01, .001);
						new_g += qromb_midpnt( 0.001, .0001);
						new_g += qromb_midpnt( 0.0001, .00001);
						new_g += qromb_midpnt( 0.00001, .000001);
						new_g += qromb_midpnt( 0.000001, .0000001);
						new_g += qromb_midpnt( 0.0000001, .00000001);
						new_g += qromb_midpnt( 0.00000001, xd);
					}
				} else {
					new_g = 0;
				}
			} else {
				new_g = 0.0;
			}
			if ((use.surface_ptr->only_counter_ions == TRUE) && new_g < 0) new_g = 0;
			x[j]->surface_charge->g[count_g].charge = s_x[i]->z;
			converge1 = TRUE;
			if (fabs(new_g) >= 1.) {
				if (fabs((new_g - x[j]->surface_charge->g[count_g].g)/new_g) > epsilon) {
					converge1 = FALSE;
				}
			} else {
				if (fabs(new_g - x[j]->surface_charge->g[count_g].g) > epsilon) {
					converge1 = FALSE;
				}
			}
			if (converge1 == FALSE) {
				converge = FALSE;
				if (debug_diffuse_layer == TRUE) {
					output_msg(OUTPUT_MESSAGE, "\t%12f\t%12.4e\t%12.4e\t%12.4e\n", 
						(double) x[j]->surface_charge->g[count_g].charge, 
						(double) x[j]->surface_charge->g[count_g].g, 
						(double) new_g,
						(double) (new_g - x[j]->surface_charge->g[count_g].g) );
				}
			}
			x[j]->surface_charge->g[count_g].g = new_g;
			if (new_g == 0) {
				x[j]->surface_charge->g[count_g].dg = 0;
			} else {
				if (x[j]->surface_charge->grams > 0.0) {
					x[j]->surface_charge->g[count_g].dg = surface_charge_ptr->grams * surface_charge_ptr->specific_area * alpha * g_function(xd) / F_C_MOL;
					x[j]->surface_charge->g[count_g].dg *=  -2. / 
						(exp(x[j]->master[0]->s->la * LOG_10) *
						 exp(x[j]->master[0]->s->la * LOG_10));
					if ((xd - 1) < 0.0) {
						x[j]->surface_charge->g[count_g].dg *= -1.0;
					}
					if (fabs(x[j]->surface_charge->g[count_g].dg) < 1e-8) {
						xd1 = exp(-2 * 1e-3 * LOG_10);

					
						new_g = qromb_midpnt( 1.0, xd1);
						x[j]->surface_charge->g[count_g].dg = new_g/.001;
					}
				} else {
					x[j]->surface_charge->g[count_g].dg = 0.0;
				}
			}
			s_x[i]->diff_layer[count_charge].charge = x[j]->surface_charge;
			s_x[i]->diff_layer[count_charge].count_g = count_g;
			count_g++;

		}
		if (debug_diffuse_layer == TRUE) {
			output_msg(OUTPUT_MESSAGE, "\nSurface component %d: charge,\tg,\tdg/dlny,\txd\n", count_charge);
			for (i = 0; i < count_g; i++) {
				output_msg(OUTPUT_MESSAGE, "\t%12f\t%12.4e\t%12.4e\t%12.4e\n", 
					(double) x[j]->surface_charge->g[i].charge, 
					(double) x[j]->surface_charge->g[i].g, 
					(double) x[j]->surface_charge->g[i].dg,
					(double) xd);
			}
		}
		count_charge++;
	}
	return (converge);
}
/* ---------------------------------------------------------------------- */
LDBLE g_function(LDBLE x_value)
/* ---------------------------------------------------------------------- */
{
	LDBLE sum, return_value, sum1;
	int i, j;
	LDBLE ln_x_value;

	if (equal(x_value, 1.0, G_TOL*100) == TRUE) return(0.0);
	sum = 0.0;
	ln_x_value = log(x_value);
	for (j = 0; j < use.surface_ptr->charge[0].count_g; j++) {
		use.surface_ptr->charge[0].g[j].psi_to_z = 
			exp(ln_x_value * use.surface_ptr->charge[0].g[j].charge) - 1.0;
	}
	for (i = 0; i < count_s_x; i++) {
		if (s_x[i]->type < H2O && s_x[i]->z != 0.0) {
			for(j = 0; j < use.surface_ptr->charge[0].count_g; j++) {
				if(use.surface_ptr->charge[0].g[j].charge == s_x[i]->z) { 
					sum += s_x[i]->moles * use.surface_ptr->charge[0].g[j].psi_to_z;
					break;
				}
			}
		}
	}
	if (sum < 0.0) {
		sum = 0.0;
		sum1 = 0.0;
		output_msg(OUTPUT_MESSAGE, "Species\tmoles\tX**z-1\tsum\tsum charge\n");
		for (i = 0; i < count_s_x; i++) {
			if (s_x[i]->type < H2O && s_x[i]->z != 0.0) {
				sum += s_x[i]->moles * (pow(x_value, s_x[i]->z) - 1.0);
				sum1 += s_x[i]->moles * s_x[i]->z;
				output_msg(OUTPUT_MESSAGE, "%s\t%e\t%e\t%e\t%e\n", s_x[i]->name, (double) s_x[i]->moles, (double) (pow(x_value, (double) s_x[i]->z) - 1.0), (double) sum, (double) sum1);
			}
		}
		sprintf(error_string, "Negative sum in g_function, %e\t%e.", (double) sum, (double) x_value);
		error_msg(error_string, CONTINUE);
		sprintf(error_string, "Solutions must be charge balanced, charge imbalance is %e\n", (double) sum1);
		error_msg(error_string, STOP);
	}

	return_value = ( exp (ln_x_value * z) - 1) /  sqrt((x_value * x_value * mass_water_aq_x * sum )); 
	return(return_value);
}
/* ---------------------------------------------------------------------- */
void polint(LDBLE *xa, LDBLE *ya, int n, LDBLE xv, LDBLE *yv, LDBLE *dy)
/* ---------------------------------------------------------------------- */
{
	int i, m, ns;
	LDBLE den, dif, dift, ho, hp, w;
	LDBLE *c, *d;

	ns = 1;
	dif = fabs(xv-xa[1]);
/*
 *   Malloc work space
 */
	c = PHRQ_malloc((size_t) (n + 1) * sizeof (LDBLE) );
	if (c == NULL) malloc_error();
	d = PHRQ_malloc((size_t) (n + 1) * sizeof (LDBLE) );
	if (d == NULL) malloc_error();

	

	for (i=1; i <= n; i++) {
		dift = fabs(xv-xa[i]);
		if ( dift < dif ) {
			ns = i;
			dif = dift;
		}
		c[i] = ya[i];
		d[i] = ya[i];
	}

	*yv = ya[ns--];
	for (m = 1; m < n; m++) {
		for (i=1; i <= n - m; i++) {
			ho = xa[i] - xv;
			hp = xa[i+m] - xv;
			w = c[i+1] - d[i];
			if ( (den=ho-hp) == 0.0) {
				error_msg("In subroutine polint.", STOP);
			}
			den = w/den;
			d[i] = hp*den;
			c[i] = ho*den;
		}
		if (2 * ns < (n-m) ) {
			*dy = c[ns+1];
		} else {
			*dy = d[ns--];
		}
		*yv += *dy;

/*		*yv += (*dy = (2 * ns < (n-m) ? c[ns+1] : d[ns--])); */
	}
	c = free_check_null(c);
	d = free_check_null(d);
	return;
}
/* ---------------------------------------------------------------------- */
LDBLE midpnt (LDBLE x1, LDBLE x2, int n)
/* ---------------------------------------------------------------------- */
{
	LDBLE xv, tnm, sum, del, ddel;
	static LDBLE sv;
	int it, j;

	if (n == 1) {
		sv = (x2-x1) * g_function(0.5 * (x1 + x2));
		return(sv);
	} else {
		for (it=1, j=1; j<n-1; j++) it *=3;
		tnm = (LDBLE) it;
		del = (x2 - x1) / (3*tnm);
		ddel = del + del;
		xv = x1 + 0.5 * del;
		sum = 0.0;
		for (j=1; j <= it; j++) {
#if defined(PHREEQCI_GUI)
			if (WaitForSingleObject(g_hKill /*g_eventKill*/, 0) == WAIT_OBJECT_0)
			{
				error_msg("Execution canceled by user.", CONTINUE);
				RaiseException(USER_CANCELED_RUN, 0, 0, NULL);
			}
#endif
			sum += g_function(xv);
			xv += ddel;
			sum += g_function(xv);
			xv += del;
		}
		sv = (sv + (x2 - x1) * sum / tnm) / 3.0;
		return sv;
	}
}
/* ---------------------------------------------------------------------- */
LDBLE qromb_midpnt (LDBLE x1, LDBLE x2)
/* ---------------------------------------------------------------------- */
{
	LDBLE ss, dss;
	LDBLE sv[MAX_QUAD + 2], h[MAX_QUAD + 2];
	int j;

	h[0] = 1.0;
	sv[0] = midpnt( x1, x2, 1);
	for (j = 1; j < MAX_QUAD; j++) {
		sv[j] = midpnt( x1, x2, j + 1);
		h[j] = h[j-1] / 9.0;

		if (fabs(sv[j]-sv[j-1]) <= G_TOL * fabs(sv[j]) ) {
			sv[j] *= surface_charge_ptr->grams * surface_charge_ptr->specific_area *
				alpha  / F_C_MOL;        /* (ee0RT/2)**1/2, (L/mol)**1/2 C / m**2 */
			if ((x2 - 1) < 0.0) sv[j] *= -1.0;
			if (debug_diffuse_layer == TRUE) {
				output_msg(OUTPUT_MESSAGE, "Iterations in qromb_midpnt: %d\n", j);
			}
			return(sv[j]);
		}

		if (j >= K_POLY - 1) {
			polint( &h[j - K_POLY], &sv[j - K_POLY], K_POLY, 0.0, &ss, &dss);
			if (fabs(dss) <= G_TOL * fabs(ss) || fabs(dss) < G_TOL) {
				ss *= surface_charge_ptr->grams * surface_charge_ptr->specific_area *
					alpha / F_C_MOL;  /* (ee0RT/2)**1/2, (L/mol)**1/2 C / m**2 */
				if ((x2 - 1) < 0.0) ss *= -1.0;
				if (debug_diffuse_layer == TRUE) {
					output_msg(OUTPUT_MESSAGE, "Iterations in qromb_midpnt: %d\n", j);
				}
				return(ss);
			}
		}

	}
	sprintf(error_string, "\nToo many iterations integrating diffuse layer.\n");
	error_msg(error_string, STOP);
	return(-999.9);
}
/* ---------------------------------------------------------------------- */
int calc_init_g(void) 
/* ---------------------------------------------------------------------- */
{
	int i, j, k;
	int count_g, count_charge;

	if (use.surface_ptr == NULL) return(OK);
	
/*
 *   calculate g for each surface
 */
	count_charge = 0;
	for (j = 0; j < count_unknowns; j++) {
		if (x[j]->type != SURFACE_CB) continue;
		surface_charge_ptr = x[j]->surface_charge;
		count_g = 0;
		if (x[j]->surface_charge->g != NULL) {
			count_g = x[j]->surface_charge->count_g;
		}
		if( count_g == 0 ) {
			x[j]->surface_charge->g = PHRQ_malloc((size_t) sizeof(struct surface_diff_layer));
			if (x[j]->surface_charge->g == NULL) malloc_error();
			x[j]->surface_charge->g[0].charge = 0.0;
			x[j]->surface_charge->g[0].g = 0.0;
			x[j]->surface_charge->g[0].dg = 0.0;
			xd = exp(-2 * x[j]->master[0]->s->la * LOG_10);
			/* alpha = 0.02935 @ 25;        (ee0RT/2)**1/2, (L/mol)**1/2 C / m**2 */
			/*  second 1000 is liters/m**3 */
			alpha = sqrt( EPSILON * EPSILON_ZERO * (R_KJ_DEG_MOL * 1000.0) * 1000.0 * tk_x * 0.5);
		}
/*
 *   calculate g for given surface for each species
 */
		count_g = 1;
		for (i = 0; i < count_s_x; i++) {
			if (s_x[i]->type > HPLUS) continue;
			for (k=0; k < count_g; k++) {
				if (equal(x[j]->surface_charge->g[k].charge, s_x[i]->z, G_TOL) == TRUE) {
					s_x[i]->diff_layer[count_charge].charge = x[j]->surface_charge;
					s_x[i]->diff_layer[count_charge].count_g = k;
					s_x[i]->diff_layer[count_charge].g_moles = 0.0;
					s_x[i]->diff_layer[count_charge].dg_g_moles = 0.0;
					break;
				}
			} 
			if (k >= count_g) {

				/* malloc space to save g for charge */
				x[j]->surface_charge->g = PHRQ_realloc(x[j]->surface_charge->g, 
							     (size_t) (count_g + 1) * 
							     sizeof(struct surface_diff_layer));
				if (x[j]->surface_charge->g == NULL) malloc_error();
				
				/* save g for charge */
				x[j]->surface_charge->g[count_g].charge = s_x[i]->z;
				if (x[j]->surface_charge->grams > 0.0) {
					x[j]->surface_charge->g[count_g].g = 2 * alpha * sqrt(mu_x) * (pow(xd, s_x[i]->z / 2.0) - 1) *surface_charge_ptr->grams * surface_charge_ptr->specific_area / F_C_MOL;
					x[j]->surface_charge->g[count_g].dg = -s_x[i]->z;
					if ((use.surface_ptr->only_counter_ions == TRUE) && 
					    x[j]->surface_charge->g[count_g].g < 0) {
						x[j]->surface_charge->g[count_g].g = 0;
						x[j]->surface_charge->g[count_g].dg = 0;
					}
				} else {
					x[j]->surface_charge->g[count_g].g = 0.0;
					x[j]->surface_charge->g[count_g].dg = -s_x[i]->z;
				}
				/* save g for species */
				s_x[i]->diff_layer[count_charge].charge = x[j]->surface_charge;
				s_x[i]->diff_layer[count_charge].count_g = count_g;
				s_x[i]->diff_layer[count_charge].g_moles = 0.0;
				s_x[i]->diff_layer[count_charge].dg_g_moles = 0.0;
				count_g++;
			}
		}
		if (debug_diffuse_layer == TRUE) {
			output_msg(OUTPUT_MESSAGE, "\nSurface component %d: charge,\tg,\tdg\n", count_charge);
			for (i = 0; i < count_g; i++) {
				output_msg(OUTPUT_MESSAGE, "\t%12f\t%12.4e\t%12.4e\n", 
					(double) x[j]->surface_charge->g[i].charge, (double) x[j]->surface_charge->g[i].g, (double) x[j]->surface_charge->g[i].dg );
			}
		}
		count_charge++;
		x[j]->surface_charge->count_g = count_g;
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
int initial_surface_water(void) 
/* ---------------------------------------------------------------------- */
{
/*
 *   In initial surface calculation, need to calculate
 *   mass of water in diffuse layer.
 *   diffuse layer water + aqueous solution water = bulk water.
 *   Ionic strength is fixed, so diffuse-layer water will not change 
 */
	int i;
/*#define DEBYE*/
#ifdef DEBYE
	LDBLE debye_length;
	LDBLE volume_diffuse_layer;
#endif
	LDBLE mass_water_surface;
/*
 *   Debye  length = 1/k = sqrt[eta*eta_zero*R*T/(2*F**2*mu_x*1000)], Dzombak and Morel, p 36
 *
 *   1000 converts kJ to J; 1000 converts Liters to meter**3; debye_length is in meters.
 */
#ifdef DEBYE
	debye_length = (EPSILON * EPSILON_ZERO * R_KJ_DEG_MOL * 1000.0 * tk_x) 
		/ (2. * F_C_MOL * F_C_MOL * mu_x * 1000.);
	debye_length = sqrt(debye_length);
#endif
/*
 *   Loop through all surface components, calculate each H2O surface (diffuse layer),
 *   H2O aq, and H2O bulk (diffuse layers plus aqueous).
 */
	mass_water_surfaces_x = 0.0;
	for (i = 0; i < count_unknowns; i++) {
		if (x[i]->type != SURFACE_CB) continue;
#ifdef DEBYE
		/* 1000 converts volume from m**3 to Liters */
		volume_diffuse_layer = x[i]->surface_charge->specific_area *
			x[i]->surface_charge->grams * debye_length * 1000;
			
		/*   Assume mol/L = mol/kgw */
		mass_water_surface = volume_diffuse_layer;
#else
		/* make constant thickness of, default 1e-8 m (100 Angstroms) */
		mass_water_surface =  x[i]->surface_charge->specific_area *
			x[i]->surface_charge->grams * use.surface_ptr->thickness * 1000;
#endif
		x[i]->surface_charge->mass_water = mass_water_surface;
		mass_water_surfaces_x += mass_water_surface;
	}
	mass_water_bulk_x = mass_water_aq_x + mass_water_surfaces_x;
	return(OK);
}
/* ---------------------------------------------------------------------- */
int sum_diffuse_layer(struct surface_charge *surface_charge_ptr1)
/* ---------------------------------------------------------------------- */
{
	int i, j, count_g;
	LDBLE mass_water_surface;
	LDBLE molality, moles_excess, moles_surface;

	if (use.surface_ptr == NULL) return(OK);
/*
 *   Find position of component in list of components
 */	
	i = 0;

	for (j =0; j < use.surface_ptr->count_charge; j++) {
		if (&(use.surface_ptr->charge[j]) == surface_charge_ptr1) {
			i = j;
			break;
		}
	}
	if (j >= use.surface_ptr->count_charge) {
	        sprintf(error_string, "In sum_diffuse_layer, component not found, %s.", surface_charge_ptr1->name);
		error_msg(error_string, STOP);
	}
/*
 *   Loop through all surface components, calculate each H2O surface (diffuse layer),
 *   H2O aq, and H2O bulk (diffuse layers plus aqueous).
 */
	count_elts = 0;
	paren_count = 0;
	mass_water_surface = surface_charge_ptr1->mass_water;
	for (j = 0; j < count_s_x; j++) {
		if (s_x[j]->type > HPLUS) continue;
		molality = under(s_x[j]->lm);
		count_g = s_x[j]->diff_layer[i].count_g;
#ifdef SKIP
		moles_excess = mass_water_bulk_x * 
/*			s_x[j]->diff_layer[i].charge->g[count_g].g * molality; */
			surface_charge_ptr1->g[count_g].g * molality;
#endif
		moles_excess = mass_water_aq_x * molality * surface_charge_ptr1->g[count_g].g;
		
		moles_surface = mass_water_surface * molality + moles_excess;
/*
 *   Accumulate elements in diffuse layer
 */
		add_elt_list(s_x[j]->next_elt, moles_surface);
	}
	add_elt_list(s_h2o->next_elt, mass_water_surface / gfw_water);

	if (count_elts > 0 ) {
		qsort (elt_list, (size_t) count_elts,
		       (size_t) sizeof(struct elt_list), elt_list_compare);
		elt_list_combine();
	}
	return(OK);
}
