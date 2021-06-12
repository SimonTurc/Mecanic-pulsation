#include "shape.h"
#include "sound_len.h"
#include "movement.h"
#include "sound.h"
#include <gtk-3.0/gtk/gtk.h>
#include <gtk-3.0/gtk/gtkglarea.h>
#include <pthread.h>
#include <wait.h>
#include <err.h>
#include <errno.h>
#include <math.h>
#include <time.h>

int state;
int filter_n;
gchar *soundfile;
const float a = 0.525731112119133606; // (1 / sqrt(1 +(1 + sqrt(5))/2)²)
const float b = 0.850650808352039932; // ((1 + sqrt(5))/2) / sqrt(1 +(1 + sqrt(5))/2)²)

float float_rand(float mini, float maxi)
{
    float scale = rand() / (float) RAND_MAX;
    return mini + scale * ( maxi - mini );
}

float sst[6] = {0.0, 0.0, 0.0,0.1, 0.1, 0.1};

unsigned ssts[1] = {0};

// One dimensional matrix array[i][j] = array[i * cols + j]
float points[72] = {
    -a, 0.0, b, 0.5,0.02,0.48,
    a, 0.0, b, 0.53,0.02,0.27,
    -a, 0.0, -b, 0.23,0.84,0.49,
    a, 0.0, -b, 0.18,0.41,0.55,
    0.0, b, a, 0.26,0.65,0.6,
    0.0, b, -a, 0.65,0.87,0.55,
    0.0, -b, a, 0.96,0.42,0.37,
    0.0, -b, -a, 0.4,0.74,0.42,
    b, a, 0.0, 0.29,0.6,0.65,
    -b, a, 0.0, 0.95,0.25,0.78,
    b, -a, 0.0, 0.41,0.34,0.6,
    -b , -a, 0.0, 0.44,0.63,0.18
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

unsigned int index_points = 0;
unsigned int index_sphere = 0;
float points_sphere[126 * 2];
unsigned int indexes_sphere[240];

float normalization_value(float v[3]){
    float n = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    return n;
}

/*void add_triangle(float* v1, float* v2, float* v3, float points_array[]){
    for(int i = 0; i < 3; i++){
        points_array[index_points + i] = v1[i];
        points_array[index_points + i + 3] = v2[i];
        points_array[index_points + i + 6] = v3[i];
    }
    index_points += 9;
}*/

void add_vertex(float v[3], float points_array[]){
    for(int i = 0; i < 3; i++){
        points_array[index_points + i] = v[i];
    }
    index_points += 3;
    // Add color
    for(int i = 0; i < 3; i++){
        float x = rand() / (float) RAND_MAX;
        points_array[index_points + i] = x;
    }
    index_points += 3;
}

int vertexAlreadyExist(float v[3]){
    for(unsigned int i = 0; i < index_points / 6; i++){
        if (v[0] == points_sphere[i * 6]){
            if(v[1] == points_sphere[i * 6 + 1]){
                if(v[2] == points_sphere[i * 6 + 2]){
                    return i;
                }
            }
        }
    }
    return -1;
}

void add_to_arrays(float v[3], float points_array[]){
    int current_index = vertexAlreadyExist(v);

    if (current_index == - 1){
        indexes_sphere[index_sphere] = index_points/6;
        add_vertex(v, points_array);
    }
    else
        indexes_sphere[index_sphere] = current_index;
    index_sphere++;
}

void subdivide(float* v1, float* v2, float* v3, unsigned int SubdivisionCoef, float points_array[]) {
    float v12[3], v23[3], v31[3];
    float n12, n23, n31;

    if (SubdivisionCoef == 0){
        add_to_arrays(v1, points_array);
        add_to_arrays(v2, points_array);
        add_to_arrays(v3, points_array);
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
            v1[j] = icosahedron_points[icosahedron_index[i * 3]  * 6 + j];
            v2[j] = icosahedron_points[icosahedron_index[i * 3 + 1] * 6 + j];
            v3[j] = icosahedron_points[icosahedron_index[i * 3 + 2]  * 6 + j];
        }
        subdivide(v1, v2, v3, nb_subdivision, sphere_points_array);
    }
}

static gboolean render(GtkGLArea* area) {

  /* Nothing  */
  if(state == 0)
  {
    draw_triangle(sst, ssts, 6, 1);
  }

  /* Icosahedron  */
  if(state == 1)
  {
    on_motion(points,12);
    draw_triangle(points, indexes, 72, 60);
  }

  /* Icosphere */
  if(state == 2)
  {
    on_motion(points_sphere, 42);
    draw_triangle(points_sphere, indexes_sphere, 252, 240);
  }
  gtk_gl_area_queue_render(area);
  return TRUE;
}


