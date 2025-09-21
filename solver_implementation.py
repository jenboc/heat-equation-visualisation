# An example of a Solver implementation
# NOTE: This does NOT solve the heat equation, it's just an example.

from solver import Solver
import numpy as np


class SolverExample(Solver):
    def __init__(self) -> None:
        super().__init__(1, 1, lambda x: 1)

    def u(self, x: np.ndarray, t: np.ndarray) -> np.ndarray:
        return np.sin(x[None, :] - t[:, None]) * np.exp(-0.1 * t[:, None])
