#include "windows/main_window.h"

int main(int argc, char* argv[]) {
  g_log_set_handler(
      "GLib-GIO",
      (GLogLevelFlags)(G_LOG_LEVEL_CRITICAL),
      [](const gchar*, GLogLevelFlags, const gchar*, gpointer) {},
      nullptr);
  MainWindow* main_window = MainWindow::GetInstance();
  return g_application_run(G_APPLICATION(main_window->app()), argc, argv);
}
