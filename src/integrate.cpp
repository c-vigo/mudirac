/**
 * MuDirac - A muonic atom Dirac equation solver
 * by Simone Sturniolo (2019)
 * 
 * integrate.cpp
 * 
 * Routines for integrating differential equations with the shooting method
 * 
 * @author Simone Sturniolo
 * @version 0.1 04/02/2019
 */

#include "integrate.hpp"
#include <iostream>

/**
 * @brief  Integrate a single ODE
 * @note   Integrate one differential equation of the form:
 * 
 *      Q' = A*Q+B
 * 
 * with a second order shooting method, up to a given index, either forward or backwards.
 * 
 * @param  &Q: Vector for Q. Will return the integrated values, must contain already the first two as boundary conditions.
 * @param  A: Vector for A (see definition above). Same size as Q.
 * @param  B: Vector for B.
 * @param  h: Integration step (default = 1).
 * @param  stop_i: Index to stop integration. Run the whole range if -1 (default = -1).
 * @param  dir: Integration direction, either forward 'f' or backwards 'b' (default = 'f').
 * @retval None
 */
void shootQ(vector<double> &Q, vector<double> A, vector<double> B, double h, int stop_i, char dir)
{
    int N = Q.size();
    int step = (dir == 'f') ? 1 : -1;
    int from_i = (step == 1) ? 2 : N - 3;
    double QA, QB;

    // First, check size
    if (A.size() != N || B.size() != N)
    {
        throw "Invalid size for one or more arrays passed to shootQ";
    }

    if (stop_i == -1)
    {
        stop_i = (step == 1) ? N - 1 : 0;
    }

    for (int i = from_i; step * (i - stop_i) <= 0; i += step)
    {
        QA = 1.5 * step / h - A[i];
        QB = (2 * Q[i - step] - 0.5 * Q[i - 2 * step]) * step / h + B[i];
        Q[i] = QB / QA;
    }

    return;
}

/**
 * @brief  Integrate two coupled ODEs
 * @note   Integrate a system of coupled differential equations of the form:
 * 
 *      Q' = AA*Q+AB*P
 *      P' = BA*Q+BB*P
 * 
 * with a second order shooting method, up to a given index, either forward or backwards.
 * 
 * @param  &Q: Vector for Q. Will return the integrated values, must contain already the first two as boundary conditions.
 * @param  &P: Vector for P. Will return the integrated values, must contain already the first two as boundary conditions.
 * @param  AA: Vector for AA (see definition above). Same size as Q and P.
 * @param  AB: Vector for AB.
 * @param  BA: Vector for BA.
 * @param  BB: Vector for BB.
 * @param  h:  Integration step (default = 1).
 * @param  stop_i: Index to stop integration. Run the whole range if -1 (default = -1).
 * @param  dir: Integration direction, either forward 'f' or backwards 'b' (default = 'f').
 * @retval None
 */
void shootQP(vector<double> &Q, vector<double> &P, vector<double> AA, vector<double> AB, vector<double> BA, vector<double> BB,
             double h, int stop_i, char dir)
{
    int N = Q.size();
    int step = (dir == 'f') ? 1 : -1;
    int from_i = (step == 1) ? 2 : N - 3;
    double QA, QB, QC, PA, PB, PC;

    // First, check size
    if (P.size() != N || AA.size() != N || AB.size() != N || BA.size() != N || BB.size() != N)
    {
        throw "Invalid size for one or more arrays passed to shootQP";
    }

    if (stop_i == -1)
    {
        stop_i = (step == 1) ? N - 1 : 0;
    }

    for (int i = from_i; step * (i - stop_i) <= 0; i += step)
    {
        QC = 1 / h * (2 * Q[i - step] - 0.5 * Q[i - 2 * step]) * step;
        PC = 1 / h * (2 * P[i - step] - 0.5 * P[i - 2 * step]) * step;
        QA = (step * 1.5 / h - AA[i]);
        QB = -AB[i];
        PA = (step * 1.5 / h - BB[i]);
        PB = -BA[i];
        Q[i] = (PC * QB - PA * QC) / (PB * QB - PA * QA);
        P[i] = (PB * QC - PC * QA) / (PB * QB - PA * QA);
    }

    return;
}

