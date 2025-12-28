#include <iostream>
#include <gtk/gtk.h>

/* 应用激活时创建窗口 */
static void on_activate(GtkApplication* app, gpointer /* user_data */) {
  std::cout << "on_activate" << std::endl;
  GtkWidget* window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "GTK4 C++ Window");
  gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
  gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char* argv[]) {
  GtkApplication* app = gtk_application_new("com.example.gtk4.cpp", G_APPLICATION_DEFAULT_FLAGS);

  g_signal_connect(app, "activate", G_CALLBACK(on_activate), nullptr);

  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}
