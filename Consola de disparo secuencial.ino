/*******************************************************************************
 * Consola de disparo secuencial
 * Autor: José Francisco Lorenzo Hernández
 * Fecha de creación: 15/06/2013
 * Fecha de última edición: 25/10/2013
 *******************************************************************************/

/*******************************************************************************
 * Importación de librerías
 *******************************************************************************/
#include <LiquidCrystal.h>
#include <EEPROM.h>

/*******************************************************************************
 * Asignaciones a pines
 *******************************************************************************/
LiquidCrystal lcd(2,3,4,5,6,7);
int lineas[6] = {
  0,14,15,16,17,18};
int gnd = 15;
#define BOTON_IZQ 8 //Definir contantes o pines así
int boton_arriba = 9;
int boton_abajo = 10;
int boton_der = 11;
int linea = 1;
int boton_select=12;
int cursor_segundo = 12;

int cursor_minuto = 9;
int estadoBotonizq,estadoanteriorizq,estadoBotonarriba,
estadoanteriorarriba,estadoBotonabajo,estadoanteriorabajo,estadoBotonder,
estadoanteriorder,estadoBotonselect,estadoanteriorselect,k,
numero_pulsaciones_select,numero_pulsaciones_izquierdo,
numero_pulsaciones_derecho, lugar,segundo,minuto = 0;
int nivel = -1;
int espacio_countdown_segundos = 3;
long int tiempo,inicio;
int espera = 100;
int segundos = 14;
int milisegundos = 733;
unsigned long tiempoultimapulsacion;
int m,s,l;                               
int mu=0,md=0,su=0,sd=0,lu=0,ld=0,lc=0;
int direccion_linea=0;
int direccion_minuto=1;
int direccion_segundo=2;
int lineaROM,segundoROM,minutoROM;

byte i_tilde[8] = {
  B00010,
  B00100,
  B01100,
  B00100,
  B00100,
  B00100,
  B01110,
  B00000
};

byte flecha_izq[8] = {
  B00001,
  B00011,
  B00111,
  B01111,
  B00111,
  B00011,
  B00001,
  B00000
};
byte flecha_der[8] = {
  B10000,
  B11000,
  B11100,
  B11110,
  B11100,
  B11000,
  B10000,
  B00000
};
byte interrogacion_izq[8] = {
  B00100,
  B00000,
  B00100,
  B01000,
  B10000,
  B10001,
  B01110,
  B00000
};


void setup() 
{
  lcd.begin(16, 2);
  lcd.createChar(7, flecha_izq);
  lcd.createChar(8, flecha_der);
  lcd.createChar(9, i_tilde);
  lcd.createChar(10, interrogacion_izq);

  digitalWrite(gnd, LOW);
  pinMode(gnd, OUTPUT);
  for(int i=1;i<sizeof(lineas);i++)
  {
    pinMode(lineas[i], OUTPUT);
  }

  pinMode(BOTON_IZQ, INPUT);
  pinMode(boton_der, INPUT); 
  pinMode(boton_select, INPUT);
  pinMode(boton_abajo, INPUT); 
  pinMode(boton_arriba, INPUT);
  digitalWrite(boton_der,HIGH);
  digitalWrite(BOTON_IZQ,HIGH);
  digitalWrite(boton_select,HIGH);
  digitalWrite(boton_abajo,HIGH);
  digitalWrite(boton_arriba,HIGH);

  lcd.setCursor(0,0);
  lcd.print("Bienvenido");
  for (k=0;k<=16;k++){
    lcd.setCursor(k,1);
    lcd.write(255);
    delay(espera);
  }
  lcd.clear();
  MenuPrincipal();
}

