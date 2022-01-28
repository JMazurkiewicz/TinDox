import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.util.logging.Level;
import java.util.logging.Logger;


public class Client {
    private static final String serverHost = "localhost";
    public static final int serverPort = 37666;

    private static final int RECEIVING_BUFFER_SIZE = 8192;
    private static final int SENDING_BUFFER_SIZE = 64;

    private final Selector selector;
    private final Connection connection;
    private final SocketChannel socketChannel;
    private final static Logger logger = Logger.getLogger(Connection.class.getSimpleName());
    private CharBuffer outBuf;
    private static final Charset charset  = Charset.forName("ISO-8859-2");
    private final ResponseAnalyzer responseAnalyzer;



    public Client() throws IOException {
        logger.setLevel(Level.SEVERE);
        socketChannel = SocketChannel.open();
        socketChannel.configureBlocking(false);

        socketChannel.connect(new InetSocketAddress(serverHost, serverPort));
        selector = Selector.open();
        SelectionKey key = socketChannel.register(selector, SelectionKey.OP_CONNECT);
        connection = new Connection(key, ByteBuffer.allocate(RECEIVING_BUFFER_SIZE), ByteBuffer.allocate(SENDING_BUFFER_SIZE));
        outBuf = CharBuffer.allocate(64);
        responseAnalyzer = new ResponseAnalyzer();

    }

    private void connect(Connection connection) {
        try {
            if (connection.channel.finishConnect()) {
                onFinishedConnect(connection);
            }
        }
        catch (IOException e) {
            logger.log(Level.SEVERE, "", e);
            shutdown();

        }

    }
    private void onFinishedConnect(Connection connection) {
        connection.key.interestOps( SelectionKey.OP_READ);
        logger.info("Connected to the server");

    }

    public void initiateSending(Connection connection,String msg) {

            boolean hasOpConnect = (connection.key.interestOps() & SelectionKey.OP_CONNECT) == SelectionKey.OP_CONNECT;
            boolean hasOpWrite = (connection.key.interestOps() & SelectionKey.OP_WRITE) == SelectionKey.OP_WRITE;
            if (!hasOpConnect && !hasOpWrite) {
                connection.key.interestOps(SelectionKey.OP_READ);
                connection.sendingBuffer.clear();
                outBuf = CharBuffer.wrap(msg);
                connection.sendingBuffer.put(charset.encode(outBuf));
                connection.sendingBuffer.flip();

                connection.key.interestOps(SelectionKey.OP_WRITE | SelectionKey.OP_READ);
            }
        }

    private void send(Connection connection) {

            try {
                long sentBytesCount = connection.channel.write(connection.sendingBuffer);
                logger.info("Wrote " + sentBytesCount + " bytes");
                if(sentBytesCount!=0){
                    System.out.println("Wrote " + sentBytesCount + " bytes");
                }
                if (!connection.sendingBuffer.hasRemaining()) {
                    onFinishedSend(connection);
                }
            } catch (IOException e) {
                logger.log(Level.WARNING, "", e);
                terminateConnection(connection);
            }
        }




    private void onFinishedSend(Connection connection) {
        connection.key.interestOps(SelectionKey.OP_READ);

    };

    private void receive(Connection connection, JavaClientGUI fr) {
        try {
            long receivedBytesCount = connection.channel.read(connection.receivingBuffer);
            logger.info("Read " + receivedBytesCount + " bytes");
            if (receivedBytesCount!=0){
            System.out.println("Read " + receivedBytesCount + " bytes");
            }
            if (receivedBytesCount == -1) {
                terminateConnection(connection);
            }
            else {

                onFinishedReceive(connection, fr);
            }
        }
        catch (IOException e) {
            logger.log(Level.WARNING, "", e);
            terminateConnection(connection);
        }
    }

    private void onFinishedReceive(Connection connection, JavaClientGUI fr) {
        if (connection.receivingBuffer.position()!=0 || !responseAnalyzer.isResponseClear()) {
            connection.receivingBuffer.flip();
            byte[] bytes = new byte[connection.receivingBuffer.remaining()];
            connection.receivingBuffer.get(bytes); // read the bytes that were written
            String response = new String(bytes);
            connection.receivingBuffer.clear();
            responseAnalyzer.analyze(response, fr);
        }
    }

    private void terminateConnection(Connection connection) {
        connection.key.cancel();
        try {
            connection.channel.close();
        }
        catch (IOException e) {
            logger.log(Level.SEVERE, "", e);
        }
    }

    //close all
    private void shutdown() {
        terminateConnection(connection);
        try {
            selector.close();
        }
        catch (IOException e) {
            logger.log(Level.SEVERE, "", e);
        }
    }
    public void run(JavaClientGUI fr, ResponseAnalyzer respAnalyzer) {
        while (connection.key.isValid()) {
            if (!fr.getSent()){
                initiateSending( connection,fr.getMsg());
                fr.setSent(true);
            }
            try {
                    selector.selectNow();
                if (connection.key.isValid() && connection.key.isConnectable()) {
                    connect(connection);
                }

                if (connection.key.isValid() && connection.key.isWritable()) {
                    send(connection);
                }

                if (connection.key.isValid() && connection.key.isReadable()) {
                    receive(connection, fr);
                }
            }
            catch (IOException e) {
                logger.log(Level.SEVERE, "", e);
                shutdown();
            }
        }
        shutdown();
    }

    public static void main(String[] args) throws IOException{
        JavaClientGUI frame = new JavaClientGUI("Java client");

        frame.setVisible(true);
        ResponseAnalyzer responseAnalyzer = new ResponseAnalyzer();

        try {

            Client client = new Client();
            logger.info("Starting a client");
            client.run(frame, responseAnalyzer);
        }
        catch (IOException e) {
            logger.log(Level.SEVERE, "", e);
        }
    }

}
