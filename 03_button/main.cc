#include "gtk4.h"

int main(int argc, char* argv[]) {
  Gtk4* gtk4 = Gtk4::GetInstance();
  return g_application_run(G_APPLICATION(gtk4->app()), argc, argv);
}
