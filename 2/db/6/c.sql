UPDATE kunde SET adresse='Unter den Eichen 5, 65195 Wiesbaden' WHERE p_nr=(SELECT p_nr FROM person WHERE vorname='Jan' AND name='Kramer')
