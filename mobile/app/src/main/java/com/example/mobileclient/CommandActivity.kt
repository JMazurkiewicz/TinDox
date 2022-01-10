package com.example.mobileclient

import android.os.Bundle
import android.util.Log
import android.view.KeyEvent
import android.view.View
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import kotlinx.coroutines.Dispatchers.IO
import kotlinx.coroutines.Dispatchers.Main
import java.io.BufferedReader
import java.io.DataInputStream
import java.io.DataOutputStream
import android.view.inputmethod.EditorInfo

import android.widget.TextView.OnEditorActionListener
import kotlinx.coroutines.*


class CommandActivity: AppCompatActivity() {

    private lateinit var submitButton: Button
    private lateinit var commandText: EditText
    private lateinit var commandTextView: TextView
    private lateinit var responseTextView: TextView



    private var message: String = ""
    private var answer: String = ""
    private lateinit var output: DataOutputStream
    private lateinit var input: DataInputStream

    fun submit(view: View) {
        CoroutineScope(IO).launch {
            send()
            setResponseText()
            setCommandText()
        }

        commandText.text = null
    }

    private fun send(){

        if (commandText.text.toString() != "") {

            message += commandText.text.toString() + "\n"
            Log.i("message", message)

        }else if (message != "") {
            message += "\n"
            output = DataOutputStream(SocketHandler.getOutputStream())
            output.write(message.toByteArray())

            message = ""

            input = DataInputStream(SocketHandler.getInputStream())
            val reader = BufferedReader(input.reader())
            var c: Int
            var nl: Boolean = false
            val stringBuffer = StringBuilder()
            answer = ""

            c = reader.read()

            loop@ while (c != -1) {

                if (c.toChar() == '\n') {
                    if(nl){
                        break@loop
                    }else{
                        nl = true
                    }
                }
                else {
                    nl = false
                }
                stringBuffer.append(c.toChar())

                c = reader.read()

            }

            answer = stringBuffer.toString()


        }

    }

    suspend fun setResponseText(){
        withContext(Dispatchers.Main){
            responseTextView.text = answer
        }
    }

    suspend fun setCommandText(){
        withContext(Dispatchers.Main){
            commandTextView.text = message
        }
    }


    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_command)

        submitButton = findViewById(R.id.submitButton)
        commandText = findViewById(R.id.commandText)
        commandTextView = findViewById(R.id.responseTextView)
        responseTextView = findViewById(R.id.commandTextView)

//        commandText.setOnEditorActionListener(OnEditorActionListener { _, actionId, event ->
//            if (event != null && event.keyCode === KeyEvent.KEYCODE_ENTER || actionId == EditorInfo.IME_ACTION_DONE) {
//                submit(submitButton)
//            }
//            false
//        })

    }
}
