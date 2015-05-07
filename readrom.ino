int pin_d0 = 2;
int pin_d1 = 3;
int pin_d2 = 4;
int pin_d3 = 5;
int pin_d4 = 6;
int pin_d5 = 7;
int pin_d6 = 8;
int pin_d7 = 9;

int pin_sel_low = 10;
int pin_sel_high = 11;
int pin_cs = 12;

void set_port_as_input()
{
  pinMode(pin_d0, INPUT);
  pinMode(pin_d1, INPUT);
  pinMode(pin_d2, INPUT);
  pinMode(pin_d3, INPUT);
  pinMode(pin_d4, INPUT);
  pinMode(pin_d5, INPUT);
  pinMode(pin_d6, INPUT);
  pinMode(pin_d7, INPUT);
}

void set_port_as_output()
{
  pinMode(pin_d0, OUTPUT);
  pinMode(pin_d1, OUTPUT);
  pinMode(pin_d2, OUTPUT);
  pinMode(pin_d3, OUTPUT);
  pinMode(pin_d4, OUTPUT);
  pinMode(pin_d5, OUTPUT);
  pinMode(pin_d6, OUTPUT);
  pinMode(pin_d7, OUTPUT);
}

void set_port_value(byte value)
{
  digitalWrite(pin_d0, (value&1) ? HIGH : LOW);
  digitalWrite(pin_d1, (value&2) ? HIGH : LOW);
  digitalWrite(pin_d2, (value&4) ? HIGH : LOW);
  digitalWrite(pin_d3, (value&8) ? HIGH : LOW);
  digitalWrite(pin_d4, (value&16) ? HIGH : LOW);
  digitalWrite(pin_d5, (value&32) ? HIGH : LOW);
  digitalWrite(pin_d6, (value&64) ? HIGH : LOW);
  digitalWrite(pin_d7, (value&128) ? HIGH : LOW);
}

byte read_port_value()
{
  byte val = 0;
  if(digitalRead(pin_d0))
    val |= 1;
  if(digitalRead(pin_d1))
    val |= 2;
  if(digitalRead(pin_d2))
    val |= 4;
  if(digitalRead(pin_d3))
    val |= 8;
  if(digitalRead(pin_d4))
    val |= 16;
  if(digitalRead(pin_d5))
    val |= 32;
  if(digitalRead(pin_d6))
    val |= 64;
  if(digitalRead(pin_d7))
    val |= 128;
   
   return val;
}

byte read_cycle(short address)
{ 
  digitalWrite(pin_cs, HIGH);
  digitalWrite(pin_sel_low, LOW);
  digitalWrite(pin_sel_high, LOW);
  
  set_port_as_output();
  set_port_value(address & 0xff);
  digitalWrite(pin_sel_low, HIGH);
  //delay(10);
  digitalWrite(pin_sel_low, LOW);
  set_port_value((address & 0xff00)>>8);
  digitalWrite(pin_sel_high, HIGH);
  //delay(10);
  digitalWrite(pin_sel_high, LOW);
  set_port_as_input();
  digitalWrite(pin_cs, LOW);
  //delay(10);
  byte val = read_port_value();
  digitalWrite(pin_cs, HIGH);
 
  return val;
}

char cmdbuf[64];
int cmd_index;

void setup() 
{
  // put your setup code here, to run once:
  set_port_as_input();
  
  pinMode(pin_sel_low, OUTPUT);
  digitalWrite(pin_sel_low, LOW);
  
  pinMode(pin_sel_high, OUTPUT);
  digitalWrite(pin_sel_high, LOW);
  
  pinMode(pin_cs, OUTPUT);
  digitalWrite(pin_cs, HIGH);
  
  Serial.begin(9600);

  cmd_index = 0;
  cmdbuf[0] = 0;
}

void loop() 
{
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0)
  {
    char b = Serial.read();
    if(b!='\n')
    {
      if(cmd_index<63)
        cmdbuf[cmd_index++] = b;
    }
    else
    {
      cmdbuf[cmd_index] = 0;
      if(cmd_index>0)
      {
        if(cmdbuf[0]=='m' && cmdbuf[1]==' ')
        {
          short addr_start;
          short addr_end;
          int num = sscanf(cmdbuf+2, "%x%x", &addr_start, &addr_end);
          if(num==0)
            Serial.println('?');
          else
          {
            if(num==1)
              addr_end = addr_start + 16;
            short addr = addr_start;
            short n = 0;
            while(addr!=addr_end)
            {
              if(n==0)
              {
                char buf[16];
                sprintf(buf, "\n%04x ", addr);
                Serial.print(buf);
              }
              n++;
              if(n==16)
                n = 0;
                
              byte data = read_cycle(addr);
              char buf[16];
              sprintf(buf, "%02x ", data);
              Serial.print(buf);
              addr++;
            }
          }
          Serial.println(' ');
        }
        else
        {
          Serial.println('?');
        }

        cmd_index = 0;
      }
    }
  }
}
