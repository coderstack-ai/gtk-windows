#include <windows.h>
#include "main_window.h"
#include <gdk/win32/gdkwin32.h>
#include <gio/gfile.h>
#include <iostream>

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
    gtk_window_set_title(window_, "GTK4 图片查看器");
    gtk_window_set_default_size(window_, 800, 600);
    gtk_window_set_resizable(window_, TRUE); /* 允许调整大小以便查看图片 */
    gtk_window_set_decorated(window_, TRUE); /* 设置窗口有装饰 */

    /* 主容器 Box */
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_margin_top(box, 0);
    gtk_widget_set_margin_bottom(box, 0);
    gtk_widget_set_margin_start(box, 0);
    gtk_widget_set_margin_end(box, 0);

    /* 创建图片显示组件 */
    picture_ = GTK_PICTURE(gtk_picture_new());
    gtk_picture_set_content_fit(picture_, GTK_CONTENT_FIT_CONTAIN); /* 保持宽高比并适应容器 */
    gtk_picture_set_can_shrink(picture_, TRUE);
    gtk_widget_set_hexpand(GTK_WIDGET(picture_), TRUE);
    gtk_widget_set_vexpand(GTK_WIDGET(picture_), TRUE);
    gtk_box_append(GTK_BOX(box), GTK_WIDGET(picture_));

    SetUpMenu();

    /* 设置主窗口的子窗口 */
    gtk_window_set_child(window_, GTK_WIDGET(box));
    g_signal_connect(window_, "destroy", G_CALLBACK(MainWindow::OnWindowDestroy), this);
  }

  gtk_window_present(window_);

  CenterWindowOnScreen();
}

void MainWindow::SetUpMenu() {
  /* 1. 创建 HeaderBar */
  GtkWidget* header = gtk_header_bar_new();
  gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(header), FALSE);

  /* 2. 创建菜单栏（View） */
  GtkWidget* menu_bar = gtk_popover_menu_bar_new_from_model(nullptr);

  /* 3. 菜单栏放进 HeaderBar（⚠️ 只放这里） */
  gtk_header_bar_pack_start(GTK_HEADER_BAR(header), menu_bar);

  /* 4. 设置为窗口标题栏 */
  gtk_window_set_titlebar(window_, header);

  /* 5. 创建菜单 Model */
  GMenu* menubar = g_menu_new();
  GMenu* menubar_file = g_menu_new();
  GMenu* menubar_settings = g_menu_new();

  // action名称一般用"app."作为前缀（表示应用级 action，通常注册在 GtkApplication
  // 上），之后可以用"."分隔以表达层级关系 例如 "app.file.open"
  // 表示应用的“文件”菜单下的“打开”动作，"app.open" 只表示顶级“打开”动作，"open"
  // 是最简单的名称常用于 widget 级别 推荐用 "app.file.open" 这种有层级意义的
  g_menu_append(menubar_file, "打开荧光分析数据", "app.file.open");
  g_menu_append(menubar_file, "保存荧光分析数据", "app.file.save");
  g_menu_append(menubar_file, "保存当前灰度照片", "app.file.save_now_gray");
  g_menu_append(menubar_file, "保存当前彩色照片", "app.file.save_now_color");
  g_menu_append(menubar_file, "保存所有灰度照片", "app.file.save_all_gray");
  g_menu_append(menubar_file, "保存所有彩色照片", "app.file.save_all_color");
  g_menu_append(menubar_file, "退出", "app.file.quit");
  g_menu_append_submenu(menubar, "文件", G_MENU_MODEL(menubar_file));

  g_menu_append(menubar_settings, "灯光设置", "app.settings.light");
  g_menu_append(menubar_settings, "相机设置", "app.settings.camera");
  g_menu_append_submenu(menubar, "设置", G_MENU_MODEL(menubar_settings));

  /* 6. 绑定 Model 到菜单栏 */
  gtk_popover_menu_bar_set_menu_model(GTK_POPOVER_MENU_BAR(menu_bar), G_MENU_MODEL(menubar));

  /* 7. 注册 action（必须在 app_ 上） */
  static const GActionEntry app_actions[] = {
      {"file.open", MainWindow::OnMenuClicked, nullptr, nullptr, nullptr},
      {"file.save", MainWindow::OnMenuClicked, nullptr, nullptr, nullptr},
      {"file.save_new_gray_image", MainWindow::OnMenuClicked, nullptr, nullptr, nullptr},
      {"file.save_new_color_image", MainWindow::OnMenuClicked, nullptr, nullptr, nullptr},
      {"file.save_all_gray_images", MainWindow::OnMenuClicked, nullptr, nullptr, nullptr},
      {"file.save_all_color_images", MainWindow::OnMenuClicked, nullptr, nullptr, nullptr},
      {"file.quit", MainWindow::OnMenuClicked, nullptr, nullptr, nullptr},
  };

  g_action_map_add_action_entries(G_ACTION_MAP(app_), app_actions, G_N_ELEMENTS(app_actions), this);

  /* 8. 释放你自己的引用 */
  /* 注意：只释放 GMenu 对象，不要释放 GTK widget！
   * menu_bar 和 header 是 widget，它们已经被添加到父容器中，
   * 父容器会管理它们的生命周期，不需要手动释放 */
  g_object_unref(menubar_file);
  g_object_unref(menubar);
  g_object_unref(menubar_settings);
  /* menu_bar 和 header 是 widget，由 GTK 自动管理，不要调用 g_object_unref() */
}

