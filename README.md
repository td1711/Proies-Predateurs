# Proies-Predateurs
Simulation d'un monde constitué de proies et de prédateurs par Tom DARQUES et Clément MAGNIEZ.
Une proie (représentée en gris) meurt si elle est mangée par un prédateur. Un prédateur (représenté en rouge) meurt si il ne mange pas suffisamment de proies

Compiler le fichier test_mouvement.cpp et l'éxécuter en respectant les paramètres ci-dessous

Modèle parametres : ./main NbIndividus Largeur Hauteur NbGénérations Graine

Largeur*Hauteur doit être inférieur environ à 2300


Voici quelques exemples d'éxécution en fonction des paramètres

./main 100 40 40 10 0
Nombre espèces : 274
Nombre proies : 217
Nombre prédateurs : 57
Energie moyenne des prédateurs : 44.5965

./main 100 40 40 20 0
Nombre espèces : 193
Nombre proies : 75
Nombre prédateurs : 118
Energie moyenne des prédateurs : 34.2203

./main 100 40 40 50 0
Nombre espèces : 225
Nombre proies : 135
Nombre prédateurs : 90
Energie moyenne des prédateurs : 38.8667

./main 100 40 40 100 0
Nombre espèces : 89
Nombre proies : 20
Nombre prédateurs : 69
Energie moyenne des prédateurs : 20.2899

./main 100 40 40 500 0
Nombre espèces : 232
Nombre proies : 118
Nombre prédateurs : 114
Energie moyenne des prédateurs : 38.0526
