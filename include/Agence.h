#ifndef AGENCE_H
#define AGENCE_H

#include "Compte.h"

// Types d'opérations possibles
enum TypeOperation {
    CONSULTATION,
    DEPOT,
    RETRAIT,
    VIREMENT
};

// Structure pour stocker les données d'une opération
struct Operation {
    TypeOperation type;
    Compte* compte_source;
    Compte* compte_destination;  // nullptr pour opérations simples
    double montant;
    
    // Constructeur par défaut
    Operation() 
        : type(CONSULTATION), 
          compte_source(nullptr), 
          compte_destination(nullptr), 
          montant(0.0) {}
};

class Agence {
private:
    int id_agence;
    int nb_comptes;
    Compte** comptes;         // Tableau dynamique de pointeurs
    int nb_operations;          // Nombre d'opérations à effectuer
    
    // Statistiques
    int operations_reussies;
    int operations_echouees;

public:
    // Constructeur
    Agence(int id, Compte** liste_comptes, int nb_compte, int nb_ops);
    
    // Méthode principale (appelée par les  thread)
    void run();
    
    // Afficher les statistiques finales
    void afficherStatistiques();

private:
    // Générer une opération aléatoire
    Operation genererOperation();
    
    // Exécuter une opération et mettre à jour les stats
    bool executerOperation(const Operation& op);
};

#endif // AGENCE_H