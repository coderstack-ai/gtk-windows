#pragma once
#include <gtk/gtk.h>

class Gtk4 {
 public:
  static Gtk4* GetInstance();

  GtkApplication* app() const { return app_; }
  void OnActivate(GtkApplication* app);

 private:
  Gtk4();
  ~Gtk4();

  Gtk4(const Gtk4&) = delete;
  Gtk4& operator=(const Gtk4&) = delete;

  static void OnActivateStatic(GtkApplication* app, gpointer user_data);
  static void OnWindowDestroy(GtkWidget* widget, gpointer user_data);
  static void OnButtonClicked(GtkButton* button, gpointer user_data);

 private:
  GtkApplication* app_ = nullptr;
  GtkWindow* main_window_ = nullptr;
  GtkWidget* main_box_ = nullptr;

  GtkButton* button_ = nullptr;
  GtkLabel* label_ = nullptr;
};
