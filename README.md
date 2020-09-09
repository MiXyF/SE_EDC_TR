SE_EDC_TR
Stéphane Youssef
Johanna Forunat

======= E5 SE Etude de cas de temps réel distribué =======


* objectif

L'objectif de cette étude est de parcourir l'ensemble d'un cycle de développement d'un système embarquable réparti et temps réel, de l'analyse à la réalisation d'un prototype opérationnel.
Le système retenu intègrera au final :
des procédés réels à commander,
leurs interfaces électroniques,
éventuellement des procédés simulés à commander,
plusieurs processeurs assurant différentes fonctionnalités (acquisition, conversion de commande, contrôleur, IHM, journal de trace ...),
des réseaux de communication entre ces processeurs.
Durant l'étude de cas, nous nous attacherons à d'abord établir des modèles fonctionnels et à en simuler le comportement avant de passer à la réalisation. Tout élément devra être testé et validé.

* Déroulement

- Synthèse de contrôleurs
Modélisation des procédés
Définition et synthèse de contrôleurs
Définition des intervalles possibles d'échantillonnage et de retards.
Définition des critères de qualité
Simulation d'une implantation distribuée

- Implantation mono-processeur
Prise en main de l'architecture matérielle et de l'architecture logicielle
Développement de drivers d'E/S
Implantation temps réel des lois de commandes
Tests et validation avec le procédé

- Extension multi-processeurs
Découpage de la loi de commande
Développement de drivers de communication
Implantation distribuée temps réel
Tests et validation avec le procédé
Intégration du controle du deuxième procédé

- Comparaison avec les simulations
Prise en compte, dans les simulations, des caractéristiques de l'implantation réalisée
Comparaison avec les simulation réalisées en 2)
Comparaison avec le comportement réel
Conclusion



===== Compilation d'un projet =======

Chaque projet est lancé via un script qui exécute un Makefile
Dans le Makefile : 1ère ligne : fichier objet cible modifiable
                    ligne KIT : ajouter "lan". ex: arcom11.lan.esiee.fr
Commandes du Makefile :
make ./     : lance le script
make clean  : nettoie le projet des fichiers générés
make send   : lance le programme sur la carte

ID ssh  : arcom
MDP ssh : arcom

Après connection sur la carte Arcom:
$<>: ./runarcom
dans runarcom : $1.ko : le "$1" correspond à l'argument passé au module à exécuter 


source & docs : https://perso.esiee.fr/~kocikr/ecas.html