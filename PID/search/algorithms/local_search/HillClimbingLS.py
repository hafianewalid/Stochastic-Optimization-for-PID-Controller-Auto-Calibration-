from generic import *
class hillclimbingLS(LocalSearchAlgorithm):
    def __init__(self, prob, options):
        """ Constructeur de la super classe
        """
        LocalSearchAlgorithm.__init__(self, prob, options)

    def get_neighbors(self):
        """ retourner les voisins de la solution courante
        """
        return self._solution.neighbors()

    def filter_neighbors(self, neighbors):
        """ filtrer toutes les solutions violant les contraintes
        """
        return [n for n in neighbors if self._problem.feasable(n)]

    def select_next_solution(self, candidates):
        """ Si il y des solutions (apres filtrage), retourner la solution qui maximise l'evaluation
        """
        if len(candidates) > 0:
          evalbest=self._problem.eval(candidates[0])
          solbest = candidates[0]
          for s in candidates:
             val=self._problem.eval(s)
             if self.better(val,evalbest):
                 evalbest=val
                 solbest=s
          return solbest

        return None

    def accept(self, new_solution):
        """ accepte la solution si newsol est millieur que la millieur sol
        """
        cur_val = self._solution.value
        new_val = self._problem.eval(new_solution)
        return self.better(new_val,cur_val)