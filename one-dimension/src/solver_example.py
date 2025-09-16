# An example of a Solver implementation
# NOTE: This does NOT solve the heat equation, it's just an example.

from solver import Solver
import numpy as np
import matplotlib

matplotlib.use("Qt5Agg", force=True)


class SolverExample(Solver):
    def __init__(self) -> None:
        super().__init__(1, 1, lambda x: 1)

    def u(self, x: np.ndarray, t: np.ndarray) -> np.ndarray:
        return np.sin(x[None, :] - t[:, None]) * np.exp(-0.1 * t[:, None])


if __name__ == "__main__":
    xs = np.linspace(-5, 5, 400)
    t_max = 1.0

    SolverExample().visualise(xs, t_max, 0.01)