void loop() 
{ 
  if (segundo == 0&&(nivel==3)){
    lcd.setCursor (11,1);
    lcd.print("0");
  }

  //Acciones botón izquierdo.
  estadoBotonizq = digitalRead(BOTON_IZQ);
  if (estadoBotonizq != estadoanteriorizq)
  {
    if(estadoBotonizq == LOW)
    {
      numero_pulsaciones_izquierdo++;
      if(numero_pulsaciones_izquierdo>3)
      {
        numero_pulsaciones_izquierdo = 0;
      }
    }
    if((estadoBotonizq == LOW&&(nivel==5)))
    {
      lcd.clear();
      lcd.print("Borrando...");
      numero_pulsaciones_select = 0;
      numero_pulsaciones_derecho = 0;
      nivel=0;
      for (int lugar = 0; lugar < 512; lugar++)
      {
        EEPROM.write(lugar, 0);
      }
      if (lugar == 512){
        lugar=0;
      }
      lcd.clear();
      menu();
    }
    if((estadoBotonizq == LOW)&&(nivel==1)){
      lcd.clear();
      secuencia();
    }
    if(estadoBotonizq == LOW&&(linea>1)&&(nivel==2)){
      linea--;
      lcd.setCursor(8,0);
      lcd.print(linea);
    }
    if(estadoBotonizq == LOW&&(segundo>0)&&(nivel==3)){
      if (segundo==10){
        cursor_segundo = 12;
        lcd.setCursor(11,1);
        lcd.print("0");
      }
      segundo--;
      lcd.setCursor(cursor_segundo,1);
      lcd.print(segundo);
    }
    if(estadoBotonizq == LOW&&(segundo==-1)&&(nivel==3)&&(minuto>=1)){
      minuto--;
      lcd.setCursor(cursor_minuto,1);
      lcd.print(minuto);
      cursor_segundo = 11;
      lcd.setCursor(cursor_segundo,1);
      segundo = 59;   
      lcd.print(segundo);
      lcd.write(8);
    }
    estadoanteriorizq = estadoBotonizq;
  }
  if((estadoBotonizq == LOW)&&(nivel==-1)&&
    (numero_pulsaciones_izquierdo==1)){
    lcd.clear();
    menu();
  }
  if((estadoBotonizq == LOW)&&(nivel==0)&&
    (numero_pulsaciones_izquierdo==2)){
    lcd.clear();
    disparo();
  }
  if((estadoBotonizq == LOW)&&(nivel==4)&&
    (numero_pulsaciones_izquierdo==3)){
    lcd.clear();
    Disparar();
  }
  //------------------------------------------------------------------------------

  //Acciones botón derecho.
  estadoBotonder = digitalRead(boton_der);
  if(estadoBotonder != estadoanteriorder){
    if(estadoBotonder == LOW){
      numero_pulsaciones_derecho++;
      if(numero_pulsaciones_derecho>2){
        numero_pulsaciones_derecho = 0;
      }
    }
    if(estadoBotonder == LOW&&(linea<8)&&(nivel==2)){
      linea++;
      lcd.setCursor(8,0);
      lcd.print(linea);
    }
    if(estadoBotonder == LOW&&(nivel==5)){
      lcd.clear();
      numero_pulsaciones_select = 0;
      numero_pulsaciones_derecho = 0;
      nivel=0;
      menu();
    }
    if(estadoBotonder == LOW&&(segundo<=60)&&(nivel==3)){
      if (segundo==9){
        cursor_segundo = 11;
      }
      segundo++;
      lcd.setCursor(cursor_segundo,1);
      lcd.print(segundo);
    }
    if(estadoBotonder == LOW&&(segundo==60)&&(nivel==3)){
      minuto++;
      lcd.setCursor(cursor_minuto,1);
      lcd.print(minuto);
      lcd.setCursor(11,1);
      lcd.print("0");
      cursor_segundo = 12;
      segundo = 0;
    }
    if(estadoBotonder == LOW&&(linea ==8)){
      linea=0;
    }
    if(estadoBotonder == LOW&&(segundo >59)){
      segundo=0;
    }
    estadoanteriorder = estadoBotonder;
  }
  if (estadoBotonder == LOW&&(nivel==0)&& (numero_pulsaciones_derecho==1)){
    lcd.clear();
    configurar();
  }
  if (estadoBotonder == LOW&&(numero_pulsaciones_derecho == 2)&&
    (nivel==1)){
    lcd.clear();
    numero_pulsaciones_derecho = 0;
    borrar();
  }

  //------------------------------------------------------------------------------    

  //Acciones botón select.
  estadoBotonselect = digitalRead(boton_select);
  if (estadoBotonselect != estadoanteriorselect){
    if (estadoBotonselect == LOW && (nivel == 8)){
      inicio=millis();
arriba:
      lineaROM = EEPROM.read(direccion_linea);
      minutoROM = EEPROM.read(direccion_minuto);
      segundoROM = EEPROM.read(direccion_segundo);
      if ((m==minutoROM)&&(s==segundoROM)){
        digitalWrite(lineas[lineaROM],HIGH);
        delay(1000);
        digitalWrite(lineas[lineaROM],LOW);
        direccion_linea= direccion_linea + 3;
        direccion_minuto= direccion_minuto + 3;
        direccion_segundo= direccion_segundo + 3;
      }
      //Calculamos el tiempo que paso desde que se activo el sensor start/stop
      tiempo=millis()-inicio;    
      //Calculamos los minutos       
      m=tiempo/60000;                
      //Descomponemos los minutos y sacamos el valor de las unidades     
      mu=m%10;          
      //Descomponemos los minutos y sacamos el valor de las decenas       
      md=(m-mu)/10;  
      //Calculamos los segundos       
      s=(tiempo/1000)-(m*60);  
      //Descomponemos los segundos y sacamos el valor de las unidades       
      su=s%10;          
      //Descomponemos los segundos y sacamos el valor de las decenas       
      sd=(s-su)/10;           
      //Calculamos las milesimas de segundo       
      l=tiempo-(s*1000)-(m*60000);  
      //Descomponemos las milesimas y sacamos el valor de las unidades      
      lu=l%10;       
      //Descomponemos las milesimas y sacamos el valor de las decenas       
      ld=((l-lu)/10)%10;      
      //Descomponemos las milesimas y sacamos el valor de las centenas       
      lc=(l-(ld*10)-lu)/100;   
      //Ponemos el cursor en la linea 2 y el caracter 5       
      lcd.setCursor(0, 1); 
      //Imprimimos los valores en el display       
      lcd.print(md);                      
      lcd.print(mu);              
      lcd.print(":");
      lcd.print(sd);
      lcd.print(su);
      lcd.print(":");
      lcd.print(lc);
      lcd.print(ld);
      lcd.print(lu);
      goto arriba;
    }


    if (estadoBotonselect == LOW){
      numero_pulsaciones_select++;
      tiempoultimapulsacion = millis();
    }
    estadoanteriorselect = estadoBotonselect;
  }
  if (numero_pulsaciones_select == 2){
    lcd.clear();
    numero_pulsaciones_select = 0;
    numero_pulsaciones_derecho = 0;
    numero_pulsaciones_izquierdo = 0;
    nivel = -1;
    MenuPrincipal();
  }
  else if (estadoBotonselect == LOW && 
    (numero_pulsaciones_select == 1) &&
    (millis() - tiempoultimapulsacion > 1000 )&& 
    (nivel==2||nivel==3)){
    EEPROM.write(direccion_linea, linea);
    EEPROM.write(direccion_minuto, minuto);
    EEPROM.write(direccion_segundo,segundo);
    direccion_linea = direccion_linea + 3;
    direccion_minuto = direccion_minuto + 3;
    direccion_segundo = direccion_segundo + 3;
    lcd.setCursor(14,0);
    lcd.print("OK");
    delay(1000);
    lcd.setCursor(14,0);
    lcd.print("  ");
    numero_pulsaciones_select = 0;
  }
  if (millis() - tiempoultimapulsacion > 2000){
    numero_pulsaciones_select = 0;
  }
  //----------------------------------------------------------------------------------

  //Acciones botón arriba.
  estadoBotonarriba = digitalRead(boton_arriba);
  if (estadoBotonarriba != estadoanteriorarriba){
    if(estadoBotonarriba == LOW&&(nivel==3)){
      nivel=2;
    } 
    estadoanteriorarriba = estadoBotonarriba;
  }
  //------------------------------------------------

  //Acciones botón abajo.
  estadoBotonabajo = digitalRead(boton_abajo);
  if (estadoBotonabajo != estadoanteriorabajo){
    if(estadoBotonabajo == LOW&&(nivel==2)){
      nivel=3;
    }
    estadoanteriorabajo = estadoBotonabajo;
  }
}
//------------------------------------------------


