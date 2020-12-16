from algorithms.population_search.MuComaLambdaGA import MuComaLambdaGA


class MuPlusLambdaGA(MuComaLambdaGA) :
    def __init__(self, prob, options):
        MuComaLambdaGA.__init__(self, prob, options)

    # redefini la methode pour routourner les parents et enfants
    # la methode de la classe mere ( MuComaLambdaGA ) return que les
    # enfants alors on utilise cette methode et on ajout les parents
    def evolve_pop(self, parents):
        return super(MuPlusLambdaGA, self).evolve_pop(parents)+parents