Server:
	config:
		rep-port = 4242 [port utilisé pour recevoir les commandes des clients et répondre]
		pub-port = 4243 [port utilisé pour envoyer les notifications aux clients]
		cycle [nombre en microsecondes correspondant à un cycle (> 0)]
		log [fichier de log]
		size [taille de la map, >= 5]
		v (active le mode verbose (loglevel INFO))@
