#ifndef GENERATEUR_POISSON_H
#define GENERATEUR_POISSON_H

#include <random>

class GenerateurPoisson {
private:
    double lambda;  // Taux d'opérations par seconde (λ)
    
    // Générateurs de nombres aléatoires 
    std::default_random_engine generateur;
    std::uniform_real_distribution<double> distribution;

public:
    // Constructeur : on initialise avec λ
    GenerateurPoisson(double lambda);
    
    // Générer un intervalle de temps aléatoire (en secondes)
    double genererIntervalle();
    
    // Faire une pause (sleep) pendant un intervalle aléatoire
    void attendreIntervalle();
};

#endif // GENERATEUR_POISSON_H