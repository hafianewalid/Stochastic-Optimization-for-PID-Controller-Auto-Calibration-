import math

from generic.evolution_strategy import EvolutionStrategy


class One_Plus_One_EA(EvolutionStrategy):
    def __init__(self, prob, options):
        EvolutionStrategy.__init__(self, prob, options)

    def sample_solutions(self):
        x=self._m.clone()
        # calcule le vecteur z
        z=self.normal(x._dim)
        z = [i * self._sigma for i in z]
        # appliquer une mutation avec le vecteur z
        for i in range(len(z)):
         x.solution[i]+=z[i]
        return [x,self._m.clone()]

    def update_sigma(self, sample):
        # mise a jour de sigma

        if (self.better(sample[0]._value,sample[1]._value,)):
           self._sigma*=math.exp(1./3.)
        else:
           self._sigma /=math.pow(math.exp(1./3.),1./4.)

        self.sort_pop(sample)


    def update_m(self, sample):
        # mise a jour de la moy ( m )
        if(self.better(sample[0]._value,self._m._value)):
         self._m=sample[0]


    # fonction de comparaison
    def better(self, v1, v2) :
        if self._problem.maximize and v1 >= v2:
            return True
        if self._problem.minimize and v1 <= v2:
            return True
        return False
