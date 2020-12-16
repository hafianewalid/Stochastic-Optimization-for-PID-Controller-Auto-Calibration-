#! /usr/bin/env python
# -*- coding: utf-8 -*-
import numpy as np
from random import shuffle
import random as rrnd

"""
Classe abstraite a concrétiser, voire plus bas.

"""

class Solution(object):

    def __init__(self, dim=None, x=None):
        """ constructeur presque vide  voire les classe concrèrte plus bas """
        if dim is None and x is None :
            raise ValueError("Il faut spécifier la dimension ou une solution")
        
        if dim is None :
            self._dim = len(x)
        else :
            self._dim = dim
        if x is not None :
            self._sol = np.copy(x)
            
        self._value = None

    @property
    def solution(self):
        return self._sol

    @property
    def value(self):
        return self._value
    
    def random(self):
        """ 
        crée une solution aléatoire
        retourne un instance de Solution 
        """
        raise NotImplementedError
    
    def neighbors(self):
        """
        permet de construire l'ensemble des solutions voisines de la solution
	    courante
        retourne ensemble solutions voisines de this
        """
        raise NotImplementedError
        
    def clone(self):
        """ clone la solution dans une nouvelle instance """
        raise NotImplementedError

    def __str__(self):
        """ une méthode to string """
        raise NotImplementedError
        
    def __eq__(self, other):
        """ 
        une méthode pour vérifier l'égalité de deux solutions 
        Il faut la même dimention et que les tableaus soient identiques
       
        """
        if  self._dim != other._dim : 
            return False
        if self._sol.shape != other._sol.shape :
            return False
        return (self._sol == other._sol).all()

"""
Classe abstraite pour representer une solution comme un tableau binnaire

"""

class BinarySolution(Solution):
    
    def __init__(self, dim=None, x=None):
        Solution.__init__(self, dim, x)
        if x is None :
            self._sol = np.zeros(dim, dtype=np.bool_)

    def random(self):
        """ Retourne une solution aléatoire """
        rnd = np.random.random(self._dim) < 0.5
        return BinarySolution(x=rnd)
    
    def neighbors(self):
        """ Retourne toutes les solutions voisines i.e. differentes de 1 """
        N = []
        for i in xrange(len(self._sol)):
            n = np.copy( self._sol )
            n[i] = not n[i]
            N.append(BinarySolution(x=n))

        # mélanger pour rendre le parcours non déterministe 
        shuffle(N)
        return N
    
    def clone(self):
        """ Pour cloner la solution """
        clone_sol =  BinarySolution( x=self._sol )   
        clone_sol._value = self.value
        return clone_sol
    
    def __str__(self):
        """ une méthode to string pour afficher la solution """
        return "".join([ '1' if i else '0' for i in  self._sol ])
    


"""
Classe abstraite pour representer une solution comme un tableau de permutation

"""

class PermutationSolution(Solution):

    def __init__(self, dim=None, x=None):
        Solution.__init__(self, dim, x)
        if x is None :
            self._sol = np.zeros(dim, dtype=np.int)

    def random(self):
        """ Retourne une solution aléatoire """
        rnd = np.arange(self._dim, dtype=np.int)
        np.random.shuffle(rnd)
        return PermutationSolution(x=rnd)
        
    def neighbors(self):
        """ 
        Retourne toutes les solutions voisines i.e. differentes de 1 
        échanger 2 elements du tableau 
        
        """
        N = []
        for i in xrange(len(self._sol)):
            for j in xrange(i+1,len(self._sol)):
                if i != j :
                    n = np.copy( self._sol )
                    tmp = n[i] 
                    n[i] = n[j]
                    n[j] = tmp
                    N.append( PermutationSolution (x=n) )

        # mélanger pour rendre le parcour non déterministe 
        shuffle(N)
        return N
    
    def clone(self):
        """ Pour cloner la solution """
        clone_sol = PermutationSolution( x=self._sol )
        clone_sol._value = self.value
        return clone_sol
    
    def __str__(self):
        """ une méthode to string pour afficher la solution """
        return ",".join([ str(i) for i in  self._sol ])
    


    
"""
Classe abstraite pour representer une solution comme un vecteur de réels

"""

class RealSolution(Solution):
    
    def __init__(self, dim=None, x=None):
        Solution.__init__(self, dim, x)
        if x is None :
            self._sol = np.zeros(dim, dtype=np.float_)
        self.k = dim;

    def random(self):
        """ Retourne une solution aléatoire dans [-5, 5]^dim """
        rnd = np.random.random(self._dim)
        rnd *= 10
        rnd -= 5
        return RealSolution(x=rnd)
    
    def neighbors(self):
        V=[]
        for j in range(3):
            X=[]
            for i in range(self._sol.size):
                k=rrnd.uniform(0,1)
                if(k>=0.5):
                    X.append(self._sol[i]+rrnd.uniform(0,5))
                else:
                    X.append(self._sol[i]-rrnd.uniform(0,5))
            v = Solution.__init__(self, self._sol.size,x=X)
        return [v]
    
    def clone(self):
        """ Pour cloner la solution """
        clone_sol =  RealSolution( x=self._sol )   
        clone_sol._value = self.value
        return clone_sol
    
    def __str__(self):
        """ une méthode to string pour afficher la solution """
        return ",".join([ str(i) for i in  self._sol ])


class IntSolution(Solution):
    def __init__(self, dim=None,x=None,k=None):
        Solution.__init__(self, dim, x)
        if x is None:
            self._sol = np.zeros(dim, dtype=np.dtype(int))
        self.k=k;

    def random(self):
        """ Retourne une solution aléatoire dans [1,k]^dim """
        rnd = np.random.randint(self.k, size=self._dim)

        return IntSolution(x=rnd,k=self.k)

    # ensmble des voisin d'une solution
    def neighbors(self):
        N=[]
        for i in range(len(self.solution)):
            next=self.nextval(self.solution[i])
            v=self.clone()
            v.solution[i]=next[0]
            N.append(v)
            v = self.clone()
            v.solution[i] = next[1]
            N.append(v)

        shuffle(N)
        return N

    # gestion de valeur serculaire
    def nextval(self,val):
        valsup=val+1
        valinf=val-1
        if valsup==self.k:
            valsup=0;
        if valinf == -1 :
            valinf = self.k-1;
        return (valsup,valinf)

    def clone(self):
        """ Pour cloner la solution """
        clone_sol = IntSolution(x=self._sol,k=self.k)
        clone_sol._value = self.value
        return clone_sol

    def __str__(self):
        """ une méthode to string pour afficher la solution """
        return ",".join([str(i) for i in self._sol])

    
