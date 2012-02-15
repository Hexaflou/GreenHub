/*
 * Mini-appli pour tester la partie réseau de l'appli Server des SunSPOTs
 */

package mocktesteur;

import java.lang.Thread;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.text.DateFormat;
import java.util.Date;

public class MockTesteur {
  public static void main(String args[]) throws Exception {
        // Il nous faut tout déclarer ici (que les variables soient accessibles)
        DateFormat fmt = DateFormat.getTimeInstance();
        DatagramSocket serverSocket;
        InetAddress IPAddress = InetAddress.getByName("localhost");
        int portServer = 1337;
        
        try {
            // Ouvre un socket Unix sur lequel on va renvoyer nos messages, port 1337
            serverSocket = new DatagramSocket();
        } catch (Exception e) {
             System.err.println("Erreur lors de la création du socket Unix : " + e.getMessage());
             
             throw e; // Arrête le programme
        }

        // Boucle infinie
        // On envoie des données de test à une fréquence un peu plus rapide, toutes les 5 secondes
        while (true) {
            try {               
                String address = "0014.4F01.0000.1337";     // Id toujours du même capteur
                String time = "1328693680881";              // heure aléatoire
                int brightness = (int) (Math.random()*255); // on "génère" une luminosité
                double temperature = Math.random()*165;      // température aléatoire, entre 0 et 165
                
                // on ré-arrondit la temprétaure à un seul chiffre après la virgule
                temperature = (Math.round(temperature*10))/10;
                
                System.out.println("At " + time + " from: " + address + " brightness: " + brightness + " temperature: " + temperature);
                
                // On va renvoyer les données sur un socket.
                // on commence par préparer le message
                         
                // Données
                String contentData = address + ";" + time + ";" + String.valueOf(brightness)+ ";" + String.valueOf(temperature);
                                
                // On construit le message, rajoute l'entête
                    // 4 premiers octets : A55A
                    // ;
                    // 2 pour le type (réservés, 5, 6, 7) - on choisir abitrairement 3, met un zéro devant
                    // ;
                    // contenu
                String messageData = "A55A;03;" + contentData;
                
                // Maintenant on renvoie tout ceci vers notre socket
                DatagramPacket sendPacket = new DatagramPacket(messageData.getBytes(), messageData.getBytes().length, IPAddress, portServer);
                serverSocket.send(sendPacket);
                
                Thread.sleep(5000); // Pause de 5 secondes
            } catch (Exception e) {
                System.err.println("Erreur lors de la lecture des capteurs : " + e);
                
                throw e;
            }
        }
  }
}