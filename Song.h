/*
    Author: Reyes Vilchis Juan Jose
    Fecha: 10/12/2017
    Licencia:
    This file is part of ReproductorMusica.
    ReproductorMusica is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.*/
#ifndef __Song_H__
#define __Song_H__
#include<vlc/vlc.h>
#include<stdlib.h>
typedef struct song{
  libvlc_instance_t *instance;
  libvlc_media_player_t *mediaPlayer;
  libvlc_media_t *media;
}Song;
/*create_song recibe de parametro una cadena, que corresponde a una ruta y
devuelve el apuntador a dicha cancion creada*/
Song* create_song(char* ruta);
/*recibe de parametro un apuntador a "song" y la reproduce*/
void play_song(Song*);
/*recibe de parametro un apuntador a "song" y la pausa*/
void pause_song(Song*);
/*recibe de parametro un apuntador a "song" y la detiene totalmente*/
void stop_song(Song*);
/*Carga toda la media de una cancion y la reproduce*/
void open_media(Song*);
/*Borra totalmente la cancion, necesita de un doble apuntador a "song"*/
int delete_song(Song **);
/*Esta funcion detiene una cancion o la reproduce dependiendo si se encuentra en pausa o sonando*/
void on_playpause(Song*);
#endif
