#include "windows/main_window.h"

int main(int argc, char* argv[]) {
  MainWindow* main_window = MainWindow::GetInstance();
  return g_application_run(G_APPLICATION(main_window->app()), argc, argv);
}
