from generic import *

from problems import ContinuousFunctionProblem
import PID
from generic.solution import RealSolution


class pid_prob(Problem):
    def __init__(self, size, max_eval=1000):
        Problem.__init__(self, max_eval)
        self._name = "pid_prob"
        self._minimize = True

    def eval(self, sol):
        if isinstance(sol, RealSolution):
            self.nb_evaluations += 1
            x = sol.solution
            sol._value = PID.param_eval(x)/1000
            return sol._value
        else:
            return 9999999999

    def feasable(self, sol) :
        return True

    def generate_initial_solution(self, sol_type='empty'):
        initial_solution = RealSolution(dim=6).random()
        return initial_solution

    def print_solution(self, sol):
        return "val:{} sol:{}".format(str(sol._value), str(sol))



def generate_pid_instance(max_eval):
    problem = pid_prob(size=6, max_eval=max_eval)
    return problem
