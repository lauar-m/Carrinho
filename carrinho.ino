// PINOS DO MOTOR E ENCODERS
#define ENA 23
#define ENB 27

#define IN1 18
#define IN2 19
#define IN3 25
#define IN4 26

#define ENC_LEFT  14
#define ENC_RIGHT 13

// VARIÁVEIS GLOBAIS
volatile long pulsesLeft = 0;
volatile long pulsesRight = 0;

void IRAM_ATTR isr_left()  { pulsesLeft++; }
void IRAM_ATTR isr_right() { pulsesRight++; }

// CONTROLE
float Kp = 0.7;
float Ki = 0.01;

float erro_int = 0;

int pwm_base = 180;   

// PARAMETROS DE AMOSTRAGEM
const int STEP_MS = 10;
const int BUCKETS = 5;

long velL_hist[BUCKETS];
long velR_hist[BUCKETS];

int idx = 0;

// PWM – CONFIGURAÇÃO LEDC
void setup_pwm() {
  // pino, frequência, resolução
  ledcAttach(ENA, 20000, 8);
  ledcAttach(ENB, 20000, 8);
}

// FUNÇÃO PARA CALCULAR VELOCIDADES MÉDIAS
void calc_vel(long &vL, long &vR) {
  noInterrupts();
  static long lastL = 0, lastR = 0;

  long dL = pulsesLeft  - lastL;
  long dR = pulsesRight - lastR;

  lastL = pulsesLeft;
  lastR = pulsesRight;
  interrupts();

  velL_hist[idx] = dL;
  velR_hist[idx] = dR;

  idx = (idx + 1) % BUCKETS;

  long sumL = 0, sumR = 0;
  for (int i = 0; i < BUCKETS; i++) {
    sumL += velL_hist[i];
    sumR += velR_hist[i];
  }

  vL = sumL / BUCKETS;
  vR = sumR / BUCKETS;
}

// CONTROLE PI PARA IR RETO
void controle(long vL, long vR) {
  const int MIN_PWM = 100;
  const int MIN_SPEED = 1;

  if (vL < MIN_SPEED && vR < MIN_SPEED) {
    erro_int = 0;
    ledcWrite(ENA, pwm_base);
    ledcWrite(ENB, pwm_base);
    return;
  }

  long erro = vL - vR;

  erro_int += erro;
  erro_int = constrain(erro_int, -800, 800);

  int u = Kp * erro + Ki * erro_int;

  int pwmL = pwm_base - u;
  int pwmR = pwm_base + u;

  if (pwmL < MIN_PWM) pwmL = MIN_PWM;
  if (pwmR < MIN_PWM) pwmR = MIN_PWM;

  pwmL = constrain(pwmL, MIN_PWM, 255);
  pwmR = constrain(pwmR, MIN_PWM, 255);

  ledcWrite(ENA, pwmL);
  ledcWrite(ENB, pwmR);

  // Debug
  Serial.print("vL="); Serial.print(vL);
  Serial.print(" vR="); Serial.print(vR);
  Serial.print(" erro="); Serial.print(erro);
  Serial.print(" u="); Serial.print(u);
  Serial.print(" PWM="); Serial.print(pwmL);
  Serial.print("/");
  Serial.println(pwmR);
}

// SETUP
void setup() {
  Serial.begin(115200);

  setup_pwm();

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // frente
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  pinMode(ENC_LEFT, INPUT_PULLUP);
  pinMode(ENC_RIGHT, INPUT_PULLUP);

  attachInterrupt(ENC_LEFT, isr_left, RISING);
  attachInterrupt(ENC_RIGHT, isr_right, RISING);
}

// LOOP PRINCIPAL
void loop() {
  static unsigned long last = 0;
  if (millis() - last >= STEP_MS) {
    last = millis();

    long vL, vR;
    calc_vel(vL, vR);   // 1) lê pulsos + calcula velocidades
    controle(vL, vR);   // 2) aplica PI
  }
}

