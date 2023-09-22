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

static std::complex<double> drift_term(field *f, double m2, double lambda, int site){
    std::complex<double> terms[2] = {0};

    std::cerr << "dt_f = " << f->get_dt(site, 'f') << std::endl;

    terms[0] += (f->getNextField(site) - f->get_field(site))/(f->get_dt(site, 'f'));
    terms[0] += (f->get_field(site) - f->getPrevField(site))/(f->get_dt(site, 'b'));

    terms[1] += m2*f->get_dt(site, 'f')*f->get_field(site);
    terms[1] += lambda*f->get_dt(site, 'f')*f->get_field(site)*f->get_field(site)*f->get_field(site);

    return terms[0] + terms[1];
}

static std::complex<double> noise(double alpha_noise){
    double real = std::sqrt(alpha_noise)*rand_normal(0, SQRT_PI/SQRT_2);
    double tmp = rand_normal(0, SQRT_PI/SQRT_2);
    double imag = std::sqrt(1-alpha_noise)*rand_normal(0, SQRT_PI/SQRT_2);
    return std::complex<double>(real, imag);
}

double update(field *f, settings *s){
    double dt = s->epsilon;
    std::complex<double> k_max;
    for(int i = 0; i < f->get_N();i++){
        f->set_k(drift_term(f, s->m2, s->lambda, i),i);
        f->set_noise(noise(s->alpha_noise),i);
    }
    f->find_k_max();
    k_max = f->get_k_max();
    if(k_avg.i < 10000){
        k_avg.avg_k *= k_avg.i;
        k_avg.avg_k += abs(k_max);
        k_avg.i++;
        k_avg.avg_k /= k_avg.i;
    }

    dt *= (k_avg.avg_k/abs(k_max));

    for(int i = 0; i < f->get_N();i++){
        f->set_field(f->get_field(i) + dt*f->get_k(i) + sqrt(dt)*f->get_noise(i),i);
    }

    return dt;
}

