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
    const double SOLDE_INITIAL = 500000.0;  // 500 000 FCFA
    
    std::cout << "Création des comptes..." << std::endl;
    
    // Créer les 3 comptes avec soldes initiaux
    Compte c1(1, SOLDE_INITIAL);
    Compte c2(2, SOLDE_INITIAL);
    Compte c3(3, SOLDE_INITIAL);
    
    // Créer un tableau de pointeurs vers ces comptes
    Compte* tab_comptes[NB_COMPTES] = {&c1, &c2, &c3};
    
    std::cout << "\n";
    
    // ========================================================================
    // ÉTAPE 2 : CRÉER LES AGENCES
    // ========================================================================
    
    const int NB_AGENCES = 4;
    const int NB_OPERATIONS_PAR_AGENCE = 5;
    
    std::cout << "Création des agences..." << std::endl;
    
    // Créer les 4 agences
    Agence agence1(1, tab_comptes, NB_COMPTES, NB_OPERATIONS_PAR_AGENCE);
    Agence agence2(2, tab_comptes, NB_COMPTES, NB_OPERATIONS_PAR_AGENCE);
    Agence agence3(3, tab_comptes, NB_COMPTES, NB_OPERATIONS_PAR_AGENCE);
    Agence agence4(4, tab_comptes, NB_COMPTES, NB_OPERATIONS_PAR_AGENCE);
    
    std::cout << "\n";
    
    // ========================================================================
    // ÉTAPE 3 : LANCER LES THREADS
    // ========================================================================
    
    afficherSeparateur("DÉBUT DES OPÉRATIONS");
    
    // Créer les 4 threads (un par agence)
    // Chaque thread exécute la méthode run() de son agence
    std::thread t1(&Agence::run, &agence1);
    std::thread t2(&Agence::run, &agence2);
    std::thread t3(&Agence::run, &agence3);
    std::thread t4(&Agence::run, &agence4);
    
    // ========================================================================
    // ÉTAPE 4 : ATTENDRE LA FIN DE TOUS LES THREADS
    // ========================================================================
    
    // join() = "Attends que ce thread se termine avant de continuer"
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    
    afficherSeparateur("FIN DES OPÉRATIONS");
    
    // ========================================================================
    // ÉTAPE 5 : AFFICHER LES STATISTIQUES DES AGENCES
    // ========================================================================
    
    std::cout << "\n";
    agence1.afficherStatistiques();
    std::cout << "\n";
    agence2.afficherStatistiques();
    std::cout << "\n";
    agence3.afficherStatistiques();
    std::cout << "\n";
    agence4.afficherStatistiques();
    std::cout << "\n";
    
    // ========================================================================
    // ÉTAPE 6 : AFFICHER LES SOLDES FINAUX
    // ========================================================================
    
    afficherSoldesFinaux(tab_comptes, NB_COMPTES);
    
    // ========================================================================
    // FIN DU PROGRAMME
    // ========================================================================
    
    std::cout << "\n✓ Programme terminé avec succès !" << std::endl;
    
    return 0;
}