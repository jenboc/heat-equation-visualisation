# Solving the heat equation using the series that arises from
# Separation of Variables

from typing import Callable
from solver import Solver
import numpy as np


class SeriesSolver(Solver):
    def __init__(self, diffusivity: float, rod_length: float,
                 initial_condition: Callable[[np.ndarray], np.ndarray],
                 num_terms: int, n_int_points: int):
        super().__init__(diffusivity, rod_length, initial_condition)

        # Number of terms that we take from the infinite series
        self.num_terms = num_terms

        # We can actually precompute the coefficients An here since
        # these do not vary per evaluation
        self.coeffs = self.calculate_coefficients(n_int_points)

    def calculate_coefficients(self, n_points: int) -> np.ndarray:
        """ Integrate numerically to find the coefficients of the terms of
        the series (the An)

        """
        # An = 2/L * int_0^L ( f(x) * sin(n pi x / L) ) dx
        coeffs = []

        for n in range(1, self.num_terms + 1):
            coeffs.append(self.integrate(
                lambda x, n=n: np.sin(n * np.pi * x / self.length)
                * self.initial_condition(x),
                0,
                self.length,
                n_points
            ))

        return (2 / self.length) * np.array(coeffs)

    def integrate(self, f: Callable[[np.ndarray], np.ndarray],
                  lower_limit: float, upper_limit: float,
                  n_points: int) -> float:
        """ Integrate f(x) between the limits (inclusive) using (Composite)
        Simpson's Rule

        Parameters:
            f : Callable[[np.ndarray], np.ndarray]
                The integrand
            lower_limit : float
            upper_limit : float
            n_points : int
                The number of points to use in the approximation,
                if it's not odd, it will be rounded up to the next
                odd number
        """

        # Ensure n_points is positive and odd
        n_points = abs(n_points)
        if n_points < 2:
            n_points = 2
        if n_points % 2 == 0:
            n_points += 1

        xs = np.linspace(lower_limit, upper_limit, num=n_points)
        ys = f(xs)

        h = xs[1] - xs[0]

        s = ys[0] + ys[-1] + 4 * ys[1:-1:2].sum() + 2 * ys[2:-1:2].sum()
        return h * s / 3

    def u(self, x: np.ndarray, t: np.ndarray) -> np.ndarray:
        x = np.asarray(x)
        t = np.asarray(t)

        n = np.arange(1, self.num_terms + 1)

        spatial_part = np.sin(np.outer(n, np.pi * x / self.length))
        decay_part = np.exp(-self.kappa * ((n * np.pi / self.length) ** 2)
                            * t[:, None])

        weighted_decay = decay_part * self.coeffs[None, :]

        return weighted_decay @ spatial_part


if __name__ == "__main__":
    # Problem Statement
    diffusivity = 0.1
    rod_length = 1.0

    def initial_condition(x):
        return x * (rod_length - x)

    # Visualisation Parameters
    xs = np.linspace(0, rod_length, 4 * 500)
    t_max = 5.0
    dt = 0.1

    # Solver Parameters
    num_terms = 500
    n_int_points = 501

    solver = SeriesSolver(diffusivity, rod_length, initial_condition,
                          num_terms, n_int_points)
    solver.visualise(xs, t_max, dt)
