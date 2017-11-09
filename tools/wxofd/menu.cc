#include <wx/wxprec.h>
#include <wx/defs.h>
#include <wx/wx.h>

wxMenu *CreateMenuFile(){
    wxMenu *menuFile = new wxMenu;

    {
        // -------- 打开
        menuFile->Append(wxID_OPEN, "打开(&N)\tCtrl-O", "打开OFD文件");
        // -------- 新建 
        wxMenu *menuNew = new wxMenu;
        menuNew->Append(wxID_NEW, "空白文档(&B)", "");
        menuNew->Append(wxID_NEW, "从文件(&F)", "");
        menuNew->Append(wxID_NEW, "从扫描仪(&S)", "");
        menuFile->AppendSubMenu(menuNew, "新建(&N)", "");
    }

    menuFile->AppendSeparator();
    {
        // -------- 保存
        menuFile->Append(wxID_SAVE, "保存(&S)\tCtrl-S", "保存OFD文件");
        // -------- 另存为 
        menuFile->Append(wxID_SAVEAS, "另存为(&A)\tCtrl-Shift-S", "另存为OFD文件");
    }

    menuFile->AppendSeparator();
    {
        // -------- 导出
        menuFile->Append(wxID_SAVE, "导出(&E)\tCtrl-E", "导出OFD文件");
        // -------- 打印
        menuFile->Append(wxID_PRINT, "打印(&P)\tCtrl-P", "打印OFD文件");
        // -------- 属性
        menuFile->Append(wxID_PROPERTIES, "属性(&T)\tCtrl-D", "OFD文件属性");
    }

    menuFile->AppendSeparator();
    {
        // -------- 关闭
        menuFile->Append(wxID_CLOSE, "关闭(&C)\tCtrl-W", "关闭当前OFD文件");
    }

    return menuFile;
}

wxMenu *CreateMenuEdit(){
    wxMenu *menuEdit = new wxMenu;

    {
        // -------- 撤销
        menuEdit->Append(wxID_UNDO, "撤销(&U)\tCtrl-Z", "");
        // -------- 重做
        menuEdit->Append(wxID_REDO, "重做(&R)\tCtrl-Y", "");

    }

    menuEdit->AppendSeparator();
    {
        // -------- 复制
        menuEdit->Append(wxID_COPY, "复制(&C)\tCtrl-C", "");
        // -------- 粘贴
        menuEdit->Append(wxID_PASTE, "粘贴(&P)\tCtrl-V", "");
        // -------- 剪切
        menuEdit->Append(wxID_CUT, "剪切(&T)\tCtrl-X", "");
        // -------- 删除
        menuEdit->Append(wxID_DELETE, "删除(&D)", "");
    }

    menuEdit->AppendSeparator();
    {
        // -------- 全选
        menuEdit->Append(wxID_SELECTALL, "全选(&L)\tCtrl-A", "");
        // -------- 全不选
        menuEdit->Append(wxID_SELECTALL, "全不选(&E)\tCtrl-Shift-A", "");
        // -------- 查找
        menuEdit->Append(wxID_FIND, "查找(&F)\tCtrl-F", "");
    }

    return menuEdit;
}

wxMenu *CreateMenuView(){
    wxMenu *menuView = new wxMenu;

    {
        // -------- 文档导航
        wxMenu *menuNav = new wxMenu;
        menuNav->Append(wxID_ANY, "所有(&A)", "");
        menuNav->Append(wxID_ANY, "页面(&P)", "");
        menuNav->Append(wxID_ANY, "书签(&B)", "");
        menuNav->Append(wxID_ANY, "大纲(&O)", "");
        menuNav->Append(wxID_ANY, "附件(&M)", "");
        menuNav->Append(wxID_ANY, "标引(&P)", "");
        menuNav->Append(wxID_ANY, "注释(&A)", "");
        menuView->AppendSubMenu(menuNav, "文档导航(&N)");
        // -------- 文档和版本
        menuView->Append(wxID_ANY, "文档和版本(&V)", "");
    }

    menuView->AppendSeparator();
    {
        // -------- 缩小
        menuView->Append(wxID_ZOOM_OUT, "缩小(&O)\tCtrl--", "");
        // -------- 放大
        menuView->Append(wxID_ZOOM_IN, "放大(&I)\tCtrl-+", "");
        // -------- 缩放为
        menuView->Append(wxID_ANY, "缩放为(&C)\tCtrl-M", "");
        // -------- 缩放模式
        wxMenu *menuZoom = new wxMenu;
        menuZoom->Append(wxID_ZOOM_100, "原始大小(&O)\tCtrl-1", "");
        menuZoom->Append(wxID_ZOOM_FIT, "适合页面(&P)\tCtrl-2", "");
        menuZoom->Append(wxID_ZOOM_FIT, "适合宽度(&W)\tCtrl-3", "");
        menuZoom->Append(wxID_ZOOM_FIT, "适合高度(&H)\tCtrl-4", "");
        menuView->AppendSubMenu(menuZoom, "缩放模式(&Z)", "");
    }

    menuView->AppendSeparator();
    {
        // -------- 页面布局
        wxMenu *menuLayout = new wxMenu;
        menuLayout->Append(wxID_ANY, "单页(&S)\tCtrl-Shift-1", "");
        menuLayout->Append(wxID_ANY, "单页连续(&C)\tCtrl-Shift-2", "");
        menuLayout->Append(wxID_ANY, "对开(&F)\tCtrl-Shift-3", "");
        menuLayout->Append(wxID_ANY, "对开靠右(&E)\tCtrl-Shift-4", "");
        menuLayout->Append(wxID_ANY, "对开连续(&T)\tCtrl-Shift-5", "");
        menuLayout->Append(wxID_ANY, "连续靠右(&R)\tCtrl-Shift-6", "");
        menuView->AppendSubMenu(menuLayout, "页面布局(&L)", "");
        // -------- 旋转视图
        wxMenu *menuRotate = new wxMenu;
        menuRotate->Append(wxID_ANY, "顺时针(&C)\tCtrl-Shift-+", "");
        menuRotate->Append(wxID_ANY, "逆时针(&W)\tCtrl-Shift--", "");
        menuView->AppendSubMenu(menuRotate, "旋转视图(&R)", "");

    }

    menuView->AppendSeparator();
    {
        // -------- 显示或隐藏菜单栏
        menuView->AppendCheckItem(wxID_ANY, "隐藏菜单栏(&T)", "");
        // -------- 显示或隐藏工具栏
        menuView->AppendCheckItem(wxID_ANY, "隐藏工具栏(&M)", "");

        // -------- 全屏显示
        menuView->AppendCheckItem(wxID_ANY, "全屏显示\tCtrl-Shift-F");
    }

    return menuView;
}

