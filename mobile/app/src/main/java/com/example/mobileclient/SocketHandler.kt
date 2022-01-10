package com.example.mobileclient

import java.io.InputStream
import java.io.OutputStream
import java.net.Socket
import java.net.SocketException


object SocketHandler {

    private lateinit var nSocket: Socket

    @Synchronized
    fun setSocket(ip: String, port: Int) {
        try {
            nSocket = Socket(ip, port)
        }catch(e: SocketException){

        }
    }

    @Synchronized
    fun getSocket(): Socket {
        return nSocket
    }

    @Synchronized
    fun getOutputStream(): OutputStream{
        return nSocket.getOutputStream()
    }

    @Synchronized
    fun getInputStream(): InputStream {
        return nSocket.getInputStream()
    }

    @Synchronized
    fun connected(): Boolean{
        return nSocket.isConnected()
    }
}