void* worker2(void* arg){
  float* deformation_factors = arg;
  for(int i = 0; i < 600; i++){
    int randomPoint = rand() % 42;
    float x = sqrtf(deformation_factors[i]);
    deformation(points_sphere, randomPoint, x);
    if(randomPoint > 0 && randomPoint < 42){
      deformation(points_sphere, randomPoint + 1, x);
      deformation(points_sphere, randomPoint - 1, x);
    }
    usleep(100000);
    deformation(points_sphere, randomPoint, x);
    usleep(100000);
    deformation(points_sphere, randomPoint, 1/deformation_factors[i]);
    if(randomPoint > 0 && randomPoint < 42){
      deformation(points_sphere, randomPoint + 1, 1/x);
      deformation(points_sphere, randomPoint - 1, 1/x);
    }
  }
  return EXIT_SUCCESS;
}

void deformation_shape(float* deformation_factors){
  pthread_t thr;
  int e = pthread_create(&thr,NULL,worker2,(void*)deformation_factors);
  if(e != 0)
  {
    errno = e;
    err(EXIT_FAILURE,"pthread create()");
  }
}

void * worker(void* arg)
{
  GtkButton * button = arg ;
  gtk_widget_set_sensitive (GTK_WIDGET(button), FALSE);
  if (soundfile !=NULL)
    play_sound(soundfile);
  gtk_widget_set_sensitive (GTK_WIDGET(button), TRUE);
  return EXIT_SUCCESS;
}

void * worker_s(void* arg)
{
  GtkRange*  scale = arg ;
  gtk_range_set_value(scale,1);
  return EXIT_SUCCESS;
}

static gboolean sound_player(GtkFileChooser* file_chooser, void* arg)
{
  GtkComboBox *combo_filter = arg ;
  soundfile = gtk_file_chooser_get_filename(file_chooser);
  gtk_widget_set_sensitive (GTK_WIDGET(combo_filter), TRUE);
  g_print("%s\n",soundfile);

  return TRUE;
}

static gboolean filter_number(GtkComboBox* filter_box, void * arg)
{
  GtkComboBox *combo_box = arg ;
  switch (gtk_combo_box_get_active (filter_box))
  {
    case 0:
    filter_n = 0;
    break;

    case 1:
    filter_n = 1;
    break;

    case 2:
    filter_n = 2;
    break;

    case 3:
    filter_n = 3;
    break;

    case 4:
    filter_n = 4;
    break;

    case 5:
    filter_n = 5;
    break;

  }
  gtk_widget_set_sensitive (GTK_WIDGET(combo_box), TRUE);
  return TRUE;
}

static gboolean modele(GtkComboBox* combo_box,void * arg)
{
  GtkButton* play_button = arg ;
  switch (gtk_combo_box_get_active (combo_box))
  {
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
  gtk_widget_set_sensitive (GTK_WIDGET(play_button), TRUE);
  return TRUE;
}

static gboolean play_function(GtkButton *play_button,void * arg) 
{ 
  pthread_t thr ,thr2;
  int e = pthread_create(&thr,NULL,worker,(void*)play_button);
  if(e != 0)
  {
    errno = e;
    err(EXIT_FAILURE,"pthread create()");
  }
  int e2 = pthread_create(&thr2,NULL,worker_s, arg);
  if(e2 != 0)
  {
    errno = e2;
    err(EXIT_FAILURE,"pthread create()");
  }
  return TRUE;
}

int main() {
  GtkWidget *main_window;
  GtkWidget *gl_area;
  GtkComboBox *combo_box;
  GtkComboBox *combo_filter;
  GtkButton *play_button;
  GtkFileChooser *file_chooser_button;
  GtkScale * scale_bar;
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
  play_button = GTK_BUTTON(gtk_builder_get_object(builder, "play_button"));
  file_chooser_button = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "file_chooser"));
  gl_area = GTK_WIDGET(gtk_builder_get_object(builder, "OGLarea"));
  combo_box = GTK_COMBO_BOX(gtk_builder_get_object(builder, "combo_box"));
  combo_filter = GTK_COMBO_BOX(gtk_builder_get_object(builder, "combo_filter"));
  scale_bar = GTK_SCALE(gtk_builder_get_object(builder, "scale_bar"));

  g_signal_connect(G_OBJECT(main_window), "destroy", (GCallback)gtk_main_quit,
                   NULL);
  g_signal_connect(G_OBJECT(combo_box), "changed",
                   G_CALLBACK(modele), (void *)play_button);
  g_signal_connect(G_OBJECT(combo_filter), "changed",
                   G_CALLBACK(filter_number), (void *)combo_box);
  g_signal_connect(G_OBJECT(play_button), "clicked",
                   G_CALLBACK(play_function), (void *) scale_bar);
  g_signal_connect(G_OBJECT(file_chooser_button), "selection-changed",
                   G_CALLBACK(sound_player), (void *)combo_filter);
  g_signal_connect(G_OBJECT(gl_area), "render", G_CALLBACK(render), NULL);
  
  gtk_widget_show_all(main_window);


  create_sphere(1, points_sphere, points, indexes);
  scaling(points_sphere,42,0.6);
  scaling(points,12,0.6);
  float deformation_factors[2000];
  for(int i = 0; i < 600; i++){
    deformation_factors[i] = float_rand(1.1, 1.55);
  }
  deformation_shape(deformation_factors);
  
  gtk_main();

  return 0;
}
