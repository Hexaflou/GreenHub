/*
 * SendDataDemoHostApplication.java
 * Modified by Romaric Drigon as 11/01/2012
 * Based on SendDataDemo by Syn Microsystems, Inc. original Copyright retained
 *
 * Original Copyright : Copyright (c) 2008-2009 Sun Microsystems, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

package org.sunspotworld.server;

import com.sun.spot.io.j2me.radiogram.*;

import com.sun.spot.peripheral.ota.OTACommandServer;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.text.DateFormat;
import java.util.Date;
import java.util.Arrays;
import javax.microedition.io.*;


/**
 * Application serveur
 * Ecoute ce que lui envoie les clients SunSpot via ondes radio,
 * et les retransmets sur le réseau (via un socket)
 */
public class HostApplication {
    // Port sur lequel on écoutera via Broadcast
    private static final int HOST_PORT = 67;
        
    private void run() throws Exception {
        // Il nous faut tout déclarer ici (que les variables soient accessibles)
        RadiogramConnection rCon;
        Datagram dg;
        DateFormat fmt = DateFormat.getTimeInstance();
        DatagramSocket serverSocket;
        InetAddress IPAddress = InetAddress.getByName("localhost");
        int portServer = 1337;
         
        try {
            // Ouvre une connexion serveur, pour recevoir les données envoyées par les SunSPOTS
            rCon = (RadiogramConnection) Connector.open("radiogram://:" + HOST_PORT);
            dg = rCon.newDatagram(rCon.getMaximumLength());
        } catch (Exception e) {
             System.err.println("Erreur lors de la création du serveur BaseStation SunSPOT : " + e.getMessage());
             throw e; // Arrête le programme
        }
        
        try {
            // Ouvre un socket Unix sur lequel on va renvoyer nos messages, port 1337
            serverSocket = new DatagramSocket();
        } catch (Exception e) {
             System.err.println("Erreur lors de la création du socket : " + e.getMessage());
             throw e; // Arrête le programme
        }

        // Boucle infinie d'écoute
        while (true) {
            try {
                // On reçoit les données
                rCon.receive(dg);
                
                String address = dg.getAddress();       // lecture de l'Id de l'émetteur
                long time = dg.readLong();              // lecture de la date/heure
                int brightness = dg.readInt();          // lecture de la luminosité
                double temperature = dg.readDouble();   // lecture de la température
                
                System.out.println(fmt.format(new Date(time)) + " from: " + address + " brightness: " + brightness + " temperature: " + temperature);
                
                // On va renvoyer les données sur un socket.
                // On commence par construire le message
                         
                // Données
                String contentData = address + ";" + String.valueOf(brightness)+ ";" + String.valueOf(temperature);
                                
                // On construit le message, rajoute l'entête
                    // 4 premiers octets : A55A
                    // ;
                    // 2 pour le type (réservés, 5, 6, 7) - on choisir abitrairement 3, met un zéro devant
                    // ;
                    // contenu
                String messageData = "A55A\0;03\0;" + contentData;
                
                // Maintenant on renvoie tout ceci sur notre socket Unix
                DatagramPacket sendPacket = new DatagramPacket(messageData.getBytes(), messageData.getBytes().length, IPAddress, portServer);
                serverSocket.send(sendPacket);
            } catch (Exception e) {
                System.err.println("Erreur lors de la lecture des capteurs : " + e);
                throw e; // Arrête le programme
            }
        }
    }
    
    /*
     * Lance l'appli
     *
     * @param args : n'importe quoi
     */
    public static void main(String[] args) throws Exception {
        // enregistre le nom de l'application auprès du "OTA Command server" & le lance OTA
        OTACommandServer.start("Server");

        HostApplication app = new HostApplication();
        
        app.run();
    }
}
