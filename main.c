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
#include<stdio.h>
#include <gtk/gtk.h>
#include "Song.h"
#include"Play_list.h"
//Objetos que necesitamos que sean globales para poder modificarlos siempre que queramos.
GObject *scale,*adjust,*LabelSong,*buttonplay,*path;
GtkWidget *icon_image,*window;
float current_play_time;
//Encabezados
void gtk_play(GtkButton *button, gpointer user_data); //Reproduce una cancion
void gtk_next(GtkButton *button, gpointer user_data);//Cambia la cancion
void gtk_previous(GtkButton *button, gpointer user_data);//Regresa la cancion
void on_value_change(GtkWidget *widget, gpointer user_data);//Funcion que cambia la musica/barra de tiempo
gboolean update_scale(gpointer data); //Actualiza el valor de la escala(cuando se cambia de cancion es necesario actualizar la escala)
void gtk_random(GtkButton *button, gpointer user_data);//Funcion que revuelve las canciones que esten en la actual playlist(La cancion actual no cambia)
void gtk_setPath(GtkButton *button, gpointer user_data);//Funcion que ayuda a elegir el Path para poder seleccionar musica
/* main */
int main(int argc,char* argv[]) {
  Play_list * l1 = 0;  //l1 es nuestra Play List (debe ser tipo apuntador a PlayList)
  GtkBuilder *builder; //Builder para crear la interfaz con una plantilla
  GObject *button; //Botton auxiliar para conectar "clicks" con acciones
  /*Inicializar GTK*/
  gtk_init(&argc,&argv);
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder,"interfaz.glade",NULL);//Cargamos la interfaz
  window = GTK_WIDGET(gtk_builder_get_object(builder,"ventana"));//Obtener la ventana
  gtk_window_set_default_size (GTK_WINDOW (window), 300, 200);//Tamaño por defecto
  gtk_window_set_title (GTK_WINDOW(window),"Reproductor de Musica");//Titulo
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);//Conectamos el boton "x"(salir) con la salida del programa
  buttonplay = gtk_builder_get_object (builder, "BtnPlay");//El boton de play es global y lo conectamos con la funcion gtk_play
  g_signal_connect(G_OBJECT(buttonplay), "clicked", G_CALLBACK(gtk_play), &l1);
  button = gtk_builder_get_object (builder, "BtnSiguiente");//Boton siguiente conectado con la funcion gtk_next
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_next), &l1);
  //Boton anterior conectado con la funcion gtk_next (Note que podemos reutilizar la variable)
  button = gtk_builder_get_object (builder, "BtnAnterior");
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_previous), &l1);
  button = gtk_builder_get_object (builder, "random");//Boton random conectado con la funcion gtk_random
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_random), &l1);
  button = gtk_builder_get_object (builder, "setPath");//Boton sethPath conectado con la funcion gtk_setPath
  path = gtk_builder_get_object (builder, "Path");
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_setPath), &l1);
  scale = gtk_builder_get_object (builder, "BarraTiempo");//Cargamos la barra de tiempo y se lo asignamos "scale"
  adjust = gtk_builder_get_object (builder, "Tiempo");//Cargamos los ajustes de Tiempo y lo asignamos a adjust
  LabelSong = gtk_builder_get_object (builder, "LabelSong");//Cargamos el label de "current song" que se llama "LabelSong"
  gtk_scale_set_draw_value (GTK_SCALE((GtkWidget *)scale), FALSE);
  gtk_scale_set_has_origin (GTK_SCALE((GtkWidget *)scale), TRUE);
  gtk_scale_set_value_pos(GTK_SCALE((GtkWidget *)scale), 5);
  g_signal_connect(G_OBJECT(scale),"value_changed", G_CALLBACK(on_value_change),&l1);//Creamos accion cuando cambia el valor de la barra de tiempo
  /*Mostramos el menu*/
  g_object_unref(builder);
  gtk_widget_show_all(window);
  gtk_main();
  if(l1!=0){//Si se quedo cargada la playlist y salimos del programa borramos los datos.
    delete_play_list(&l1);
  }
  return 0;
}