void MainWindow::OnMenuClicked(GSimpleAction* action, GVariant* parameter, gpointer user_data) {
  auto* self = static_cast<MainWindow*>(user_data);
  if (strcmp(g_action_get_name(G_ACTION(action)), "file.open") == 0) {
    self->OnMenuFileOpen();
  } else if (strcmp(g_action_get_name(G_ACTION(action)), "file.save") == 0) {
    self->OnMenuFileSave();
  } else if (strcmp(g_action_get_name(G_ACTION(action)), "file.quit") == 0) {
    self->OnMenuFileExit();
  } else {
    std::cout << "OnMenuClicked: " << g_action_get_name(G_ACTION(action)) << std::endl;
  }
}

void MainWindow::OnMenuFileOpen() {
  std::cout << "OnMenuFileOpen" << std::endl;
  GtkFileDialog* dialog = gtk_file_dialog_new();
  gtk_file_dialog_set_title(dialog, "打开图片文件");

  /* 设置初始文件夹 */
  GFile* folder = g_file_new_for_path("C:/");
  gtk_file_dialog_set_initial_folder(dialog, folder);
  g_object_unref(folder);

  /* === JPG/JPEG 图片文件过滤器 === */
  GtkFileFilter* filter = gtk_file_filter_new();
  gtk_file_filter_set_name(filter, "JPEG 图片 (*.jpg, *.jpeg)");

  /* 推荐：MIME + 后缀 双保险 */
  gtk_file_filter_add_mime_type(filter, "image/jpeg");
  gtk_file_filter_add_suffix(filter, "jpg");
  gtk_file_filter_add_suffix(filter, "jpeg");

  GListStore* filters = g_list_store_new(GTK_TYPE_FILE_FILTER);
  g_list_store_append(filters, filter);

  gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(filters));
  gtk_file_dialog_set_default_filter(dialog, filter);

  gtk_file_dialog_open(dialog,
                       window_,  // 父窗口（很重要）
                       nullptr,  // cancellable
                       MainWindow::OnFileOpenFinish,
                       this);
}

