#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_vector.h"
#include "math.h"

/// \brief Classe Option abstraite
class Option
{
public:
    double T_; /*! maturité */
    int dates_; /*! nombre de dates d'exercice */
    int size_; /*! dimension du modèle, redondant avec BlackScholesModel::size_ */
    double strike_; /*! strike de l'option */
    double rate_; /*! taux du marché */

    /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (dates_+1) x size_
     * contenant une trajectoire du modèle telle que créée
     * par la fonction asset.
     * @param[in] t temps auquel on calcule le payoff
     * @return payoff
     */
    virtual double payoff(const PnlVect *path, double t) = 0;
};


