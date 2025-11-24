#include "Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>

// Définition du mutex global
std::mutex log_mutex;

// Codes ANSI pour les couleurs dans le terminal
const std::string COULEUR_VERTE = "\033[32m";  // Vert
const std::string COULEUR_ROUGE = "\033[31m";  // Rouge
const std::string RESET_COULEUR = "\033[0m";   // Reset

void logMessage(const std::string& message, bool success) {
    // Verrouillage pour éviter que plusieurs threads écrivent en même temps
    std::lock_guard<std::mutex> lock(log_mutex);
    
    // Récupérer l'heure actuelle
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    // Choix de la couleur et du symbole selon le succès
    std::string couleur = success ? COULEUR_VERTE : COULEUR_ROUGE;
    std::string symbole = success ? "✓" : "✗";
    
    // Récupération de l'ID du thread
    std::stringstream ss;
    ss << std::this_thread::get_id();

    // Affichage du message horodaté : [HH:MM:SS] [Thread-XXX] [✓/✗] Message
    std::cout << "["
              << std::put_time(std::localtime(&time), "%H:%M:%S")
              << "] [Thread-" << ss.str() << "] "
              << couleur << "[" << symbole << "]" << RESET_COULEUR
              << " " << message
              << std::endl;
}