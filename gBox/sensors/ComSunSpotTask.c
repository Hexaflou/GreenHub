#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include "ComSunSpotTask.h"
#include "ComponentInterface.h"
#include "../gCommunication/comIncludes.h"


/***************************PRIVATE DECLARATION***********************/
static void *ListenSunSpot(void *ptr);

static pthread_t comSunSpotThread;
static SOCKET sock = 0;
static char * message;

/************************PUBLIC***************************************/

/* Initialisation de la tâche */
int comSunSpotTaskInit() {
	pthread_create(&comSunSpotThread, NULL, &ListenSunSpot, NULL);
	return 0;
}

/* Destruction de la tâche */
int comSunSpotTaskClose() {
	/* close task */
	int ret;
	ret = pthread_cancel(comSunSpotThread);
	if (sock != 0)
		close(sock);
	if (message != NULL)
		free(message);
	return (ret);
}

/************************PRIVATE***************************************/

/*
 * Fonction permettant l'écoute de périphérique SunSpot.
 * Si un composant SunSpot non enregistré communique, il sera
 * automatiquement enregistré dans notre liste de capteurs.
 */
void *ListenSunSpot(void *message1) {
	char buffer[47];
	long n;
	struct sockaddr_in serverAddr;
	char * fragmentString;
	socklen_t serverAddrLen = sizeof (serverAddr);

	/* Variable pour la trame a gerer, seront utilises bien plus tard */
	char* idCapteur;
	/* char* dateTime; */ /* (inutilisée) */
	int temperature;
	char hexTemperature[3];
	int brightness;
	float f_brightness, f_temperature;
	char hexBrightness[3];
	char frame[23];


	/* Déclaration des infos réseau */
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* on assume que le serveur Java tournera sur la même machine */
	serverAddr.sin_port = htons(1337);

	/* Création du socket en UDP*/
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == ERROR) {
		perror("[ListenSunSpot] Erreur dans la creation du socket UDP SunSPOT ");
		return (int*) ERROR;
	}

	#if DEBUG > 0
	printf("[ListenSunSpot] Connection avec le serveur...\n");
	#endif

	/* Pas de connect à faire, il faut juste se binder */
	if (bind(sock, (struct sockaddr*) & serverAddr, serverAddrLen) == SOCKET_ERROR) {
		perror("[ListenSunSpot] Erreur dans la connection avec le serveur ");
		return (int*) SOCKET_ERROR;
	}

	#if DEBUG > 0
	printf("[ListenSunSpot] Connection avec le serveur OK\n");
	#endif

	while (1) {
		#if DEBUG > 0
		printf("[ListenSunSpot] Attente d'un début de message...\n");
		#endif

		/* on reçoit le message en une seule fois */
		if ((n = recvfrom(sock, buffer, sizeof (buffer) - 1, 0, (struct sockaddr*) & serverAddr, &serverAddrLen)) < 0) {
			perror("[ListenSunSpot] Erreur dans la réception ");

			break;
		}

		#if DEBUG > 0
		printf("[ListenSunSpot] Message d'un capteur reçu..\n");
		#endif

		/*
		 *        On va maintenant parser le message
		 *        Il a le même format que celui des capteurs EnOcean
		 */

		/* message = (char*) buffer; */ /* on mets ça dans un char*, passe à une chaîne de charactères */
		/*strncpy(message, buffer, 30);
		 *
		 * printf(message);*/

		printf("MESSAGE : %s\n",buffer);

		fragmentString = strtok(buffer, ";");
		printf("fragmentString avant A55A : %s\n",fragmentString);

			/* on regarde l'entête, vérifie que c'est bien "A55A" */
			if (strcmp(fragmentString, "A55A") != 0) {
				printf("[ListenSunSpot] Mauvais header.\n");
			}

			fragmentString = strtok(NULL, ";");
			printf("fragmentString apres A55A : %s\n",fragmentString);

				/* on vérifie maintenant si on est bien sur un vrai capteur SunSpot : type "03" */
				if (strcmp(fragmentString, "03") != 0) {
					printf("[ListenSunSpot] Bon type de capteur.\n");
				}

				/*
				 *        Maintenant on a, séparés par des ; :
				 *        adresse du capteur (id)
				 *        date/time
				 *        luminosité
				 *        température
				 *
				 *        On récupère le restant du message, pour construire une "pseudo-trame",
				 *        similaire à ce que les capteurs EnOcean envoient
				 */

				/* id du capteur :
				 *        forme 0014.4F01.0000.5620,
				 *        les 15 premiers charactères sont toujours les mêmes,
				 *        du coup on raccourcit sans problèmes pour ne garder que les 4 derniers
				 */

				idCapteur = str_sub(strtok(NULL, ";"), 15, 18);
				printf("idCapteur : %s\n",idCapteur);

					/* date et heure de la mesure : info pas utilisée pour l'instant */
					/* dateTime = strtok(NULL, ";"); */

					/* luminosité */
					brightness = atoi(strtok(NULL, ";")); /* on récupère déjà la valeur dans un int */
					printf("brightness : %i\n",brightness);

					/* il faut qu'on applique un coefficient de nouveau :
					 *        la valeur envoyée par le capteur est entre 0 et 750 (cf datasheet)
					 *        il faut que cela tienne dans un int entre 0 et 255
					 *
					 *        détail du calcul :
					 *           (750-0)/(255-0)
					 */

					f_brightness = (float)brightness / 2.94117647;				
					sprintf(hexBrightness, "%02X", (int) f_brightness);
					
					printf("hexBrightness : %s\n",hexBrightness);
					/* température - même fonctionnement */
					temperature = atoi(strtok(NULL, ";"));

					/* il faut qu'on applique un coefficient (diviseur)
					 *        détail du calcul :
					 *        (scaleMax-scaleMin)/(rangeMax/rangeMin)
					 *        on remplace avec les valeurs, issues de la datasheet pour la scale et le range est celui d'un int :
					 *        (165-0)/(255-0) = 0,647058824
					 */
					f_temperature = (float)temperature * 0.647058824;
					sprintf(hexTemperature, "%02X", (int) f_temperature);

					/*
					 *        On construit concrètement la pseudo trame maintenant :
					 *           FF : code org
					 *           00 : "trou"
					 *           LL : valeur de la luminosité
					 *           TT : valeur de la température
					 *           00 : "trou"
					 *           0000 : "faux" ID
					 *           IDID : ID réel du SPOT
					 *           0000 : "trou"
					 */
					strcpy(frame,"FF00");
					strcat(frame,hexBrightness);
					strcat(frame,hexTemperature);
					strcat(frame,"000000");
					strcat(frame,idCapteur);
					strcat(frame,"0000");

					free(idCapteur);
					#if DEBUG == 0
					printf("frame : %s\n",frame);
					#endif

					ManageMessage(frame);
	}

	return (void *) NULL;
}