# Solving the heat equation using the series that arises from
# Separation of Variables

from typing import Callable
from solver import Solver
import numpy as np


class SeriesSolver(Solver):
    def __init__(self, diffusivity: float, rod_length: float,
                 initial_condition: Callable[[np.ndarray], np.ndarray]):
        super().__init__(diffusivity, rod_length, initial_condition)

    def u(self, x: np.ndarray, t: np.ndarray) -> np.ndarray:
        pass


if __name__ == "__main__":
    # Problem Statement
    diffusivity = 1
    rod_length = np.pi

    def initial_condition(x):
        return np.sin(x)

    # Visualisation Parameters
    xs = np.linspace(-5, 5, 400)
    t_max = 1.0
    dt = 0.1

    solver = SeriesSolver(diffusivity, rod_length, initial_condition)
    solver.visualise(xs, t_max, dt)
