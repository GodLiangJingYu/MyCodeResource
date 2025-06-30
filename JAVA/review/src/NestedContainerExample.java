import javax.swing.*; // 导入用于图形界面的Swing库
import java.awt.*;    // 导入用于布局管理的AWT库

public class NestedContainerExample {
    public static void main(String[] args) {
        // 创建主窗口对象
        JFrame frame = new JFrame("Nested Container Example"); // 设置窗口标题为 "Nested Container Example"

        // 创建一个主面板（容器）
        JPanel mainPanel = new JPanel(); // 创建主面板
        mainPanel.setLayout(new BorderLayout()); // 设置主面板的布局为BorderLayout

        // 创建一个子面板（嵌套容器）
        JPanel subPanel = new JPanel(); // 创建子面板
        subPanel.setLayout(new FlowLayout()); // 设置子面板的布局为FlowLayout（流式布局）

        // 向子面板添加组件（两个标签）
        subPanel.add(new JLabel("Label 1")); // 添加第一个标签
        subPanel.add(new JLabel("Label 2")); // 添加第二个标签

        // 将子面板添加到主面板的中心位置
        mainPanel.add(subPanel, BorderLayout.CENTER);

        // 将主面板添加到主窗口
        frame.add(mainPanel);

        // 设置窗口关闭行为为退出程序
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // 设置窗口大小为300x200像素
        frame.setSize(1000, 1000);

        // 显示窗口
        frame.setVisible(true);
    }
}
