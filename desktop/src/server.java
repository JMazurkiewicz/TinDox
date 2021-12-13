import java.io.*;
import java.net.*;

public class server {
    public static void main (String[] args) throws IOException {
        ServerSocket ss = new ServerSocket(4999);
        Socket s = ss.accept();

        System.out.println("client connected");
        PrintWriter pr = new PrintWriter(s.getOutputStream());
        pr.println(("hello"));
        pr.flush();
    }
}
