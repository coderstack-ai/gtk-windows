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
  static void OnWindowRealize(GtkWidget* widget, gpointer user_data);
  static void OnButtonClicked(GtkButton* button, gpointer user_data);

  void SetUpMenu();
  static void OnMenuClicked(GSimpleAction* action, GVariant* parameter, gpointer user_data);

  void OnMenuFileOpen();
  static void OnFileOpenFinish(GObject* source, GAsyncResult* result, gpointer user_data);

  void OnMenuFileSave();
  static void OnFileSaveFinish(GObject* source, GAsyncResult* result, gpointer user_data);

  void OnMenuFileExit();
  void CenterWindowOnScreen();
  void LoadAndDisplayImage(GFile* file);  // 加载并显示图片

 private:
  GtkApplication* app_ = nullptr;
  GtkWindow* window_ = nullptr;

  GtkPicture* picture_ = nullptr;  // 用于显示图片
};
