import random
from generic import *


class RandHillCliming(LocalSearchAlgorithm):
    def __init__(self, prob,  options):
        """ Constructeur de la super classe
        """
        LocalSearchAlgorithm.__init__(self, prob, options)
        self.alpha=options["alpha"]

    def get_neighbors(self):
        """ retourner les voisins de la solution courante
        """
        return self._solution.neighbors()

    def filter_neighbors(self, neighbors):
        """ filtrer toutes les solutions violant les contraintes
        """
        return [n for n in neighbors if self._problem.feasable(n)]

    def select_next_solution(self, candidates):
        if len(candidates) > 0:
            # choisissant une solution voisine aleatoire avec proba alpha
            p = random.uniform(0,1)
            if p<self.alpha:
                return random.choice(candidates)
            # choisissant la meilleure solution avec proba 1-alpha
            else:
                evalbest = self._problem.eval(candidates[0])
                solbest = candidates[0]
                for s in candidates:
                    val = self._problem.eval(s)
                    if self.better(val, evalbest):
                        evalbest = val
                        solbest = s
                return solbest

        return None

    def accept(self, new_solution):
        cur_val = self._solution.value
        new_val = self._problem.eval(new_solution)
        return True




