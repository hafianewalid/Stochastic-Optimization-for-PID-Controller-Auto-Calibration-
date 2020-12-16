#! /usr/bin/env python
# -*- coding: utf-8 -*-
import numpy as np

import random

from algorithms.population_search.Oparateurs_genetique import operateurgenitique
from generic import *


class MuComaLambdaGA(PopulationSearchAlgorithm):
    def __init__(self, prob, options):
        """
        Entrées :

        * Un problème donnée instance de la classe Problem.
        Seules les problèmes dont les solution sont des tableaus booléens sont
        acceptée.

        * Un dictionnaire des paramètres des algorithmes

        """
        PopulationSearchAlgorithm.__init__(self, prob, options)
        self.op = operateurgenitique()
        self.pc=options["Pc"]
        self.pm=options["Pm"]

    def make_parent_pop(self):
        return self._pop


    def evolve_pop(self, parents):
        offspring = []
        while len(offspring)<self._lambda :
            # on prend deux solution
            x = random.choice(parents).clone()
            y = random.choice(parents).clone()

            # mutation avec une proba  = pm
            a = random.uniform(0, 1)
            if a<self.pm:
             x = self.op.mutation_uniforme(x)
             y = self.op.mutation_uniforme(y)

            # croisement avec proba = pc
            a = random.uniform(0, 1)
            if a < self.pc:
             #x,y = self.op.croisement_point(x,y)
             x=self.op.croisement_uniforme(x,y)
             y=self.op.croisement_uniforme(x,y)

            # on a rajoute a la liste x et y
            offspring.append(x)
            offspring.append(y)

        return offspring

    def make_new_pop(self, offspring):
        # on garde le mu meilleurs ( selection des mu solution )

        return self.selection_deterministe(offspring,self._mu)
        #return self.selection_roulette_fitness(offspring,self._mu)
        #return self.selection_roulette_rang(offspring,self._mu,2)
        #return self.tournoi(offspring,5,self._mu)


    # methode deterministe de selection de ( pression elem parmi les m elem de offspring)
    def selection_deterministe(self,offspring,Presion):
        #selecionner les P millieur solutions
        self.sort_pop(offspring)
        return  offspring[:Presion]

    # methode selection_roulette_fitness de selecion des n elem parmi les m elem de population P
    def selection_roulette_fitness(self,P,n):
        # initaliser la somme par epsilon pour eviter la division par 0
        sommefitness=0.00000000001
        # on fais la somme des fitness des solution faisable
        for e in P :
            if(e.value!=float("inf") and e.value!=float("-inf") ):
                sommefitness+=e.value

        # calcule de probabilite proportionelle par fitness
        Pr=[float(i.value)/sommefitness for i in P]

        #lancer la roulette n fois  et returner les resultat ( les n elem selecionner )
        return self.roulette(P,Pr,n)

    # methode selection_roulette_rang de selection des n elem parmi les m elem de population P
    def selection_roulette_rang(self,P,n,s):
        # tri de population
        self.sort_pop(P)
        m=len(P)
        d=(m*(m-1))
        # calcule de probabilite proportionelle par rang
        Pr=[]
        for j in range(m):
            i=m-j-1 # calculer le rang tq le rang de millieur est m-1
            Pr.append (  ( (2.0-s)/m ) + ( ( 2.0*i*(s-1) )/d) ) # calcule de proba de j'eme elem

        # lancer la roulette n fois  et returner les resultat ( les n elem selecionner )
        return self.roulette(P,Pr,n)

    # lance de roulette n fois pour
    # selectionner n elem pami les m de la population P
    # avec des proba pour chaque elem de la population ( Pr )
    def roulette(self,P,Pr,n):
        # calcule De  CDF pour tout les m elem de population
        CDF=[Pr[0]]
        for i in range(1,len(Pr)):
            CDF.append(CDF[i-1]+Pr[i])

        # lancer la roulette n fois et selecionner n elem parmi les m elem de p
        S=[]
        for i in range(n):
            r=random.uniform(0,1)
            j=0
            while(j<len(CDF)-1 and CDF[j]<r):
                j=j+1
            S.append(P[j].clone())

        return S

    # methode tournoi de selection des n elem parmi les m elem de population P
    def tournoi(self,P,k,n):
        S=[]
        # selecionner n elem
        for i in range(n):

         # choisir aleatoirement k elem
         l=[]
         for j in range(k):
             l.append(random.choice(P))

         # choisir le millieur elem parmi les k elem
         best=l[0]
         for j in range(1,len(l)):
             if self.better(l[j].value,best.value):
                 best=l[j]
         # ajouter le millieur elem a la list des elem selecionnner
         S.append(best.clone())

        return S