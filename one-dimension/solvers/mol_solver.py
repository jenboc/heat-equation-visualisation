# Solving the heat equation using Finite Difference Schemes
# i.e. the Method of Lines, specifically the Crank-Nicholson Scheme

from typing import Callable
from solvers.solver import Solver
from scipy.linalg import solve_banded
import numpy as np


class MoLSolver(Solver):
    def __init__(self, diffusivity: float, rod_length: float,
                 initial_condition: Callable[[np.ndarray], np.ndarray]):
        super().__init__(diffusivity, rod_length, initial_condition)

    def u(self, x: np.ndarray, t: np.ndarray) -> np.ndarray:
        # (I - dt/2 A) * u_n+1 = (I + dt/2 A) * u_n

        # We're going to implement this with the assumption that the passed
        # array of x and t values ARE uniformly spaced points.
        dx = x[1] - x[0]
        dt = t[1] - t[0]

        # The number of interior points on the x grid
        m = len(x) - 2

        # === TIME STEP THE ODE ===
        # The recurrence essentially comes down to solving a system of
        # equations; we COULD take the inverse but this is very expensive
        # so we use solve_banded from scipy instead

        # This requires a matrix constructed in a specific way
        r = self.kappa * dt / (dx ** 2)
        main_diag = (1 + r) * np.ones(m)
        off_diag = (-r / 2) * np.ones(m - 1)

        banded_matrix = np.zeros((3, m))
        banded_matrix[0, 1:] = off_diag
        banded_matrix[1, :] = main_diag
        banded_matrix[2, :-1] = off_diag

        rhs = np.diag((1 - r) * np.ones(m)) \
            + np.diag((r / 2) * np.ones(m - 1), 1) \
            + np.diag((r / 2) * np.ones(m - 1), -1)

        sol = np.zeros((len(t), len(x)))
        sol[0, :] = self.initial_condition(x)

        current_u = sol[0, 1:-1]
        for n in range(1, len(t)):
            current_u = solve_banded((1, 1), banded_matrix, rhs @ current_u)
            sol[n, 1:-1] = current_u

        # Boundary conditions = 0; so we don't need to do anything at the
        # boundaries

        return sol
