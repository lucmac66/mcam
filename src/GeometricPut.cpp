#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "GeometricPut.hpp"
#include <iostream>

using namespace std;

/// \brief Classe GeometricPut

GeometricPut::GeometricPut(double T, int dates, int size, double strike, double rate){
    this->T_ = T;
    this->dates_ = dates;
    this->size_ = size;
    this->strike_ = strike;
    this->rate_ = rate;
}

/**
 * Calcule la valeur du payoff sur la trajectoire
 *
 * @param[in] path est une matrice de taille (N+1) x d
 * contenant une trajectoire du modèle telle que créée
 * par la fonction asset.
 * @return phi(trajectoire)
 */
double GeometricPut::payoff(const PnlVect *path, double t)
{
    double produit = 1;

    for (int k = 0; k < path->size; k++)
    {
        produit *= pnl_vect_get(path, k);
    }
    double puissance = 1./(double)(this->size_);
    produit = pow(produit, puissance);
    if (strike_ > produit)
    {
        return exp(-rate_*(T_-t))*(strike_ - produit);
    }
    return 0;
}