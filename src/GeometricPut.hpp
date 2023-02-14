#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Option.hpp"

/// \brief Classe Option abstraite
class GeometricPut : public Option
{
public:

  /**
   * Constructeur de la classe GeometricPut
   *
   * @param T maturité
   * @param dates nombre de dates d'exercice
   * @param size nombre de sous-jacents
   * @param strike strike de l'option
   * @param rate taux d'intérêt
   */
  GeometricPut(double T, int dates, int size, double strike, double rate);

  /**
   * Calcule la valeur du payoff sur la trajectoire
   *
   * @param[in] path est une matrice de taille (N+1) x d
   * contenant une trajectoire du modèle telle que créée
   * par la fonction asset.
   * @param[in] t temps auquel on calcule le payoff
   * @return payoff
   */
  double payoff(const PnlVect *path, double t) override;
};