wxMenu *CreateMenuDoc(){
    wxMenu *menuDoc = new wxMenu;

    {
        // -------- 首页
        menuDoc->Append(wxID_FIRST, "首页(&F)\tHome", "");
        // -------- 上一页
        menuDoc->Append(wxID_ANY, "上一页(&P)\tPageUp", "");
        // -------- 下一页
        menuDoc->Append(wxID_ANY, "下一页(&N)\tPageDown", "");
        // -------- 末页
        menuDoc->Append(wxID_LAST, "末页(&L)\tEnd", "");
        // -------- 跳到页面
        menuDoc->Append(wxID_JUMP_TO, "跳到页面(&G)\tCtrl-G", "");
    }

    menuDoc->AppendSeparator();
    {
        // -------- 应用模板
        menuDoc->Append(wxID_ANY, "应用模板(&T)", "");
        // -------- 插入页面
        menuDoc->Append(wxID_ANY, "插入页面(&I)", "");
        // -------- 删除页面
        menuDoc->Append(wxID_ANY, "删除页面(&D)", "");
        // -------- 提取页面
        menuDoc->Append(wxID_ANY, "提取页面(&S)", "");
    }

    menuDoc->AppendSeparator();
    {
        // -------- 附件
        menuDoc->Append(wxID_ANY, "附件(&M)", "");
        // -------- 水印
        menuDoc->Append(wxID_ANY, "水印(&W)", "");
    }

    return menuDoc;
}

wxMenu *CreateMenuTool(){
    wxMenu *menuTool = new wxMenu;

    {
        // -------- 手型工具
        menuTool->Append(wxID_ANY, "手型工具(&H)", "");
        // -------- 选择注释
        menuTool->Append(wxID_ANY, "选择注释(&A)", "");
        // -------- 选择文本
        menuTool->Append(wxID_ANY, "选择文本(&T)", "");
        // -------- 快照工具
        menuTool->Append(wxID_ANY, "快照工具(&N)", "");
    }

    menuTool->AppendSeparator();
    {
        // -------- 绘制线条
        menuTool->Append(wxID_ANY, "绘制线条(&L)", "");
        // -------- 绘制矩形
        menuTool->Append(wxID_ANY, "绘制矩形(&R)", "");
        // -------- 绘制折线
        menuTool->Append(wxID_ANY, "绘制折线(&P)", "");
    }

    return menuTool;
}

wxMenu *CreateMenuHelp(){
    wxMenu *menuHelp = new wxMenu;

    {
        // -------- 产中信息
        menuHelp->Append(wxID_ABOUT, "产品信息(&P)\tCtrl-A", "");
        // -------- 用户说明
        menuHelp->Append(wxID_ANY, "用户说明(&U)", "");
        // -------- 命令行
        menuHelp->Append(wxID_ANY, "命令行(&C)", "");
        // -------- 快捷键
        menuHelp->Append(wxID_ANY, "快捷键(&S)", "");
    }

    return menuHelp;
}

wxMenuBar *CreateMainMenubar() {

    wxMenuBar *mbar = new wxMenuBar;

    wxMenu *menuFile = CreateMenuFile();
    mbar->Append(menuFile, "文件(&F)");

    wxMenu *menuEdit = CreateMenuEdit();
    mbar->Append(menuEdit, "编辑(&E)");

    wxMenu *menuView = CreateMenuView();
    mbar->Append(menuView, "视图(&V)");

    wxMenu *menuDoc = CreateMenuDoc();
    mbar->Append(menuDoc, "文档(&D)");

    wxMenu *menuTool = CreateMenuTool();
    mbar->Append(menuTool, "工具(&T)");

    //menuFile->Append(wxID_EXIT, "&Exit\tAlt-X", "Quit the program");

    wxMenu *menuHelp = CreateMenuHelp();
    mbar->Append(menuHelp, "帮助(&H)");


    return mbar;
}
