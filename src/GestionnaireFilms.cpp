/// Gestionnaire de films.
/// \author Misha Krieger-Raynauld, Samuel Lavallée et Anthelme Clisson 
/// \date 2020-04 -17


#include "GestionnaireFilms.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "Foncteurs.h"
#include "RawPointerBackInserter.h"

/// Constructeur par copie.
/// \param other    Le gestionnaire de films à partir duquel copier la classe.
GestionnaireFilms::GestionnaireFilms(const GestionnaireFilms& other)
{
    films_.reserve(other.films_.size());
    filtreNomFilms_.reserve(other.filtreNomFilms_.size());
    filtreGenreFilms_.reserve(other.filtreGenreFilms_.size());
    filtrePaysFilms_.reserve(other.filtrePaysFilms_.size());

    for (const auto& film : other.films_)
    {
        // TODO: Uncomment une fois que la fonction ajouterFilm est écrite
        ajouterFilm(*film);
    }
}

/// Opérateur d'assignation par copie utilisant le copy-and-swap idiom.
/// \param other    Le gestionnaire de films à partir duquel copier la classe.
/// \return         Référence à l'objet actuel.
GestionnaireFilms& GestionnaireFilms::operator=(GestionnaireFilms other)
{
    std::swap(films_, other.films_);
    std::swap(filtreNomFilms_, other.filtreNomFilms_);
    std::swap(filtreGenreFilms_, other.filtreGenreFilms_);
    std::swap(filtrePaysFilms_, other.filtrePaysFilms_);
    return *this;
}

/// Affiche les informations des films gérés par le gestionnaire de films à la sortie du stream donné.
/// \param outputStream         Le stream auquel écrire les informations des films.
/// \param gestionnaireFilms    Le gestionnaire de films à afficher au stream.
/// \return                     Une référence au stream.
std::ostream& operator<<(std::ostream& outputStream, const GestionnaireFilms& gestionnaireFilms)
{
    // TODO: Uncomment une fois que la fonction getNombreFilms est écrite
    outputStream << "Le gestionnaire de films contient "  << gestionnaireFilms.getNombreFilms() << " films.\n"
                 << "Affichage par catégories:\n";

    // TODO: Réécrire l'implémentation avec des range-based for et structured bindings (voir énoncé du TP)
    for (const auto& [key,value] : gestionnaireFilms.filtreGenreFilms_)
    {
		std::vector<const Film*> listeFilms = value;
        outputStream << "Genre: " << getGenreString(key) << " (" << listeFilms.size() << " films):\n";

        for (auto& i : listeFilms)
        {
            outputStream << '\t' << *i << '\n';
        }
    }
    return outputStream;
}

/// Ajoute les films à partir d'un fichier de description des films.
/// \param nomFichier   Le fichier à partir duquel lire les informations des films.
/// \return             True si tout le chargement s'est effectué avec succès, false sinon.
bool GestionnaireFilms::chargerDepuisFichier(const std::string& nomFichier)
{
    std::ifstream fichier(nomFichier);
    if (fichier)
    {
        films_.clear();
        filtreNomFilms_.clear();
        filtreGenreFilms_.clear();
        filtrePaysFilms_.clear();

        bool succesParsing = true;

        std::string ligne;
        while (std::getline(fichier, ligne))
        {
            std::istringstream stream(ligne);

            std::string nom;
            int genre;
            int pays;
            std::string realisateur;
            int annee;

            if (stream >> std::quoted(nom) >> genre >> pays >> std::quoted(realisateur) >> annee)
            {
                // TODO: Uncomment une fois que la fonction ajouterFilm est écrite
                 ajouterFilm(Film{nom, static_cast<Film::Genre>(genre), static_cast<Pays>(pays), realisateur, annee});
            }
            else
            {
                std::cerr << "Erreur GestionnaireFilms: la ligne " << ligne
                          << " n'a pas pu être interprétée correctement\n";
                succesParsing = false;
            }
        }
        return succesParsing;
    }
    std::cerr << "Erreur GestionnaireFilms: le fichier " << nomFichier << " n'a pas pu être ouvert\n";
    return false;
}

