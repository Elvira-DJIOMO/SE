#include "Agence.h"
#include "Logger.h"
#include "GenerateurPoisson.h"
#include <sstream>
#include <iomanip>
#include <thread>    // pour this thread
#include <chrono>
#include <cstdlib>   // Pour rand()
#include <ctime>     // Pour time()

// CONSTRUCTEUR

Agence::Agence(int id, Compte** liste_comptes, int nb_comptes, int nb_ops)
    : id_agence(id),
      nb_comptes(nb_comptes),
      comptes(liste_comptes),
      nb_operations(nb_ops),
      // vu qu'au debut rien n'a ete fait on initialise les echecs et succes a 0
      operations_reussies(0),    
      operations_echouees(0)     
{
    // Message de création de l'agence
    std::stringstream ss;
    ss << "Agence " << id_agence << "a accès à " 
       << nb_comptes << " comptes";
    logMessage(ss.str(), true);
}

// GÉNÉRER UNE OPÉRATION ALÉATOIRE


Operation Agence::genererOperation() {
    Operation op;  //  opération vide
    
    
    // ÉTAPE 1 : Choix du type d'opération (équiprobabilite : 25% chacune)
    
    
    int choix_type = rand() % 4;  // Nombre entre 0 et 3
    
    switch (choix_type) {
        case 0:
            op.type = CONSULTATION;
            break;
        case 1:
            op.type = DEPOT;
            break;
        case 2:
            op.type = RETRAIT;
            break;
        case 3:
            op.type = VIREMENT;
            break;
    }
    
    // ÉTAPE 2 : Choisir le(s) COMPTE(S)
    
    // a) Choisir un compte source aléatoire
    int index_source = rand() % nb_comptes;
    op.compte_source = comptes[index_source];
    
    // Si c'est un VIREMENT, choisir aussi un compte destination (différent)
    if (op.type == VIREMENT) {
        int index_dest;
        
        // Boucle pour s'assurer que la destination est differente de la source
        do {
            index_dest = rand() % nb_comptes;
        } while (index_dest == index_source && nb_comptes > 1);
        // Tant que c'est le même compte ET qu'on a plus d'1 compte
        
        op.compte_destination = comptes[index_dest];
    } else {
        // Pour les autres opérations, pas de compte destination
        op.compte_destination = nullptr;
    }
    
    // ÉTAPE 3 : Générer un MONTANT aléatoire
    
    if (op.type != CONSULTATION) {
        // Montant entre 100000 et 150 000 FCFA
        int montant_min = 100000;
        int montant_max = 250000;
        
        op.montant = montant_min + (rand() % (montant_max - montant_min + 1));
    } else {
        // Pour la consultation, le montant n'est pas utilisé
        op.montant = 0.0;
    }
    
    return op;
}

// Executer une operation

bool Agence::executerOperation(const Operation& op) {
    bool succes = false;
    
    // Créer un message avec l'ID de l'agence
    std::stringstream prefixe;
    prefixe << "[Agence " << id_agence << "] ";
    
    // Exécuter selon le TYPE d'opération
    
    switch (op.type) {
        
        case CONSULTATION: {
            double solde_actuel;
            succes = op.compte_source->consulter(solde_actuel);
            
            // Le message est déjà loggé par Compte::consulter()
            break;
        }
        
        case DEPOT: {
            succes = op.compte_source->deposer(op.montant);
            
            // Le message est déjà loggé par Compte::deposer()
            break;
        }
        
        case RETRAIT: {
            succes = op.compte_source->retirer(op.montant);
            
            // Le message est déjà loggé par Compte::retirer()
            break;
        }
        
        case VIREMENT: {
            succes = Compte::virement(*op.compte_source, 
                                      *op.compte_destination, 
                                      op.montant);
            
            // Le message est déjà loggé par Compte::virement()
            break;
        }
    }
    
    // Mettre à jour les statistique
    
    if (succes) {
        operations_reussies++;
    } else {
        operations_echouees++;
    }
    
    return succes;
}

// Methode PRINCIPALE run

void Agence::run() {
    // Message de démarrage
    std::stringstream ss;
    ss << "Agence " << id_agence << " démarre ses opérations ("
       << nb_operations << " opérations prévues)";
    logMessage(ss.str(), true);
    
    // Créer le générateur de Poisson avec lambda = 2.0
    GenerateurPoisson poisson(2.0);
    
    // BOUCLE PRINCIPALE : Effectuer les opérations
    
    for (int i = 0; i < nb_operations; i++) {
        // Étape 1 : Générer une opération aléatoire
        Operation op = genererOperation();
        
        // Étape 2 : L'exécuter
        executerOperation(op);
        
        // Étape 3 : Attendre un intervalle aléatoire (loi de Poisson) Sauf pour la dernière opération (pas besoin d'attendre après)

        /*if (i < nb_operations - 1) {
            poisson.attendreIntervalle();
        }*/
       if (i < nb_operations - 1) { // i doit au plus etre egal a l'avant derniere operation.

    // Calculer l'intervalle AVANT d'attendre (pour l'afficher et permettre de visualiser poisson)
    double intervalle = poisson.genererIntervalle();
    
    // LOG : Affichage du temps de pause
    std::stringstream ss_pause;
    ss_pause << "[Agence " << id_agence << "] Pause de " 
             << std::fixed << std::setprecision(3) 
             << intervalle << " secondes";
    logMessage(ss_pause.str(), true);
    
    // faire la pause
    std::this_thread::sleep_for(
        std::chrono::milliseconds(static_cast<int>(intervalle * 1000))
    );
    }
  }
    
    // Message de fin
    std::stringstream ss_fin;
    ss_fin << "Agence " << id_agence << " a terminé ses opérations";
    logMessage(ss_fin.str(), true);
}

// AFFICHER LES STATISTIQUES

void Agence::afficherStatistiques() {
    std::stringstream ss;
    
    // Calcul du taux de succès
    int total = operations_reussies + operations_echouees;
    double taux_succes = (total > 0) 
        ? (operations_reussies * 100.0 / total) 
        : 0.0;
    
    // Construction du message
    ss << "=== Statistiques Agence " << id_agence << " ==="
       << "\n  Opérations réussies : " << operations_reussies
       << "\n  Opérations échouées : " << operations_echouees
       << "\n  Total               : " << total
       << "\n  Taux de succès      : " << std::fixed << std::setprecision(1)
       << taux_succes << "%";
    
    logMessage(ss.str(), true);
}


