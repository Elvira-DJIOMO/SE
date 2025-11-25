#include "Compte.h"
#include "Agence.h"
#include "Logger.h"
#include <iostream>
#include <thread>
#include <vector>
#include <iomanip>
#include <cstdlib>  // Pour srand()
#include <ctime>    // Pour time()

// ============================================================================
// FONCTION POUR AFFICHER UN SÉPARATEUR VISUEL
// ============================================================================

void afficherSeparateur(const std::string& titre = "") {
    std::cout << "\n";
    std::cout << "========================================";
    std::cout << "========================================" << std::endl;
    if (!titre.empty()) {
        std::cout << "  " << titre << std::endl;
        std::cout << "========================================";
        std::cout << "========================================" << std::endl;
    }
}

// ============================================================================
// FONCTION POUR AFFICHER LES SOLDES DE TOUS LES COMPTES
// ============================================================================

void afficherSoldesFinaux(Compte** comptes, int nb_comptes) {
    afficherSeparateur("SOLDES FINAUX DES COMPTES");
    
    double somme_totale = 0.0;
    
    for (int i = 0; i < nb_comptes; i++) {
        double solde = comptes[i]->getSolde();
        somme_totale += solde;
        
        std::cout << "  Compte " << comptes[i]->getId() << " : "
                  << std::fixed << std::setprecision(2)
                  << solde << " FCFA" << std::endl;
    }
    
    std::cout << "\n  SOMME TOTALE : "
              << std::fixed << std::setprecision(2)
              << somme_totale << " FCFA" << std::endl;
    
    std::cout << "========================================";
    std::cout << "========================================" << std::endl;
}

// ============================================================================
// FONCTION PRINCIPALE
// ============================================================================

int main() {
    // ========================================================================
    // INITIALISATION
    // ========================================================================
    
    // Initialiser le générateur de nombres aléatoires avec l'heure actuelle
    srand(static_cast<unsigned int>(time(nullptr)));
    
    afficherSeparateur("SYSTÈME BANCAIRE MULTI-AGENCES");
    std::cout << "  Projet : Concurrence et Synchronisation" << std::endl;
    std::cout << "  Lambda (λ) : 2.0 opérations/seconde" << std::endl;
    std::cout << "========================================";
    std::cout << "========================================\n" << std::endl;
    
    // ========================================================================
    // ÉTAPE 1 : CRÉER LES COMPTES
    // ========================================================================
    
    const int NB_COMPTES = 3;
    const double SOLDE_INITIAL = 500000.0;  
    
    std::cout << "Création des comptes..." << std::endl;
    
    // Créer les comptes dynamiquement
    std::vector<Compte> comptes_vec;
    for (int i = 1; i <= NB_COMPTES; i++) {
        comptes_vec.emplace_back(i, SOLDE_INITIAL);
    }
    
    // Créer un tableau de pointeurs vers ces comptes
    std::vector<Compte*> tab_comptes;
    for (auto& c : comptes_vec) {
        tab_comptes.push_back(&c);
    }
    
    std::cout << "\n";
    
    // ========================================================================
    // ÉTAPE 2 : CRÉER LES AGENCES
    // ========================================================================
    
    const int NB_AGENCES = 4;
    const int NB_OPERATIONS_PAR_AGENCE = 5;
    
    std::cout << "Création des agences..." << std::endl;
    
    std::vector<Agence> agences;
    for (int i = 1; i <= NB_AGENCES; i++) {
        agences.emplace_back(i, tab_comptes.data(), NB_COMPTES, NB_OPERATIONS_PAR_AGENCE);
    }
    
    std::cout << "\n";
    
    // ========================================================================
    // ÉTAPE 3 : LANCER LES THREADS
    // ========================================================================
    
    afficherSeparateur("DÉBUT DES OPÉRATIONS");

    std::vector<std::thread> threads;
    for (int i = 0; i < NB_AGENCES; i++) {
        threads.emplace_back(&Agence::run, &agences[i]);
    }
    
    // ========================================================================
    // ÉTAPE 4 : ATTENDRE LA FIN DE TOUS LES THREADS
    // ========================================================================
    
 for (auto& T : threads) {
        T.join();
    }

    afficherSeparateur("FIN DES OPÉRATIONS");
    
    // ========================================================================
    // ÉTAPE 5 : AFFICHER LES STATISTIQUES DES AGENCES
    // ========================================================================
    

    for (int i = 0; i < NB_AGENCES; i++) {
        agences[i].afficherStatistiques();
        std::cout << "\n";
    }
    
    // ========================================================================
    // ÉTAPE 6 : AFFICHER LES SOLDES FINAUX
    // ========================================================================

    afficherSoldesFinaux(tab_comptes.data(), NB_COMPTES);
    
    
    // ========================================================================
    // FIN DU PROGRAMME
    // ========================================================================
    
    std::cout << "\n Programme terminé avec succès !" << std::endl;
    
    return 0;
}