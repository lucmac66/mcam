#include "BlackScholesModel.hpp"
#include <math.h>
BlackScholesModel::BlackScholesModel(int size, double r, double rho, PnlVect* sigma, PnlVect* divid, PnlVect* spots){
    this->size_ = size;
    this->r_ = r;
    this->rho_ = rho;
    this->sigma_ = sigma;
    this->divid_ = divid;
    this->spots_ = spots;
}

void BlackScholesModel::asset(PnlMat *path, double T, int dates, PnlRng* rng){
    /*On set la première ligne avec les prix spot*/
    pnl_mat_set_row(path, spots_, 0);
    double dt = T / dates;

    /*Transformation de Cholesky de la matrice sigma*/
    PnlMat* L = pnl_mat_create_from_scalar(size_, size_, rho_);
    for (int k = 0; k < size_; k++)
    {
        pnl_mat_set(L, k, k, 1.0);
    }
    pnl_mat_chol(L);

    /*Création du vecteur brownien*/
    PnlVect *G = pnl_vect_create_from_zero(size_);
    PnlVect *L_j = pnl_vect_create_from_zero(size_);

    for (int k = 1; k < dates + 1; k++)
    {
        pnl_vect_rng_normal(G, size_, rng);
        for (int underlyingAsset = 0; underlyingAsset < size_; underlyingAsset++)
        {
            double volatility = pnl_vect_get(sigma_, underlyingAsset);
            double delta = pnl_vect_get(divid_, underlyingAsset);
            
            pnl_mat_get_row(L_j, L, underlyingAsset);
            double scalar_product = pnl_vect_scalar_prod(L_j, G);
            double new_price = pnl_mat_get(path, k - 1, underlyingAsset) * exp((r_ - delta - (volatility * volatility / 2)) * dt + (volatility * sqrt(dt) * scalar_product));
            pnl_mat_set(path, k, underlyingAsset, new_price);
        }
    }
}
