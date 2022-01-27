import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class JavaClientGUI extends JFrame {
    private String msg="";
    public boolean sent = true;
    private JPanel mainPanel;
    private JButton treeButton;
    private JButton renameButton;
    private JButton ulButton;
    private JTextArea textArea;
    private JTextField textField;
    private JButton authButton;
    private JButton lsButton;
    private JButton logoutButton;
    private JButton rmButton;
    private JButton cpButton;
    private JButton mkdirButton;
    private JButton namePermsButton;
    private JButton pwdButton;
    private JButton mvButton;
    private JButton dlButton;
    private JButton cdButton;
    private JTextField textField1;
    private JTextField textField2;

    public String getMsg(){
        return msg;
    }
    public void setMsg(String msg){
        this.msg= msg;
    }
    public boolean getSent(){
        return sent;
    }
    public void setSent(boolean sent){
        this.sent = sent;
    }

    public void setTextArea(String text) {
        this.textArea.setText(text);
    }
    public void appendTextArea(String text){
        this.textArea.append(text);
    }

    public JavaClientGUI(String title) {
        super (title);

        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setContentPane(mainPanel);
        this.pack();

        textArea.setEditable(false);
        textArea.append("Hello in TINdox\n");


        authButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                setMsg("auth\n login : admin \n passwd : admin\n\n");
                setSent(false);
            }
        });
        logoutButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                setMsg("logout\n\n");
                setSent(false);
            }
        });

        namePermsButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                setMsg("name\n\nperms\n\n");
                setSent(false);
            }
        });
        treeButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                setMsg("tree\n\n");
                setSent(false);
            }
        });


        lsButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String msg = "ls\n";
                String path= textField.getText();
                String size= textField1.getText();
                String mod= textField2.getText();
                if (!path.isEmpty())
                    msg += "path" + ":" + path + "\n";

                if (!size.isEmpty())
                    msg += "size" + ":" + size + "\n";

                if (!mod.isEmpty())
                    msg += "mod" + ":" + mod + "\n";

                msg += "\n";
                setMsg(msg);
                setSent(false);
            }
        });
        pwdButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                setMsg("pwd\n\n");
                setSent(false);
            }
        });

        rmButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String msg = "rm\n";
                String name= textField.getText();
                msg += "name" + ":" + name + "\n";
                msg += "\n";

                setMsg(msg);
                setSent(false);
            }
        });
        cpButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String msg = "cp\n";
                String name= textField.getText();
                String path= textField1.getText();

                msg += "name" + ":" + name + "\n";
                msg += "path" + ":" + path + "\n";
                msg += "\n";

                setMsg(msg);
                setSent(false);

            }
        });
        cdButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String msg = "cd\n";
                String path= textField.getText();
                msg += "path" + ":" + path + "\n";
                msg += "\n";

                setMsg(msg);
                setSent(false);

            }
        });
        mkdirButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String msg = "mkdir\n";
                String name= textField.getText();
                msg += "name" + ":" + name + "\n";
                msg += "\n";

                setMsg(msg);
                setSent(false);
            }
        });
        mvButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String msg = "cp\n";
                String name= textField.getText();
                String path= textField1.getText();

                msg += "name" + ":" + name + "\n";
                msg += "path" + ":" + path + "\n";
                msg += "\n";

                setMsg(msg);
                setSent(false);

            }
        });
        renameButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String msg = "rename\n";
                String oname= textField.getText();
                String nname= textField1.getText();

                msg += "oname" + ":" + oname + "\n";
                msg += "nname" + ":" + nname + "\n";
                msg += "\n";

                setMsg(msg);
                setSent(false);
            }
        });
        dlButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String msg = "dl\n";
                String name= textField.getText();

                msg += "name" + ":" + name + "\n";
                msg += "\n";

                setMsg(msg);
                setSent(false);
            }
        });

        //todo size, retry
        ulButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String msg = "ul\n";
                String name= textField.getText();
                String size= textField1.getText();
                String retry= textField1.getText();

                msg += "name" + ":" + name + "\n";
                msg += "size" + ":" + size + "\n";
                msg += "retry" + ":" + retry + "\n";
                msg += "\n";

                setMsg(msg);
                setSent(false);
            }
        });
    }


    /*public static void main (String[] args) {
        JFrame frame = new JavaClientGUI( "Java client");
        frame.setVisible(true);
    }*/
}