void MainWindow::OnFileOpenFinish(GObject* source, GAsyncResult* result, gpointer user_data) {
  auto* self = static_cast<MainWindow*>(user_data);
  GtkFileDialog* dialog = GTK_FILE_DIALOG(source);

  GError* error = nullptr;
  GFile* file = gtk_file_dialog_open_finish(dialog, result, &error);

  if (!file) {
    if (error) {
      g_error_free(error);
    }
    return;  // 用户取消
  }

  char* path = g_file_get_path(file);
  if (path) {
    g_print("选择的文件路径: %s\n", path);

    // 加载并显示图片
    self->LoadAndDisplayImage(file);

    g_free(path);
  }

  g_object_unref(file);
}

void MainWindow::OnMenuFileSave() {
  std::cout << "OnMenuFileSave" << std::endl;
  GtkFileDialog* dialog = gtk_file_dialog_new();
  gtk_file_dialog_set_title(dialog, "保存文件");

  /* 默认文件名（很重要，体验好） */
  gtk_file_dialog_set_initial_name(dialog, "output.mp4");

  /* === 只允许保存 MP4 === */
  GtkFileFilter* filter = gtk_file_filter_new();
  gtk_file_filter_set_name(filter, "MP4 视频 (*.mp4)");
  gtk_file_filter_add_mime_type(filter, "video/mp4");
  gtk_file_filter_add_suffix(filter, "mp4");

  GListStore* filters = g_list_store_new(GTK_TYPE_FILE_FILTER);
  g_list_store_append(filters, filter);

  gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(filters));
  gtk_file_dialog_set_default_filter(dialog, filter);

  gtk_file_dialog_save(dialog,
                       window_,  // 父窗口
                       nullptr,  // cancellable
                       MainWindow::OnFileSaveFinish,
                       this);
}

void MainWindow::OnFileSaveFinish(GObject* source, GAsyncResult* result, gpointer user_data) {
  GtkFileDialog* dialog = GTK_FILE_DIALOG(source);

  GError* error = nullptr;
  GFile* file = gtk_file_dialog_save_finish(dialog, result, &error);

  if (!file) {
    if (error)
      g_error_free(error);
    return;  // 用户取消
  }

  char* path = g_file_get_path(file);
  if (path) {
    g_print("保存路径: %s\n", path);

    /* TODO: 在这里真正写文件 */
    // SaveDataToFile(path);

    g_free(path);
  }

  g_object_unref(file);
}

void MainWindow::LoadAndDisplayImage(GFile* file) {
  if (!file || !picture_) {
    return;
  }

  // 使用 GtkPicture 加载图片文件
  gtk_picture_set_file(picture_, file);

  // 更新窗口标题显示当前图片文件名
  char* basename = g_file_get_basename(file);
  if (basename) {
    char title[256];
    g_snprintf(title, sizeof(title), "GTK4 图片查看器 - %s", basename);
    gtk_window_set_title(window_, title);
    g_free(basename);
  }
}

void MainWindow::OnMenuFileExit() {
  std::cout << "OnMenuFileExit" << std::endl;
  g_application_quit(G_APPLICATION(app_));
}

void MainWindow::OnWindowDestroy(GtkWidget*, gpointer user_data) {
  auto* self = static_cast<MainWindow*>(user_data);
  self->window_ = nullptr;
}

void MainWindow::CenterWindowOnScreen() {
  GtkNative* native = gtk_widget_get_native(GTK_WIDGET(window_));
  if (!native)
    return;

  GdkSurface* surface = gtk_native_get_surface(native);
  if (!surface)
    return;

  HWND hwnd = (HWND)gdk_win32_surface_get_handle(surface);
  if (!hwnd)
    return;

  RECT rect;
  GetWindowRect(hwnd, &rect);

  int win_w = rect.right - rect.left;
  int win_h = rect.bottom - rect.top;

  int screen_w = GetSystemMetrics(SM_CXSCREEN);
  int screen_h = GetSystemMetrics(SM_CYSCREEN);

  int x = (screen_w - win_w) / 2;
  int y = (screen_h - win_h) / 2;

  SetWindowPos(hwnd, nullptr, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
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