#ifndef SETTINGS
    #include "libs/settings.cpp"
#endif

#ifndef FIELD
    #include "libs/field.cpp"
#endif

#ifndef RANDOM
    #include "libs/random.cpp"
#endif

#ifndef UPDATE
    #include "libs/update.cpp"
#endif

#include <iostream>
#include <iomanip>


static void write_contour(field *f){
    std::ofstream contour_file(global_settings.contour_file);
    for (int i = 0; i < global_settings.nodes; i++){
        contour_file << f->get_t(i).real() << " " << f->get_t(i).imag() << std::endl;
    }
    contour_file.close();
}

static void print_starting_log(std::ofstream &log, settings set){
    log << "Starting simulation with the following settings:" << std::endl;
    log << "\t[FILES]" << std::endl;
    log << "Bterms_file:\t" << set.Bterms_file << std::endl;
    log << "output_file:\t" << set.output_file << std::endl;
    log << "contour_file:\t" << set.contour_file << std::endl;
    log << "log_file:\t" << set.log_file << std::endl;
    log << "\t[PARAMETERS]" << std::endl;
    log << "nodes:\t" << set.nodes << std::endl;
    log << "m2:\t" << set.m2 << std::endl;
    log << "lambda:\t" << set.lambda << std::endl;
    log << "alpha_noise:\t" << set.alpha_noise << std::endl;
    log << "t_max:\t" << set.t_max << std::endl;
    log << "k_cutoff:\t" << set.k_cutoff << std::endl;
    log << "n_point_Bterms:\t" << set.n_point_Bterms << std::endl;
    log << "max_Y:\t" << set.max_Y << std::endl;
    log << "contour_option:\t" << set.contour_option << std::endl;
    log << "epsilon:\t" << set.epsilon << std::endl;
    log << "n_trajectories:\t" << set.n_trajectories << std::endl;
    log << "therm_Langevintime:\t" << set.therm_Ltime << std::endl;
    log << "simulation_Langevintime:\t" << set.simulation_Ltime << std::endl;
    log << "ms_frequency:\t" << set.ms_frequency << std::endl;
    log << "beta:\t" << set.beta << std::endl;                std::cerr.precision(2);

    log << "F1:\t" << set.F1 << std::endl;
    log << "F2:\t" << set.F2 << std::endl;
}

int main(){
    read_settings("settings.txt");
    std::ofstream log_file(global_settings.log_file);
    print_starting_log(log_file, global_settings);
    field f(global_settings.nodes, global_settings.contour_option, global_settings.t_max, global_settings.beta, global_settings.F1, global_settings.F2);
    double t, dt, last_ms_time;
    int traj;
    write_contour(&f);
    std::ofstream output_file(global_settings.output_file);




    for(traj = 0; traj < global_settings.n_trajectories; traj++){

        log_file << "Starting trajectory " << traj << std::endl;
        log_file << "Starting thermalization" << std::endl;
        for(t = 0.; t < global_settings.therm_Ltime;){
            dt = update(&f, &global_settings);
            t += dt;
            //set precision to 2 decimal places
            std::cerr << "TRAJECTORY " << traj+1 << " THERMALIZATION   " << std::setw(5) << (t*100)/global_settings.therm_Ltime<< " %          \r";
        }
        std::cerr << std::endl;

        log_file << "Thermalization complete" << std::endl;
        log_file << "Starting simulation" << std::endl;
        std::cerr << "Thermalization complete" << std::endl;
        std::cerr << "Starting simulation" << std::endl;

        last_ms_time = 0.;
        for(t = 0.; t < global_settings.simulation_Ltime;){
            dt = update(&f, &global_settings);
            t += dt;
            if(t-last_ms_time > global_settings.ms_frequency){
                log_file << "Writing to file at t = " << t << std::endl;
                last_ms_time = t;
                output_file << t;
                for(int i = 0; i < global_settings.nodes; i++){
                    output_file  << "\t" << f.get_field(i).real() << "\t" << f.get_field(i).imag();
                }
                output_file << std::endl;
                std::cerr << "TRAJECTORY " << traj+1 << " SIMULATION   " << std::setw(5) << (t*100)/global_settings.simulation_Ltime<< " %          \r";
            }
        }
    }
    output_file.close();
    return 0;
}