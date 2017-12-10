#include "Song.h"
#include <stdio.h>
/*La funcion "create_song recibe de parametro una ruta al a cual accedera VLC
para poder crear el mediaplayer y entonces reproducir la cancion"*/
Song* create_song(char* ruta){
  Song* s = (Song*)malloc(sizeof(Song));
  s->instance = libvlc_new(0,NULL);
  s->mediaPlayer = libvlc_media_player_new(s->instance);
  s->media = libvlc_media_new_location(s->instance,ruta);
  //Regresa apuntador a cancion
  return s;
}
/*Open media solo se hará cada que se reproduzca una nueva cancion,
en esta funcion se reproduce se establece el mediaplayer a reproducir y se reproduce.*/
void open_media(Song* s) {
    libvlc_media_player_set_media(s->mediaPlayer,s->media);
    libvlc_media_parse(s->media);
    play_song(s);
}
//"Play_song" recibe de parametros un apuntador a "Song" y la reproduce, primero se debe usar la funcion open media
void play_song(Song* s){
  libvlc_media_player_play(s->mediaPlayer);
}
//"pause_song" pausa una cancion, recibe de parametro apuntador a "Song", no olvidar usar antes open_media
void pause_song(Song* s){
  libvlc_media_player_pause(s->mediaPlayer);
}
//"stop_song" detiene totalmente la cancion.
void stop_song(Song* s){
  libvlc_media_player_stop(s->mediaPlayer);
}
//on_playpause sirve para pausar o detener una cancion,
void on_playpause(Song* s) {
    if(libvlc_media_player_is_playing(s->mediaPlayer) == 1) {//Si se esta reproduciendo
        pause_song(s);//Pausa la cancion
        printf("Pause song!\n");
    }
    else {//Si no se esta reproduciendo
      if(libvlc_media_player_get_length(s->mediaPlayer)==-1){//Si el tamaño del player es -1 significa que no hay media agregada
        open_media(s);//Agregamos y reproducimos
      }
      else{
        play_song(s);//Si ya hay media agregada, solo reproducimos
      }
        printf("Play song!\n");
    }
}