void MenuPrincipal(){
  lcd.setCursor(9,0);
  lcd.print("Manual");
  lcd.write(8);
  lcd.setCursor(0,1);
  lcd.write(7);
  lcd.print("Secuencial");
}
void menu (){
  nivel = 0;
  lcd.setCursor(5,0);
  lcd.print("Configurar");
  lcd.write(8);
  lcd.setCursor(0,1);
  lcd.write(7);
  lcd.setCursor(1,1);
  lcd.print("Disparo");
}
void disparo (){
  nivel=4;
  lcd.setCursor(6,0);
  lcd.print("Comprobar");
  lcd.write(8);
  lcd.setCursor(0,1);
  lcd.write(7);
  lcd.setCursor(1,1);
  lcd.print("Disparar");
}

void configurar () {
  nivel=1;
  lcd.setCursor(9,0);
  lcd.print("Borrar");
  lcd.write(8);
  lcd.setCursor(0,1);
  lcd.write(7);
  lcd.setCursor(1,1);
  lcd.print("Secuencia");
}

void secuencia () {
  nivel=2;
  lcd.setCursor(0,1);
  lcd.print("Tiempo:");
  lcd.write(7);
  lcd.print("0");
  lcd.print(minuto);
  lcd.print(":");
  lcd.print("0");
  lcd.print(segundo);
  lcd.setCursor(13,1);
  lcd.write(8);
  lcd.setCursor(0,0);
  lcd.print("L");
  lcd.write(9);
  lcd.print("nea:");
  lcd.write(7);
  lcd.print("0");
  lcd.print(linea);
  lcd.setCursor(9,0);
  lcd.write(8);
}