/**
 * @brief  Integrate a single 2nd order ODE with Numerov's method
 * @note   Integrate one differential equation of the form:
 * 
 *      Q'' = A*Q+B
 * 
 * with Numerov's method, up to a given index, either forward or backwards.
 * 
 * @param  &Q: Vector for Q. Will return the integrated values, must contain already the first two as boundary conditions.
 * @param  A: Vector for A (see definition above). Same size as Q.
 * @param  B: Vector for B.
 * @param  h: Integration step (default = 1).
 * @param  stop_i: Index to stop integration. Run the whole range if -1 (default = -1).
 * @param  dir: Integration direction, either forward 'f' or backwards 'b' (default = 'f').
 * @retval None
 */
void shootNumerov(vector<double> &Q, vector<double> A, vector<double> B, double h, int stop_i, char dir)
{

    int N = Q.size();
    int step = (dir == 'f') ? 1 : -1;
    int from_i = (step == 1) ? 2 : N - 3;
    double h2_12 = h * h / 12;
    double QA0, QA1, QA2, QB;

    // First, check size
    if (A.size() != N || B.size() != N)
    {
        throw "Invalid size for one or more arrays passed to shootNumerov";
    }

    if (stop_i == -1)
    {
        stop_i = (step == 1) ? N - 1 : 0;
    }

    for (int i = from_i; step * (i - stop_i) <= 0; i += step)
    {
        QA0 = (1 - h2_12 * A[i]);
        QA1 = (1 + 5 * h2_12 * A[i - step]);
        QA2 = (1 - h2_12 * A[i - 2 * step]);
        QB = h2_12 * (B[i] + 10 * B[i - step] + B[i - 2 * step]);
        Q[i] = (2 * Q[i - step] * QA1 - Q[i - 2 * step] * QA2 + QB) / QA0;
    }

    return;
}

/**
 * @brief  Integrate the radial Dirac equation on a logarithmic grid
 * @note   Perform integration of the radial Dirac equation on a logarithmic grid, forward and backwards, up to the turning point.
 * The coupled equations have the form:
 * 
 *      Q' = k/r*Q+[mc-(E-V)/c]*P
 *      P' = -k/r*P+[mc+(E-V)/c]*Q
 * 
 * With k the quantum number: if j=|l+s|, then k = -(j+1/2)*sign(s), and E the expected energy (including the rest mass term).
 * The function will return a struct containing the index of the 'turning point', where the forward and backwards integration meet,
 * as well as the values of Q and P integrated forward (Qi, Pi) and backwards (Qe, Pe) at it.
 *   
 * @param  &Q: Vector for Q. Will return the integrated values, must contain already the first and last two as boundary conditions.
 * @param  &P: Vector for P. Will return the integrated values, must contain already the first and last two as boundary conditions.
 * @param  r:  Radial (logarithmic) grid
 * @param  V:  Potential
 * @param  E:  Energy (binding + mc^2)
 * @param  k:  Quantum number (default = -1)
 * @param  m:  Mass of the particle (default = 1)
 * @param  dx: Integration step (default = 1)
 * @retval turn_i: Turning point index
 */
TurningPoint shootDiracLog(vector<double> &Q, vector<double> &P, vector<double> r, vector<double> V,
                           double E, int k, double m, double dx)
{

    int N = Q.size(), turn_i;
    double B; // Binding energy
    TurningPoint out;
    vector<double> AA(N), AB(N), BA(N), BB(N); // Define the equation
    vector<double> dydE(N);

    // Check size
    if (P.size() != N || r.size() != N || V.size() != N)
    {
        throw "Invalid size for one or more arrays passed to shootDiracLog";
    }

    B = E - m * pow(Physical::c, 2);

    // Find the turning point
    for (turn_i = 0; turn_i < V.size(); ++turn_i)
    {
        if (V[turn_i] > B)
            break;
    }

    // Now define the other arrays
    for (int i = 0; i < N; ++i)
    {
        AA[i] = k;
        AB[i] = -r[i] * (B - V[i]) * Physical::alpha;
        BB[i] = -k;
        BA[i] = r[i] * ((B - V[i]) * Physical::alpha + 2 * m * Physical::c);
    }

    // Integrate forward
    shootQP(Q, P, AA, AB, BA, BB, dx, turn_i + 1);
    out.Qi = Q[turn_i];
    out.Pi = P[turn_i];
    // Integrate backwards
    shootQP(Q, P, AA, AB, BA, BB, dx, turn_i, 'b');
    out.Qe = Q[turn_i];
    out.Pe = P[turn_i];

    out.i = turn_i;

    return out;
}