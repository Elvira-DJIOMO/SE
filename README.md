## **README.md** (MIS À JOUR)

<artifact identifier="readme-banque" type="text/markdown" title="README.md - Système Bancaire Simplifié">
# Système Bancaire Multi-Agences

## Description

Projet de système d'exploitation en C++ démontrant les **modèles de concurrence et de synchronisation des ressources** à travers la simulation d'un système bancaire multi-agences avec opérations concurrentes.

## Objectifs Pédagogiques

Ce projet illustre les problèmes classiques de programmation concurrente :
- **Deadlock/Interblocage** : Évitement via ordre global sur les verrous
- **Race Condition** : Protection par mutex et lock_guard 
- **Atomicité** : Transactions ACID pour les virements
- **Cohérence** : Mise à jour thread-safe des soldes
- **Concurrence** : Gestion de multiples threads accédant aux mêmes ressources

## Architecture

### Entités
- **Comptes bancaires** : 3 comptes avec soldes initiaux fixes
- **Agences** : 4 threads représentant des agences bancaires effectuant des opérations concurrentes
- **Générateur Poisson** : Génère des intervalles de temps aléatoires entre opérations

### Opérations
1. **Consultation** : Lecture thread-safe du solde
2. **Dépôt** : Ajout de fonds avec vérification du montant
3. **Retrait** : Retrait avec vérification de solde (pas de découvert)
4. **Virement** : Transfert atomique entre comptes avec ordre global

### Synchronisation
- **Mutex (`std::mutex`)** : Protection des données partagées (comptes)
- **Lock Guard (`std::lock_guard`)** : Gestion automatique des verrous (RAII)
- **Ordre global** : Verrouillage par ID croissant (évite les deadlocks)

### Loi de Poisson
- Utilisée pour générer des **intervalles de temps** aléatoires entre opérations
- Simule l'arrivée d'événements dans un système bancaire réel
- Paramètre λ (lambda) : Taux moyen d'opérations par seconde

## Structure du Projet
```
Banque/
├── include/                    # Fichiers d'en-tête (.h)
│   ├── Compte.h               # Classe Compte avec mutex
│   ├── Logger.h               # Système de    logs colorés
│   ├── Agence.h               # Classe Agence (threads)
│   └── GenerateurPoisson.h    # Générateur d'intervalles Poisson
│
├── src/                        # Fichiers source (.cpp)
│   ├── main.cpp               # Point d'entrée du programme
│   ├── Compte.cpp             # Implémentation des opérations bancaires
│   ├── Logger.cpp             # Implémentation du système de logs
│   ├── Agence.cpp             # Implémentation du comportement des agences
│   └── GenerateurPoisson.cpp  # Implémentation du générateur
│
├── build/                      # Dossier de compilation (généré)
├── CMakeLists.txt             # Configuration CMake
└── README.md                  # Ce fichier