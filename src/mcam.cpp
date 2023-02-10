#include <iostream>
#include <ctime>
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "PricingResults.hpp"
#include "BlackScholesModel.hpp"
#include "Option.hpp"
#include "jlparser/parser.hpp"
#include "GeometricPut.hpp"
#include "MonteCarlo.hpp"
#include "BasketOption.hpp"
#include "BestOfOption.hpp"
#include "pnl/pnl_matrix.h"

using namespace std;

int main(int argc, char **argv)
{
    double T, r, strike, rho;
    PnlVect *spot, *sigma, *divid, *lambda;
    string type;
    int size, mc_iteration, deg, dates;

    char *infile = argv[1];
    Param *P = new Parser(infile);

    P->extract("option type", type);
    P->extract("maturity", T);
    P->extract("model size", size);
    P->extract("spot", spot, size);
    P->extract("volatility", sigma, size);
    P->extract("interest rate", r);
    P->extract("correlation", rho);
    P->extract("MC iterations", mc_iteration);
    P->extract("degree for polynomial regression", deg);
    P->extract("dates", dates);
    if (P->extract("dividend rate", divid, size, true) == false)
    {
        divid = pnl_vect_create_from_zero(size);
    }

    P->extract("strike", strike);
    P->extract("MC iterations", mc_iteration);
    
    /*Set up*/
    BlackScholesModel* bs = new BlackScholesModel(size, r, rho, sigma, divid, spot);
    Option* opt;

    if (type == "geometric_put"){
        opt = new GeometricPut(T, dates, size, strike, r);
    }
    else{
        P->extract("payoff coefficients", lambda, size);
        if (type == "bestof"){

            opt = new BestOfOption(T, dates, size, strike, lambda, r);
        }
        else{
            opt = new BasketOption(T, dates, size, strike, lambda, r);
        }
    }

    MonteCarlo* mc = new MonteCarlo(bs, opt, mc_iteration, deg);

    double price = mc->price();
    cout << PricingResults(price) << endl;
    return 0;
}
