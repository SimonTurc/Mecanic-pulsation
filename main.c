#include "shape.h"
#include "movement.h"
#include "sound.h"
#include <gtk-3.0/gtk/gtk.h>
#include <gtk-3.0/gtk/gtkglarea.h>
#include <pthread.h>
#include <wait.h>
#include <err.h>
#include <errno.h>
#include <math.h>

int state;

void print_mat(float points[])
{
  for (size_t i = 0; i < 3; i++)
  {
    for (size_t j = 0; j < 12; j++)
    {
      g_print("%f  ",points[i*3+j]);
    }
    g_print("\n");
  }
  g_print("\n");
  g_print(" -------------------------------------\n");
  g_print("\n");
}

gchar *soundfile;
const float a = 0.525731112119133606; // (1 / sqrt(1 +(1 + sqrt(5))/2)²)
const float b = 0.850650808352039932; // ((1 + sqrt(5))/2) / sqrt(1 +(1 + sqrt(5))/2)²)

// One dimensional matrix array[i][j] = array[i * cols + j]
float points[36] = {
    -a, 0.0, b,
    a, 0.0, b,
    -a, 0.0, -b,
    a, 0.0, -b,
    0.0, b, a,
    0.0, b, -a,
    0.0, -b, a,
    0.0, -b, -a,
    b, a, 0.0,
    -b, a, 0.0,
    b, -a, 0.0,
    -b , -a, 0.0
};

// triangle connectivity
unsigned int indexes[60] = {
        0,1,4,
        0,4,9,
        9,4,5,
        4,8,5,
        4,1,8,
        8,1,10,
        8,10,3,
        5,8,3,
        5,3,2,
        2,3,7,
        7,3,10,
        7,10,6,
        7,6,11,
        11,6,0,
        0,6,1,
        6,10,1,
        9,11,0,
        9,2,11,
        9,5,2,
        7,11,2
};

unsigned int index_points;
float points_sphere[2880];
unsigned int indexes_sphere[2880];

float normalization_value(float v[3]){
    float n = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    return n;
}

void add_triangle(float* v1, float* v2, float* v3, float points_array[]){
    for(int i = 0; i < 3; i++){
        points_array[index_points + i] = v1[i];
        points_array[index_points + i + 3] = v2[i];
        points_array[index_points + i + 6] = v3[i];
    }
    index_points += 9;
}

void subdivide(float* v1, float* v2, float* v3, unsigned int SubdivisionCoef, float points_array[]) {
    float v12[3], v23[3], v31[3];
    float n12, n23, n31;

    if (SubdivisionCoef == 0){
        add_triangle(v1, v2, v3, points_array);
        return;
    }

    for(int i = 0; i < 3; i++){
        v12[i] = (v1[i] + v2[i]) / 2.0;
        v23[i] = (v2[i] + v3[i]) / 2.0;
        v31[i] = (v3[i] + v1[i]) / 2.0;
    }

    n12 = normalization_value(v12);
    n23 = normalization_value(v23);
    n31 = normalization_value(v31);
    
    for(int j = 0; j < 3; j++){
        v12[j] /= n12;
        v23[j] /= n23;
        v31[j] /= n31;
    }
    subdivide(v1, v12, v31, SubdivisionCoef - 1, points_array);
    subdivide(v2, v23, v12, SubdivisionCoef - 1, points_array);
    subdivide(v3, v31, v23, SubdivisionCoef - 1, points_array);
    subdivide(v12, v23, v31, SubdivisionCoef - 1, points_array);
}

void create_sphere(unsigned int nb_subdivision, float sphere_points_array[], float icosahedron_points[], unsigned int icosahedron_index[]){
    float v1[3];
    float v2[3];
    float v3[3];
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 3; j++){
            v1[j] = icosahedron_points[icosahedron_index[i * 3]  * 3 + j];
            v2[j] = icosahedron_points[icosahedron_index[i * 3 + 1] * 3 + j];
            v3[j] = icosahedron_points[icosahedron_index[i * 3 + 2]  * 3 + j];
        }
        subdivide(v1, v2, v3, nb_subdivision, sphere_points_array);
    }
}

static gboolean render(GtkGLArea* area) {

  /* Icosahedron  */
  if(state == 1)
  {
    on_motion(points,12);
    draw_triangle(points, indexes, 36, 60);
  }

  /* Icosphere */
  if(state == 2)
  {
    on_motion(points_sphere,720 / 3);
    draw_triangle(points_sphere, indexes_sphere, 720, 720);
  }
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

static gboolean fucker(GtkComboBox* combo_box)
{
  if (gtk_combo_box_get_active (combo_box)== 0)
  {
    state =1;
  }
  else
  {
    state =2;
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
  GtkComboBox *combo_box;
  GtkButton *button_filter1;
  GtkButton *button_filter2;
  GtkButton *button_filter3;
  GtkFileChooser *file_chooser_button;
  GtkBuilder *builder;
  GError *error;
  gchar *filename;

  create_sphere(1, points_sphere, points, indexes);
  for(int i = 0; i < 720; i++){
      indexes_sphere[i] = i;
  }

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
  file_chooser_button = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "file_chooser"));
  gl_area = GTK_WIDGET(gtk_builder_get_object(builder, "OGLarea"));
  combo_box = GTK_COMBO_BOX(gtk_builder_get_object(builder, "combo_box"));

  g_signal_connect(G_OBJECT(main_window), "destroy", (GCallback)gtk_main_quit,
                   NULL);
  g_signal_connect(G_OBJECT(combo_box), "changed",
                   G_CALLBACK(fucker), &state);
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
