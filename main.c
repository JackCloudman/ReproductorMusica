#include<stdio.h>
#include <gtk/gtk.h>
#include "Song.h"
#include"Play_list.h"
GObject *scale,*adjust,*LabelSong,*buttonplay;
gboolean _update_scale(gpointer data);
GtkWidget *icon_image;
float current_play_time;
void gtk_play(GtkButton *button, gpointer user_data){
  Play_list** l= user_data;
  current_play_time = 0.0f;
  gtk_label_set_text((GtkLabel*)LabelSong,(*l)->title);
  float video_length = libvlc_media_player_get_length((*l)->s->mediaPlayer);
  gtk_scale_set_value_pos(GTK_SCALE(scale), current_play_time/video_length*100);
  on_playpause((*l)->s);
  if(libvlc_media_player_is_playing((*l)->s->mediaPlayer) == 0){
    icon_image = gtk_image_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_BUTTON);
  }
  else{
    icon_image = gtk_image_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_BUTTON);
  }
  gtk_button_set_image(GTK_BUTTON(buttonplay), icon_image);
  g_timeout_add(300,_update_scale,l);
}
void gtk_next(GtkButton *button, gpointer user_data){
  Play_list** l= user_data;
  next_song(l);
  icon_image = gtk_image_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_BUTTON);
  gtk_button_set_image(GTK_BUTTON(buttonplay), icon_image);
  gtk_label_set_text((GtkLabel*)LabelSong,(*l)->title);
}
void gtk_previous(GtkButton *button, gpointer user_data){
  Play_list** l= user_data;
  previous_song(l);
  icon_image = gtk_image_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_BUTTON);
  gtk_button_set_image(GTK_BUTTON(buttonplay), icon_image);
  gtk_label_set_text((GtkLabel*)LabelSong,(*l)->title);
}
void on_value_change(GtkWidget *widget, gpointer user_data){
  float scale_value = gtk_adjustment_get_value((GtkAdjustment*)adjust);
  Play_list** l= user_data;
  libvlc_media_player_set_position((*l)->s->mediaPlayer, scale_value/100);
}
gboolean _update_scale(gpointer data){
	Play_list **l = data;
	float video_length = libvlc_media_player_get_length((*l)->s->mediaPlayer);
	current_play_time = libvlc_media_player_get_time((*l)->s->mediaPlayer);
  //printf("Current: %f,length:%f\n",current_play_time,video_length);
	g_signal_handlers_block_by_func(G_OBJECT(scale), on_value_change, l);
  float value = current_play_time/video_length*100;
  if(libvlc_media_player_is_playing((*l)->s->mediaPlayer) == 0){
    if(current_play_time-video_length>-300){
      gtk_next(NULL,l);
    }
  }
	gtk_adjustment_set_value((GtkAdjustment*)adjust,value);
	g_signal_handlers_unblock_by_func(G_OBJECT(scale), on_value_change, l);
	return G_SOURCE_CONTINUE;
}
int main(int argc,char* argv[]) {
  Play_list * l1 = 0,*aux = 0;
  int n = 0;
  GtkBuilder *builder;
  GtkWidget *window;
  GObject *button;
  gtk_init(&argc,&argv);
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder,"interfaz.glade",NULL);
  window = GTK_WIDGET(gtk_builder_get_object(builder,"ventana"));
  gtk_window_set_default_size (GTK_WINDOW (window), 300, 200);
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  //Usar tus propios path list
  add_multiple_songs(&l1,"/home/jack/Desktop/Reproductor/");
  /*add_song(&l1,"file:///home/jack/Desktop/Reproductor/Don't Stop Me Now (Remastered 2011).mp3");
  add_song(&l1,"file:///home/jack/Desktop/Reproductor/Foster The People - Pumped up Kicks   (MUSIC VIDEO).mp3");
  add_song(&l1,"file:///home/jack/Desktop/Reproductor/Banana Brain.mp3");
  add_song(&l1,"file:///home/jack/Desktop/Reproductor/Ser Humano (letra) - Camilo Séptimo.mp3");*/
  buttonplay = gtk_builder_get_object (builder, "BtnPlay");
  g_signal_connect(G_OBJECT(buttonplay), "clicked", G_CALLBACK(gtk_play), &l1);
  button = gtk_builder_get_object (builder, "BtnSiguiente");
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_next), &l1);
  button = gtk_builder_get_object (builder, "BtnAnterior");
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_previous), &l1);
  aux = l1;
  printf("Current list:\n");
  do{
    printf("%d- %s\n",n+1,aux->title);
    aux = aux->next;
    n+=1;
  }while(aux!=l1);
  printf("Creado con exito!\n");
  scale = gtk_builder_get_object (builder, "BarraTiempo");
  adjust = gtk_builder_get_object (builder, "Tiempo");
  LabelSong = gtk_builder_get_object (builder, "LabelSong");
  gtk_scale_set_draw_value (GTK_SCALE((GtkWidget *)scale), FALSE);
  gtk_scale_set_has_origin (GTK_SCALE((GtkWidget *)scale), TRUE);
  gtk_scale_set_value_pos(GTK_SCALE((GtkWidget *)scale), 5);
  g_signal_connect(G_OBJECT(scale),"value_changed", G_CALLBACK(on_value_change),&l1);
  g_object_unref(builder);
  gtk_widget_show_all(window);
  gtk_main();
  return 0;
}
