#include <gtk/gtk.h>
#include <gtk-3.0/gtk/gtkglarea.h>
#include <epoxy/gl.h>
#include <glib.h>
#include <math.h>

#define nCoordsComponents 3
#define nColorComponents 3
#define nLines 3
#define nVerticesPerLine 2
#define nFaces 6
#define nVerticesPerFace 3

const char* vertex_shader =
    "attribute vec3 aCoords;"
    "attribute vec3 aColor;"
    "uniform mat4 umvMat;"
    "uniform mat4 upMat;"
    "varying vec3 vColor;"
    "void main () {"
        "gl_Position = upMat * umvMat * vec4(aCoords, 1.0);"
        "vColor = aColor;"
    "}";
 
const char* fragment_shader =
    "varying vec3 vColor;"
    "void main () {"
        "gl_FragColor = vec4 (vColor, 1.0);"
    "}";

GLuint* vboIds = NULL;
GLuint* vaoIds = NULL;
GLuint program = 0;
GLuint VERTEX_ATTR_COORDS = 1;
GLuint VERTEX_ATTR_COLOR = 2;

float av[] = { 0.0, 0.0, 0.0,    // origin
               2.0, 0.0, 0.0,    // x-axis
               0.0, 2.0, 0.0,    // y-axis
               0.0, 0.0, 2.0 };  // z-axis

GLubyte avi[] = { 0, 1,
                  0, 2,
                  0, 3 };

float ac[] = { 1.0, 0.0, 0.0,    // red   x-axis
               0.0, 1.0, 0.0,    // green y-axis
               0.0, 0.0, 1.0 };  // blue  z-axis

GLubyte aci[] = { 0, 0,
                  1, 1,
                  2, 2 };

float ave[nLines*nVerticesPerLine*nCoordsComponents];
void expandAxesVertices()
{
    for (int i=0; i<6; i++)
    {
        ave[i*3+0] = av[avi[i]*3+0];
        ave[i*3+1] = av[avi[i]*3+1];
        ave[i*3+2] = av[avi[i]*3+2];
    }
}

float ace[nLines*nVerticesPerLine*nColorComponents];
void expandAxesColors()
{
    for (int i=0; i<6; i++)
    {
        ace[i*3+0] = ac[aci[i]*3+0];
        ace[i*3+1] = ac[aci[i]*3+1];
        ace[i*3+2] = ac[aci[i]*3+2];
    }
}float pv[] = { 0.5, 0.5, 0.5,    // 0
               0.5, 1.5, 0.5,    // 1
               1.5, 1.5, 0.5,    // 2
               1.5, 0.5, 0.5,    // 3
               1.0, 1.0, 1.5 };  // 4

GLubyte pvi[] = {0, 1, 2,
                 2, 3, 0,
                 0, 3, 4,
                 3, 2, 4,
                 2, 1, 4,
                 1, 0, 4};

float pve[nFaces*nVerticesPerFace*nCoordsComponents];
void expandPyramidVertices()
{
    for (int i=0; i<nFaces; i++)
    {
        for (int j=0; j<nVerticesPerFace; j++)
        {
            for (int k=0; k<nCoordsComponents; k++)
            {
                pve[(i*3+j)*3+k] = pv[pvi[i*3+j]*3+k];
            }
        }
    }
}

float pc[] = { 0.3f, 0.30f, 0.3f,
               1.0f, 0.70f, 0.0f,
               1.0f, 0.62f, 0.0f,
               1.0f, 0.40f, 0.0f,
               1.0f, 0.48f, 0.0f};

GLubyte pci[] = { 0, 0, 0,
                  0, 0, 0,
                  1, 1, 1,
                  2, 2, 2,
                  3, 3, 3,
                  4, 4, 4 };

float pce[nFaces*nVerticesPerFace*nColorComponents];
void expandPyramidColors()
{
    for (int i=0; i<nFaces; i++)
    {
        for (int j=0; j<nVerticesPerFace; j++)
        {
            for (int k=0; k<nColorComponents; k++)
            {
                pce[(i*3+j)*3+k] = pc[pci[i*3+j]*3+k];
            }
        }
    }
}

float n[nFaces*nVerticesPerFace*nCoordsComponents];
 
static void initShaders()
{
    GLuint vs = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vs, 1, &vertex_shader, NULL);
    glCompileShader (vs);
 
    GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fs, 1, &fragment_shader, NULL);
    glCompileShader (fs);
 
    program = glCreateProgram();
    glAttachShader (program, fs);
    glAttachShader (program, vs);
 
    glBindAttribLocation(program, VERTEX_ATTR_COORDS, "aCoords");
    glBindAttribLocation(program, VERTEX_ATTR_COLOR, "aColor");
 
    glLinkProgram (program);
 
    glUseProgram (program);
}
static void defineVAO()
{
    vaoIds = malloc(2* sizeof(GLuint));
    glGenVertexArrays(2, vaoIds);

    vboIds = malloc(4* sizeof(GLuint));
    glGenBuffers(4, vboIds);

    GLint vertexAttribCoords = glGetAttribLocation(program, "aCoords");
    GLint vertexAttribColor = glGetAttribLocation(program, "aColor");

    // set current (bind) VAO to define axes data
    glBindVertexArray(vaoIds[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);  // coordinates
    glBufferData(GL_ARRAY_BUFFER, sizeof(ave), ave, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexAttribCoords, nCoordsComponents, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexAttribCoords);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);  // color
    glBufferData(GL_ARRAY_BUFFER, sizeof(ace), ace, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexAttribColor, nColorComponents, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexAttribColor);

    // Set current (bind) VAO to define pyramid data
    glBindVertexArray(vaoIds[1]);
    
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[2]);  // coordinates
    glBufferData(GL_ARRAY_BUFFER, sizeof(pve), pve, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexAttribCoords, nCoordsComponents, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexAttribCoords);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[3]);  // color
    glBufferData(GL_ARRAY_BUFFER, sizeof(pce), pce, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexAttribColor, nColorComponents, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexAttribColor);

    // Disable VAO
    glBindVertexArray(0);
}

static void drawShaderWithVertexArrayObject()
{
    initShaders();
    defineVAO();

    // Get the variables from the shader to which data will be passed
    GLint mvloc = glGetUniformLocation(program, "umvMat");
    GLint ploc = glGetUniformLocation(program, "upMat");

    // Pass the model-view matrix to the shader
    GLfloat mvMat[16]; 
    glGetFloatv(GL_MODELVIEW_MATRIX, mvMat); 
    glUniformMatrix4fv(mvloc, 1, GL_FALSE, mvMat);

    // Pass the projection matrix to the shader
    GLfloat pMat[16]; 
    glGetFloatv(GL_PROJECTION_MATRIX, pMat); 
    glUniformMatrix4fv(ploc, 1, GL_FALSE, pMat);

    // Enable VAO to set axes data
    glBindVertexArray(vaoIds[0]);
    
    // Draw axes
    glDrawArrays(GL_LINES, 0, nLines*nVerticesPerLine);

    // Enable VAO to set pyramid data
    glBindVertexArray(vaoIds[1]);

    // Draw pyramid
    glDrawArrays(GL_TRIANGLES, 0, nFaces*nVerticesPerFace);

    // Disable VAO
    glBindVertexArray(0);
}
static gboolean render (GtkGLArea *area, GdkGLContext *context)
{
    glClearColor (0.5, 0.5, 0.5, 1.0);
    glClear (GL_COLOR_BUFFER_BIT);
    drawShaderWithVertexArrayObject();
    glFlush();
    return 1;
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