/// Affiche les informations des films gérés par le gestionnaire de films à la sortie du stream donné.
/// \param outputStream         Le stream auquel écrire les informations des films.
/// \param gestionnaireFilms    Le gestionnaire de films à afficher au stream.
/// \return                     Une référence au stream.
bool GestionnaireFilms::ajouterFilm(const Film& film)
{
	
	if (getFilmParNom(film.nom))
		return false;
	films_.push_back(std::make_unique<Film>(film));
	Film* ptr = films_[films_.size() - 1].get();
	filtreGenreFilms_[film.genre].push_back(ptr);
	filtrePaysFilms_[film.pays].push_back(ptr);
	return filtreNomFilms_.emplace(film.nom, ptr).second;


}

/// Supprimer film dont le nom est passé en paramètre.
/// \param nomFilm		        nom du film
/// \return                     bool effacé ou pas trouvé
bool GestionnaireFilms::supprimerFilm(const std::string& nomFilm)
{
	auto existe = [nomFilm](std::unique_ptr<Film>& ptr)->bool { return (ptr->nom == nomFilm); };
	auto it = std::find_if(films_.begin(), films_.end(), existe);
	if (it != films_.end()) {
		filtreNomFilms_.erase(nomFilm);

		std::unique_ptr<Film>& ptr = *it;
		Film::Genre genre = (*it)->genre;
		Pays pays =(*it)->pays;

		std::vector<const Film*>& filmsDuGenre = filtreGenreFilms_[genre];
		std::vector<const Film*>& filmsDuPays = filtrePaysFilms_[pays];

		filmsDuGenre.erase(std::remove(filmsDuGenre.begin(), filmsDuGenre.end(), ptr.get()), filmsDuGenre.end());
		filmsDuPays.erase(std::remove(filmsDuPays.begin(), filmsDuPays.end(), ptr.get()), filmsDuPays.end());
		films_.erase(it);
		return true;
	}
	return false;
}

/// Retourne nombre de films du gestionnnaire
/// \return                     le nb de films
std::size_t GestionnaireFilms::getNombreFilms() const
{
	return films_.size();
}

/// Retourne un pointeur vers le film dont le nom est passé en paramètre
/// \param nom	        nom du film
/// \return             un pointeur vers le film
const Film* GestionnaireFilms::getFilmParNom(const std::string& nom) const
{
	if (filtreNomFilms_.find(nom) != filtreNomFilms_.end())
	return filtreNomFilms_.find(nom)->second;
	return nullptr;
	

}

/// Retourne les films par genre
/// \param Genre	            genre recherché
/// \return                     vector contenant tous les films de ce genre
std::vector<const Film*> GestionnaireFilms::getFilmsParGenre(Film::Genre genre) const
{
	std::vector<const Film*> vect;
	if (filtreGenreFilms_.find(genre) != filtreGenreFilms_.end())
		return filtreGenreFilms_.find(genre)->second;
	return vect;
	

}

/// Retourne les films par pays
/// \param Genre	            pays recherché
/// \return                     vector contenant tous les films de ce pays
std::vector<const Film*> GestionnaireFilms::getFilmsParPays(Pays pays) const
{
	std::vector<const Film*> vect;
	if (filtrePaysFilms_.find(pays) != filtrePaysFilms_.end())
		return filtrePaysFilms_.find(pays)->second;
	return vect;

}
/// Retourne les films par année
/// \param Genre	            année recherchée
/// \return                     vector contenant tous les films de ces années
std::vector<const Film*> GestionnaireFilms::getFilmsEntreAnnees(int anneeDebut, int anneeFin)
{
	std::vector<const Film*> vect;
	std::copy_if(films_.begin(), films_.end(), RawPointerBackInserter(vect), EstDansIntervalleDatesFilm(anneeDebut, anneeFin));
	return vect;
}
