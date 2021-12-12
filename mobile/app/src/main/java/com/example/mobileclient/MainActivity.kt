package com.example.mobileclient

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import android.widget.Toast

class MainActivity : AppCompatActivity() {
    private lateinit var ipAddressEditText: EditText
    private lateinit var portEditText: EditText
    private lateinit var submitButton: Button
    private lateinit var resultTextView: TextView

    fun submit(view: View) {
        Toast.makeText(this, ipAddressEditText.text, Toast.LENGTH_SHORT).show()
        val message = "message " + ipAddressEditText.text + " " + portEditText.text
        Log.i("debug", ipAddressEditText.text.toString())
        resultTextView.text = message
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