/* Funciones */
/*gtk_play es una funcion que sirve para reproducir una nueva cancion y actualiza los iconos*/
void gtk_play(GtkButton *button, gpointer user_data){//Recibimos de parametro la playlist
  Play_list** l= user_data;
  if(*l==0){//Si no esta vacia
    printf("No music in current list!\n");
    return;
  }
  current_play_time = 0.0f;  //Establecemos la barra en 0's
  float video_length = libvlc_media_player_get_length((*l)->s->mediaPlayer);//Obtenemo la longitud de la cancion que se esta reproduciendo
  gtk_label_set_text((GtkLabel*)LabelSong,(*l)->title);  //Actualizamos el label con el titulo de la cancion
  gtk_scale_set_value_pos(GTK_SCALE(scale), current_play_time/video_length*100);//Obtenemos la nueva escala, corresponde a una regla de 3
  play_list_play(*l);//Reproducimos cancion
  if(libvlc_media_player_is_playing((*l)->s->mediaPlayer) == 0){//Cambiamos los iconos
    icon_image = gtk_image_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_BUTTON);
  }
  else{
    icon_image = gtk_image_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_BUTTON);
  }
  gtk_button_set_image(GTK_BUTTON(buttonplay), icon_image);//Establecemos los iconos
  g_timeout_add(300,update_scale,l);//Se establece una funcion para llamarla por intervalos(Corresponde a cambiar la barra de tiempo)
}
/*gtk_next recibe de parametros un apuntador doble a lista y cambia la cancion actual*/
void gtk_next(GtkButton *button, gpointer user_data){
  Play_list** l= user_data;
  if(*l==0){//si no hay canciones en la lista regresa un mensaje
    printf("No music in current list!\n");
    return;
  }
  next_song(l);//Caso contrario llamamos a la funcio next_song (Revisar Play_list.c)
  icon_image = gtk_image_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_BUTTON);
  gtk_button_set_image(GTK_BUTTON(buttonplay), icon_image);//Siempre que cambiamos de cancion se reproduce, por lo tanto cambiamos los iconos
  gtk_label_set_text((GtkLabel*)LabelSong,(*l)->title);
}
/*gtk_previous, recibe apuntador doble a lista y cambia a la cancion anterior(o al principio segun corresponda)*/
void gtk_previous(GtkButton *button, gpointer user_data){
  Play_list** l= user_data;
  if(*l==0){//si no hay cancion regresa un mensaje
    printf("No music in current list!\n");
    return;
  }
  previous_song(l);//Caso contrario regresa la cancion (Revisar Play_list.c)
  icon_image = gtk_image_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_BUTTON);//Cambia los iconos
  gtk_button_set_image(GTK_BUTTON(buttonplay), icon_image);
  gtk_label_set_text((GtkLabel*)LabelSong,(*l)->title);//Cambia el label de la cancion
}
/*on_value_change sirve para cambiar el frame de la cancion en curso */
void on_value_change(GtkWidget *widget, gpointer user_data){
  float scale_value = gtk_adjustment_get_value((GtkAdjustment*)adjust);
  Play_list** l= user_data;
  if(*l==0){
    return;
  }
  libvlc_media_player_set_position((*l)->s->mediaPlayer, scale_value/100);//Cambiamos el fram de la cancion
}
/*Funcion que revisa y actualiza la barra de tiempo (Para que se mueva cuando se reproduce una cancion)*/
gboolean update_scale(gpointer data){
	Play_list **l = data;
  if(*l==0){//si no se esta reproduciendo nada regresamos un 0
    icon_image = gtk_image_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(buttonplay), icon_image);//Cambiamos los iconos
    gtk_label_set_text((GtkLabel*)LabelSong,"No hay musica");
    printf("No music in current list!\n");
    return 0;
  }
	float video_length = libvlc_media_player_get_length((*l)->s->mediaPlayer);//Obtenemos la duracion de la cancion
	current_play_time = libvlc_media_player_get_time((*l)->s->mediaPlayer);//Obtenemos donde se encuentra en este momento
	g_signal_handlers_block_by_func(G_OBJECT(scale), on_value_change, l);//Cambiamos la barra de tiempo
  /*verificamos que no haya terminado la cancion*/
  float value = current_play_time/video_length*100;
  if(libvlc_media_player_is_playing((*l)->s->mediaPlayer) == 0){//si ya no se esta reproduciendo nada
    if(current_play_time-video_length>-300){//y la diferencia entre la posicion actual de la barra y el tamaño de la cancion es mayor a -300
      gtk_next(NULL,l);//Cambiamos de cancion(Esto se hace para poder cambiar a la siguiente cancion)
    }
  }
  //Si aun no acaba la cancion
	gtk_adjustment_set_value((GtkAdjustment*)adjust,value);//Cambiamos el valor de la escala
	g_signal_handlers_unblock_by_func(G_OBJECT(scale), on_value_change, l);//Actualizamos la barra
	return G_SOURCE_CONTINUE;//G_SOURCE_CONTINUE sirve para que se siga ejecutando la funcion
}
/*gtk_random es una funcion que revuelve una play list */
void gtk_random(GtkButton *button, gpointer user_data){
  Play_list** l = user_data;
  if(*l==0){//si la play list actual esta vacia regresamos un mensaje de error
    printf("No music in current list!\n");
    return;
  }
  random_play_list(l);//llamamos a la funcion random_play_list (Ver Play_list.c)
  printf("New Random list:\n");//Mostramos la lista nueva
  show_play_list(*l);
}
/*gtk_setPath Sirve para establecer una nueva carpeta de donde buscara las canciones*/
void gtk_setPath(GtkButton *button, gpointer user_data){
  Play_list** l = user_data;
  Play_list*aux = 0;
  GtkWidget *dialog; //Widget de ayuda para abrir una ventana donde se elegira la carpeta
  GtkFileChooserAction action =  GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
  dialog = gtk_file_chooser_dialog_new("open file", GTK_WINDOW(window), action, ("Cancel"), GTK_RESPONSE_CANCEL, ("Open"), GTK_RESPONSE_ACCEPT, NULL);
  gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog),1);
  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {//Si se eligio una carpeta
      char *uri;
      uri = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(dialog))+7;//Obtenemos la direccion es +7 por que "file://" tiene 7 caracteres
      if(*l!=0){//Si la lista anterior no estaba vacia
        delete_play_list(l);//La borramos
      }
      add_multiple_songs(l,uri);//Agregamos la nueva lista (Checar add_multiple_songs en Play_list.c)
      g_free(uri-7);//Liberamos la memoria
      aux = *l;
      if(aux==0){
        printf("There aren't songs in this folder!\n");
        gtk_label_set_text((GtkLabel*)LabelSong,"No hay canciones en este folder!");
        gtk_widget_destroy(dialog);
        return;
      }
      gtk_label_set_text((GtkLabel*)LabelSong,"Musica cargada!");
      printf("New Song list:\n");
      show_play_list(*l);//Mostramos la nueva play list
  }
  gtk_widget_destroy(dialog);
}
