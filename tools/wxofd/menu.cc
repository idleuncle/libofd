#include <wx/wxprec.h>
#include <wx/defs.h>
#include <wx/wx.h>

#include "Global.h"

wxMenu *CreateMenuFile(){
    wxMenu *menuFile = new wxMenu;
    //menuFile->SetLabel(wxID_ANY, "File");

    {
        // -------- 打开
        menuFile->Append(wxID_OPEN, wxT("打开(&N)\tCtrl-O"), wxT("打开OFD文件"));
        // -------- 新建 
        wxMenu *menuNew = new wxMenu;
        menuNew->Append(ID_FILE_NEW, wxT("空白文档(&B)"), wxT("空白文档(&B)"));
        menuNew->Append(ID_FILE_NEW_FROM_FILE, wxT("从文件(&F)"), wxT("从文件(&F)"));
        menuNew->Append(ID_FILE_NEW_FROM_SCANNER, wxT("从扫描仪(&S)"), wxT("从扫描仪(&S)"));
        menuFile->AppendSubMenu(menuNew, wxT("新建(&N)"), wxT("新建(&N)"));
    }

    menuFile->AppendSeparator();
    {
        // -------- 保存
        menuFile->Append(wxID_SAVE, wxT("保存(&S)\tCtrl-S"), wxT("保存OFD文件"));
        // -------- 另存为 
        menuFile->Append(wxID_SAVEAS, wxT("另存为(&A)\tCtrl-Shift-S"), wxT("另存为OFD文件"));
    }

    menuFile->AppendSeparator();
    {
        // -------- 导出
        menuFile->Append(wxID_ANY, wxT("导出(&E)\tCtrl-E"), wxT("导出OFD文件"));
        // -------- 打印
        menuFile->Append(wxID_PRINT, wxT("打印(&P)\tCtrl-P"), wxT("打印OFD文件"));
        // -------- 属性
        menuFile->Append(wxID_PROPERTIES, wxT("属性(&T)\tCtrl-D"), wxT("OFD文件属性"));
    }

    menuFile->AppendSeparator();
    {
        // -------- 关闭
        menuFile->Append(wxID_CLOSE, wxT("关闭(&C)\tCtrl-W"), wxT("关闭当前OFD文件"));
    }

    return menuFile;
}

wxMenu *CreateMenuEdit(){
    wxMenu *menuEdit = new wxMenu;

    {
        // -------- 撤销
        menuEdit->Append(wxID_UNDO, wxT("撤销(&U)\tCtrl-Z"), wxT(""));
        // -------- 重做
        menuEdit->Append(wxID_REDO, wxT("重做(&R)\tCtrl-Y"), wxT(""));

    }

    menuEdit->AppendSeparator();
    {
        // -------- 复制
        menuEdit->Append(wxID_COPY, wxT("复制(&C)\tCtrl-C"), wxT(""));
        // -------- 粘贴
        menuEdit->Append(wxID_PASTE, wxT("粘贴(&P)\tCtrl-V"), wxT(""));
        // -------- 剪切
        menuEdit->Append(wxID_CUT, wxT("剪切(&T)\tCtrl-X"), wxT(""));
        // -------- 删除
        menuEdit->Append(wxID_DELETE, wxT("删除(&D)"), wxT(""));
    }

    menuEdit->AppendSeparator();
    {
        // -------- 全选
        menuEdit->Append(wxID_SELECTALL, wxT("全选(&L)\tCtrl-A"), wxT(""));
        // -------- 全不选
        menuEdit->Append(wxID_ANY, wxT("全不选(&E)\tCtrl-Shift-A"), wxT(""));
        // -------- 查找
        menuEdit->Append(wxID_FIND, wxT("查找(&F)\tCtrl-F"), wxT(""));
    }

    return menuEdit;
}

