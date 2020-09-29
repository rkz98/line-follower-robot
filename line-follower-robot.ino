#include <Servo.h>

#define QUANTITY_OF_CI 1

#define PIN_DS 8
#define PIN_SH_CP 4
#define PIN_ENABLE 7
#define PIN_ST_CP 12

#define BIT_MOTOR_1_A 2
#define BIT_MOTOR_1_B 3
#define BIT_MOTOR_2_A 1
#define BIT_MOTOR_2_B 4

#define PIN_MOTOR_2_PWM 3
#define PIN_MOTOR_1_PWM 11

#define COMPLETE_ROTATION 20

#define PIN_LASER_SENSOR_LEFT 0
#define PIN_LASER_SENSOR_RIGHT 1
#define PIN_LASER_SENSOR_CENTRAL 2

#define PIN_LASER_SENSOR_LEFT_BACK 3
#define PIN_LASER_SENSOR_RIGHT_BACK 4

unsigned int rotationStep = 0;
unsigned int completeRotations = 0;
unsigned int lastLaserSensorState = 0;

void setup() {
	Serial.begin(9600);

	pinMode(PIN_DS, OUTPUT);
	pinMode(PIN_SH_CP, OUTPUT);
	pinMode(PIN_ST_CP, OUTPUT);
	pinMode(PIN_ENABLE, OUTPUT);

	pinMode(PIN_MOTOR_1_PWM, OUTPUT);
	pinMode(PIN_MOTOR_2_PWM, OUTPUT);

	pinMode(PIN_LASER_SENSOR_LEFT, INPUT);
	pinMode(PIN_LASER_SENSOR_RIGHT, INPUT);
	pinMode(PIN_LASER_SENSOR_CENTRAL, INPUT);

	pinMode(PIN_LASER_SENSOR_LEFT_BACK, INPUT);
	pinMode(PIN_LASER_SENSOR_RIGHT_BACK, INPUT);

	digitalWrite(PIN_ENABLE, LOW);

	setMotorSpeed(PIN_MOTOR_1_PWM, 255);
	setMotorSpeed(PIN_MOTOR_2_PWM, 255);

	setMotorStatusByPin(BIT_MOTOR_1_B, LOW);
	setMotorStatusByPin(BIT_MOTOR_1_A, HIGH);

	setMotorStatusByPin(BIT_MOTOR_2_B, LOW);
	setMotorStatusByPin(BIT_MOTOR_2_A, HIGH);
}

void setMotorStatusByPin(byte pin, bool status) {
	static byte ciBuffer[QUANTITY_OF_CI];

	bitWrite(ciBuffer[pin / 8], pin % 8, status);

	digitalWrite(PIN_DS, LOW);
	digitalWrite(PIN_SH_CP, LOW);
	digitalWrite(PIN_ST_CP, LOW);

	for (unsigned int nC = QUANTITY_OF_CI - 1; nC >= 0; nC--) {
		for (unsigned int nB = 7; nB >= 0; nB--) {
			digitalWrite(PIN_SH_CP, LOW);
			digitalWrite(PIN_DS, bitRead(ciBuffer[nC], nB));
			digitalWrite(PIN_SH_CP, HIGH);
			digitalWrite(PIN_DS, LOW);
		}
	}

	digitalWrite(PIN_ST_CP, HIGH);
}

void setMotorSpeed(unsigned int pinPwmToUpdate, unsigned int speedValue) {
	if (speedValue < 25) speedValue = 25;
	if (speedValue > 255) speedValue = 255;
	analogWrite(pinPwmToUpdate, speedValue);
}

void checkIfCompleteRotation() {
	if (rotationStep == COMPLETE_ROTATION) {
		rotationStep = 0;
		completeRotations += 1;
	}
}

void startRunningEncoderVerification() {
	unsigned int laserSensorState = digitalRead(PIN_LASER_SENSOR_LEFT_BACK);
	if (laserSensorState == LOW) {
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

}

void loop() {
	startRunningEncoderVerification();
}
