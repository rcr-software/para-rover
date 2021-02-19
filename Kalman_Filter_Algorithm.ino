
#include <stlport.h>

#include <Eigen30.h>
#include <EigenAVR.h>

using Eigen::MatrixXd;

MatrixXd P(2,2);
MatrixXd K(2,1); // Kalman gain - This is a 2x1 vector
  
  //delcaration
float rate = 0;
float bias = 0;
float angle = 0;
float dt = 0;
  //I have found that the following variances works perfectly for most IMUs
float Q_angle = 0.001;
float Q_gyroBias = 0.003;
float R_measure = 0.03;


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
void Kalman(float newAngle, float newRate, float dt) {
    
    /* Step 1 */
    
    rate = newRate - bias;
    angle += dt * rate;

    // Update estimation error covariance - Project the error covariance ahead
    /* Step 2 */
    P(0,0) += dt * (dt*P(1,1) - P(0,1) - P(1,0) + Q_angle);
    P(0,1) -= dt * P(1,1);
    P(1,0) -= dt * P(1,1);
    P(1,1) += Q_gyroBias * dt;

    // Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
    // Calculate Kalman gain - Compute the Kalman gain
    /* Step 4 */
    float S = P(0,0) + R_measure; // Estimate error
    /* Step 5 */
    // Kalman gain - This is a 2x1 vector
    K(0) = P(0,0) / S;
    K(1) = P(1,0) / S;

    // Calculate angle and bias - Update estimate with measurement zk (newAngle)
    /* Step 3 */
    float y = newAngle - angle; // Angle difference
    /* Step 6 */
    angle += K(0) * y;
    bias += K(1) * y;

    // Calculate estimation error covariance - Update the error covariance
    /* Step 7 */
    float P00_temp = P(0,0);
    float P01_temp = P(0,1);

    P(0,0) -= K(0) * P00_temp;
    P(0,1) -= K(0) * P01_temp;
    P(1,0) -= K(1) * P00_temp;
    P(1,1) -= K(1) * P01_temp;

    return angle;
};
