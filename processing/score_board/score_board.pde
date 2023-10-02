import processing.serial.*;
import java.awt.event.KeyEvent;
import java.io.IOException;
Serial myPort;
String data="";

PFont font;

int conn;
int goal;
int team1;
int team2;

void setup() {
  //size (1000, 800);
  fullScreen(); 

  font = createFont("font.ttf", 32);
  textFont(font);


  String portName = Serial.list()[3];
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
}
void draw() {
  background(0);

  if (conn == 0) {
    textAlign(CENTER);
    textSize(100);
    fill(255, 149, 43);
    text("Waiting For", width/2, 150);
    text("Connection", width/2, 300);
  } else {
    textSize(100);
    
    textAlign(CENTER);

    fill(255, 149, 43);
    text(goal, width/2, 150);

    textAlign(LEFT);
    fill(255, 43, 43);
    text(team1, width/4, 300);

    textAlign(RIGHT);
    fill(43, 100, 255);
    text(team2, width/4*3, 300);
    
    int t = 5;
    
    int indent = width/8;
    
    int boxw = width-indent*2;
    int boxh = height-400;
   
    
    fill(50, 200, 50);
    rect(indent, 350, boxw, boxh, 10);
    
    line(width/2, 350+t, width/2, 350+boxh-t);
    
    stroke(255);
    strokeWeight(t*2);
    
    line(indent+t, 350+boxh/2, boxw/6*3-t, 350+boxh/2);
    line(boxw/6*5+t, 350+boxh/2, boxw+indent-t, 350+boxh/2);
    
    line(boxw/6*3-t, 350+t, boxw/6*3-t, 350+boxh-t);
    line(boxw/6*5+t, 350+t, boxw/6*5+t, 350+boxh-t);
    
    
  }
}

void serialEvent (Serial myPort) { 
  data = myPort.readStringUntil('\n');

  if (data != null) {
    data = trim(data);
    // split the string at "/"
    String items[] = split(data, ':');
    if (items.length > 1) {
      conn = int(items[0]);
      goal = int(items[1]);
      team1 = int(items[2]);
      team2 = int(items[3]);

      print(conn);
      print("\t");
      print(goal);
      print("\t");
      print(team1);
      print("\t");
      println(team2);
    }
  }
}
