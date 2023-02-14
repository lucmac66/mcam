#pragma once

#include "Option.hpp"
#include "BlackScholesModel.hpp"
#include "pnl/pnl_random.h"
#include "pnl/pnl_basis.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_vector.h"
#include <vector>

class MonteCarlo
{
public:
    BlackScholesModel *mod_; /*! pointeur vers le modèle */
    Option *opt_; /*! pointeur sur l'option */
    int MC_iterations_; /*! nombre d'itérations du modèle de Monte Carlo*/
    int deg_; /*! nombre de degré de la base polynomiale*/

    /**
     * Constructeur de la classe MonteCarlo
     *
     * @param mod pointeur vers le modèle BlackScholes
     * @param opt pointeur vers l'otpion
     * @param MC_iterations nombre d'itérations MonteCarlo
     * @param deg degré de la régression polynomiale
     */
    MonteCarlo(BlackScholesModel *mod, Option* opt, int MC_iterations, int deg);

    /**
     * Calcule le prix de l'option à la date 0
     *
     * @return valeur de l'estimateur Monte Carlo
     */
    double price();

};


