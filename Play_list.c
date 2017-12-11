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
  char* name = 0,file[300];//el maximo tamaño para el path es de 300
  struct dirent *de;
  DIR *dr = opendir(path);//Abrimos la ruta
  if (dr == NULL){
    *l = 0;
    return -1;//Si no encontramos ningun elemento devolvemos un error
  }
  while ((de = readdir(dr)) != NULL){//leemos cada uno de los archivos
      name = strrchr(de->d_name, '.');
      if(name!=0){
        if(strcmp(name,".mp3")==0){//si sus extensiones son iguales a .mp3
          strcpy(file,"");//Creamos la ruta
          strcat(file,"file://");
          strcat(file,path);
          strcat(file,"/");
          strcat(file,de->d_name);
          add_song(l,file,de->d_name);//añadimos la nueva cancion
        }
      }
  }
  return 0;
}
int len_play_list(Play_list** l){
  Play_list *aux = 0;
  int i=0;
  if(*l==0){//Si el apuntador a lista es igual a 0 significa que no hay elementos
    return i;
  }
  aux = *l;//Recorremos la lista
  do{
    i++;
    aux = aux->next;
  }while(aux!=*l);//mientras que el auxiliar sea distinto del principio
  return i;//regresamos el valor
}
Play_list* quit_song(Play_list** l,int index){
  int i = 0;
  Play_list *aux = 0;
  if(*l==0){//si no hay una play_list devolvemos NULL
    return NULL;
  }
  if((index>len_play_list(l)-1)||index<0){
    return NULL;//si el indice es mayor que la longitud de la lista o es menor a 0 devolvemos NULL
  }
  aux = *l;
  if(len_play_list(l)==1){//Si la longitud de la lista es 1
    *l = 0; //Modificamos el apuntador y devolvemos el auxiliar
    return aux;
  }//si no recorremos la lista
  while(i!=index){
    i++;
    aux = aux->next;
  }//Encontramos el elemento y correspondemos a cambiar los apuntadores
  aux->next->previous = aux->previous;//Cambiamos apuntadores del elemento siguiente
  aux->previous->next = aux->next;//Cambiamos apuntadores del elemento anterior
  *l = aux->next;
  aux->next = aux;
  aux->previous = aux;
  return aux;//devolvemos el apuntador que encontramos
}
void link_play_list(Play_list **l1,Play_list **l2){
  if(*l1==0||*l2==0){
    return;//si algunas de las dos listas esta vacia no unimos nada
  }
  Play_list *aux = (*l1)->previous;//en caso contrario usaremos dos auxiliares(aunque no son necesarios)
  Play_list *aux2 = *l2;
  aux->next = aux2;//Unimos las 4 conexiones correspondientes
  (*l1)->previous = aux2->previous;
  aux2->previous->next = *l1;
  aux->next->previous = aux;
}
void random_play_list(Play_list** l){
  srand(time(NULL));//stablecemos la nueva semilla de generacion
  Play_list *aux = 0,*new = 0;
  new = (Play_list*)malloc(sizeof(Play_list));//creamos una nueva play_list
  new->title = (*l)->title;//respaldamos la informacion
  new->s = (*l)->s;
  new->next = new;
  new->previous = new;
  aux = quit_song(l,0);
  while(len_play_list(l)!=0){
    aux = quit_song(l,rand()%(len_play_list(l)));//asi como vamos quitando de forma aleatoria
    link_play_list(&new,&aux);//Vamos uniendo
  }
  *l = new;//devolvemos el nuevo apuntador
}
int delete_play_list(Play_list** l){
  Play_list* aux = 0;
  if(*l==0){//si la play_list esta vacia regresamos un -1
    return -1;
  }
  aux = (*l)->next;
  if(len_play_list(l)!=1){//si la longitud del play_list es distinta de 1
    (*l)->previous->next = 0;//al ultimo elemento le establecemos que su siguiente sea 0(para saber cuando detenernos)
    while(aux->next!=0){
      delete_song(&(aux->previous->s));//Vamos recorriendo la play_list e iremos borrando cancion
      free(aux->previous);// y liberando memoria
      aux = aux->next;//recorremos la lista
    }
  }
  delete_song(&(aux->s));//Eliminamos entonces el ultimo elemento
  free(aux);
  *l = 0;//el auntador a PlayList ahora sera 0
  return 0;
}
int show_play_list(Play_list* l){
  int n = 0;
  Play_list *aux = 0;
  if(l==0){//Si la play_list esta vacia entonces devolvemos un -1
    return -1;
  }
  else{//En caso contrario vamos recorriendo la PlayList
    aux = l;
    do{
      printf("%d- %s\n",n+1,aux->title);//Vamos motrando el indice y el nombre de la cancion
      aux = aux->next;
      n+=1;
    }while(aux!=(l));
    printf("====================\n");
  }
  return 0;//Todo salio bien!
}
void play_list_play(Play_list* l){//reproducimos la cancion
  if(l==0){//si no hay canciones regresamos
    return;
  }
  on_playpause(l->s);//Caso contrario reproducimos la cancion
}
