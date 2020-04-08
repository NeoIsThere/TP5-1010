#ifndef FONCTEUR_H
#define FONCTEUR_H

#include<memory>
#include"Film.h"
#include"LigneLog.h"



class EstDansIntervalleDatesFilm {
	
public:
	EstDansIntervalleDatesFilm(int borneInf, int borneSup) : borneInf_(borneInf), borneSup_(borneSup) {  }

	bool operator()(const std::unique_ptr<Film>& film) {
		int annee = film->annee;
		return(borneInf_ <= annee && borneSup_ >= annee);
	}

private:
	int borneInf_;
	int borneSup_;


};

class ComparateurLog {
public:
	bool operator()(const LigneLog& premiere, const LigneLog& deuxieme) {
		return(premiere.timestamp < deuxieme.timestamp);
	}

};

template<typename T1, typename T2>
class ComparateurSecondElementPaire  {
public:
	bool operator()(const std::pair< T1, T2>& p1, const std::pair<T1, T2>& p2) {
		return(p1.second < p2.second);
	}
};



#endif