/* Práctica Lab1: Interfaces paralelos
 * 
 * Descripción: Proyecto base con esquema, definiciones y programa demo
 * Programa demo: parpadeo de todos los segmentos de los dígitos extremos del display (D1-D4)
 * y enviar información a la consola o pantalla (Virtual Terminal)
 *
 * Fichero: 	24-25_plab1_base.pdsprj
 * Creado: 		04 septiembre 2024
 * Autor:			Carlos Mireles Rodríguez
*/


// Definición para pulsadores
// PORTC: PC[7:0] --> PINES 30-31-32-33-34-35-36-37
#define PRIGHT  30    //	PC[7] pulsador right 
#define PDOWN   31    //	PC[6]		"" 		down
#define PLEFT   32    //	PC[5]		"" 		left
#define PENTER 	33    //	PC[4]		"" 		entrar
#define PUP     34    //	PC[3]		"" 		up
#define SPEAKER 37    // 	PC[0]					speaker 


// Definiciones para teclado/cátodos de display (para el manejo de los 4 dígitos del display de forma entrelazada)
// PORTL: PL[7:0] --> PINES 42-43-44-45-46-47-48-49
//														 R1-R2-R3-R4-D1-D2-D3-D4 --> FILAS_TECLADO (R) | CÁTODOS DISPLAY (D)

#define D4    0xFE    // 1111 1110 visualiza 	unidades y pull-up para las filas del teclado
#define D3    0xFD    // 1111 1101 		"				decenas y pull-up para las filas del teclado
#define D2    0xFB    // 1111 1011 		"				centenas y pull-up para las filas del teclado
#define D1    0xF7    // 1111 0111 		"				millares y pull-up para las filas del teclado
#define DOFF  0xFF    // 1111 1111 todos los dígitos apagados y pull-up para las filas del teclado
#define DON   0xF0    // 1111 0000 todos los dígitos activos (cátodos comunes a "0") y pull-up para las filas del teclado

/*
//Otra forma de definición de señales de cátodos para manejo de forma individual 
#define D4 49   				// El pin 49 controla el cátodo común del dígito de las unidades (D4)
#define D3 48   				// El pin 48 controla el cátodo común del dígito de las decenas (D3)
#define D2 47   				// El pin 47 controla el cátodo común del dígito de las centenas (D2)
#define D1 46   				// El pin 46 controla el cátodo común del dígito de las unidades de millar (D1)
*/

// Mapeo matricial para las teclas del teclado
char teclado_map[][3] = { {'1','2','3'},
                        {'4','5','6'},
                        {'7','8','9'},
                        {'*','0','#'}};

//  Array que contiene los números del 0-9 para mostrar en el display			
int num[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x58, 0x5E, 0x79, 0x71};


// Variables para el manejo de las teclas central, derecha e izquierda.
long	time_old_enter = 0L;
long	time_old_up = 0L;
long	time_old_down = 0L;	
long	time_old_right = 0L;
long	time_old_left = 0L;	


long	time_now_enter = 0L;
long	time_now_up = 0L;
long	time_now_down = 0L;	
long	time_now_right = 0L;
long	time_now_left = 0L;	

int contador = 000;
int contador_max = 999;
int incremento = 1;

int modo_old = 1;
int modo = 1;
int digit = 0;

String NumTec;				// String que almacena los numeros introducidos por teclado


void setup() {
  
	// Inicialización del canal serie para comunicarnos a traves de una consola (ventana) en la pantalla del PC
	Serial.begin(9600);		// inicialización del canal serie para comunicarnos con:
												// Arduino IDE --> "Herramientas/monitor serie" en la plataforma Arduino
												// Proteus --> consola en Proteus (VIRTUAL TERMINAL)
	
	//Programación del PORTA de salida. Conexión a los segmentos a-b-c-d-e-f-g-dp del display 7-segmentos)
  DDRA = 0xFF;    					// También se puede expresar en binario con B11111111 (DDRA = B1111111;)
	
	// Escritura en el PORTA --> Todos los segmentos a "1": encendidos
  PORTA = num[0];  					// o también B11111111
	
	// Programación del PORTL: filas_teclado(Rn:input) | cátodos display (Dn:output) --> R1-R2-R3-R4-D1-D2-D3-D4
	DDRL = 0x0F;		// PL[7:4] entradas (filas del teclado: R1-R2-R3-R4)
									// PL[3:0] salidas (cátodos comunes display/columnas del teclado: D1-D2/C3-D3/C2-D4/C1
									
	// Escritura en el PORTL								
	PORTL = 0xFF; 	// activación resistencias de pull-up en las entradas de las filas de teclado PL[7:4]; display off


	// Programación del PORTC
	DDRC = B00000001; // 0x01:Todos los bits de entrada menos PC0 que es de salida (speaker)
	
	// Escritura en el PORTC
	PORTC = B11111110;	// 0xFE: activación de todas las resistencias de pull-up excepto la de PC0
	
	// otra forma de declarar un pin de entrada o salida
	
	// declarar el pin 13 (led del sistema) de salida
	pinMode(13, OUTPUT);	// pin 13 (PB7) de salida. OUTPUT = 1; INPUT = 0
	// también...
	pinMode(13,1);
		
		Serial.println("-----------------------------------------");
		Serial.println(" 1. - Modo normal de visualización (tres dígitos ): OFF-centenas-decenas-unidades");
		Serial.println(" 2. - Modo reducido-inferior de visualización (dos dígitos): OFF-OFF-decenas-unidades");
		Serial.println(" 3. - Modo reducido-superior de visualización (dos dígitos): decenas-unidades-OFF-OFF");
		Serial.println("-----------------------------------------");
		cli();
		EICRA |= (1<<ISC31) | (1<<ISC30);
		EIMSK |= (1<<INT3);
		sei();
	}

