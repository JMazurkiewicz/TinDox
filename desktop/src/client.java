import javax.swing.*;
import java.io.*;
import java.net.*;

public class client {
    public static void main (String[] args) throws IOException {
        JFrame frame = new JavaClientGUI( "Java client");
        frame.setVisible(true);

        Socket s = new Socket("localhost",4999);

        InputStreamReader in = new InputStreamReader(s.getInputStream());
        BufferedReader bf = new BufferedReader(in);

        String str = bf.readLine();
        System.out.println("server: " + str);
        JOptionPane.showMessageDialog(frame, "server: " +str );

    }
}