wxMenu *CreateMenuView(){
    wxMenu *menuView = new wxMenu;

    {
        // -------- 文档导航
        wxMenu *menuNav = new wxMenu;
        menuNav->Append(wxID_ANY, wxT("所有(&A)"), wxT(""));
        menuNav->Append(wxID_ANY, wxT("页面(&P)"), wxT(""));
        menuNav->Append(wxID_ANY, wxT("书签(&B)"), wxT(""));
        menuNav->Append(wxID_ANY, wxT("大纲(&O)"), wxT(""));
        menuNav->Append(wxID_ANY, wxT("附件(&M)"), wxT(""));
        menuNav->Append(wxID_ANY, wxT("标引(&P)"), wxT(""));
        menuNav->Append(wxID_ANY, wxT("注释(&A)"), wxT(""));
        menuView->AppendSubMenu(menuNav, wxT("文档导航(&N)"), wxT("文档导航(&N)"));
        // -------- 文档和版本
        menuView->Append(wxID_ANY, wxT("文档和版本(&V)"), wxT(""));
    }

    menuView->AppendSeparator();
    {
        // -------- 缩小
        menuView->Append(wxID_ZOOM_OUT, wxT("缩小(&O)\tCtrl--"), wxT(""));
        // -------- 放大
        menuView->Append(wxID_ZOOM_IN, wxT("放大(&I)\tCtrl-+"), wxT(""));
        // -------- 缩放为
        menuView->Append(wxID_ANY, wxT("缩放为(&C)\tCtrl-M"), wxT(""));
        // -------- 缩放模式
        wxMenu *menuZoom = new wxMenu;
        menuZoom->Append(wxID_ZOOM_100, wxT("原始大小(&O)\tCtrl-1"), wxT(""));
        menuZoom->Append(wxID_ZOOM_FIT, wxT("适合页面(&P)\tCtrl-2"), wxT(""));
        menuZoom->Append(wxID_ZOOM_FIT, wxT("适合宽度(&W)\tCtrl-3"), wxT(""));
        menuZoom->Append(wxID_ZOOM_FIT, wxT("适合高度(&H)\tCtrl-4"), wxT(""));
        menuView->AppendSubMenu(menuZoom, wxT("缩放模式(&Z)"), wxT("缩放模式(&Z)"));
    }

    menuView->AppendSeparator();
    {
        // -------- 页面布局
        wxMenu *menuLayout = new wxMenu;
        menuLayout->Append(wxID_ANY, wxT("单页(&S)\tCtrl-Shift-1"), wxT(""));
        menuLayout->Append(wxID_ANY, wxT("单页连续(&C)\tCtrl-Shift-2"), wxT(""));
        menuLayout->Append(wxID_ANY, wxT("对开(&F)\tCtrl-Shift-3"), wxT(""));
        menuLayout->Append(wxID_ANY, wxT("对开靠右(&E)\tCtrl-Shift-4"), wxT(""));
        menuLayout->Append(wxID_ANY, wxT("对开连续(&T)\tCtrl-Shift-5"), wxT(""));
        menuLayout->Append(wxID_ANY, wxT("连续靠右(&R)\tCtrl-Shift-6"), wxT(""));
        menuView->AppendSubMenu(menuLayout, wxT("页面布局(&L)"), wxT("页面布局(&L)"));
        // -------- 旋转视图
        wxMenu *menuRotate = new wxMenu;
        menuRotate->Append(wxID_ANY, wxT("顺时针(&C)\tCtrl-Shift-+"), wxT(""));
        menuRotate->Append(wxID_ANY, wxT("逆时针(&W)\tCtrl-Shift--"), wxT(""));
        menuView->AppendSubMenu(menuRotate, wxT("旋转视图(&R)"), wxT("旋转视图(&R)"));

    }

    menuView->AppendSeparator();
    {
        // -------- 显示或隐藏菜单栏
        menuView->AppendCheckItem(wxID_ANY, wxT("隐藏菜单栏(&T)"), wxT(""));
        // -------- 显示或隐藏工具栏
        menuView->AppendCheckItem(wxID_ANY, wxT("隐藏工具栏(&M)"), wxT(""));

        // -------- 全屏显示
        menuView->AppendCheckItem(wxID_ANY, wxT("全屏显示\tCtrl-Shift-F"), wxT(""));
    }

    return menuView;
}

