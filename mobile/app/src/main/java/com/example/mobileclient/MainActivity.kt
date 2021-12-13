package com.example.mobileclient

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import android.widget.Toast
import kotlinx.coroutines.*
import kotlinx.coroutines.Dispatchers.IO
import java.io.BufferedReader
import java.io.InputStreamReader
import java.net.InetAddress
import java.net.Socket
import java.nio.ByteBuffer
import java.util.*

class MainActivity : AppCompatActivity() {
    private lateinit var ipAddressEditText: EditText
    private lateinit var portEditText: EditText
    private lateinit var submitButton: Button
    private lateinit var resultTextView: TextView

    private var active: Boolean = false
    private var data: String = ""

    private suspend fun client( ip: String, port: Int) {

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


    fun submit(view: View) {


        val address = ipAddressEditText.text.toString()
        val port = Integer.parseInt(portEditText.text.toString())

        if(!active){
            submitButton.text ="disconnect"
            active = true
            CoroutineScope(IO).launch{
                client( address, port )
            }

            resultTextView.text = data

        }else
        {
            active = false
            submitButton.text = "connect"

        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        ipAddressEditText = findViewById(R.id.addressEditText)
        portEditText = findViewById(R.id.portEditText)
        submitButton = findViewById(R.id.submitButton)
        resultTextView = findViewById(R.id.resultTextView)

    }
}
