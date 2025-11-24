#ifndef COMPTE_H
#define COMPTE_H

#include <mutex>
#include <string>

class Compte {
private:
    int id;                      // Identifiant unique du compte
    double solde;                // Solde du compte (en FCFA)
    std::mutex mutex_compte;     // Mutex pour protéger l'accès au solde
    
public:
    // Constructeur
    Compte(int id, double solde_initial);
    
    // Accesseurs (getters)
    int getId() const;
    double getSolde() const;
    
    // Opérations de base
    bool consulter(double& solde_actuel);      // Lecture du solde
    bool deposer(double montant);              // Ajout d'argent
    bool retirer(double montant);              // Retrait avec vérification
    
    // Opération de virement (statique car opère sur 2 comptes)
    static bool virement(Compte& source, Compte& destination, double montant);
    
    // Accès au mutex (pour ordre global dans virements)
    std::mutex& getMutex();
};

#endif // COMPTE_H