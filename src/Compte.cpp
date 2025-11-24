#include "Compte.h"
#include "Logger.h"
#include <sstream>
#include <iomanip>

// Constructeur
Compte::Compte(int id, double solde_initial) 
    : id(id), solde(solde_initial) {
    
    std::stringstream ss;
    ss << "Compte " << id << " créé avec solde initial : " 
       << std::fixed << std::setprecision(2) << solde_initial << " FCFA";
    logMessage(ss.str(), true);
}

// Getters
int Compte::getId() const {
    return id;
}

double Compte::getSolde() const {
    return solde;
}

std::mutex& Compte::getMutex() {
    return mutex_compte;
}

// Consultation du solde (thread-safe)
bool Compte::consulter(double& solde_actuel) {
    std::lock_guard<std::mutex> lock(mutex_compte);
    
    solde_actuel = solde;
    
    std::stringstream ss;
    ss << "Consultation - Compte " << id << " : " 
       << std::fixed << std::setprecision(2) << solde << " FCFA";
    logMessage(ss.str(), true);
    
    return true;
}

// Dépôt
bool Compte::deposer(double montant) {
    if (montant <= 0) {
        std::stringstream ss;
        ss << "Dépôt refusé - Compte " << id 
           << " : Montant invalide (" << montant << " FCFA)";
        logMessage(ss.str(), false);
        return false;
    }
    
    std::lock_guard<std::mutex> lock(mutex_compte);
    
    solde += montant;
    
    std::stringstream ss;
    ss << "Dépôt réussi - Compte " << id << " : +" 
       << std::fixed << std::setprecision(2) << montant 
       << " FCFA (Nouveau solde : " << solde << " FCFA)";
    logMessage(ss.str(), true);
    
    return true;
}

// Retrait
bool Compte::retirer(double montant) {
    if (montant <= 0) {
        std::stringstream ss;
        ss << "Retrait refusé - Compte " << id 
           << " : Montant invalide (" << montant << " FCFA)";
        logMessage(ss.str(), false);
        return false;
    }
    
    std::lock_guard<std::mutex> lock(mutex_compte);
    
    // Vérifier si le solde est suffisant
    if (solde < montant) {
        std::stringstream ss;
        ss << "Retrait refusé - Compte " << id 
           << " : Solde insuffisant (Disponible : " 
           << std::fixed << std::setprecision(2) << solde 
           << " FCFA, Demandé : " << montant << " FCFA)";
        logMessage(ss.str(), false);
        return false;
    }
    
    // Effectuer le retrait
    solde -= montant;
    
    std::stringstream ss;
    ss << "Retrait réussi - Compte " << id << " : -" 
       << std::fixed << std::setprecision(2) << montant 
       << " FCFA (Nouveau solde : " << solde << " FCFA)";
    logMessage(ss.str(), true);
    
    return true;
}

// Virement avec ordre global (évite les deadlocks)
bool Compte::virement(Compte& source, Compte& destination, double montant) {
    // Vérification du montant
    if (montant <= 0) {
        std::stringstream ss;
        ss << "Virement refusé - Montant invalide : " << montant << " FCFA";
        logMessage(ss.str(), false);
        return false;
    }
    
    // Vérifier qu'on ne vire pas vers le même compte
    if (source.getId() == destination.getId()) {
        std::stringstream ss;
        ss << "Virement refusé - Compte source = Compte destination (Compte " 
           << source.getId() << ")";
        logMessage(ss.str(), false);
        return false;
    }
    
    // ORDRE GLOBAL : Toujours locker dans l'ordre croissant des IDs
    // Cela évite les deadlocks lors de virements croisés
    Compte* premier = nullptr;
    Compte* second = nullptr;
    
    if (source.getId() < destination.getId()) {
        premier = &source;
        second = &destination;
    } else {
        premier = &destination;
        second = &source;
    }
    
    // Locker les deux comptes dans l'ordre
    std::lock_guard<std::mutex> lock1(premier->getMutex());
    std::lock_guard<std::mutex> lock2(second->getMutex());
    
    // Vérifier si le solde source est suffisant
    if (source.solde < montant) {
        std::stringstream ss;
        ss << "Virement refusé - Compte " << source.getId() 
           << " → Compte " << destination.getId() 
           << " : Solde insuffisant (Disponible : " 
           << std::fixed << std::setprecision(2) << source.solde 
           << " FCFA, Demandé : " << montant << " FCFA)";
        logMessage(ss.str(), false);
        return false;
    }
    
    // Transaction atomique : Débit + Crédit dans la même section critique
    source.solde -= montant;
    destination.solde += montant;
    
    std::stringstream ss;
    ss << "Virement réussi - Compte " << source.getId() 
       << " → Compte " << destination.getId() 
       << " : " << std::fixed << std::setprecision(2) << montant << " FCFA"
       << " (Soldes : C" << source.getId() << "=" << source.solde 
       << " FCFA, C" << destination.getId() << "=" << destination.solde << " FCFA)";
    logMessage(ss.str(), true);
    
    return true;
}