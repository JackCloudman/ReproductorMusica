#include<stdio.h>
#include <gtk/gtk.h>
#include "Song.h"
#include"Play_list.h"
void gtk_play(GtkButton *button, gpointer user_data){
  Play_list** l= user_data;
  play_music_list(l);
}
void gtk_next(GtkButton *button, gpointer user_data){
  Play_list** l= user_data;
  next_song(l);
}
void gtk_previous(GtkButton *button, gpointer user_data){
  Play_list** l= user_data;
  previous_song(l);
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
  add_song(&l1,"file:///home/jack/Desktop/Reproductor/Don't Stop Me Now (Remastered 2011).mp3");
  add_song(&l1,"file:///home/jack/Desktop/Reproductor/Foster The People - Pumped up Kicks   (MUSIC VIDEO).mp3");
  button = gtk_builder_get_object (builder, "BtnPlay");
  printf("Play list directory: %p %p %s\n",l1,&l1,l1->path);
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_play), &l1);
  button = gtk_builder_get_object (builder, "BtnSiguiente");
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_next), &l1);
  button = gtk_builder_get_object (builder, "BtnAnterior");
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_previous), &l1);
  aux = l1;
  do{
    printf("%d- %s\n",n+1,aux->path);
    aux = aux->next;
    n+=1;
  }while(aux!=l1);
  printf("Creado con exito!\n");
  g_object_unref(builder);
  gtk_widget_show(window);
  gtk_main();
  return 0;
}
