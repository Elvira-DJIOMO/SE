#include "GenerateurPoisson.h"
#include <cmath>      // Pour log() (logarithme népérien)
#include <chrono>     // Pour gérer le temps
#include <thread>     // Pour faire dormir les threads

// Constructeur
GenerateurPoisson::GenerateurPoisson(double lambda) 
    : lambda(lambda),
      distribution(0.0, 1.0)  // Distribution uniforme entre 0 et 1
{
    /* on initialise le générateur avec une graine basée sur l'heure actuelle
    pour avoir des nombres vraiment aléatoires à chaque exécution)*/

    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    generateur.seed(seed);
}

// Générer un intervalle de temps selon la loi exponentielle
double GenerateurPoisson::genererIntervalle() {
    // 1. Générer un nombre aléatoire U entre 0 et 1
    double U = distribution(generateur);
    
    // 2. on evite U = 0 car le log n'es pas defini en 0.
    if (U == 0.0) {
        U = 0.0001;  // on prend un très petit nombre au lieu de 0
    }
    
    // 3. Appliquer la formule : T = -ln(U) / λ
    double intervalle = - std::log(U) / lambda;
    
    return intervalle;  // En secondes
}

// Faire une pause pendant un intervalle aléatoire avant une procahine operation sur une agence

void GenerateurPoisson::attendreIntervalle() {
    // 1. Générer l'intervalle (en secondes)
    double intervalle_sec = genererIntervalle();
    
    // 2. Convertir en millisecondes (pour std::this_thread::sleep_for)
    int intervalle_ms = static_cast<int>(intervalle_sec * 1000);
    
    // 3. Faire dormir le thread(agence) pendant ce temps
    std::this_thread::sleep_for(std::chrono::milliseconds(intervalle_ms));
}