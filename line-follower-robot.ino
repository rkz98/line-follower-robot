#define PIN_ENABLE 7

#define BIT_MOTOR_1_A 2
#define BIT_MOTOR_1_B 3
#define BIT_MOTOR_2_A 1
#define BIT_MOTOR_2_B 4
#define BIT_MOTOR_3_A 5
#define BIT_MOTOR_3_B 7
#define BIT_MOTOR_4_A 0
#define BIT_MOTOR_4_B 6

#define PIN_MOTOR_3_PWM 5
#define PIN_MOTOR_2_PWM 3
#define PIN_MOTOR_4_PWM 6
#define PIN_MOTOR_1_PWM 11

#define COMPLETE_ROTATION 20

#define MIN_ANALOG_VALUE 500

#define PIN_LASER_SENSOR_LEFT A0
#define PIN_LASER_SENSOR_RIGHT A1
#define PIN_LASER_SENSOR_CENTRAL A2

#define PIN_LASER_SENSOR_LEFT_BACK A3
#define PIN_LASER_SENSOR_RIGHT_BACK A4

#define MOTOR_VELOCITY_0 0
#define MOTOR_VELOCITY_1 25
#define MOTOR_VELOCITY_2 80
#define MOTOR_VELOCITY_3 180
#define MOTOR_VELOCITY_4 255

unsigned int rotationStep = 0;
unsigned int completeRotations = 0;
unsigned int lastLaserSensorState = 0;

void setup() {
	Serial.begin(9600);

	pinMode(PIN_ENABLE, OUTPUT);

	pinMode(PIN_MOTOR_1_PWM, OUTPUT);
	pinMode(PIN_MOTOR_2_PWM, OUTPUT);
	pinMode(PIN_MOTOR_3_PWM, OUTPUT);
	pinMode(PIN_MOTOR_4_PWM, OUTPUT);

	pinMode(PIN_LASER_SENSOR_LEFT, INPUT);
	pinMode(PIN_LASER_SENSOR_RIGHT, INPUT);
	pinMode(PIN_LASER_SENSOR_CENTRAL, INPUT);

	pinMode(PIN_LASER_SENSOR_LEFT_BACK, INPUT);
	pinMode(PIN_LASER_SENSOR_RIGHT_BACK, INPUT);

	digitalWrite(PIN_ENABLE, LOW);

	digitalWrite(BIT_MOTOR_1_B, LOW);
	digitalWrite(BIT_MOTOR_1_A, HIGH);

	digitalWrite(BIT_MOTOR_2_B, LOW);
	digitalWrite(BIT_MOTOR_2_A, HIGH);

	digitalWrite(BIT_MOTOR_3_B, LOW);
	digitalWrite(BIT_MOTOR_3_A, HIGH);

	digitalWrite(BIT_MOTOR_4_B, LOW);
	digitalWrite(BIT_MOTOR_4_A, HIGH);

	setMotorSpeed(PIN_MOTOR_1_PWM, MOTOR_VELOCITY_3);
	setMotorSpeed(PIN_MOTOR_2_PWM, MOTOR_VELOCITY_3);
	setMotorSpeed(PIN_MOTOR_2_PWM, MOTOR_VELOCITY_3);
	setMotorSpeed(PIN_MOTOR_4_PWM, MOTOR_VELOCITY_3);
}

void setMotorSpeed(unsigned int pinPwmToUpdate, unsigned int speedValue) {
	analogWrite(pinPwmToUpdate, speedValue);
}

void checkIfCompleteRotation() {
	if (rotationStep == COMPLETE_ROTATION) {
		rotationStep = 0;
		completeRotations += 1;
	}
}

void startRunningEncoderVerification() {
	unsigned int laserSensorState = analogRead(PIN_LASER_SENSOR_LEFT_BACK);
	if (laserSensorState <= MIN_ANALOG_VALUE) {
		if (lastLaserSensorState == HIGH) {
			rotationStep += 1;
			lastLaserSensorState = LOW;
			checkIfCompleteRotation();
		}
	} else if (lastLaserSensorState == LOW) {
		rotationStep += 1;
		lastLaserSensorState = HIGH;
		checkIfCompleteRotation();
	}
}

void startRunningMotor() {
	unsigned int laserSensorLeftState = analogRead(PIN_LASER_SENSOR_LEFT);
	unsigned int laserSensorRightState = analogRead(PIN_LASER_SENSOR_RIGHT);
	unsigned int laserSensorCentralState = analogRead(PIN_LASER_SENSOR_CENTRAL);

	Serial.println(laserSensorCentralState);

	if (laserSensorCentralState > MIN_ANALOG_VALUE) {
		if (laserSensorLeftState <= MIN_ANALOG_VALUE) {
			setMotorSpeed(PIN_MOTOR_1_PWM, MOTOR_VELOCITY_3);
			setMotorSpeed(PIN_MOTOR_2_PWM, MOTOR_VELOCITY_2);
		}

		if (laserSensorRightState <= MIN_ANALOG_VALUE) {
			setMotorSpeed(PIN_MOTOR_1_PWM, MOTOR_VELOCITY_2);
			setMotorSpeed(PIN_MOTOR_2_PWM, MOTOR_VELOCITY_3);
		}
	} else {
		setMotorSpeed(PIN_MOTOR_1_PWM, MOTOR_VELOCITY_3);
		setMotorSpeed(PIN_MOTOR_2_PWM, MOTOR_VELOCITY_3);
	}

//    if (laserSensorLeftState <= MIN_ANALOG_VALUE) {
//      setMotorSpeed(PIN_MOTOR_1_PWM, MOTOR_VELOCITY_2);
//      setMotorSpeed(PIN_MOTOR_2_PWM, MOTOR_VELOCITY_3);
//      setMotorSpeed(PIN_MOTOR_3_PWM, MOTOR_VELOCITY_0);
//      setMotorSpeed(PIN_MOTOR_4_PWM, MOTOR_VELOCITY_0);
//    } else {
//      setMotorSpeed(PIN_MOTOR_1_PWM, MOTOR_VELOCITY_3);
//      setMotorSpeed(PIN_MOTOR_2_PWM, MOTOR_VELOCITY_2);
//      setMotorSpeed(PIN_MOTOR_3_PWM, MOTOR_VELOCITY_3);
//      setMotorSpeed(PIN_MOTOR_4_PWM, MOTOR_VELOCITY_3);
//    }
}

void loop() {
	startRunningMotor();
	startRunningEncoderVerification();
}
