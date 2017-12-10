#include "Play_list.h"
#include<stdio.h>
int add_song(Play_list** l,char* uri){
  Play_list* aux=0;
  if(*l==0){
    *l = (Play_list*)malloc(sizeof(Play_list));
    (*l)->next = *l; //El siguiente y el anterior del primer elemento es la direccion 0
    (*l)->previous = *l;
    (*l)->s = create_song(uri);
    (*l)->path = uri;
    return 0;
  }
  else{
    aux = (*l)->previous;
    aux->next = (Play_list*)malloc(sizeof(Play_list));//Se crea el siguiente dato
    aux->next->previous = aux;//El anterior del siguiente dato es el auxiliar
    aux = aux->next;//Ahora auxiliar es el ultimo dato
    aux->next = *l;//Su siguiente es 0
    aux->s = create_song(uri);//Asignamos el dato
    aux->path = uri;
    (*l)->previous = aux;
    return 0;
  }
}
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
int next_song(Play_list** l){
  if(*l==0){
    return -1;
  }
  if(((*l))==((*l)->next)){
    return 1;
  }
  stop_song((*l)->s);
  *l = (*l)->next;
  open_media((*l)->s);
  return 0;
}
int previous_song(Play_list** l){
  if(*l==0){
    return -1;
  }
  if(((*l))==((*l)->previous)){
      return 1;
    };
  stop_song((*l)->s);
  *l = (*l)->previous;
  open_media((*l)->s);
  return 0;
}
