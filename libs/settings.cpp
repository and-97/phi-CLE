#define SETTINGS

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>


#define SQRT_PI 1.7724538509055160272981674833411451827975494561223871282138
#define ONE_SIXTH 0.1666666666
#define SQRT_2 1.41421356237309504880168872420969807856967187537694807317667973799

struct settings
{   
    double epsilon;
    double beta;
    int nodes;
    int therm_Ltime;
    int simulation_Ltime;
    double ms_frequency;
    int n_trajectories;
    int contour_option;
    double F1;
    double F2;

    double m2;
    double lambda;

    double alpha_noise;

    int t_max;
    int k_cutoff;

    int n_point_Bterms;
    int max_Y;

    //string that stores the name of the settings file 
    std::string settings_file;
    //string that stores the name of the output file
    std::string output_file;
    //string that stores the name of the Bterms file
    std::string Bterms_file;
    //string that stores the name of the contour file
    std::string contour_file;  
    //string that stores the name of the log file
    std::string log_file;
};

static settings global_settings = {0};

void read_settings(std::string settings_file){   
        std::ifstream settings_file_stream;
        settings_file_stream.open(settings_file);
    
        std::string line;
        std::string word;
    
        while (std::getline(settings_file_stream, line)){
            std::stringstream ss(line);
            ss >> word;
            if (word == "epsilon"){
                ss >> global_settings.epsilon;
            }
            else if (word == "beta"){
                ss >> global_settings.beta;
            }
            else if (word == "nodes"){
                ss >> global_settings.nodes;
            }
            else if (word == "therm_Langevintime"){
                ss >> global_settings.therm_Ltime;
            }
            else if (word == "simulation_Langevintime"){
                ss >> global_settings.simulation_Ltime;
            }
            else if (word == "ms_frequency"){
                ss >> global_settings.ms_frequency;
            }
            else if (word == "n_trajectories"){
                ss >> global_settings.n_trajectories;
            }
            else if (word == "m2"){
                ss >> global_settings.m2;
            }
            else if (word == "lambda"){
                ss >> global_settings.lambda;
            }
            else if (word == "alpha_noise"){
                ss >> global_settings.alpha_noise;
            }
            else if (word == "t_max"){
                ss >> global_settings.t_max;
            }
            else if (word == "k_cutoff"){
                ss >> global_settings.k_cutoff;
            }
            else if (word == "n_point_Bterms"){
                ss >> global_settings.n_point_Bterms;
            }
            else if (word == "max_Y"){
                ss >> global_settings.max_Y;
            }
            else if (word == "Bterms_file"){
                ss >> global_settings.Bterms_file;
            }
            else if (word == "output_file"){
                ss >> global_settings.output_file;
            }
            else if (word == "contour_file"){
                ss >> global_settings.contour_file;
            }
            else if(word == "contour_option"){
                ss >> global_settings.contour_option;
            }
            else if(word == "log_file"){
                ss >> global_settings.log_file;
            }
            else if(word == "F1"){
                ss >> global_settings.F1;
            }
            else if(word == "F2"){
                ss >> global_settings.F2;
            }
            else{
                std::cout << "Error: " << word << " is not a valid setting" << std::endl;
            }
        }
        settings_file_stream.close();
//check if the settings are correct. Settings that are not provided by the user are set to default values
        if (global_settings.epsilon == 0){
            global_settings.epsilon = 0.0005;
        }
        if (global_settings.beta == 0){
            global_settings.beta = 1;
        }
        if (global_settings.nodes == 0){
            global_settings.nodes = 40;
        }
        if (global_settings.therm_Ltime == 0){
            global_settings.therm_Ltime = 40;
        }
        if (global_settings.simulation_Ltime == 0){
            global_settings.simulation_Ltime = 2500;
        }
        if (global_settings.ms_frequency == 0){
            global_settings.ms_frequency = 0.001;
        }
        if (global_settings.n_trajectories == 0){
            global_settings.n_trajectories = 20;
        }
        if (global_settings.m2 == 0){
            global_settings.m2 = 1;
        }
        if (global_settings.lambda == 0){
            global_settings.lambda = 24;
        }
        if (global_settings.alpha_noise == 0){
            global_settings.alpha_noise = 0.5;
        }
        if (global_settings.t_max == 0){
            global_settings.t_max = 1;
        }
        if (global_settings.k_cutoff == 0){
            global_settings.k_cutoff = 20000000;
        }
        if (global_settings.n_point_Bterms == 0){
            global_settings.n_point_Bterms = 500;
        }
        if (global_settings.max_Y == 0){
            global_settings.max_Y = 100;
        }
        if (global_settings.Bterms_file == ""){
            global_settings.Bterms_file = "Bterms.dat";
        }
        if (global_settings.output_file == ""){
            global_settings.output_file = "output.dat";
        }
        if (global_settings.contour_file == ""){
            global_settings.contour_file = "contour.dat";
        }
        if (global_settings.contour_option == 0){
            global_settings.contour_option = 1;
        }
        if (global_settings.log_file == ""){
            global_settings.log_file = "log.dat";
        }
        if (global_settings.F1 == 0){
            global_settings.F1 = 0;
        }
        if (global_settings.F2 == 0){
            global_settings.F2 = 1;
        }
}



void print_settings(){
    std::cout << "\t[FILES]" << std::endl;
    std::cout << "Bterms_file:\t" << global_settings.Bterms_file << std::endl;
    std::cout << "output_file:\t" << global_settings.output_file << std::endl;
    std::cout << "contour_file:\t" << global_settings.contour_file << std::endl;
    std::cout << "\n\t[SIMULATION PARAMETERS]" << std::endl;
    std::cout << "nodes:\t" << global_settings.nodes << std::endl;
    std::cout << "epsilon:\t" << global_settings.epsilon << std::endl;
    std::cout << "n_trajectories:\t" << global_settings.n_trajectories << std::endl;
    std::cout << "therm_Langevintime:\t" << global_settings.therm_Ltime << std::endl;
    std::cout << "simulation_Langevintime:\t" << global_settings.simulation_Ltime << std::endl;
    std::cout << "ms_frequency:\t" << global_settings.ms_frequency << std::endl;
    std::cout << "alpha_noise:\t" << global_settings.alpha_noise << std::endl;
    std::cout << "k_cutoff:\t" << global_settings.k_cutoff << std::endl;
    std::cout << "n_point_Bterms:\t" << global_settings.n_point_Bterms << std::endl;
    std::cout << "max_Y:\t" << global_settings.max_Y << std::endl;
    std::cout << "\n\t[PHYSICAL PARAMETERS]" << std::endl;
    std::cout << "beta:\t" << global_settings.beta << std::endl;
    std::cout << "t_max:\t" << global_settings.t_max << std::endl;
    std::cout << "contour_option:\t" << global_settings.contour_option << std::endl;
    std::cout << "m2:\t" << global_settings.m2 << std::endl;
    std::cout << "lambda:\t" << global_settings.lambda << std::endl;
}

