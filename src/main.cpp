/* *******************************************************************************************************************
 * Copyright：Zix
 * Author：Zix
 * Date：2017/01/27
 * Description：linux下的一款桌面便签程序。
 *
 * Version：1.0
 * Copyright：Zix
 * Author：Zix
 * Date：2016/02/02
 * Description: 1. 基本完成了必须的功能，
 *                - 支持便签的增删改管理
 *                - 提供对便签颜色样式的修改
 *                - 支持桌面固定便签
 * TODO list: 1. 便签云同步
 *            2. 程序设置
 *            3. 便签查找
 *
 * Version：1.1
 * Copyright：Zix
 * Author：Zix
 * Date：2016/02/13
 * Description: 1. 修改文件保存位置至home目录
 *              2. 新增托盘右键新建
 * TODO list: 1. 便签云同步
 *            2. 程序设置
 *            3. 便签查找
 *            4. 全局快捷键
 *            5. 便签置顶
 *
 * Version：1.2
 * Copyright：Zix
 * Author：Zix
 * Date：2017/05/08
 * Description: 1. 添加便签置顶功能
 * TODO list: 1. 双击托盘临时置顶所有便签
 *            2. 支持markdown语法
 *
 * Version：1.2.1
 * Copyright：Zix
 * Author：Zix
 * Date：2017/05/09
 * Description: 1. 添加单击托盘置顶所有可见便签功能
 ********************************************************************************************************************/
#include "XMemo.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    if(translator.load(":/language/zh_CN.qm"))
    {
        a.installTranslator(&translator);
    }

    XMemo w;

    return a.exec();
}
