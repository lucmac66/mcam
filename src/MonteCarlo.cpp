#include "MonteCarlo.hpp"
#include <vector>
#include <iostream>

using namespace std;

MonteCarlo::MonteCarlo(BlackScholesModel* mod, Option* opt, int MC_iterations, int deg){
    this->mod_ = mod;
    this->opt_ = opt;
    this->MC_iterations_ = MC_iterations;
    this->deg_ = deg;
}

double MonteCarlo::price(){

    /*Initialisation de la matrice des trajectoires*/
    vector<PnlMat*> simulations;
    for(int i = 0; i < MC_iterations_; i++){
        PnlMat *path = pnl_mat_create_from_zero(this->opt_->dates_ + 1, this->opt_->size_);
        simulations.push_back(path);
    }

    /*Initialisation du vecteur rng*/
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, (unsigned long)time(NULL));

    /*Création des différentes simulations*/
    for(int i = 0; i < MC_iterations_; i++){
        mod_->asset(simulations[i], this->opt_->T_, this->opt_->dates_, rng);
    }

    /*Création du vecteur des taus à partir du scalaire T*/
    PnlVect * taus = pnl_vect_create_from_scalar(MC_iterations_, this->opt_->dates_);

    for (int k = this->opt_->dates_; k > 0; k--){
        /* Création de la base de polynomes d'Hermite */
        PnlBasis *Hermite = pnl_basis_create_from_degree(PNL_BASIS_HERMITE, deg_, this->opt_->size_);
        PnlMat *x = pnl_mat_create(MC_iterations_, this->opt_->size_);
        PnlVect *y = pnl_vect_create(MC_iterations_);
        PnlVect *Stk = pnl_vect_create(MC_iterations_);

        /*Allocation des bonnes valeurs pour X et Y*/
        for (int i = 0; i < MC_iterations_; i++){
            pnl_mat_get_row(Stk, simulations[i], k);
            pnl_mat_set_row(x, Stk, i);

            PnlVect* prices_tau = pnl_vect_create_from_zero(this->opt_->size_);
            pnl_mat_get_row(prices_tau, simulations[i], pnl_vect_get(taus, i));
            pnl_vect_set(y, i, opt_->payoff(prices_tau, pnl_vect_get(taus, i)/this->opt_->dates_));
            pnl_vect_free(&prices_tau);
        }    

        pnl_vect_free(&Stk);

        /*Optimisation pour trouver les aplha k */
        PnlVect* coef = pnl_vect_new();
        pnl_basis_fit_ls(Hermite, coef, x, y);

        /*Iteration de l'Algorithme de LongStaff pour déterminer les TA optimaux*/
        for (int l = 0; l<MC_iterations_; l++){
            PnlVect *prices = pnl_vect_create(this->opt_->size_);
            pnl_mat_get_row(prices, simulations[l], k);
            double payoff = opt_->payoff(prices, k/this->opt_->dates_);
            PnlVect *x_i = pnl_vect_new();
            pnl_mat_get_row(x_i, x, l);
            double scalar = pnl_basis_eval_vect(Hermite, coef, x_i);
            if (scalar > 0){
                if (payoff > scalar){
                    /*Mise à jour de tau_k si l'indicatrice vaut 1*/
                    pnl_vect_set(taus, l, k);
                }
            }
            pnl_vect_free(&prices);
            pnl_vect_free(&x_i);
        }
        pnl_basis_free(&Hermite);
        pnl_mat_free(&x);
        pnl_vect_free(&y);
    }


    /*Calcul du prix en 0*/

    PnlVect* spots = pnl_vect_create_from_zero(this->opt_->size_);
    for(int i = 0; i < spots->size; i++){
        pnl_vect_set(spots, i, pnl_vect_get(this->mod_->spots_, i));
    }
    double payoff_zero = opt_->payoff(spots, 0);
    pnl_vect_free(&spots);

    /*Calcul de la moyenne des prices en tau_k*/
    
    double average_price = 0;
    for (int i = 0; i < MC_iterations_; i++){
        PnlVect *prices = pnl_vect_create_from_zero(this->opt_->size_);
        pnl_mat_get_row(prices, simulations[MC_iterations_-i-1], pnl_vect_get(taus, MC_iterations_-i-1));
        average_price += opt_->payoff(prices, pnl_vect_get(taus, MC_iterations_-i-1)/this->opt_->dates_);
        pnl_vect_free(&prices);
        pnl_mat_free(&simulations[MC_iterations_-i-1]);
    }
    average_price /= MC_iterations_;
    pnl_vect_free(&taus);
    pnl_rng_free(&rng);
    simulations.clear();

    if(payoff_zero > average_price){
        return payoff_zero;
    }else{
        return average_price;
    }

}