void borrar(){
  nivel=5;
  lcd.setCursor(0,0);
  lcd.write(10);
  lcd.print("Borrar?");
  lcd.setCursor(0,1);
  lcd.write(7);
  lcd.print("S");
  lcd.write(9);
  lcd.setCursor(13,1);
  lcd.print("No");
  lcd.write(8);
}

void Disparar(){
  lcd.print("Cuenta atras");
  lcd.setCursor(0,1);
  lcd.print("00:14:999");
  static unsigned long ultimotick = 0; 
  while (milisegundos > 0) {
    if (segundos <= 9&&(milisegundos ==735)){
      lcd.setCursor(3,1);
      lcd.print("0");
      espacio_countdown_segundos = 4;
      lcd.setCursor(espacio_countdown_segundos,1);
      lcd.print(segundos);
    }
    if (millis() - ultimotick >= 1) {
      ultimotick = millis();

      milisegundos--;
      lcd.setCursor(6,1);
      lcd.print(milisegundos);

    }

    if (milisegundos == 0) {

      ultimotick = millis();
      segundos--;

      if (segundos == 0 && (milisegundos == 0)){
        milisegundos = 0;
        lcd.setCursor(espacio_countdown_segundos,1);
        lcd.print(segundos);
        delay(200);
        Cronometro ();
        break;
      }
      milisegundos = 735;
      lcd.setCursor(3,1);
      lcd.print(segundos);
    }
  }
}
void Cronometro(){
  nivel =8;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Cronometro");
  lcd.setCursor(0,1);
  lcd.print("00:00:000");
}