void loop() {
		bottonUp();
		bottonDown();
		bottonEnter();
		bottonRight();
		bottonLeft();
		comprobarOpcionIntroducida();
}


void comprobarTeclado(int digit){
		if (digitalRead(42) == 0){
			while (digitalRead(42) == 0){}    
			NumTec += teclado_map[0][digit];
		}
    
		if (digitalRead(43) == 0){
			while (digitalRead(43) == 0){}    
			NumTec += teclado_map[1][digit];
		}    

		if (digitalRead(44) == 0){
			while (digitalRead(44) == 0){}    
			NumTec += teclado_map[2][digit];
		}    

		if (digitalRead(45) == 0 && digit ==1){
			while (digitalRead(45) == 0){}
			NumTec += teclado_map[3][digit];
		}

		if (digitalRead(45) == 0 && digit == 2){
            while (digitalRead(45) == 0){}
            contador = NumTec.toInt();
            Serial.print("Contador = ");
			Serial.println(contador);
            NumTec = "";
		}
}


void comprobarOpcionIntroducida(){
		if (Serial.available()>0){
			int temp = 0;
			temp=Serial.read();		// Lee la consola	
			temp=temp-0x30;		// Le resta 0x30 para obtenerlo en entero
			if (temp > 0 && temp < 4){
				modo_old=modo;	
				modo = temp;
				Serial.println("-----------------------");
				Serial.print("Se ha seleccionado la opción: ");
				Serial.println(modo);
			}
		}
}


void bottonUp(){
		if (digitalRead(PUP) == 0){
				time_now_up= millis();
				if ((time_now_up - time_old_up) > 300){contador = contador + incremento; time_old_up = time_now_up;}
				if (contador > contador_max) contador = 0;
				 tone(SPEAKER,1000,100);
		}
}


void bottonDown(){
		if (digitalRead(PDOWN) == 0){
				time_now_down= millis();
				if ((time_now_down - time_old_down) > 300){contador = contador - incremento; time_old_down = time_now_down;}
				if (contador < 0) {contador = (contador_max - (incremento - 1));}
				tone(SPEAKER,1000,100);
		}
}


void bottonEnter(){
		if (digitalRead(PENTER) == 0){
				time_now_enter= millis();
				if ((time_now_enter - time_old_enter) > 300){contador = 0; time_old_enter = time_now_enter;}
				tone(SPEAKER,1000,100);
		}
}


void bottonRight(){
		if (digitalRead(PRIGHT) == 0){
				time_now_right= millis();
				if ((time_now_right - time_old_right) > 300){incremento = 2; time_old_right = time_now_right;}
		}
}


void bottonLeft(){
		if (digitalRead(PLEFT) == 0){
				time_now_left= millis();
				if ((time_now_left - time_old_left) > 300){incremento = 1; time_old_left = time_now_left;}
		}
}


ISR(INT3_vect){
		PORTL = DOFF;
		if (modo == 1){
			switch(digit){
				case 0:
					PORTA = num[contador % 10];
					PORTL = D4;
					comprobarTeclado(digit);
					digit++;
					break;
				case 1:
					PORTA = num[int(contador/10) % 10];
					PORTL = D3;
					comprobarTeclado(digit);
					digit++;
					break;
				case 2:
					PORTA = num[int(contador/100) % 10];
					PORTL = D2;
					comprobarTeclado(digit);
					digit = 0;
					break;
			}
		}
		else if (modo == 2){
			switch(digit){
				case 0:
					PORTA = num[contador % 10];
					PORTL = D4;
					comprobarTeclado(digit);
					digit++;
					break;
				case 1:
					PORTA = num[int(contador/10) % 10];
					PORTL = D3;
					comprobarTeclado(digit);
					digit = 0;
					break;
			}
		}
		else if (modo == 3){
				switch(digit){
				case 0:
					PORTA = num[contador % 10];
					PORTL = D2;
					comprobarTeclado(digit);
					digit++;
					break;
				case 1:
					PORTA = num[int(contador/10) % 10];
					PORTL = D1;
					comprobarTeclado(digit);
					digit = 0;
					break;
				}
		}
}