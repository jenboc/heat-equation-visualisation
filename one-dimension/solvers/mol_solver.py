# Solving the heat equation using Finite Difference Schemes
# i.e. the Method of Lines, specifically the Crank-Nicholson Scheme

from typing import Callable
from solvers.solver import Solver
from scipy.linalg import lu_factor, lu_solve
import numpy as np


class MoLSolver(Solver):
    def __init__(self, diffusivity: float, rod_length: float,
                 initial_condition: Callable[[np.ndarray], np.ndarray]):
        super().__init__("Method of Lines",
                         diffusivity, rod_length, initial_condition)

    def u(self, x: np.ndarray, t: np.ndarray) -> np.ndarray:
        # (I - dt/2 A) * u_n+1 = (I + dt/2 A) * u_n

        # We're going to implement this with the assumption that the passed
        # array of x and t values ARE uniformly spaced points.
        dx = x[1] - x[0]
        dt = t[1] - t[0]

        # The number of interior points on the x grid
        m = len(x) - 2

        # The recurrence essentially comes down to solving a system of
        # equations; we COULD take the inverse but this is very expensive
        # so we use LU decomposition instead.

        # This requires a matrix constructed in a specific way
        r = self.kappa * dt / (dx ** 2)
        tridiag = np.diag(-2 * np.ones(m)) \
            + np.diag(np.ones(m - 1), 1) \
            + np.diag(np.ones(m - 1), -1)

        id = np.eye(m)
        lhs = id - 0.5 * r * tridiag
        rhs = id + 0.5 * r * tridiag

        lu, piv = lu_factor(lhs)

        # Initialise solution array with the initial condition
        sol = np.zeros((len(t), len(x)))
        sol[0, :] = self.initial_condition(x)

        # Timestep the vector ODE
        current_u = sol[0, 1:-1]
        for n in range(1, len(t)):
            current_u = lu_solve((lu, piv), rhs @ current_u)
            sol[n, 1:-1] = current_u

        # Boundary conditions = 0; so we don't need to do anything at the
        # boundaries

        return sol
