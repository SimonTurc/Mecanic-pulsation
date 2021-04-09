#include "shape.h"
#include "movement.h"
#include <gtk-3.0/gtk/gtk.h>
#include <gtk-3.0/gtk/gtkglarea.h>

float points[] = {
        //     x,     y,    z,    r,    g,    b,
            0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f
  };

void print_mat(float points[])
{
  for (size_t i = 0; i < 6; i++)
  {
    for (size_t j = 0; j < 6; j++)
    {
      g_print("%f  ",points[i*6+j]);
    }
    g_print("\n");
  }
  g_print("\n");
  g_print(" -------------------------------------\n");
  g_print("\n");
}

static gboolean render() {
  on_motion(points,6);
  print_mat(points);
  draw_triangle(points);
  return TRUE;
}

void function_test() { g_print("Pass\n"); }

int main() {
  GtkWidget *main_window;
  GtkGLArea *gl_area;
  GtkButton *button_filter1;
  GtkButton *button_filter2;
  GtkButton *button_filter3;
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
  gl_area = GTK_GL_AREA(gtk_builder_get_object(builder, "OGLarea"));

  g_signal_connect(G_OBJECT(main_window), "destroy", (GCallback)gtk_main_quit,
                   NULL);
  g_signal_connect(G_OBJECT(button_filter1), "clicked",
                   G_CALLBACK(function_test), NULL);
  g_signal_connect(G_OBJECT(button_filter2), "clicked",
                   G_CALLBACK(function_test), NULL);
  g_signal_connect(G_OBJECT(button_filter3), "clicked",
                   G_CALLBACK(function_test), NULL);
  g_signal_connect(G_OBJECT(gl_area), "render", G_CALLBACK(render), NULL);

  gtk_widget_show_all(main_window);
  gtk_main();

  return 0;
}