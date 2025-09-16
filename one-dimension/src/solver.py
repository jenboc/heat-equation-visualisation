# Abstract class for implementing a solver for the equation.

from abc import ABC, abstractmethod
from typing import Callable
from matplotlib.animation import FuncAnimation
import matplotlib.pyplot as plt
import numpy as np


class Solver(ABC):
    def __init__(self, diffusivity: float, rod_length: float,
                 initial_condition: Callable[[np.ndarray], np.ndarray]):
        # These three things well-define our problem
        self.kappa = diffusivity
        self.length = rod_length
        self.initial_condition = initial_condition

    @abstractmethod
    def u(self, x: np.ndarray, t: np.ndarray) -> np.ndarray:
        """ What's the value of the solution at these values of xs and ts?

        Parameters:
            x : np.ndarray
                The values of x to calculate u(x,t) at
            t : np.ndarray
                The values of t to calculate u(x,t) at
        Return Value : np.ndarray
            Shape = (len(t), len(x))
        """

        raise NotImplementedError()

    def visualise(self, xs: np.ndarray, t_max: float, dt: float) -> None:
        """ Visualise the solution

        Parameters:
            xs : np.ndarray
                The values of x to plot the solution at
            t_max: float
                The maximum value of t to plot before looping to t=0
            dt: float
                The difference in time between frames of the animation
        """

        ts = np.arange(dt, t_max + dt, dt)
        us = self.u(xs, ts)

        # Plot these us and animate over t
        fig, ax = plt.subplots()
        ax.set_xlabel("x")
        ax.set_ylabel("u(x,t)")
        ax.set_xlim(xs.min(), xs.max())
        ax.set_ylim(us.min(), us.max())
        line, = ax.plot([], [], lw=2)

        def init_anim():
            line.set_data([], [])

        def update_anim(frame):
            line.set_data(xs, us[frame])
            ax.set_title(f"t = {ts[frame]:.2f}")

        anim = FuncAnimation(fig, update_anim, frames=len(ts), 
                             init_func=init_anim, blit=False, interval=50,
                             repeat=True)

        plt.show()
