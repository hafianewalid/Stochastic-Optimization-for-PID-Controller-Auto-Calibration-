import math

from generic.evolution_strategy import EvolutionStrategy


class Mu_Coma_Lambda_ES(EvolutionStrategy):
    def __init__(self, prob, options):

        EvolutionStrategy.__init__(self, prob, options)
        # initalisation
        self.w=[1./self._mu]*self._mu
        self.mu_w=1./sum([x*x for x in self.w])
        self.n=self._m._dim
        self.P=[0]*self.n
        self.c=4./math.sqrt(self.n)
        self.EN=math.sqrt(self.n)*( 1. - (1./(4*self.n)) + (1./(21.*self.n*self.n)))

        self.a = (1. - self.c)
        self.b = math.sqrt(1. - (self.a * self.a)) * math.sqrt(self.mu_w)

        self.Z=[[0]*self.n]*self._lambda

    def sample_solutions(self):
        # construction d'une population
        # a partire de lambda mutation
        # appliquer a la moy m
        X=[]
        self.Z=[0]*self._lambda
        for j in range(self._lambda):
            x=self._m.clone()
            z=self.normal(x._dim)
            z = [i * self._sigma for i in z]
            self.add(x.solution,z)
            self.Z[j]=z
            X.append(x)

        return X

    def update_m(self, sample):
        # mise a jour de moy
        # selectionner + ordonner ( par f )
        # les millieur mu individu parmi lamda individu
        sample,self.Z=self.best_mu(sample,self.Z)
        self._m._sol=[0]*self.n
        # calcule la moy de millieur mu individu
        for i in range(self._mu):
           v=[x*self.w[i] for x in sample[i].solution]
           self.add(self._m.solution,v)

    def update_sigma(self, sample):
        # mise a jour de sigma
        # calcule de zw
        zw = [0] * self.n
        for i in range(self._mu):
            v = [x*self.w[i] for x in self.Z[i]]
            self.add(zw, v)
        # mise a jour de P
        #self.P=[ (( x*self.a )+( y*self.b )) for x, y in zip(self.P, zw)]
        for i in range(len(self.P)):
            self.P[i]=(self.P[i]*self.a)+(zw[i]*self.b)
        # calcule de norme de P

        pl=math.sqrt(sum([ x*x for x in self.P]))
        #print self._sigma
        #print zw
        #print pl

        # calcule de valeur de mise a jour
        e = math.exp(self.c * ((pl / self.EN) - 1))

        # mise a jour de sigma
        # + teste pour que segma
        # ne vaut pas une valeur null
        if(self._sigma*e>0.0001):
         self._sigma*=0.9999
        #print self._sigma


    # une fonction selecionne les mu
    # millieur sol parmi lamda sol
    def best_mu(self,X,Z):
        f=[x.value for x in X]
        #print f
        if self._problem.maximize :
            tri_ind = sorted(range(len(f)), key=lambda k: f[k],reverse=True)
        else:
            tri_ind=sorted(range(len(f)), key=lambda k: f[k])
        resX=[]
        resZ=[]
        for i in range(self._mu):
            resX.append(X[tri_ind[i]])
            resZ.append(Z[tri_ind[i]])

        return resX,resZ

    # une fonction qui calcule
    # une somme des vecteurs
    # X=X+Y
    def add(self,X,Y):
        for i in range(len(X)):
            X[i]+=Y[i]
