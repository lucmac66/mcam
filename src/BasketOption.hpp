#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Option.hpp"

/// \brief Classe Option abstraite
class BasketOption : public Option
{
public:

  PnlVect *lambda_;

  BasketOption(double T, int dates, int size, double strike, PnlVect *lambda, double rate);


  /**
   * Calcule la valeur du payoff sur la trajectoire
   *
   * @param[in] path est une matrice de taille (N+1) x d
   * contenant une trajectoire du modèle telle que créée
   * par la fonction asset.
   * @return phi(trajectoire)
   */
  double payoff(const PnlVect *path, double t) override;
};