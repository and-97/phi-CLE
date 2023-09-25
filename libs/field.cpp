#define FIELD

// Path: libs/field.cpp
#include <complex.h>
#include <iostream>
#include <fstream>

#ifndef RANDOM
    #include "random.cpp"
#endif



#define NINETYNINETH 0.101010101010101010101010101010101010101010101010101010101010101010101010101010101010101

static void set_contour(int contour_option, double t_max, double beta, std::complex<double> *t, std::complex<double> **_dt, int _N){
    if(contour_option == 1){
        for (int i = 0; i < _N/2; i++){
            t[i] = std::complex<double>(((double)i/(_N/2))*t_max , -(i*0.01)*beta/(_N/2));
            t[_N/2+i] = std::complex<double>(t_max*(1-((double)i/(_N/2))), -beta*(0.01 + (((double)i)/(_N/2)*(1-0.01))));
        }
        for(int i = 1; i < _N-1; i++){
            _dt[i][0] = t[i] - t[i-1];
            _dt[i][1] = t[i+1] - t[i];
        }
        _dt[0][0] = t[1] - t[0];
        _dt[0][1] = _dt[0][0];
        _dt[_N-1][0] = t[_N-1] - t[_N-2];
        _dt[_N-1][1] = _dt[_N-1][0];
    }else{
        std::cerr << "Error: contour option not recognized\n contour option ---> " << contour_option << "\n Available contour options ---> 1" << std::endl;
        exit(1);
    }
} 

class field {
    private:
        int _N;
        std::complex<double> *_field;
        std::complex<double> *_t;
        std::complex<double> **_dt;
        std::complex<double> *_k;
        std::complex<double> *_noise;
        std::complex<double> _k_max;
        double _F1;
        double _F2;
    public:
        field(int N, int contour_option, double t_max, double beta, double F1, double F2);
        ~field();
        void set_field(std::complex<double> field, int site);
        void set_k(std::complex<double> k, int site);
        void set_noise(std::complex<double> noise, int site);
        void find_k_max();
        std::complex<double> get_field(int site);
        std::complex<double> get_t(int site);
        int get_N(){return _N;};
        std::complex<double> get_dt(int site, char direction);
        std::complex<double> get_k(int site);
        std::complex<double> get_noise(int site);
        std::complex<double> get_k_max();
        std::complex<double> getPrevField(int site);
        std::complex<double> getNextField(int site);
        double field_norm();
        std::complex<double> get_drift(int site, double m2, double lambda);
};

field::field(int N, int contour_option, double t_max, double beta, double F1, double F2){
    _N = N;
    _field = new std::complex<double>[_N];
    _t = new std::complex<double>[_N];
    //allocate in _dt an array of 2*N complex numbers
    _dt = new std::complex<double>*[_N];
    _k = new std::complex<double>[_N];
    _noise = new std::complex<double>[_N];
    for (int i = 0; i < _N; i++) {
        _field[i] = (rand_normal(0, .5), rand_normal(0, .5));
        _k[i] = 0;
        _noise[i] = 0;
        _dt[i] = new std::complex<double>[2];
        for(int j = 0; j < 2; j++){
            _dt[i][j] = 0;
        }
    }
    _k_max = 0;
    set_contour(contour_option, t_max, beta, _t, _dt, _N);
    _F1 = F1;
    _F2 = F2;
}

field::~field() {
    delete[] _field;
    delete[] _t;
    for (int i = 0; i < _N; i++) {
        delete[] _dt[i];
    }
    delete[] _dt;
    delete[] _k;
    delete[] _noise;
}

void field::set_field(std::complex<double> field, int site) {
    _field[site] = field;
    if(site >= _N || site < 0){
        std::cerr << "[field::set_field] Error: site out of range" << std::endl;
        exit(1);
    }
}

void field::set_k(std::complex<double> k, int site) {
    if(site >= _N || site < 0){
        std::cerr << "[field::set_k] Error: site out of range" << std::endl;
        exit(1);
    }
    _k[site] = k;
}

void field::set_noise(std::complex<double> noise, int site) {
    _noise[site] = noise;
    if(site >= _N || site < 0){
        std::cerr << "[field::set_noise] Error: site out of range" << std::endl;
        exit(1);
    }
}

void field::find_k_max() {
    for (int i = 0; i < _N/2; i++) {
        if (abs(_k[i]) > abs(_k_max)) {
            _k_max = abs(_k[i]);
        }
    }
}

std::complex<double> field::get_field(int site) {
    if(site >= _N || site < 0){
        std::cerr << "[field::get_field] Error: site out of range" << std::endl;
        exit(1);
    }
    return _field[site];
}

std::complex<double> field::get_t(int site) {
    if(site >= _N || site < 0){
        std::cerr << "[field::get_t] Error: site out of range" << std::endl;
        exit(1);
    }
    return _t[site];
}

std::complex<double> field::get_dt(int site, char direction) {
    if(site >= _N || site < 0){
        std::cerr << "[field::get_dt] Error: site out of range" << std::endl;
        exit(1);
    }
    if(direction != 'b' && direction != 'f'){
        std::cerr << "[field::get_dt] Error: direction out of range\n Usage:\n 'f' ---> forward time difference\n 'b' ---> backwards time difference\n" << std::endl;
        exit(1);
    }
    if(direction == 'b'){
        return _dt[site][0];
    }
    else if (direction == 'f'){
        return _dt[site][1];
    }
    return 0;
}

std::complex<double> field::get_k(int site) {
    if(site >= _N || site < 0){
        std::cerr << "[field::get_k] Error: site out of range" << std::endl;
        exit(1);
    }
    return _k[site];
}

std::complex<double> field::get_noise(int site) {
    if(site >= _N || site < 0){
        std::cerr << "[field::get_noise] Error: site out of range" << std::endl;
        exit(1);
    }
    return _noise[site];
}

std::complex<double> field::get_k_max() {
    return _k_max;
}

std::complex<double> field::getPrevField(int site) {
    if(site >= _N || site < 0){
        std::cerr << "[field::getPrevField] Error: site out of range" << std::endl;
        exit(1);
    }
    if(site == 0){
        return _field[_N-1];
    }
    else{
        return _field[site-1];
    }
}

std::complex<double> field::getNextField(int site) {
    if(site >= _N || site < 0){
        std::cerr << "[field::getNextField] Error: site out of range" << std::endl;
        exit(1);
    }
    if(site == _N-1){
        return _field[0];
    }
    else{
        return _field[site+1];
    }
}

double field::field_norm(){
    double norm = 0;
    for(int i = 0; i < _N; i++){
        norm += _F1*(_field[i].real()*_field[i].real()) + _F2*(_field[i].imag()*_field[i].imag());
    }
    return norm;
}

std::complex<double> field::get_drift(int site, double m2, double lambda){
    std::complex<double> terms[2] = {0};

    terms[0] += (_field[(site+1)%_N] - _field[site])/(_dt[site][1]);
    if(site == 0){
        terms[0] += (_field[_N-1] - _field[site])/(_dt[site][0]);
    }else{
        terms[0] += (_field[site-1] - _field[site])/(_dt[site][0]);
    }


    terms[1] += m2*_dt[site][1]*_field[site];
    terms[1] += lambda*_dt[site][1]*_field[site]*_field[site]*_field[site];

    return terms[0] - terms[1];
}
