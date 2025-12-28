#include <iostream>
#include <gtk/gtk.h>

/* 保存主窗口指针 */
static GtkWindow* main_window = nullptr;

/* 应用激活时创建窗口 */
static void on_activate(GtkApplication* app, gpointer /* user_data */) {
  if (main_window == nullptr) {
    main_window = GTK_WINDOW(gtk_application_window_new(app));
    gtk_window_set_title(main_window, "GTK4 C++ Window");
    gtk_window_set_default_size(main_window, 640, 480);

    /* 当窗口销毁时，把指针清空 */
    g_signal_connect(main_window,
                     "destroy",
                     G_CALLBACK(+[](GtkWidget*, gpointer) { main_window = nullptr; }),
                     nullptr);
  }
  gtk_window_present(main_window);
}

int main(int argc, char* argv[]) {
  GtkApplication* app = gtk_application_new("com.example.gtk4.cpp", G_APPLICATION_DEFAULT_FLAGS);

  g_signal_connect(app, "activate", G_CALLBACK(on_activate), nullptr);

  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}
