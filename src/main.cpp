#include "Compte.h"
#include "Agence.h"
#include "Logger.h"
#include <iostream>
#include <thread>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <limits>


// FONCTION POUR AFFICHER UN SÉPARATEUR VISUEL


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


// FONCTION POUR LIRE UN ENTIER AVEC VALIDATION


bool lireEntierPositif(const std::string& prompt, int& valeur) {
    std::cout << prompt;
    
    if (!(std::cin >> valeur)) {
        // Erreur de lecture (non-entier)
        std::cin.clear(); // Effacer le flag d'erreur
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Vider le buffer
        return false;
    }
    
    if (valeur <= 0) {
        return false;
    }
    
    return true;
}


// FONCTION POUR AFFICHER LES SOLDES DE TOUS LES COMPTES


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


// FONCTION PRINCIPALE


int main() {
    
    // INITIALISATION
    
        // Initialiser le générateur de nombres aléatoires avec l'heure actuelle

    srand(static_cast<unsigned int>(time(nullptr)));
    
    afficherSeparateur("SYSTÈME BANCAIRE MULTI-AGENCES");
    std::cout << "  Projet : Concurrence et Synchronisation" << std::endl;
    std::cout << "  Lambda (λ) : 2.0 opérations/seconde" << std::endl;
    std::cout << "========================================";
    std::cout << "========================================\n" << std::endl;
    
    
    // ÉTAPE 0 : SAISIE DES PARAMÈTRES
    
    int NB_COMPTES, NB_AGENCES, NB_OPERATIONS_PAR_AGENCE;
    
    std::cout << "Configuration de la simulation" << std::endl;
    std::cout << "==============================\n" << std::endl;
    
      // Lecture du nombre de comptes
    if (!lireEntierPositif("Entrez le nombre de comptes (Il doit etre > 0) : ", NB_COMPTES)) {
        std::cerr << "\n ERREUR : Le nombre de comptes doit être un entier strictement positif !" << std::endl;
        return 1;
    }
    
      // Lecture du nombre d'agences
    if (!lireEntierPositif("Entrez le nombre d'agences (Il doit etre > 0) : ", NB_AGENCES)) {
        std::cerr << "\n ERREUR : Le nombre d'agences doit être un entier strictement positif !" << std::endl;
        return 1;
    }
    
      // Lecture du nombre d'opérations par agence
    if (!lireEntierPositif("Entrez le nombre d'opérations par agence (Il doit etre > 0) : ", NB_OPERATIONS_PAR_AGENCE)) {
        std::cerr << "\n ERREUR : Le nombre d'opérations par agence doit être un entier strictement positif !" << std::endl;
        return 1;
    }
    
      // Afficher un récapitulatif
    std::cout << "\n Configuration validée :" << std::endl;
    std::cout << "  - Comptes : " << NB_COMPTES << std::endl;
    std::cout << "  - Agences : " << NB_AGENCES << std::endl;
    std::cout << "  - Opérations par agence : " << NB_OPERATIONS_PAR_AGENCE << std::endl;
    std::cout << "  - Nombre total d'opérations : " << (NB_AGENCES * NB_OPERATIONS_PAR_AGENCE) << std::endl;
    

    // ÉTAPE 1 : CRÉER LES COMPTES DYNAMIQUEMENT
    
    
    const double SOLDE_INITIAL = 500000.0;
    
    std::cout << "\nCréation des comptes..." << std::endl;
    
      // Allouer dynamiquement le tableau de pointeurs de comptes
    Compte** comptes_array = new Compte*[NB_COMPTES];
    
      // Créer chaque compte avec new
    for (int i = 0; i < NB_COMPTES; i++) {
        comptes_array[i] = new Compte(i + 1, SOLDE_INITIAL);
    }
    
    std::cout << "\n";
    
    
    // ÉTAPE 2 : CRÉER LES AGENCES DYNAMIQUEMENT
    
    std::cout << "Création des agences..." << std::endl;
    
      // Allouer dynamiquement le tableau de pointeurs d'agences
    Agence** agences_array = new Agence*[NB_AGENCES];
    
      // Créer chaque agence avec new
    for (int i = 0; i < NB_AGENCES; i++) {
        agences_array[i] = new Agence(i + 1, comptes_array, NB_COMPTES, NB_OPERATIONS_PAR_AGENCE);
    }
    
    std::cout << "\n";
    
    
    // ÉTAPE 3 : LANCER LES THREADS
    
    
    afficherSeparateur("DÉBUT DES OPÉRATIONS");
    
      // Allouer dynamiquement le tableau de pointeurs de threads
    std::thread** threads = new std::thread*[NB_AGENCES];
    
      // Créer et lancer chaque thread
    for (int i = 0; i < NB_AGENCES; i++) {
        threads[i] = new std::thread(&Agence::run, agences_array[i]);
    }
    

    // ÉTAPE 4 : ATTENDRE LA FIN DE TOUS LES THREADS
    
    
    for (int i = 0; i < NB_AGENCES; i++) {
        threads[i]->join();
        delete threads[i];  // Libérer chaque thread après join
    }
    
    delete[] threads;  // Libérer le tableau de threads
    
    afficherSeparateur("FIN DES OPÉRATIONS");
    
    
    // ÉTAPE 5 : AFFICHER LES STATISTIQUES DES AGENCES

    
    std::cout << "\n";
    for (int i = 0; i < NB_AGENCES; i++) {
        agences_array[i]->afficherStatistiques();
        std::cout << "\n";
    }
    
    
    // ÉTAPE 6 : AFFICHER LES SOLDES FINAUX
    
    
    afficherSoldesFinaux(comptes_array, NB_COMPTES);
    
    
    // Etape 7: NETTOYAGE DE LA MÉMOIRE
    
    
      // Libérer les comptes
    for (int i = 0; i < NB_COMPTES; i++) {
        delete comptes_array[i];
    }
    delete[] comptes_array;
    
      // Libérer les agences
    for (int i = 0; i < NB_AGENCES; i++) {
        delete agences_array[i];
    }
    delete[] agences_array;
    

    // Etape 8: FIN DU PROGRAMME

    
    std::cout << "\n Programme terminé avec succès !" << std::endl;
    
    return 0;
}