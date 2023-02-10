#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "BestOfOption.hpp"

/// \brief Classe BestOfOption


BestOfOption::BestOfOption(double T, int dates, int size, double strike, PnlVect* lambda, double rate){
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
double BestOfOption::payoff(const PnlVect *path, double t)
{
    double max = 0;

    for (int k = 0; k < path->size; k++)
    {
        double challenger = pnl_vect_get(path, k) * pnl_vect_get(lambda_, k);
        if (challenger > max){
            max = challenger;
        }
    }
    max -= strike_;
    if (max > 0)
    {
        return max*exp(-rate_*(T_ - t));
    }
    return 0;
}