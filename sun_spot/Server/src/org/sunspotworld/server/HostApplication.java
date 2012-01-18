/*
 * SendDataDemoHostApplication.java
 * Modified by Romaric Drigon as 11/01/2012
 * Based on SendDataDemo by Syn Microsystems, Inc. original Copyright retained
 *
 * Copyright (c) 2008-2009 Sun Microsystems, Inc.
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
import java.text.DateFormat;
import java.util.Date;
import javax.microedition.io.*;


/**
 * Application serveur
 * Ecoute ce que lui envoie les clients SunSpot via radio,
 * le retransmets via réseau
 */
public class HostApplication {
    // Commentaire d'origine : Broadcast port on which we listen for sensor samples
    private static final int HOST_PORT = 67;
        
    private void run() throws Exception {
        RadiogramConnection rCon;
        Datagram dg;
        DateFormat fmt = DateFormat.getTimeInstance();
         
        try {
            // Commentaire d'origine : Open up a server-side broadcast radiogram connection
            // to listen for sensor readings being sent by different SPOTs
            rCon = (RadiogramConnection) Connector.open("radiogram://:" + HOST_PORT);
            dg = rCon.newDatagram(rCon.getMaximumLength());
        } catch (Exception e) {
             System.err.println("Erreur lors de la création du serveur SunSPOT BaseStation : " + e.getMessage());
             throw e; // arrête le programme
        }
        
        try {
            // Ouvre un socket Unix sur lequel on va renvoyer nos messages
        } catch (Exception e) {
             System.err.println("Erreur lors de la création du socket Unix : " + e.getMessage());
             throw e; // arrête le programme
        }

        // Commentaire d'origine : Main data collection loop
        while (true) {
            try {
                // Commentaire d'origine : Read sensor sample received over the radio
                rCon.receive(dg);
                
                String address = dg.getAddress();       // read sender's Id
                long time = dg.readLong();              // read time of the reading
                int brightness = dg.readInt();          // read the brightness sensor value
                double temperature = dg.readDouble();   // rajouté : température
                
                System.out.println(fmt.format(new Date(time)) + " from: " + address + " brightness: " + brightness + " temperature: " + temperature);
                
                // envoi une commande pour allumer la LED
                /*dg.writeUTF("LED");
                rCon.send(dg);*/
                
                // prépare le datagramme à envoyer
                // 4 premiers octets : A55A
                // 2 octets pour la taille
                // 2 pour le type (réservés, 5, 6, 7)
            } catch (Exception e) {
                System.err.println("Caught " + e +  " while reading sensor samples.");
                throw e;
            }
        }
    }
    
    /*
     * Lance l'appli
     *
     * @param args any command line arguments
     */
    public static void main(String[] args) throws Exception {
        // Commentaire d'origine : register the application's name with the OTA Command server & start OTA running
        OTACommandServer.start("Server");

        HostApplication app = new HostApplication();
        
        app.run();
    }
}
