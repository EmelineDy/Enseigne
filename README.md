# Enseigne

Projet Enseigne Pilotable en DMX

Lien video -> https://we.tl/t-515NL5uiBc

Ce bureau d'étude avait pour but la création d'un objet connecté mêlant actionneurs et capteurs. 
Ce projet devait répondre à une besoin et nous nous sommes donc penchées sur la création d'une enseigne LED commandable 
avec une table de mixage, pour une utilisation en concert.


Le DMX est un protocole de communication basé sur un bus numérique utilisant 3 fils (2 pour le signal qui est symétrisé + la masse). 
En fait, il se contente de transmettre des données (des nombres compris entre 0 et 255). 

Les Canaux 1, 2 et 3 de la table de mixage permettent de gérer respectivement la couleur Rouge, Verte et Bleu des Lettres. Le Canal 4 est le canal Animation. 
Nous avons mappé 5 types d'animations selon un découpage proportionnel entre 0 et 255 (qui correspondent aux valeur min et max du fader). 

Si le canal Animation est placé entre 0 et 50, nous pouvons allumer le ruban du tour avec les caneau 6, 7 et 8 respectivement pour envoyer du Rouge, Vert et Bleu. 

Si on positionne le fader 4 entre 51 et 101, on anime un chargement (alumage progressif). On peut modifier la vitesse du chargement avec le canal 5. 

Nous avons également le "chase" lorsque le fader 4 est positionné entre 102 et 152 qui conciste à voir le ruban totalement allumé à l'exception de quelques led 
periodiquement éteintes. Ces leds éteintes changent et donnent l'impression d'un mouvement. Egalement, la vitesse est controllable avec le fader 5. 

L'animation Tour Eiffel elle se déclenche lorsque le fader est positionné entre 153 et 204. Nous pouvons régler la couleur et la vitesse de clignottement. 
L'animation donne l'impression que le ruban du tour "brille".

Enfin, l'animation rainbow, déclenchée lorsque le fader est positionné entre 205 et 255, affiche un arc en ciel qu'il est possible d'animer en vitesse avec le canal 5. 
On a l'impression que les couleurs se déplacement, plus ou moins vite selon la valeur du potentiomètre du fader vitesse. 


Nous avons décidé de découper notre programme en quatre classes, séparant les différents types de LEDs, ainsi que les animations.

Contact
Camille Margout - margout@insa-toulouse.fr
Emeline Delhay - delhay@insa-toulouse.fr 

