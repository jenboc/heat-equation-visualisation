from solvers.mol_solver import MoLSolver
import numpy as np
import sys

out_path = sys.argv[1] if len(sys.argv) > 1 else None

kappa = 0.1
length = 1.0


def initial_condition(x):
    return np.sin(2 * np.pi * x)


num_terms = 500
num_integration_points = 501
solver = MoLSolver(kappa, length, initial_condition)

xs = np.linspace(0, length, 4 * num_terms)
t_max = 1.5
dt = 0.01

solver.visualise(xs, t_max, dt, out_path=out_path)
