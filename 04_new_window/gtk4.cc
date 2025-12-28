#include "gtk4.h"

Gtk4* Gtk4::GetInstance() {
  static Gtk4 instance;
  return &instance;
}

Gtk4::Gtk4() {
  app_ = gtk_application_new("com.example.gtk4.cpp", G_APPLICATION_DEFAULT_FLAGS);

  g_signal_connect(app_, "activate", G_CALLBACK(Gtk4::OnActivateStatic), this);
}

Gtk4::~Gtk4() {
  if (app_) {
    g_object_unref(app_);
    app_ = nullptr;
  }
}

void Gtk4::OnActivateStatic(GtkApplication* app, gpointer user_data) {
  auto* self = static_cast<Gtk4*>(user_data);
  self->OnActivate(app);
}

void Gtk4::OnActivate(GtkApplication* app) {
  if (!main_window_) {
    main_window_ = GTK_WINDOW(gtk_application_window_new(app));
    gtk_window_set_title(main_window_, "GTK4 C++ Window");
    gtk_window_set_default_size(main_window_, 640, 480);

    /* 主容器 Box */
    main_box_ = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_top(main_box_, 20);
    gtk_widget_set_margin_bottom(main_box_, 20);
    gtk_widget_set_margin_start(main_box_, 20);
    gtk_widget_set_margin_end(main_box_, 20);

    /* 按钮 */
    button_ = GTK_BUTTON(gtk_button_new_with_label("Click me"));
    g_signal_connect(button_, "clicked", G_CALLBACK(Gtk4::OnButtonClicked), this);
    gtk_box_append(GTK_BOX(main_box_), GTK_WIDGET(button_));

    /* 标签 */
    label_ = GTK_LABEL(gtk_label_new("1234567890"));
    gtk_box_append(GTK_BOX(main_box_), GTK_WIDGET(label_));

    /* 设置主窗口的子窗口 */
    gtk_window_set_child(main_window_, GTK_WIDGET(main_box_));
    g_signal_connect(main_window_, "destroy", G_CALLBACK(Gtk4::OnWindowDestroy), this);
  }

  gtk_window_present(main_window_);
}

void Gtk4::OnWindowDestroy(GtkWidget*, gpointer user_data) {
  auto* self = static_cast<Gtk4*>(user_data);
  self->main_window_ = nullptr;
}

void Gtk4::OnButtonClicked(GtkButton*, gpointer user_data) {
  auto* self = static_cast<Gtk4*>(user_data);
  gtk_label_set_text(GTK_LABEL(self->label_), "Button clicked!");

  // 创建新窗口
  if (!self->child_window_) {
    self->child_window_ = GTK_WINDOW(gtk_application_window_new(self->app_));
    gtk_window_set_title(self->child_window_, "New Window");
    gtk_window_set_default_size(self->child_window_, 400, 300);

    /* ⭐ 关键 1：设置父窗口 */
    gtk_window_set_transient_for(self->child_window_, self->main_window_);

    /* ⭐ 关键 2：设置模态 */
    gtk_window_set_modal(self->child_window_, TRUE);

    /* ⭐ 关键 3：设置不可调整大小 */
    gtk_window_set_resizable(self->child_window_, FALSE);

    /* ⭐ 关键 4：设置窗口无装饰 */
    gtk_window_set_decorated(self->child_window_, FALSE);

    // 在新窗口中添加一个标签
    GtkWidget* new_label = gtk_label_new("这是一个新窗口！");
    gtk_widget_set_margin_top(new_label, 20);
    gtk_widget_set_margin_bottom(new_label, 20);
    gtk_widget_set_margin_start(new_label, 20);
    gtk_widget_set_margin_end(new_label, 20);

    gtk_window_set_child(self->child_window_, new_label);
    g_signal_connect(self->child_window_, "destroy", G_CALLBACK(Gtk4::OnChildWindowDestroy), self);
  }

  gtk_window_present(self->child_window_);
}

void Gtk4::OnChildWindowDestroy(GtkWidget*, gpointer user_data) {
  auto* self = static_cast<Gtk4*>(user_data);
  self->child_window_ = nullptr;
}