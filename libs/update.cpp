#define UPDATE

#ifndef FIELD
    #include "field.cpp"
#endif

#ifndef SETTINGS
    #include "settings.cpp"
#endif

#ifndef RANDOM
    #include "random.cpp"
#endif

static struct avg_k{
    double avg_k = 0;
    int i = 0;
}k_avg;

static std::complex<double> noise(double alpha_noise){
    double real = std::sqrt(alpha_noise)*rand_normal(0, SQRT_PI/SQRT_2);
    double tmp = rand_normal(0, SQRT_PI/SQRT_2);
    double imag = std::sqrt(1-alpha_noise)*rand_normal(0, SQRT_PI/SQRT_2);
    return std::complex<double>(real, imag);
}

double update(field *f, settings *s){
    double dt = s->epsilon;
    std::complex<double> k_max;
    std::complex<double> k_tmp;
    std::complex<double> field_tmp;
    for(int i = 0; i < f->get_N();i++){
        k_tmp = f->get_drift(i, s->m2, s->lambda);
        f->set_k(k_tmp,i);
        f->set_noise(noise(s->alpha_noise),i);
    }
    f->find_k_max();

    k_max = f->get_k_max();

    k_avg.avg_k *= k_avg.i;
    k_avg.avg_k += abs(k_max);
    k_avg.i++;
    k_avg.avg_k /= k_avg.i;

    dt *= (k_avg.avg_k/abs(k_max));

    for(int i = 0; i < f->get_N();i++){
        field_tmp = f->get_field(i);
        field_tmp += f->get_k(i)*dt;
        field_tmp += f->get_noise(i)*std::sqrt(dt);
        f->set_field(field_tmp,i);
        if(std::isnan(f->get_field(i).real()) || std::isnan(f->get_field(i).imag())){
            std::cerr << "[update] Error: field is NaN" << std::endl;
            exit(1);
        }
    }

    return dt;
}

