import javax.swing.*;
import java.io.*;
import java.net.*;
import java.nio.*;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;

import static java.lang.Thread.sleep;


public class client {

    private SocketChannel socket;
    private static ByteBuffer buf;
    private CharBuffer cbuf;
    public final static int port = 37666;
    private final String server = "localhost";
    private StringBuffer lastSentCommand;
    private StringBuffer receivedResponse;
    private static final Charset charset  = Charset.forName("ISO-8859-2");



    public client () throws IOException {
        try {
            socket = SocketChannel.open();
            socket.configureBlocking(false);
            connect();
        } catch(UnknownHostException exc) {
            System.err.println("Uknown host " + server);
            System.exit(1);
            // ...
        } catch(Exception exc) {
            exc.printStackTrace();
            System.exit(2);
            // ...
        }
        
    }

    public static void setBuf(ByteBuffer buf) {
        client.buf = buf;
    }

    // Łączenie kanału z serwerem
    private void connect() throws UnknownHostException, IOException {
        if (!socket.isOpen()) socket = SocketChannel.open();
        socket.connect(new InetSocketAddress(server, port));
        System.out.print("Łącze się ...");
        while (!socket.finishConnect()) {
            try { sleep(200); } catch(Exception exc) { return; }
            System.out.print(".");
            // ...
        }
        System.out.println("\nPołączony.");
    }
    private void closeConnection(){
        if (socket.isOpen()) {
            try {
                socket.close();
            } catch (IOException e) {
                e.printStackTrace();
                // ..
            }
        }

    }


    // .. sprawdzanie poprawnosci wpisanych pól+ jaki przycisk
    /*public boolean sendCommand( String command){
        return true;
    }*/
    public void sendToServer(String msg){
        System.out.print("\n" + msg);
        if (!socket.isConnected()) try {
            connect();
        } catch(Exception exc) {
            exc.printStackTrace();
            // ..
        }
        try {
            cbuf = CharBuffer.wrap(msg + "\n");
            ByteBuffer outBuf = charset.encode(cbuf);
            socket.write(outBuf);

    } catch (IOException e) {
            e.printStackTrace();
            // ..

        }
    }

    public void receiveMsgFromServer(String msg) {
        msg = "";
        if (!socket.isConnected()) try {
            connect();
        } catch (Exception exc) {
            exc.printStackTrace();
            return;
        }
        while (true) {
            buf.clear();
            int readBytes = 0;
            try {
                readBytes = socket.read(buf);
            } catch (IOException e) {
                e.printStackTrace();
            }
            if (readBytes == 0) {
                System.out.print("Czekam na odpowiedź servera... ");
                try {
                    sleep(200);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                    return;
                }
                continue;
            } else if (readBytes == -1) {
                System.out.print("Kanał zamknięty");
                try {
                    socket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
                break;
            } else {
                buf.flip();
                cbuf = charset.decode(buf);
                msg=cbuf.toString();
                System.out.print(msg+ "\n");
                break;
            }

        }

    }




    public static void main (String[] args) throws IOException {
        JFrame frame = new JavaClientGUI("Java client");
        frame.setVisible(true);

        client cli = new client();
        JOptionPane.showMessageDialog(frame, "server: " + " Połaczony");
        buf = ByteBuffer.allocateDirect(64);
        setBuf(buf);
        //auth
        cli.sendToServer("auth\n" + "login : admin \n passwd : admin\n");
        cli.receiveMsgFromServer(String.valueOf(cli.receivedResponse));
        //ls
        cli.sendToServer("ls\n" );
        cli.receiveMsgFromServer(String.valueOf(cli.receivedResponse));

        //tree
        cli.sendToServer("tree\n" );
        cli.receiveMsgFromServer(String.valueOf(cli.receivedResponse));

        //rename
        cli.sendToServer("rename\n oname : big \n nname : bigger\n");
        cli.receiveMsgFromServer(String.valueOf(cli.receivedResponse));

        cli.closeConnection();








       /* InputStreamReader in = new InputStreamReader(socket.getInputStream());
        BufferedReader bf = new BufferedReader(in);

        String str = bf.readLine();
        System.out.println("server: " + str);
        JOptionPane.showMessageDialog(frame, "server: " +str );*/
    }



}
