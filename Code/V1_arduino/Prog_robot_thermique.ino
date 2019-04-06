#include <TimerOne.h>
#include <time.h>
#include <Wire.h>

// asservisseemnt vitesse
// r�glages :   kPM1 et kPM2
// et intervalAsserv


#define DEBUG 0  // commenter pour une machine de prod
#define RS232 Serial1

int val = 0;

const int ledPin = 13;  // Teensy 3.0 has the LED on pin 13
int dObstacle = 10;

// sorties  de controle des moteurs
const int M2Enable = 23;
const int M1Enable = 22;
const int M2Out1 = 16;
const int M2Out0 = 17;
const int M1Out1 = 14;
const int M1Out0 = 15;
// entr�e capteur
const int M1In1 = 5;
const int M1In0 = 6;
const int M2In1 = 8;  // sens inverse de M1
const int M2In0 = 7;


// interruption sur M1In0
//variables globales
volatile long int posM1 = 0;   // volatile pour modif dans l'interruption  multi processus
volatile long int posM2 = 0;   // position sur 32 bits
volatile unsigned long int timeout=0;   // au cas o�
volatile long int posM1Anc = 0;   // volatile pour modif dans l'interruption  multi processus
volatile long int posM2Anc = 0;   // position pr�c�dente pour le calcul de la vitesse
volatile float vM1 = 0;   // vitesse en pas par seconde
volatile float vM2 = 0;
int intervalAsserv = 20000;  // 50000�s entre chaque interruption du timer 50ms
float intervalAsservSec = (float)intervalAsserv / 1.0E+6;
volatile float fAux1 = 0;  // variable de retour
volatile float fAux2 = 0;


// coeffiscient de correction de puissance
float kPM1 = 2.5;
float kPM2 = 2.5;

float consigneVitesseM1 = 0;  // vitesse demand�e
float consigneVitesseM2 = 0;
// liste des param�tres globaux
float paramVitesse = 1000;   //  

// fonction simple de calcul de la puissance en fonction de l'erreur sur la vitesse
int fonctionP( float v, float consigne, float k)
{
	if (fabs(consigne ) <10)
		return 0;  // c'est un arret

	float erreur = consigne - v;
	float puissance = k * erreur;
	fAux1 = erreur;
	fAux2 = puissance;

	if ( fabs(puissance) < 10)
		return 0;  // arret
	else if ( puissance < -255)
		return -255;
	else if ( puissance > 255)
		return 255;
	else
		return (int)puissance;
}
// fonctions d'asservissement
void regulV()
{
		
	MotorCC1( fonctionP( vM1, consigneVitesseM1, kPM1));  // calcul de puissance et application
	MotorCC2( fonctionP( vM2, consigneVitesseM2, kPM2));
}
// interruption du timer : calcul de la vitesse et asservissement
void ITimer1()
{
	if (timeout > 0)
		timeout--;
	// calcul de vitesse
	vM1 = (float)(posM1 - posM1Anc) / intervalAsservSec;
	vM2 = (float)(posM2 - posM2Anc) / intervalAsservSec;
	posM1Anc = posM1;
	posM2Anc = posM2;
	// appel fonctions d'asservissement
	regulV();

}

void MotorCC1( int iPuissance)
{
	if ( iPuissance>=0)
	{
		digitalWrite( M1Out0, HIGH);    // rive gauche du pont � Vc
		digitalWrite( M1Out1, LOW);    // rive droite du pont � 0
	}
	else
	{   // sens n�gatif
		iPuissance = -iPuissance;
		digitalWrite( M1Out0, LOW);    // rive gauche du pont � 0
		digitalWrite( M1Out1, HIGH);    // rive droite du pont � Vc
	}

	analogWrite( M1Enable, iPuissance);  // 
	
}
void MotorCC2( int iPuissance)
{
	if ( iPuissance>=0)
	{
		digitalWrite( M2Out0, HIGH);    // rive gauche du pont � Vc
		digitalWrite( M2Out1, LOW);    // rive droite du pont � 0
	}
	else
	{   // sens n�gatif
		iPuissance = -iPuissance;
		digitalWrite( M2Out0, LOW);    // rive gauche du pont � 0
		digitalWrite( M2Out1, HIGH);    // rive droite du pont � Vc
	}

	analogWrite( M2Enable, iPuissance);  

}

void posOrigine()
{
	posM1 = 0;
	posM2 = 0;
	
	digitalWrite(ledPin, LOW);    // no error
}


// initialisation arduino
void setup()
{
  pinMode(ledPin, INPUT);  
	
  // interruption
	pinMode( M1In1, INPUT); // inter
	pinMode( M1In0, INPUT); // quadrature
	// moteur 2
	pinMode( M2In1, INPUT); // inter
	pinMode( M2In0, INPUT); // quadrature

	// sortie
  pinMode( M1Enable, OUTPUT);  
  pinMode( M2Enable, OUTPUT);  
  pinMode( M1Out0, OUTPUT);  
  pinMode( M1Out1, OUTPUT);  
  pinMode( M2Out0, OUTPUT);  
  pinMode( M2Out1, OUTPUT);  

   // timer pour l'asservissement
  Timer1.initialize(intervalAsserv); // choix du delais
  Timer1.attachInterrupt( ITimer1);  // accroche de la routine d'interruption
  
  // liaisons s�ries

  RS232.begin(9600);

}

// boucle principale
void loop()
{
	// exemple d'utilisation avec la liaison S�rie via USB de la Teensy
  // module -> PC attente des connexion au serveur
  if ( RS232.available() > 0)
  {
	String sRet;
   sRet = RS232.readString( );
  //sRet = RS232.read();


  
  val = RS232.read();
  if (val=='1') 
    digitalWrite(ledPin, HIGH);
  if (val=='0')
    digitalWrite(ledPin,LOW);
    
	// attendre la r�ponse du module
	if (sRet.indexOf( "al")>=0)
		digitalWrite(ledPin, HIGH);    // set the LED on
	if (sRet.indexOf( "et")>=0)
		digitalWrite(ledPin, LOW);    // set the LED on

	if (sRet.indexOf( "av")>=0)
	{
		consigneVitesseM1 = paramVitesse;
		consigneVitesseM2 = paramVitesse;						
	}
	if (sRet.indexOf( "ar")>=0)
	{
		consigneVitesseM1 = -paramVitesse;
		consigneVitesseM2 = -paramVitesse;						
	}
	if (sRet.indexOf( "st")>=0)
	{
		consigneVitesseM1 = 0;
		consigneVitesseM2 = 0;						
	}
	if (sRet.indexOf( "dr")>=0)
	{
		consigneVitesseM1 = paramVitesse;
		consigneVitesseM2 = paramVitesse/2;						
	}
	if (sRet.indexOf( "ga")>=0)
	{
		consigneVitesseM1 = paramVitesse/2;
		consigneVitesseM2 = paramVitesse;						
	}		
	if (sRet.indexOf( "ad")>=0)
	{
		consigneVitesseM1 = -paramVitesse;
		consigneVitesseM2 = -paramVitesse/2;						
	}
	if (sRet.indexOf( "ag")>=0)
	{
		consigneVitesseM1 = -paramVitesse/2;
		consigneVitesseM2 = -paramVitesse;						
	}		

  }

}


