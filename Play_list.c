#include "Play_list.h"
#include<stdio.h>
//Agregamos una cancion
int add_song(Play_list** l,char* uri,char* title){
  Play_list* aux=0;
  if(*l==0){//Si el apuntador a Play_list es 0, agregamos el primer nodo
    *l = (Play_list*)malloc(sizeof(Play_list));//Creamos el primer nodo
    (*l)->next = *l; //El siguiente y el anterior del primer elemento es la misma direccion creada
    (*l)->previous = *l;
    (*l)->s = create_song(uri); //usamos la funcion create_song para crear la cancion
    (*l)->title = title; //El path es solo para saber a que direccion apunta(No es necesaria)
    return 0;
  }
  else{//Si no es el primer nodo
    aux = (*l)->previous;//Iremos al ultimo elemento
    aux->next = (Play_list*)malloc(sizeof(Play_list));//Se crea el siguiente dato
    aux->next->previous = aux;//El anterior del siguiente dato es el auxiliar
    aux = aux->next;//Ahora auxiliar es el ultimo dato
    aux->next = *l;//Su siguiente es el nodo del principio
    aux->s = create_song(uri);//Asignamos el dato
    aux->title = title; //Agregamos el path
    (*l)->previous = aux; //El anterior del primero ahora será el ultimo (El nuevo creado)
    return 0;
  }
}
/*Funcion no recomendada, ya que fue la primera que usaba de prueba, sirve para
agregar una cancion y reproducirla (No para la musica actual)*/
int play_music_list(Play_list** l){
  if((*l)==0){
    return -1;
  }
  else{
    Song* s = (*l)->s;
    open_media(s);
  }
  return 0;
}
/*Esta funcion cambia la cancion*/
int next_song(Play_list** l){
  if(*l==0){//Si el apuntador a lista es 0, regresa un error(No se puede cambiar la cancion)
    return -1;
  }
  //si solo hay una cancion, no tiene sentido cambiar a la siguiente(En teoria)
  if(((*l))==((*l)->next)){
    return 1;
  }
  /*Si hay mas de una cancion ahora modificamos el apuntador
  Play_list para poder que la cancion actual cambie a la seguna, esta funcion no borra
  el playlist, solo modifica el apuntador para poder reproducir la siguiente cancion*/
  stop_song((*l)->s);//Paramos la cancion actual
  *l = (*l)->next;//Cambiamos el apuntador
  open_media((*l)->s);//reproducimos la cancion nueva
  return 0;
}
/*Igual que la funcion next_song, pero aqui el apuntador
cambia al anterior, es decir, apuntador a Play_list ahora va a ser
su anterior(previous)*/
int previous_song(Play_list** l){
  if(*l==0){//Si el apuntador es 0, no se regresa la cancion y retorna -1(Error)
    return -1;
  }
  if(libvlc_media_player_get_time((*l)->s->mediaPlayer)<3500){
    stop_song((*l)->s);//Detenemos la musica
    *l = (*l)->previous;
  }
  else{
    stop_song((*l)->s);//Detenemos la musica
  }
  //Aqui si tiene sentido regresar al principio de la cancion XD
  /*if(((*l))==((*l)->previous)){
      return 1;
    };*/
  //Volvemos al principio(Que es el inicio)
  open_media((*l)->s);//Agregamos la media y volvemos a reproducir
  return 0;
}
int add_multiple_songs(Play_list** l,char* path){
  char* name = 0,file[300];
  struct dirent *de;
  DIR *dr = opendir(path);
  if (dr == NULL){
    return -1;
  }
  while ((de = readdir(dr)) != NULL){
      name = strrchr(de->d_name, '.');
      if(name!=0){
        if(strcmp(name,".mp3")==0){
          strcpy(file,"");
          strcat(file,"file://");
          strcat(file,path);
          strcat(file,de->d_name);
          add_song(l,file,de->d_name);
        }
      }
  }
  return 0;
}
