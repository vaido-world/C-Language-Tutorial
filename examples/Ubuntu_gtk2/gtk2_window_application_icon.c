#if 0
  set -e; 
  tcc -run `pkg-config --libs --cflags gtk+-2.0` "$0"
  exit
#endif

#include <gtk/gtk.h>

//http://zetcode.com/gui/gtk2/firstprograms/
// tcc -run `pkg-config --libs --cflags gtk+-3.0` new.c
// Needs permission: Allow executing file as program
// Needs pkg-config when used as: tcc -run


GdkPixbuf *create_pixbuf(const gchar * filename) {
    
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   
   if (!pixbuf) {
       
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}

int main(int argc, char *argv[]) {
    
  GtkWidget *window;
  GdkPixbuf *icon;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Icon");
  gtk_window_set_default_size(GTK_WINDOW(window), 230, 150);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  
  // If the path is removed, the logo will be searched around source launched file.
  icon = create_pixbuf("/usr/share/icons/hicolor/48x48/status/view-minimize.png");  
  gtk_window_set_icon(GTK_WINDOW(window), icon);
  
  gtk_widget_show(window);

  g_signal_connect(G_OBJECT(window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);
      
  g_object_unref(icon);    

  gtk_main();

  return 0;
}
