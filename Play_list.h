/*
    Author: Reyes Vilchis Juan Jose
    Fecha: 10/12/2017
    Licencia:
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/
#ifndef __PLAY_LIST_H__
#define __PLAY_LIST_H__
#include<stdlib.h>
#include <string.h>
#include <dirent.h>
#include<time.h>
#include"Song.h"
typedef struct play_list{
  Song* s;
  struct play_list* next;
  struct play_list* previous;
  char* title;
}Play_list;
/*Add song recibe de parametro un apuntador doble a Play_list, una cadena que es la ruta del archivo
y el titulo del a cancion, devuelve un 0 si se pudo guardar o un error*/
int add_song(Play_list**,char* uri,char* title);
/*Recibe de parametros un apuntador doble a Play_list y cambia la cancion, regresa un 0 si se pudo cambiar
o un error en caso contrario*/
int next_song(Play_list**);
/*previous_song Recibe de parametros un apuntador doble a Play_list y regresa un 0 si se pudo regresar la cancion
esta funcion regresa al principio de la cancion si el tiempo transcurrido es mayor a 3.5 segundos,
si es menor se va a la cancion anterior*/
int previous_song(Play_list**);
/*Esta funcion agrega todas las canciones de una carpeta, recibe de parametro un
apuntador doble a Play_list y una cadena (ruta donde se buscara), por ahora solo funciona
con archivos .mp3*/
int add_multiple_songs(Play_list**,char*);
/*Recibe se parametros un apuntador doble a Play List y regresa un entero que corresponde
a la cantidad de elementos que hay en la Play_list*/
int len_play_list(Play_list**);
/*Precaucion: Quit_song quita una cancion de la lista pero NO BORRA la informacion, devuelve
el propio elemento a buscar, recibe de parametro apuntador doble a Play_list y el indice de elemento
a sacar, si no se encuentra el indice regresa NULL*/
Play_list* quit_song(Play_list**,int);
/*link_play_list une dos play_list, recibe apuntadores doble a las Play_list y lo guarda
No borra informacion*/
void link_play_list(Play_list **l1,Play_list **l2);
/*delete_play_list Borra totalmente las canciones y devuelve NULL despues de haberlas borrado*/
int delete_play_list(Play_list**);
/*random_play_list Revuelve las play_list, recibe de parametro el apuntador doble a Play_list,
este apuntador no se modificara, pero los siguientes elementos de la Play_list si*/
void random_play_list(Play_list**);
/*show_play_list Recibe apuntador a play_list y muestra todos los elementos de esta.*/
int show_play_list(Play_list*);
void play_list_play(Play_list*);
#endif
