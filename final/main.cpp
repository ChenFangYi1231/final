#include "mbed.h"
#include "bbcar.h"

Ticker servo_ticker;
Serial pc(USBTX,USBRX);
PwmOut pin9(D9), pin8(D8);
Serial xbee(D12, D11);
Serial uart(D1,D0);
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalInOut pin10(D10);
BBCar car(pin8, pin9, servo_ticker);

Thread t, t1;
EventQueue queue(32 * EVENTS_EVENT_SIZE);
parallax_ping  ping1(pin10);

void xbee_rx_interrupt(void);
void xbee_rx(void);
void reply_messange(char *xbee_reply, char *messange);
void check_addr(char *xbee_reply, char *messenger);

void recieve_thread(){
   while(1) {
      if(uart.readable()){
            char recv = uart.getc();
            if(recv == '0'){
                led1 = 0;
                led2 = 1;
                led3 = 1;
            }
            else if(recv == '1'){
                led1 = 1;
                led2 = 0;
                led3 = 1;
            }
            else if(recv == '2'){
                led1 = 1;
                led2 = 1;
                led3 = 0;
            }
            else{
                led1 = 1;
                led2 = 1;
                led3 = 0;
            }
            xbee.printf("%c\r\n", recv);
      }
   }
}

void send_uart()
{
    char s[21];
    sprintf(s,"image_classification");
    uart.puts(s);
    wait(0.5);
    
}

void detect(float dis[])
{
    
    if(dis[0] > 40){
        xbee.printf("right triangle\r\n");
        
    }
    else if(dis[0] > 30){
        xbee.printf("one triangle\r\n");
        
    }
    else if(dis[0] > 20){
        xbee.printf("two triangle\r\n");
    }
    else{
        xbee.printf("square\r\n");
    }
}

int main() 
{
    float dis[4];
    int done, i;
    pc.baud(9600);
    t.start(recieve_thread);
    
    led1 = 1;
    led2 = 1;
    led3 = 1;

    char xbee_reply[4];

    xbee.printf("start\r\n");
    
    done = 1;
    xbee.printf("go straight\r\n");
    while(done){// go straight
        car.goStraight(-80);
        wait(0.5);
        i++;
        if(i == 20 || (float)ping1 < 27){
            done = 0;
        }
    }
    
    car.stop();
    // turn left
    xbee.printf("turn left\r\n");
    car.turn(-50,-0.1);
    wait(2.3);
    car.stop();
    
    // go to 3
    done = 1;
    xbee.printf("go straight\r\n");
    while(done){// go straight
        car.goStraight(-80);
        wait(0.5);
        if((float)ping1 < 10){
            done = 0;
        }
    }
    car.stop();
    
    // turn back right
    xbee.printf("turn right\r\n");
    car.turn(50,-0.1);
    wait(2);
    car.stop();
    // go back  3
    xbee.printf("go back\r\n");
    car.goStraight(80);
    wait(1.5);
    car.stop();
    wait(1);
    //go straight
    xbee.printf("go straight\r\n");
    car.goStraight(-80);
    wait(1.5);
    car.stop();
    // right
    xbee.printf("turn right\r\n");
    car.turn(-50,0.1);
    wait(2);
    car.stop();
    //straight
    xbee.printf("go straight\r\n");
    car.goStraight(-80);
    wait(3);
    car.stop();
    // turn to take a picture
    xbee.printf("turn right\r\n");
    car.turn(-50,-0.1);
    wait(2);
    car.stop();
    // go back
    xbee.printf("go back\r\n");
    car.goStraight(80);
    wait(1);
    car.stop();
    xbee.printf("take a picture\r\n");
    send_uart();
    
    wait(1);

    // straight
    xbee.printf("go straight\r\n");
    car.goStraight(-80);
    wait(1);
    car.stop();
    // right
    xbee.printf("turn right\r\n");
    car.turn(-50,0.1);
    wait(2);
    car.stop();
    // go to mission2
    done = 1;
    xbee.printf("go straight\r\n");
    while(done){// go straight
        car.goStraight(-80);
        wait(0.5);
        if((float)ping1 < 40){
            done = 0;
        }
    }
    car.stop();
    // right
    xbee.printf("turn right\r\n");
    car.turn(-50,0.1);
    wait(2);
    car.stop();

    done = 1;
    xbee.printf("go straight\r\n");
    while(done){// go straight
        car.goStraight(-80);
        wait(0.5);
        if((float)ping1 < 25){
            done = 0;
        }
    }
    car.stop();
    // right
    xbee.printf("turn right\r\n");
    car.turn(-50,0.1);
    wait(2);
    car.stop();
    //straight
    xbee.printf("go straight\r\n");
    car.goStraight(-80);
    wait(3);
    car.stop();
    // right
    xbee.printf("turn right\r\n");
    car.turn(-50,0.1);
    wait(2);
    car.stop();
    // straight
    xbee.printf("go straight\r\n");
    car.goStraight(-80);
    wait(2);
    car.stop();
    // left
    xbee.printf("turn left\r\n");
    car.turn(-50,-0.1);
    wait(2);
    car.stop();
    xbee.printf("detect distant\r\n");
    dis[0] = (float)ping1;
    wait(2);
    detect(dis);

    // turn left
    xbee.printf("turn left\r\n");
    car.turn(-50,-0.1);
    wait(2);
    car.stop();
    // straight
    xbee.printf("go straight\r\n");
    car.goStraight(-80);
    wait(1.8);
    car.stop();
    // turn right
    xbee.printf("turn right\r\n");
    car.turn(-50,0.1);
    wait(2);
    car.stop();

    done = 1;
    xbee.printf("go straight\r\n");
    while(done){// go straight
        car.goStraight(-80);
        wait(0.5);
        if((float)ping1 < 25){
            done = 0;
        }
    }
    car.stop();
    // right
    xbee.printf("turn right\r\n");
    car.turn(-50,0.1);
    wait(2);
    car.stop();
    // straight
    xbee.printf("go straight\r\n");
    car.goStraight(-80);
    wait(10);
    car.stop();
    xbee.printf("end!\r\n");

}

void xbee_rx_interrupt(void)
{
  xbee.attach(NULL, Serial::RxIrq); // detach interrupt
  queue.call(&xbee_rx);
}

void xbee_rx(void)
{
  char buf[100] = {0};
  char outbuf[100] = {0};
  while(xbee.readable()){
    for (int i=0; ; i++) {
      char recv = xbee.getc();
      if (recv == '\r') {
        break;
      }
      buf[i] = pc.putc(recv);
    }
  }
  xbee.attach(xbee_rx_interrupt, Serial::RxIrq); // reattach interrupt
}

void reply_messange(char *xbee_reply, char *messange){
  xbee_reply[0] = xbee.getc();
  xbee_reply[1] = xbee.getc();
  xbee_reply[2] = xbee.getc();
  if(xbee_reply[1] == 'O' && xbee_reply[2] == 'K'){
    pc.printf("%s\r\n", messange);
    xbee_reply[0] = '\0';
    xbee_reply[1] = '\0';
    xbee_reply[2] = '\0';
  }
}

void check_addr(char *xbee_reply, char *messenger){
  xbee_reply[0] = xbee.getc();
  xbee_reply[1] = xbee.getc();
  xbee_reply[2] = xbee.getc();
  xbee_reply[3] = xbee.getc();
  pc.printf("%s = %c%c%c\r\n", messenger, xbee_reply[1], xbee_reply[2], xbee_reply[3]);
  xbee_reply[0] = '\0';
  xbee_reply[1] = '\0';
  xbee_reply[2] = '\0';
  xbee_reply[3] = '\0';
}