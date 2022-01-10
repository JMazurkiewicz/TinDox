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




    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        ipAddressEditText = findViewById(R.id.addressEditText)
        portEditText = findViewById(R.id.portEditText)
        connectButton = findViewById(R.id.connectButton)


    }
}
