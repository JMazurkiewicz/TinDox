import javax.swing.*;

public class JavaClientGUI extends JFrame {
    private JPanel mainPanel;
    private JButton button1;
    private JButton button2;
    private JButton button3;
    private JButton button4;
    private JTree tree1;
    private JTextArea textArea1;
    private JTextField textField1;

    public JavaClientGUI(String title) {
        super (title);

        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setContentPane(mainPanel);
        this.pack();
    }

    /*public static void main (String[] args) {
        JFrame frame = new JavaClientGUI( "Java client");
        frame.setVisible(true);
    }*/
}
