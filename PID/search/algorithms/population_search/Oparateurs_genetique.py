import random
from generic import BinarySolution


class operateurgenitique :

    # l operation genitique de mutation uniforme
    def mutation_uniforme(self, X):
           # cloner l'indevedu X
           x=X.clone()
           # changer la valeur de chaque genome
           # avec un poba 1/n
           for i in range(len(x.solution)):
               a = random.uniform(0, 1)
               if a < 1.0 / len(x.solution):
                   x.solution[i] = random.uniform(-1,1)
           return x

    '''
    # l operation genitique de mutation uniforme

    def mutation_uniforme(self, X):
        # cloner l'indevedu X
        x = X.clone()
        # changer la valeur de chaque genome
        # avec un poba 1/n
        for i in range(len(x.solution)):
            a = random.uniform(0, 1)
            if a < 1.0 / len(x.solution):
                x.solution[i] = not x.solution[i]
        return x
        '''

    # l operation genitique de croisement point
    def croisement_point(self,X,Y):
        # point de croisement
        point= random.randint(0,len(X.solution)-1)

        # construction deux indevedu avec les 4 partie
        # x = partie X1 + partie Y2
        x=X.clone()
        x.solution[point:len(X.solution)]=Y.solution[point:len(X.solution)]

        # y = partie Y1 + partie X2
        y=Y.clone()
        y.solution[point:len(X.solution)]=X.solution[point:len(X.solution)]

        return x,y

    def croisement_uniforme(self,X,Y):
        x=X.clone()
        #construction x a avec une proba de
        # 1/2 que un genome d'un indv heriter de X
        # er 1/2 que un genome d'un indv heriter de Y
        for i in range(len(X.solution)):
            a=random.uniform(0, 1)
            if(a<=1/2):
                x.solution[i]=Y.solution[i]

        return x