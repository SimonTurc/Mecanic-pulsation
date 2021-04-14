#include "shape.h"
#include "movement.h"
#include "sound.h"
#include <gtk-3.0/gtk/gtk.h>
#include <gtk-3.0/gtk/gtkglarea.h>
#include "pthread.h"
#include <wait.h>
#include <err.h>
#include <errno.h>

float points[] = {
        //     x,     y,    z,    r,    g,    b,
            0.0f,  1.0f, 0.0f, 0.2f, 0.1f, 0.4f,
            0.5f, 0.0f, 0.5f, 0.3f, 0.0f, 0.7f,
            -0.5f, 0.0f, 0.5f, 0.3f, 0.0f, 0.7f,

            0.0f, -1.0f, 0.0f, 0.2f, 0.1f, 0.4f,
            0.5f, 0.0f, 0.5f, 0.3f, 0.0f, 0.7f,
            -0.5f, 0.0f, 0.5f, 0.3f, 0.0f, 0.7f,

            0.0f,  1.0f, 0.0f, 0.2f, 0.1f, 0.4f,
            0.5f, 0.0f, -0.5f, 0.3f, 0.0f, 0.7f,
            -0.5f, 0.0f, -0.5f, 0.3f, 0.0f, 0.7f,
            
            0.0f, -1.0f, 0.0f, 0.2f, 0.1f, 0.4f,
            0.5f, 0.0f, -0.5f, 0.3f, 0.0f, 0.7f,
            -0.5f, 0.0f, -0.5f, 0.3f, 0.0f, 0.7f
  };

gchar *soundfile;
void print_mat(float points[])
{
  for (size_t i = 0; i < 12; i++)
  {
    for (size_t j = 0; j < 12; j++)
    {
      g_print("%f  ",points[i*6+j]);
    }
    g_print("\n");
  }
  g_print("\n");
  g_print(" -------------------------------------\n");
  g_print("\n");
}

static gboolean render(GtkGLArea* area) {
  on_motion(points,12);
  //print_mat(points);
  draw_triangle(points);
  gtk_gl_area_queue_render(area);
  return TRUE;
}

void * worker(void* arg)
{
  GtkFileChooser * file_chooser = arg ;
  if (soundfile !=NULL)
    play_sound(soundfile);
  gtk_widget_set_sensitive (GTK_WIDGET(file_chooser), TRUE);
  return EXIT_SUCCESS;
}

static gboolean sound_player(GtkFileChooser* file_chooser)
{
  pthread_t thr;
  gtk_widget_set_sensitive (GTK_WIDGET(file_chooser), FALSE);
  soundfile = gtk_file_chooser_get_filename(file_chooser);
  g_print("%s\n",soundfile);
  int e = pthread_create(&thr,NULL,worker,(void*)file_chooser);
  if(e != 0)
  {
    errno = e;
    err(EXIT_FAILURE,"pthead create()");
  }
  return TRUE;
}

void function_test() 
{ 
  g_print("Pass\n");
}

int main() {
  GtkWidget *main_window;
  GtkWidget *gl_area;
  GtkButton *button_filter1;
  GtkButton *button_filter2;
  GtkButton *button_filter3;
  GtkFileChooser *file_chooser_button;
  GtkBuilder *builder;
  GError *error;
  gchar *filename;
  
  gtk_init(NULL, NULL);
  builder = gtk_builder_new();
  filename = g_build_filename("window.glade", NULL);
  gtk_builder_add_from_file(builder, filename, &error);
  g_free(filename);
  if (error) {
    g_printerr("Error loading file: %s\n", error->message);
    g_error_free(error);
    return error->code;
  }
  main_window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
  button_filter1 = GTK_BUTTON(gtk_builder_get_object(builder, "filter_1"));
  button_filter2 = GTK_BUTTON(gtk_builder_get_object(builder, "filter_2"));
  button_filter3 = GTK_BUTTON(gtk_builder_get_object(builder, "filter_3"));
  button_filter3 = GTK_BUTTON(gtk_builder_get_object(builder, "filter_3"));
  file_chooser_button = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "file_chooser"));
  gl_area = GTK_WIDGET(gtk_builder_get_object(builder, "OGLarea"));

  g_signal_connect(G_OBJECT(main_window), "destroy", (GCallback)gtk_main_quit,
                   NULL);
  g_signal_connect(G_OBJECT(button_filter1), "clicked",
                   G_CALLBACK(function_test), NULL);
  g_signal_connect(G_OBJECT(button_filter2), "clicked",
                   G_CALLBACK(function_test), NULL);
  g_signal_connect(G_OBJECT(button_filter3), "clicked",
                   G_CALLBACK(function_test), NULL);
  g_signal_connect(G_OBJECT(file_chooser_button), "selection-changed",
                   G_CALLBACK(sound_player), NULL);
  g_signal_connect(G_OBJECT(gl_area), "render", G_CALLBACK(render), NULL);
  
  gtk_widget_show_all(main_window);
  gtk_main();

  return 0;
}