import numpy as np

from generic import*

class K_libres_sommmes(Problem):

 def __init__(self,n,k,max_eval=1000):
    # initaliser les param de l'algo
    Problem.__init__(self, max_eval)
    self.n = n
    self.k = k
    self._name = "K_libres_sommmes"
    self._minimize = True

 def feasable(self, sol) :
        return True

 def eval(self, sol):
        self.nb_evaluations += 1
        x = sol.solution

        val = 0
        # extraire les k partition
        part=[];
        for i in range(self.k):
            part.append([])
        for i in range(self.n):
            part[x[i]].append(i+1);

        # compter le nombre de fois ou x+y=z
        for i in range(self.k):
            for xi in range(len(part[i])):
                for yi in range(xi,len(part[i])):
                    for e in part[i]:
                         if e==part[i][xi]+part[i][yi]:
                             val+=1

        sol._value = val*1.0
        return val*1

 def print_solution(self, sol):
     return "val:{} sol:{}".format(sol._value, str(sol))

 def generate_initial_solution(self, sol_type='empty'):
     if sol_type not in ['empty', 'random']:
         raise ValueError("Unknown inital solution type")
     # generer une solution INT
     initial_solution = IntSolution(x=np.ones(self.n,dtype=np.dtype(int)),k=self.k,dim=self.n)
     if sol_type is 'random':
         # generer une solution INT randome
         initial_solution = initial_solution.random()
     return initial_solution

# factory methode
def generate_k_libre_sommes_instance(max_eval,n,k):
     problem = K_libres_sommmes(max_eval=max_eval,n=n,k=k)
     return problem