void *cvode_kinetics_ptr;
int  cvode_test;
int  cvode_error;
int  cvode_n_user;
int cvode_n_reactions;
realtype cvode_step_fraction;
realtype cvode_rate_sim_time;
realtype cvode_rate_sim_time_start;
realtype cvode_last_good_time;
N_Vector cvode_last_good_y;
realtype cvode_prev_good_time;
N_Vector cvode_prev_good_y;
struct pp_assemblage *cvode_pp_assemblage_save;
struct s_s_assemblage *cvode_s_s_assemblage_save;

