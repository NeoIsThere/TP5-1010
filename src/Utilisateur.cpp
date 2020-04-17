/// Fonctions auxiliaires à la struct pour les utilisateurs.
/// \author Misha Krieger-Raynauld, Samuel Lavallée et Anthelme Clisson 
/// \date 2020-04 -17


#include "Utilisateur.h"

/// Affiche les informations d'un utilisateur à la sortie du stream donné.
/// \param outputStream Le stream auquel écrire les informations de l'utilisateur.
/// \param utilisateur  L'utilisateur à afficher au stream.
/// \return             Une référence au stream.
std::ostream& operator<<(std::ostream& outputStream, const Utilisateur& utilisateur)
{
    outputStream << "Identifiant: " << utilisateur.id << " | Nom: " << utilisateur.nom << " | Âge: " << utilisateur.age
                 << " | Pays: " << getPaysString(utilisateur.pays);
    return outputStream;
}
