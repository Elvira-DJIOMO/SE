cmake_minimum_required(VERSION 3.16)
project(Banque)

# Standard C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags de compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread")

# Inclure les headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Liste des fichiers sources
set(SOURCES
    src/main.cpp
    src/Compte.cpp
    src/Logger.cpp
    src/Agence.cpp
    src/GenerateurPoisson.cpp
)

# Créer l'exécutable
add_executable(Banque ${SOURCES})

# Lier pthread (pour les threads)
target_link_libraries(Banque pthread)


