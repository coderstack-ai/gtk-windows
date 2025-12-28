#include "main_window.h"

MainWindow* MainWindow::GetInstance() {
  static MainWindow instance;
  return &instance;
}

MainWindow::MainWindow() {
  app_ = gtk_application_new("com.example.gtk4.cpp", G_APPLICATION_DEFAULT_FLAGS);

  g_signal_connect(app_, "activate", G_CALLBACK(MainWindow::OnActivateStatic), this);
}

MainWindow::~MainWindow() {
  if (app_) {
    g_object_unref(app_);
    app_ = nullptr;
  }
}

void MainWindow::OnActivateStatic(GtkApplication* app, gpointer user_data) {
  auto* self = static_cast<MainWindow*>(user_data);
  self->OnActivate(app);
}

void MainWindow::OnActivate(GtkApplication* app) {
  if (!window_) {
    window_ = GTK_WINDOW(gtk_application_window_new(app));
    gtk_window_set_title(window_, "GTK4 C++ Window");
    gtk_window_set_default_size(window_, 640, 480);
    gtk_window_set_resizable(window_, FALSE); /* 设置不可调整大小 */
    gtk_window_set_decorated(window_, TRUE);  /* 设置窗口无装饰 */

    /* 主容器 Box */
    box_ = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_top(box_, 0);
    gtk_widget_set_margin_bottom(box_, 0);
    gtk_widget_set_margin_start(box_, 0);
    gtk_widget_set_margin_end(box_, 0);

    SetUpMenu();

    /* 设置主窗口的子窗口 */
    gtk_window_set_child(window_, GTK_WIDGET(box_));
    g_signal_connect(window_, "destroy", G_CALLBACK(MainWindow::OnWindowDestroy), this);
  }

  gtk_window_present(window_);
}

void MainWindow::SetUpMenu() {
  /* 1. 创建 HeaderBar */
  header_ = gtk_header_bar_new();
  gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(header_), FALSE);

  /* 2. 创建菜单栏（View） */
  menu_bar_ = gtk_popover_menu_bar_new_from_model(nullptr);

  /* 3. 菜单栏放进 HeaderBar（⚠️ 只放这里） */
  gtk_header_bar_pack_start(GTK_HEADER_BAR(header_), menu_bar_);

  /* 4. 设置为窗口标题栏 */
  gtk_window_set_titlebar(window_, header_);

  /* 5. 创建菜单 Model */
  menubar_ = g_menu_new();
  menubar_file_ = g_menu_new();

  g_menu_append(menubar_file_, "Exit", "app.quit");
  g_menu_append_submenu(menubar_, "File", G_MENU_MODEL(menubar_file_));

  /* 6. 绑定 Model 到菜单栏 */
  gtk_popover_menu_bar_set_menu_model(GTK_POPOVER_MENU_BAR(menu_bar_), G_MENU_MODEL(menubar_));

  /* 7. 注册 action（必须在 app_ 上） */
  static const GActionEntry app_actions[] = {
      {"quit", MainWindow::OnMenuClicked, nullptr, nullptr, nullptr},
  };

  g_action_map_add_action_entries(G_ACTION_MAP(app_), app_actions, G_N_ELEMENTS(app_actions), this);

  /* 8. 释放你自己的引用 */
  g_object_unref(menubar_file_);
  g_object_unref(menubar_);
}

void MainWindow::OnMenuClicked(GSimpleAction* action, GVariant* parameter, gpointer user_data) {
  auto* self = static_cast<MainWindow*>(user_data);
  g_application_quit(G_APPLICATION(self->app_));
}

void MainWindow::OnWindowDestroy(GtkWidget*, gpointer user_data) {
  auto* self = static_cast<MainWindow*>(user_data);
  self->window_ = nullptr;
}

void MainWindow::OnButtonClicked(GtkButton*, gpointer user_data) {
  auto* self = static_cast<MainWindow*>(user_data);

  // 创建新窗口
  // if (!self->child_window_) {
  //   self->child_window_ = GTK_WINDOW(gtk_application_window_new(self->app_));
  //   gtk_window_set_title(self->child_window_, "New Window");
  //   gtk_window_set_default_size(self->child_window_, 400, 300);

  //   /* ⭐ 关键 1：设置父窗口 */
  //   gtk_window_set_transient_for(self->child_window_, self->main_window_);

  //   /* ⭐ 关键 2：设置模态 */
  //   gtk_window_set_modal(self->child_window_, TRUE);

  //   /* ⭐ 关键 3：设置不可调整大小 */
  //   gtk_window_set_resizable(self->child_window_, FALSE);

  //   /* ⭐ 关键 4：设置窗口无装饰 */
  //   gtk_window_set_decorated(self->child_window_, FALSE);

  //   // 在新窗口中添加一个标签
  //   GtkWidget* new_label = gtk_label_new("这是一个新窗口！");
  //   gtk_widget_set_margin_top(new_label, 20);
  //   gtk_widget_set_margin_bottom(new_label, 20);
  //   gtk_widget_set_margin_start(new_label, 20);
  //   gtk_widget_set_margin_end(new_label, 20);

  //   gtk_window_set_child(self->child_window_, new_label);
  //   g_signal_connect(
  //       self->child_window_, "destroy", G_CALLBACK(MainWindow::OnChildWindowDestroy), self);
  // }

  // gtk_window_present(self->child_window_);
}