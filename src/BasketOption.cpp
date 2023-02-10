#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "BasketOption.hpp"
#include <iostream>

/// \brief Classe BasketOption

using namespace std;

BasketOption::BasketOption(double T, int dates, int size, double strike, PnlVect* lambda, double rate){
  this->T_ = T;
  this->dates_ = dates;
  this->lambda_ = lambda;
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
double BasketOption::payoff(const PnlVect *path, double t)
{
  double somme = 0;
  for (int k = 0; k < path->size; k++)
  {
    somme += pnl_vect_get(path, k) * pnl_vect_get(lambda_, k);
  }
  somme = somme - strike_;
  if (somme > 0)
  {
    return somme*exp(-rate_ * (T_ - t));
  }
  return 0;
}