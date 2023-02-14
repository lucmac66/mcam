#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Option.hpp"

/// \brief Classe Option abstraite
class BasketOption : public Option
{
public:
  PnlVect *lambda_; /*! vecteur des coefficients lambda pour le payoff */

  /**
   * Constructeur de la classe BasketOption
   *
   * @param T maturité
   * @param dates nombre de dates d'exercice
   * @param size nombre de sous-jacents
   * @param strike strike de l'option
   * @param lambda vecteur des coefficients lambda
   * @param rate taux d'intérêt
   */
  BasketOption(double T, int dates, int size, double strike, PnlVect *lambda, double rate);


  /**
   * Calcule la valeur du payoff sur la trajectoire
   *
   * @param[in] path est une matrice de taille (N+1) x d
   * contenant une trajectoire du modèle telle que créée
   * par la fonction asset.
   * @param[in] t tempas auquel on calcule le payoff
   * @return payoff
   */
  double payoff(const PnlVect *path, double t) override;
};