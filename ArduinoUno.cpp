#include <LiquidCrystal.h>

//Variables globales
LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);
int numPregunta; //Indica el # de pregunta actual
int preguntasCorrectas = 0; //Preguntas contestadas correctamente
int totalPreguntas; //Número total de preguntas
String tipoExamen; //Indica el examen seleccionado: A o B

//examenA = [
  //{
    //pregunta: "2*2",
    //opciones: ["A: 2", "B: 4", "C: 6", "D: 1"],
    //respuesta: "B"
  //}
//];

//Clase con la estructura de los examentes
class Pregunta {
  public: 
  	String texto;
  	String respuesta;
  	String opciones[4];
  
  	//Constructor
    Pregunta(String texto_, String respuesta_, String opcion1, String opcion2, String opcion3, String opcion4) {
      texto = texto_;
      respuesta = respuesta_;
      opciones[0] = opcion1;
      opciones[1] = opcion2;
      opciones[2] = opcion3;
      opciones[3] = opcion4;     
    };
};

//Puntero hacia el examen activo
Pregunta *examen;

//Examen A
Pregunta examenA[] = {
  Pregunta("2*2?", "C", "A:1", "B:2", "C:4", "D:6"),
  Pregunta("6/2?", "A", "A:3", "B:4", "C:12", "D:2"),
  Pregunta("5-4?", "D", "A:8", "B:7", "C:3", "D:1"),
  Pregunta("7+15?", "B", "A:23", "B:22", "C:21", "D:24"),
  Pregunta("(2*5)+5?", "A", "A:15", "B:50", "C:20", "D:10"),
  Pregunta("(15/3)+2?", "C", "A:3", "B:10", "C:7", "D:9"),
  Pregunta("10*2*3?", "D", "A:80", "B:70", "C:50", "D:60"),
  Pregunta("10+8-4?", "B", "A:15", "B:22", "C:14", "D:12"),
  Pregunta("20-5+3-1?", "A", "A:17", "B:18", "C:16", "D:15"),
  Pregunta("9/3+5?", "C", "A:1", "B:4", "C:8", "D:5")
};

//Examen B
Pregunta examenB[] = {
  Pregunta("5/5?", "B", "A:0", "B:1", "C:2", "D:1.5"),
  Pregunta("3*4?", "A", "A:12", "B:10", "C:13", "D:11"),
  Pregunta("5+3+2?", "D", "A:11", "B:9", "C:12", "D:10"),
  Pregunta("8-2?", "C", "A:5", "B:10", "C:7", "D:6"),
  Pregunta("7+5-3?", "A", "A:9", "B:8", "C:10", "D:7"),
  Pregunta("-3+8-1?", "B", "A:5", "B:4", "C:6", "D:3"),
  Pregunta("(5*4)/10)?", "D", "A:3", "B:4", "C:1", "D:2"),
  Pregunta("(8/4)*2)?", "C", "A:5", "B:6", "C:4", "D:3"),
  Pregunta("6*2*5?", "A", "A:60", "B:55", "C:65", "D:62"),
  Pregunta("8/2-3?", "B", "A:2", "B:1", "C:-8", "D:0")
};


void setup()
{
  //Primera pregunta
  numPregunta = 0;
  
  lcd_1.begin(16, 2);
  
  pinMode(6,INPUT); //pin de pulsador A
  pinMode(7,INPUT); //pin de pulsador B
  pinMode(8,INPUT); //pin de pulsador C
  pinMode(9,INPUT); //pin de pulsador D
  pinMode(13,INPUT); //Selección de tipo Examen
  pinMode(10,OUTPUT); //pin de motor
  
  //Examen seleccionado con el interruptor
  tipoExamen = digitalRead(13) == HIGH ? "A" : "B";
  
  //Se apunta al examen seleccionado
  if(tipoExamen == "A"){
    examen = examenA;
    
    //Tamaño del examen
    totalPreguntas = *(&examenA + 1) - examenA;
  }
  else {
    examen = examenB;
    totalPreguntas = *(&examenB + 1) - examenB;
  }
  
  //Mueve la página hacia la primera pregunta
  moverMotor();
  
  //Se muestra la primera pregunta
  lcd_1.print(examen[numPregunta].texto);
}

void loop()
{
  //Se evalúa la respuesta pulsada
  String respuesta = "";
  if (digitalRead(6) == HIGH) {
    respuesta = "A";
  }
  if (digitalRead(7) == HIGH) {
    respuesta = "B";
  }
  if (digitalRead(8) == HIGH) {
    respuesta = "C";
  }
  if (digitalRead(9) == HIGH) {
    respuesta = "D";
  }
  
  //Si no se pulsa respuesta, no se evalúa nada
  if(respuesta != "")
  	evaluarRespuesta(respuesta);
}

//Evalúa la respuesta pulsada
void evaluarRespuesta(String valor) {
  lcd_1.clear();
  
  //Se evalúa la respuesta
  if(valor == examen[numPregunta].respuesta){
    preguntasCorrectas++;
  	lcd_1.print("Correcto: " + examen[numPregunta].respuesta);
  }
  else {
    lcd_1.print("Incorrecto: " + examen[numPregunta].respuesta);
  }
  
  //Se mueve a la siguiente pregunta
  moverMotor();
  numPregunta++;
  lcd_1.clear();
  
  //Si no es la última pregunta, se imprime la siguiente
  if(numPregunta < (totalPreguntas)){
  	lcd_1.print(examen[numPregunta].texto);
  }
  else { //Caso contrario, se muestra la calificación
    lcd_1.print("Calificacion:");
    lcd_1.setCursor(0, 1);
    lcd_1.print(calcularNota());
  }
}

//Se calcula la nota
double calcularNota(){
  if(totalPreguntas == 0)
    return 0;
  else
    return ((double)preguntasCorrectas/totalPreguntas)*10.;
}

//Se mueve el motor
void moverMotor(){
  digitalWrite(10, HIGH);
  delay(1000);
  digitalWrite(10, LOW);
}