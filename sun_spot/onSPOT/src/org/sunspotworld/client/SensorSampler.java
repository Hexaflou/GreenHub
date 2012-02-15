/*
 * SensorSampler.java
 * Modified by Romaric Drigon as 11/01/2012
 * Based on SendDataDemo by Syn Microsystems, Inc. original Copyright retained
 *
 * Original Copyright : Copyright (c) 2008-2010 Sun Microsystems, Inc.
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

package org.sunspotworld.client;

import com.sun.spot.io.j2me.radiogram.*;
import com.sun.spot.resources.Resources;
import com.sun.spot.resources.transducers.ITriColorLED;
import com.sun.spot.resources.transducers.ILightSensor;
import com.sun.spot.resources.transducers.ITemperatureInput;
import com.sun.spot.util.Utils;
import javax.microedition.io.*;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;
import java.lang.Math;

/*
 * Application tournant sur les SunSPOTs (capteurs).
 * Retourne toutes les 60 secondes la luminosité ambiante et la température.
 * 
 * On choisit 60 sec., la sonde de température étant sur la carte mère sinon 
 *  elle est trop influencée par la chauffe du microprocesseur
 */
public class SensorSampler extends MIDlet {

    private static final int HOST_PORT = 67;
    private static final int SAMPLE_PERIOD = 60 * 1000; // 60 secondes, que l'on passe en millisecondes
    
    protected void startApp() throws MIDletStateChangeException {
        RadiogramConnection rCon = null;
        Datagram dg = null;
        String ourAddress = System.getProperty("IEEE_ADDRESS");

        // On accède aux ressources matérielles (capteur de luminosité, de température, LED)
        ILightSensor lightSensor = (ILightSensor)Resources.lookup(ILightSensor.class);
        ITemperatureInput tempSensor = (ITemperatureInput) Resources.lookup(ITemperatureInput.class);
        ITriColorLED led = (ITriColorLED)Resources.lookup(ITriColorLED.class, "LED7");
        
        System.out.println("Starting new sensor sampler (brightness and temperature) application on " + ourAddress + " ...");

	// On écoute les éventuelles commandes qu'on pourrait recevoir par USB
        // (si on se connecte directement par câble à l'ordi)
	new com.sun.spot.service.BootloaderListenerService().getInstance().start();

        try {
            // On ouvre une connexion "broadcast" sur le port précédement défini
            rCon = (RadiogramConnection) Connector.open("radiogram://broadcast:" + HOST_PORT);
            dg = rCon.newDatagram(50);  // création du radiogramme
        } catch (Exception e) {
            System.err.println("Caught " + e + " in connection initialization.");
            notifyDestroyed();
        }
        
        while (true) {
            try {
                // On récupère les valeurs actuelles des captures
                long now = System.currentTimeMillis();
                int brightness = lightSensor.getAverageValue(); // on prend bien une valeur moyenne
                double temperature = tempSensor.getCelsius(); // rajouté : température
                
                // petite manipulation sur la température : on a un int, directement valeur en °C, de -40 à +125
                // on le passe en un int de 0 à 165
                int convertedTemperature = ((int)Math.floor(temperature))+40;
                
                // On fait clignoter un petit coup la LED
                led.setRGB(255, 255, 255); // en blanc
                led.setOn();
                Utils.sleep(50); // 50 ms
                led.setOff();

                // On écrit toutes nos données dans le radio datagram
                dg.reset();
                dg.writeLong(now);
                dg.writeInt(brightness);
                dg.writeDouble(temperature);
                // et l'envoie
                rCon.send(dg);

                System.out.println("Time: " + now + " - Brightness: " + brightness +" - Temperature: " + temperature);
                
                // On va en veille jusqu'au prochain relevé dans 60 secondes
                Utils.sleep(SAMPLE_PERIOD - (System.currentTimeMillis() - now));
            } catch (Exception e) {
                System.err.println("Caught " + e + " while collecting/sending sensor sample.");
            }
        }
    }
    
    protected void pauseApp() {
        // On fait le squelette de la fonction, mais elle ne sera jamais appelée par  la VM Squawk
    }
    
    protected void destroyApp(boolean arg0) throws MIDletStateChangeException {
        // Sera appelé si startApp lance une exception autre que MIDletStateChangeException
    }
}
