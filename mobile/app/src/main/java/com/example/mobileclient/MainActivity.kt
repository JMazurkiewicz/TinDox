package com.example.mobileclient

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import kotlinx.coroutines.*
import kotlinx.coroutines.Dispatchers.IO
import kotlinx.coroutines.Dispatchers.Main
import java.io.BufferedReader
import java.io.DataInputStream
import java.io.DataOutputStream
import java.io.PrintWriter
import java.net.Socket
import java.net.SocketException
import java.util.*


class MainActivity : AppCompatActivity() {
    private lateinit var ipAddressEditText: EditText
    private lateinit var portEditText: EditText
    private lateinit var connectButton: Button
    private lateinit var messageText: EditText
    private lateinit var submitButton: Button
    private lateinit var resultTextView: TextView

    private var authorizationToken:String = "auth\nlogin:admin\npasswd:admin\n\n"
    //private var socket: Socket = SocketHandler.getSocket()


//    private var active: Boolean = false
//    private var data: String = ""
//
//    private var message: String = ""
//    private var answer: String = ""
//    private lateinit var output: DataOutputStream
//    private lateinit var input: DataInputStream

                /*output = DataOutputStream(socket.getOutputStream())
                output.write(authorizationToken.toByteArray())

                input = DataInputStream(socket.getInputStream())
                Log.i("message", input.readUTF())*/

    fun connect(view: View) {
        var SERVER_IP: String = "192.168.1.18"//ipAddressEditText.text.toString()
        var SERVER_PORT: Int = 37666//Integer.parseInt(portEditText.text.toString())

        CoroutineScope(IO).launch{
            establishConnection(SERVER_IP, SERVER_PORT)
            checkConnection()
        }

    }

    private suspend fun establishConnection(ip: String, port: Int){
        try {
            //socket = Socket(ip, port)
            SocketHandler.setSocket(ip, port)
        }catch (e: SocketException){
            Log.i("exc", e.toString())
        }
    }

    private fun checkConnection(){
        if(SocketHandler.connected()){
            val intent = Intent(this, CommandActivity::class.java)
            startActivity(intent)
        }
    }

//    fun submit(view: View) {
//        CoroutineScope(IO).launch {
//            send()
//
//            setResultText()
//
//        }
//
//        messageText.text = null
//            /*output = DataOutputStream(socket.getOutputStream())
//            output.write(authorizationToken.toByteArray())*/
////        val intent = Intent(this, CommandActivity::class.java)
////
////        startActivity(intent)
//
//    }

//    suspend fun send(){
//
//        if (messageText.text.toString() != "") {
//            message += messageText.text.toString() + "\n"
//        }else if (message != "") {
//            message += "\n"
//            output = DataOutputStream(SocketHandler.getOutputStream())
//            output.write(message.toByteArray())
//
//            message = ""
//
//            input = DataInputStream(SocketHandler.getInputStream())
//            val reader = BufferedReader(input.reader())
//            var c: Int
//            var nl: Boolean = false
//            val stringBuffer = StringBuilder()
//            answer = ""
//
//            c = reader.read()
//
//            loop@ while (c != -1) {
//
//                if (c.toChar() == '\n') {
//                    if(nl){
//                        break@loop
//                    }else{
//                        nl = true
//                    }
//                }
//                else {
//                    nl = false
//                }
//                stringBuffer.append(c.toChar())
//
//                c = reader.read()
//
//            }
//
//
//            Log.i("fin", "fin")
//
//            answer = stringBuffer.toString()
//
//
//        }
////        Log.i("message", message)
////        Log.i("answer", answer)
//
//    }
//
//    suspend fun setResultText(){
//        withContext(Main){
//            resultTextView.text = answer
//        }
//    }

    /*private suspend fun client( ip: String, port: Int) {

        val connection = Socket(ip, port)
        val reader = Scanner(connection.getInputStream())


        while(active)
        {
            var input: String = ""
            input = reader.nextLine()

            Log.i("yeet", input)

        }
        reader.close()
        connection.close()
    }


    fun connect(view: View) {

        val address = ipAddressEditText.text.toString()
        val port = Integer.parseInt(portEditText.text.toString())

        if(!active){
            connectButton.text ="disconnect"
            active = true
            CoroutineScope(IO).launch{
                client( address, port )
            }

            resultTextView.text = data

        }else
        {
            active = false
            connectButton.text = "connect"

        }
    }*/




    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        ipAddressEditText = findViewById(R.id.addressEditText)
        portEditText = findViewById(R.id.portEditText)
        connectButton = findViewById(R.id.connectButton)
        messageText = findViewById(R.id.messageText)
        submitButton = findViewById(R.id.submitButton)
        resultTextView = findViewById(R.id.resultTextView)


    }
}
