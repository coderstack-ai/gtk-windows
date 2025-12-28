#pragma once
#include <gtk/gtk.h>
#include "gio/gmenu.h"

class MainWindow {
 public:
  static MainWindow* GetInstance();

  GtkApplication* app() const { return app_; }
  void OnActivate(GtkApplication* app);

 private:
  MainWindow();
  ~MainWindow();

  MainWindow(const MainWindow&) = delete;
  MainWindow& operator=(const MainWindow&) = delete;

  static void OnActivateStatic(GtkApplication* app, gpointer user_data);
  static void OnWindowDestroy(GtkWidget* widget, gpointer user_data);
  static void OnButtonClicked(GtkButton* button, gpointer user_data);

  void SetUpMenu();
  static void OnMenuClicked(GSimpleAction* action, GVariant* parameter, gpointer user_data);

 private:
  GtkApplication* app_ = nullptr;
  GtkWindow* window_ = nullptr;
  GtkWidget* box_ = nullptr;
  GtkWidget* header_ = nullptr;
  GtkWidget* menu_bar_ = nullptr;
  GMenu* menubar_ = nullptr;
  GMenu* menubar_file_ = nullptr;

  GtkButton* button_ = nullptr;
};
