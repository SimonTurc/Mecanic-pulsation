#include <gtk-3.0/gtk/gtk.h>
#include <gtk-3.0/gtk/gtkglarea.h>
//#include <epoxy/gl.h>
#include <GL/glew.h>
#include <glib.h>
#include <math.h>

static gboolean render (GtkGLArea *area, GdkGLContext *context)
{
    glewExperimental = GL_TRUE;
    glewInit();

    /* OTHER STUFF GOES HERE NEXT */
    float points[] = {
            0.0f,  0.5f,  0.0f,
            0.5f, -0.5f,  0.0f,
            -0.5f, -0.5f,  0.0f
    };

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    const char* vertex_shader =
            "#version 150 core\n"
            "in vec3 vp;"
            "void main() {"
            "  gl_Position = vec4(vp, 1.0);"
            "}";

    const char* fragment_shader =
            "#version 150 core\n"
            "out vec4 frag_colour;"
            "void main() {"
            "  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
            "}";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);
    glUseProgram(shader_programme);

    glClearColor (0.3, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    return TRUE;
}

void function_test(GtkWidget *widget,gpointer data)
{
    g_print("Pass\n");
}

int main(int argc , char *argv[])
{
    GtkWidget *main_window ;
    GtkGLArea *gl_area ;
    GtkButton *button_filter1 ;
    GtkButton *button_filter2 ;
    GtkButton *button_filter3 ;
    GtkBuilder *builder ;
    GError *error ;
    gchar *filename ;

    gtk_init(NULL,NULL);
    builder = gtk_builder_new();
    filename = g_build_filename("window.glade", NULL);
    gtk_builder_add_from_file(builder, filename, &error);
    g_free(filename);
    if (error)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_error_free(error);
        return error->code;
    }

    main_window = GTK_WIDGET(gtk_builder_get_object (builder, "MainWindow"));
    button_filter1 = GTK_BUTTON(gtk_builder_get_object (builder, "filter_1"));
    button_filter2 = GTK_BUTTON(gtk_builder_get_object (builder, "filter_2"));
    button_filter3 = GTK_BUTTON(gtk_builder_get_object (builder, "filter_3"));
    button_filter3 = GTK_BUTTON(gtk_builder_get_object (builder, "filter_3"));
    gl_area = GTK_GL_AREA(gtk_builder_get_object (builder, "OGLarea"));
        
    g_signal_connect (G_OBJECT (main_window), "destroy", (GCallback)gtk_main_quit, NULL);
    g_signal_connect(G_OBJECT (button_filter1), "clicked", G_CALLBACK(function_test), NULL);
    g_signal_connect(G_OBJECT (button_filter2), "clicked", G_CALLBACK(function_test), NULL);
    g_signal_connect(G_OBJECT (button_filter3), "clicked", G_CALLBACK(function_test), NULL);
    g_signal_connect (G_OBJECT(gl_area), "render", G_CALLBACK (render), NULL);

    gtk_widget_show_all (main_window);
    gtk_main();

    return 0;
}