wxMenu *CreateMenuDoc(){
    wxMenu *menuDoc = new wxMenu;

    {
        // -------- 首页
        menuDoc->Append(wxID_FIRST, wxT("首页(&F)\tHome"), wxT(""));
        // -------- 上一页
        menuDoc->Append(wxID_ANY, wxT("上一页(&P)\tPageUp"), wxT(""));
        // -------- 下一页
        menuDoc->Append(wxID_ANY, wxT("下一页(&N)\tPageDown"), wxT(""));
        // -------- 末页
        menuDoc->Append(wxID_LAST, wxT("末页(&L)\tEnd"), wxT(""));
        // -------- 跳到页面
        menuDoc->Append(wxID_JUMP_TO, wxT("跳到页面(&G)\tCtrl-G"), wxT(""));
    }

    menuDoc->AppendSeparator();
    {
        // -------- 应用模板
        menuDoc->Append(wxID_ANY, wxT("应用模板(&T)"), wxT(""));
        // -------- 插入页面
        menuDoc->Append(wxID_ANY, wxT("插入页面(&I)"), wxT(""));
        // -------- 删除页面
        menuDoc->Append(wxID_ANY, wxT("删除页面(&D)"), wxT(""));
        // -------- 提取页面
        menuDoc->Append(wxID_ANY, wxT("提取页面(&S)"), wxT(""));
    }

    menuDoc->AppendSeparator();
    {
        // -------- 附件
        menuDoc->Append(wxID_ANY, wxT("附件(&M)"), wxT(""));
        // -------- 水印
        menuDoc->Append(wxID_ANY, wxT("水印(&W)"), wxT(""));
    }

    return menuDoc;
}

wxMenu *CreateMenuTool(){
    wxMenu *menuTool = new wxMenu;

    {
        // -------- 手型工具
        menuTool->Append(wxID_ANY, wxT("手型工具(&H)"), wxT(""));
        // -------- 选择注释
        menuTool->Append(wxID_ANY, wxT("选择注释(&A)"), wxT(""));
        // -------- 选择文本
        menuTool->Append(wxID_ANY, wxT("选择文本(&T)"), wxT(""));
        // -------- 快照工具
        menuTool->Append(wxID_ANY, wxT("快照工具(&N)"), wxT(""));
    }

    menuTool->AppendSeparator();
    {
        // -------- 绘制线条
        menuTool->Append(wxID_ANY, wxT("绘制线条(&L)"), wxT(""));
        // -------- 绘制矩形
        menuTool->Append(wxID_ANY, wxT("绘制矩形(&R)"), wxT(""));
        // -------- 绘制折线
        menuTool->Append(wxID_ANY, wxT("绘制折线(&P)"), wxT(""));
    }

    return menuTool;
}

wxMenu *CreateMenuHelp(){
    wxMenu *menuHelp = new wxMenu;

    {
        // -------- 产中信息
        menuHelp->Append(wxID_ABOUT, wxT("产品信息(&P)\tCtrl-A"), wxT(""));
        // -------- 用户说明
        menuHelp->Append(wxID_ANY, wxT("用户说明(&U)"), wxT(""));
        // -------- 命令行
        menuHelp->Append(wxID_ANY, wxT("命令行(&C)"), wxT(""));
        // -------- 快捷键
        menuHelp->Append(wxID_ANY, wxT("快捷键(&S)"), wxT(""));
    }

    return menuHelp;
}

wxMenuBar *CreateMainMenubar() {

    wxMenuBar *mbar = new wxMenuBar;

    wxMenu *menuFile = CreateMenuFile();
    mbar->Append(menuFile, wxT("文件(&F)"));

    wxMenu *menuEdit = CreateMenuEdit();
    mbar->Append(menuEdit, wxT("编辑(&E)"));

    wxMenu *menuView = CreateMenuView();
    mbar->Append(menuView, wxT("视图(&V)"));

    wxMenu *menuDoc = CreateMenuDoc();
    mbar->Append(menuDoc, wxT("文档(&D)"));

    wxMenu *menuTool = CreateMenuTool();
    mbar->Append(menuTool, wxT("工具(&T)"));

    wxMenu *menuHelp = CreateMenuHelp();
    mbar->Append(menuHelp, wxT("帮助(&H)"));

    return mbar;
}
