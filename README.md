# Organización del computador II - tp3 - 1c - 2016

Ideas para manejar la pantalla:
	
-En cada ciclo de reloj, se miran las estructuras que son
representadas en pantalla (jugadores, tareas, relojes) y se las pinta. 
Las funciones de "mover" solo actualizan las estructuras y limpian su 
posicion anterior. Hace que la interrupcion de reloj sea un poco mas lenta.

-Poseer una copia del pixel donde va a pararse el jugador 
(en que estructura?), y al irse de esa posicion, restablecer el pixel copia.
Asqueroso, pero rapido.

-Las funciones que realizan movimientos se encargan de todo: pintar, 
modificar estructuras y checkear si  en la posicion de la que me estoy yendo
hay que dibujar alguna tarea. Esta forma implica recorrer una estructura con
todas las tareas existentes y ver sus coordenadas.
La interrupcion de teclado se va a atender mas lento.



