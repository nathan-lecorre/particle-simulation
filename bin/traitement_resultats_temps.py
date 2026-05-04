import numpy as np
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


    a, b, c = np.polyfit(nb_particules[0:31], temps[0:31], 2)
    carre = [a*n*n + b*n + c for n in nb_particules[0:31]]
    plt.subplot(2, 2, index)
    plt.title(f"T = {T}")
    plt.plot(nb_particules, temps, 'o')
    plt.plot(nb_particules[0:31], carre, 'k')
    plt.legend(["temps observé", "temps théorique en O(n^2)"])
    plt.xlabel("Nombre de particules")
    plt.ylabel("Temps d'exécution de l'innovation (ms)")

test(1, 1)
test(10, 2)
test(50, 3)
test(100, 4)
plt.show()
