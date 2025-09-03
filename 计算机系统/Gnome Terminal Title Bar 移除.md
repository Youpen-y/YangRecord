在 `~/.config/gtk-3.0/` 目录下，添加 `gtk.css`，内容如下：
```css
/* 隐藏标题栏但保持窗口可拖动 */
terminal-window headerbar {
    min-height: 0px;
    padding: 0px;
    margin: 0px;
    background: transparent;
    border: none;
    box-shadow: none;
}

/* 隐藏标题栏文字和按钮 */
terminal-window headerbar *
{
    min-height: 0px;
    padding: 0px;
    margin: 0px;
    opacity: 0;
}

/* 设置窗口圆角 */
terminal-window decoration {
    border-radius: 8px;
}

/* 确保终端内容区域也有圆角 */
terminal-window {
    border-radius: 5px;
}
```