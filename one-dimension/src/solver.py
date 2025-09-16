# Abstract class for implementing a solver for the equation.

from abc import ABC, abstractmethod
from typing import Callable


class Solver(ABC):
    # These three things will define our problem
    def __init__(self, diffusivity: float, rod_length: float,
                 initial_condition: Callable[[float], float]):
        self.kappa = diffusivity
        self.length = rod_length
        self.initial_condition = initial_condition

    # What's the value of the solution at this value at x and t?
    @abstractmethod
    def u(self, x: float, t: float) -> float:
        raise NotImplementedError()

    def visualise() -> float:
        pass
