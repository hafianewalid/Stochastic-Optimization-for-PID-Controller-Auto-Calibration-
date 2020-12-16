import random

import math

from generic import *


class simulatedannealingLs(LocalSearchAlgorithm):
    def __init__(self, prob, options):
        """ Constructeur de la super classe
        """
        LocalSearchAlgorithm.__init__(self, prob, options)
        self.temperature=options["temperature"]
        self.refroidissement = options["refroidissement"]

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
            xp=random.choice(candidates)  # tirer une sol aliatoirement
            xpval=self._problem.eval(xp)  # eval la sol

            # selectioner newsol si l'eval de newsol mieux que x
            if (self.better(xpval,self._solution.value)):
                # mise a jour de temperature
                if(self.temperature*self.refroidissement>0):
                 self.temperature *= self.refroidissement
                return xp

            a = random.uniform(0, 1)
            # calcule de proba
            proba = math.exp(-math.fabs(xpval-self._solution.value) / self.temperature)
            # mise a jour de temperature
            if (self.temperature * self.refroidissement > 0):
                self.temperature *= self.refroidissement

            self.segma=proba

            # selectioner newsol avec une proba si non selectioner x
            if a <= proba:
                return xp
            else:
                return self.curent_solution

    def accept(self, new_solution):
        # accepter toujour
        cur_val = self._solution.value
        new_val = self._problem.eval(new_solution)
        return True




