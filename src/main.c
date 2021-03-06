#include "distortion.h"
#include "movement.h"
#include "shape.h"
#include "shape_create.h"
#include "sound.h"
#include "sound_len.h"
#include <err.h>
#include <errno.h>
#include <gtk-3.0/gtk/gtk.h>
#include <gtk-3.0/gtk/gtkglarea.h>
#include <pthread.h>
#include <time.h>
#include <wait.h>

struct button_state {
  GtkFileChooser *file_chooser_button;
  GtkComboBox *combo_box;
  GtkComboBox *combo_filter;
  GtkScale *scale_bar;
  GtkButton *play_button;
};

int state;
int filter_n;
gchar *soundfile;
float len;
float al =0;
int be= 1;

unsigned int index_points = 0;
unsigned int index_sphere = 0;
float points_sphere[996];
unsigned int indexes_sphere[966];

float sct[24]=  {0.50+10,1,-1,0.9,0.5,0,
    -0.50+10,-1,-1,0.9,0,0.1,
    0.45+10,1,-1,0.9,0.3,0.1,
    -0.45+10,-1,-1,0.9,0,0.1};

unsigned int scts[6] = {0,3,2,1,2,3};

// One dimensional matrix array[i][j] = array[i * cols + j]
float points[96] = {
    -a,  0.0, b,   0.5,  0.02, 0.48, a,   0.0, b,   0.53, 0.02, 0.27,
    -a,  0.0, -b,  0.23, 0.84, 0.49, a,   0.0, -b,  0.18, 0.41, 0.55,
    0.0, b,   a,   0.26, 0.65, 0.6,  0.0, b,   -a,  0.65, 0.87, 0.55,
    0.0, -b,  a,   0.96, 0.42, 0.37, 0.0, -b,  -a,  0.4,  0.74, 0.42,
    b,   a,   0.0, 0.29, 0.6,  0.65, -b,  a,   0.0, 0.95, 0.25, 0.78,
    b,   -a,  0.0, 0.41, 0.34, 0.6,  -b,  -a,  0.0, 0.44, 0.63, 0.18,
    0.50+10,1,-1,0.9,0.5,0,
    -0.50+10,-1,-1,0.9,0,0.1,
    0.45+10,1,-1,0.9,0.3,0.1,
    -0.45+10,-1,-1,0.9,0,0.1};

// triangle connectivity
unsigned int indexes[66] = {0, 1,  4,  0, 4,  9, 9, 4, 5,  4,  8, 5, 4, 1,  8,
                            8, 1,  10, 8, 10, 3, 5, 8, 3,  5,  3, 2, 2, 3,  7,
                            7, 3,  10, 7, 10, 6, 7, 6, 11, 11, 6, 0, 0, 6,  1,
                            6, 10, 1,  9, 11, 0, 9, 2, 11, 9,  5, 2, 7, 11, 2,
                            12,15,14,13,14,15};

static gboolean render(GtkGLArea *area) {

  if (be == 1)
    al += 0.001;
  else
    al -= 0.001;
  if(al > 0.12 || al < -0.10)
    be = -be;
  // Nothing
  if (state == 0) {
    //slide(sct, 4, 0);
    draw_triangle(sct, scts, 24, 6,al);
  }

  // Icosahedron
  if (state == 1) {
    //slide(points, 4,12);
    on_motion(points, 12);
    draw_triangle(points, indexes, 96, 66,al);
  }

  // Icosphere
  if (state == 2) {
    //slide(points_sphere, 4,162);
    on_motion(points_sphere, 162);
    draw_triangle(points_sphere, indexes_sphere, 996, 966,al);
  }
  gtk_gl_area_queue_render(area);
  return TRUE;
}

void *worker2(void *arg) {
  int ilen = (int)(len * 5);
  float *deformation_factors = arg;
  if (state == 1) {
    distortion_shape(deformation_factors, ilen, points, 4);
  }
  if (state == 2) {
    distortion_shape(deformation_factors, ilen, points_sphere, 43);
  }

  return EXIT_SUCCESS;
}

void deformation_shape(float *deformation_factors) {
  pthread_t thr;
  int e = pthread_create(&thr, NULL, worker2, (void *)deformation_factors);
  if (e != 0) {
    errno = e;
    err(EXIT_FAILURE, "pthread create()");
  }

  pthread_detach(thr);
}

void *worker(void *arg) {
  int ilen = (int)(len * 5);
  float deformation_factors[ilen];
  pulsation_array(soundfile, deformation_factors, ilen,filter_n);
  deformation_shape(deformation_factors);
  struct button_state *menu = arg;
  if (soundfile != NULL) {
    play_sound(soundfile,len,filter_n);
    sleep(2);
  }
  gtk_widget_set_sensitive(GTK_WIDGET(menu->play_button), TRUE);
  gtk_widget_set_sensitive(GTK_WIDGET(menu->file_chooser_button), TRUE);
  return EXIT_SUCCESS;
}

