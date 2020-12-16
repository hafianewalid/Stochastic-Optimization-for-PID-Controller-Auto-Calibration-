#! /usr/bin/env python
# -*- coding: utf-8 -*-
import numpy as np

import random

from algorithms.population_search.Oparateurs_genetique import operateurgenitique
from generic import *

class OnePlusOneEA(PopulationSearchAlgorithm):
    def __init__(self, prob, options):
        """
        Entrées :

        * Un problème donnée instance de la classe Problem.
        Seules les problèmes dont les solution sont des tableaus booléens sont
        acceptée.

        * Un dictionnaire des paramètres des algorithmes

        """
        PopulationSearchAlgorithm.__init__(self, prob, options)
        self.op=operateurgenitique()

    def make_parent_pop(self):
        return [self._pop[0]]



    def evolve_pop(self, parents):

        # on prend une solution x
        x = parents[0]

        # on la modifie y= mut(x)
        y = self.op.mutation_uniforme(x.clone())

        # on a rajoute a la liste
        offspring = [y]

        return offspring

    def make_new_pop(self, offspring):
        # on garde le meilleurs entre x et y
        if(self.better(offspring[0].value,self._pop[0].value)):
            survivors = [offspring[0]]
        else:
            survivors = [self._pop[0]]
        return survivors
