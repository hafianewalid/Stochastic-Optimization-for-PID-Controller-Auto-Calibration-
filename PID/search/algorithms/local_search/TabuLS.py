#! /usr/bin/env python
# -*- coding: utf-8 -*-
from generic import *
class tabuLS(LocalSearchAlgorithm):
    def __init__(self, prob, options):
        """ Constructeur de la super classe
        """
        LocalSearchAlgorithm.__init__(self, prob, options)
        self.taille=options["taille"]
        self.Tabu=[]

    def get_neighbors(self):
        """ retourner les voisins de la solution courante
        """
        return self._solution.neighbors()

    def filter_neighbors(self, neighbors):
        """ filtrer toutes les solutions violant les contraintes
        """
        return [n for n in neighbors if self._problem.feasable(n)]

    def select_next_solution(self, candidates):
        """ Si il y des solutions (après filtrage),
        retourner la solution qui n'est pas dans la liste Tabu
        et maximise l'evaluation
        """
        if len(candidates) > 0:
          evalbest=self._problem.eval(candidates[0])
          solbest = candidates[0]
          for s in candidates:
             val=self._problem.eval(s)
             if self.better(val,evalbest) and self.notinTabu(s):
                 evalbest=val
                 solbest=s
          return solbest

        return None

    def accept(self, new_solution):
        """ accepte la solution toutjour
        """
        cur_val = self._solution.value
        new_val = self._problem.eval(new_solution)
        # si on dépasse la taille maximale supprim deb
        if len(self.Tabu)+1>self.taille:
            self.Tabu.pop(0)
        # ajout a la fin
        self.Tabu.append(new_solution.clone())
        return True

    # une fonc qui verifer si un elem n'est pas dans la liste Tabu
    def notinTabu(self,s):

        for i in self.Tabu :
            if(i.__eq__(s)):
                return False
        return True