void *worker_s(void *arg) {
  GtkRange *scale = arg;

  for (double i = 0; i <= 100; i += 100.0f / len) {
    gtk_range_set_value(scale, i);
    sleep(1);
  }
  gtk_range_set_value(scale, 0);
  return EXIT_SUCCESS;
}

static gboolean sound_player(GtkFileChooser *file_chooser, void *arg) {
  GtkComboBox *combo_filter = arg;
  soundfile = gtk_file_chooser_get_filename(file_chooser);
  gtk_widget_set_sensitive(GTK_WIDGET(combo_filter), TRUE);
  if (soundfile != NULL)
    get_sound_len(soundfile, &len);

  return TRUE;
}

static gboolean filter_number(GtkComboBox *filter_box, void *arg) {
  GtkComboBox *combo_box = arg;
  switch (gtk_combo_box_get_active(filter_box)) {
  case 1:
    filter_n = 0;
    break;

  case 2:
    filter_n = 1;
    break;

  case 3:
    filter_n = 2;
    break;

  case 4:
    filter_n = 3;
    break;
  }
  gtk_widget_set_sensitive(GTK_WIDGET(combo_box), TRUE);
  return TRUE;
}

static gboolean modele(GtkComboBox *combo_box, void *arg) {
  GtkButton *play_button = arg;
  switch (gtk_combo_box_get_active(combo_box)) {
  case 0:
    state = 0;
    break;

  case 1:
    state = 1;
    break;

  case 2:
    state = 2;
    break;
  }
  gtk_widget_set_sensitive(GTK_WIDGET(play_button), TRUE);
  return TRUE;
}

static gboolean play_function(GtkButton *play_button, gpointer arg) {
  pthread_t thr, thr2;
  struct button_state *menu = arg;

  int e = pthread_create(&thr, NULL, worker, (void *)menu);
  if (e != 0) {
    errno = e;
    err(EXIT_FAILURE, "pthread create()");
  }
  int e2 = pthread_create(&thr2, NULL, worker_s, menu->scale_bar);
  if (e2 != 0) {
    errno = e2;
    err(EXIT_FAILURE, "pthread create()");
  }

  pthread_detach(thr);
  pthread_detach(thr2);
  gtk_widget_set_sensitive(GTK_WIDGET(play_button), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(menu->combo_filter), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(menu->combo_box), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(menu->file_chooser_button), FALSE);
  return TRUE;
}

int main() {
  GtkWidget *main_window;
  GtkWidget *gl_area;
  GtkComboBox *combo_box;
  GtkComboBox *combo_filter;
  GtkButton *play_button;
  GtkFileChooser *file_chooser_button;
  GtkScale *scale_bar;
  GtkBuilder *builder;
  // GError *error;
  gchar *filename;

  gtk_init(NULL, NULL);
  builder = gtk_builder_new();
  filename = g_build_filename("window.glade", NULL);
  gtk_builder_add_from_file(builder, filename, NULL);
  g_free(filename);
  /*if (error) {
    g_printerr("Error loading file: %s\n", error->message);
    g_error_free(error);
    return error->code;
  }*/

  main_window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
  play_button = GTK_BUTTON(gtk_builder_get_object(builder, "play_button"));
  file_chooser_button =
      GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "file_chooser"));
  gl_area = GTK_WIDGET(gtk_builder_get_object(builder, "OGLarea"));
  combo_box = GTK_COMBO_BOX(gtk_builder_get_object(builder, "combo_box"));
  combo_filter = GTK_COMBO_BOX(gtk_builder_get_object(builder, "combo_filter"));
  scale_bar = GTK_SCALE(gtk_builder_get_object(builder, "scale_bar"));
  struct button_state menu = {
      .file_chooser_button = file_chooser_button,
      .combo_box = combo_box,
      .combo_filter = combo_filter,
      .scale_bar = scale_bar,
      .play_button = play_button,
  };

  g_signal_connect(G_OBJECT(main_window), "destroy", (GCallback)gtk_main_quit,
                   NULL);
  g_signal_connect(G_OBJECT(combo_box), "changed", G_CALLBACK(modele),
                   (void *)play_button);
  g_signal_connect(G_OBJECT(combo_filter), "changed", G_CALLBACK(filter_number),
                   (void *)combo_box);
  g_signal_connect(G_OBJECT(play_button), "clicked", G_CALLBACK(play_function),
                   &menu);
  g_signal_connect(G_OBJECT(file_chooser_button), "selection-changed",
                   G_CALLBACK(sound_player), (void *)combo_filter);
  g_signal_connect(G_OBJECT(gl_area), "render", G_CALLBACK(render), NULL);

  gtk_widget_show_all(main_window);

  create_sphere(2, points_sphere, indexes_sphere, points, indexes,
                &index_points, &index_sphere);
  for (size_t i = 972 ; i < 996; i++)
  {
    points_sphere[i] = sct[i-972];
  }
  for (size_t j = 960; j < 966; j++)
  {
    indexes_sphere[j] = scts[j-960]+162;
  }
  scaling(points_sphere, 162, 0.6);
  scaling(points, 12, 0.6);

  gtk_main();

  return 0;
}
