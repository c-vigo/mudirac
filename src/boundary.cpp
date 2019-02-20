/**
 * MuDirac - A muonic atom Dirac equation solver
 * by Simone Sturniolo (2019)
 * 
 * boundary.cpp
 * 
 * Boundary conditions
 * 
 * @author Simone Sturniolo
 * @version 0.1 04/02/2019
 */

#include "boundary.hpp"

void boundaryDiracCoulomb(vector<double> &Q, vector<double> &P, vector<double> r, double E, int k, double m, double Z, bool finite)
{
    int l = k < 0 ? -(k + 1) : k; // Angular momentum number
    int N = Q.size();
    double K;

    if (P.size() != N || r.size() != N || N < 4)
    {
        throw "Invalid array size passed to boundaryDiracCoulomb";
    }

    // r = 0 limit
    // Depends on whether we consider the nucleus of finite size or point-like
    if (!finite)
    {
        // Point like
        // We set P, the major component, as having a prefactor of 1, and scale Q accordingly
        for (int i = 0; i < 2; ++i)
        {
            P[i] = pow(r[i], l + 1);
            Q[i] = -Z * Physical::alpha / (l + 2 - k) * P[i];
        }
    }
    else
    {
        // Finite size: only the k/r term matters
        for (int i = 0; i < 2; ++i)
        {
            P[i] = pow(r[i], -k);
            Q[i] = 1.0 / P[i];
        }
    }

    // r = inf limit
    // Same as above
    K = pow(m * Physical::c, 2) - pow(E * Physical::alpha, 2);
    if (K < 0)
    {
        throw "Can't compute boundary conditions for non-bound state";
    }
    K = sqrt(K);
    for (int i = 1; i < 3; ++i)
    {
        P[N - i] = exp(-K * r[N - i]);
        Q[N - i] = -K / (m * Physical::c + E * Physical::alpha) * P[N - i];
    }
}

void boundaryDiracErrorDECoulomb(vector<double> &zeta, double E, int k, double m)
{
    int l = k < 0 ? -(k + 1) : k; // Angular momentum number
    int N = zeta.size();
    double K, gp;

    if (N < 4)
    {
        throw "Invalid array size passed to boundaryDiracErrorDECoulomb";
    }

    // In the r=0 limit, it's fine to have it be 0
    zeta[0] = 0;
    zeta[1] = 0;

    // r = inf limit
    K = pow(m * Physical::c, 2) - pow(E * Physical::alpha, 2);
    gp = (m * Physical::c + E * Physical::alpha);
    if (K < 0)
    {
        throw "Can't compute boundary conditions for non-bound state";
    }
    K = sqrt(K);
    zeta[N - 1] = E / (K * gp) * pow(Physical::alpha, 2) + K / pow(gp, 2) * Physical::alpha;
    zeta[N - 2] = zeta[N - 1];
}