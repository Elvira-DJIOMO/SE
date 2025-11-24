#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <mutex>

// Fonction de log avec horodatage et couleurs
void logMessage(const std::string& message, bool success);

// Mutex global pour protéger std::cout
extern std::mutex log_mutex;

#endif // LOGGER_H