#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/// \brief Modèle de Black Scholes
class BlackScholesModel
{
public:
    int size_; /*! nombre d'actifs du modèle */
    double r_; /*! taux d'intérêt */
    double rho_; /*! paramètre de corrélation */
    PnlVect *sigma_; /*! vecteur de volatilités */
    PnlVect *divid_; /*! vecteur des dividendes */
    PnlVect *spots_; /*! valeurs initiales des sous-jacents */

    /**
     * Constructeur de la classe BlackScholesModel
     *
     * @param size nombre de sous-jacents
     * @param r taux d'intérêt
     * @param rho correlation
     * @param sigma vecteur des volatilités
     * @param divid vecteur des dividendes
     * @param spots vecteur des prix spot
     */
    BlackScholesModel(int size, double r, double rho, PnlVect* sigma, PnlVect* divid, PnlVect*spots);

    /**
     * Génère une trajectoire du modèle et la stocke dans path
     *
     * @param[out] path contient une trajectoire du modèle.
     * C'est une matrice de taille (nbTimeSteps+1) x d
     * @param[in] T  maturité
     * @param[in] nbTimeSteps nombre de dates de constatation
     * @param[in] rng générateur de nombres aléatoires
     */
    void asset(PnlMat *path, double T, int dates, PnlRng *rng);

};
