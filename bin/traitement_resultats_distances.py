import matplotlib.pyplot as plt
import csv
from math import sqrt

def test(T, index):
    with open(f'test_{T}.csv', newline='') as csvfile:
        spamreader = csv.reader(csvfile, delimiter=',')
        next(spamreader)
        nb_particules, distances_max, temps =  map(list, zip(*list(spamreader)))
        nb_particules = list(map(int, nb_particules))
        distances_max = list(map(float, distances_max))
        temps = list(map(float, temps))

    racine = [5.6*sqrt(n) for n in nb_particules[:32]]

    plt.subplot(2, 2, index)
    plt.title("T = 1")
    plt.plot(nb_particules, distances_max, 'o')
    plt.plot(nb_particules[:32], racine, 'k')
    plt.legend(["distances maximales observées", "distances théoriques en O(sqrt(n))"])
    plt.xlabel("Nombre de particules")
    plt.ylabel("Distance maximale")


test(1, 1)
test(10, 2)
test(50, 3)
test(100, 4)
plt.show()
