package ilite.display.modules.display2012;
import static java.lang.Math.PI;
import static java.lang.Math.cos;
import static java.lang.Math.pow;
import static java.lang.Math.sin;

public class ShooterLineUpMath {
	
	double robotPosX; //Inches
	double robotPosZ; //Inches
	
	double[] hoopPosX; //Inches
	double hoopPosZ; //Inches
	
	double robotFacingAngle; //Degrees
	
	public ShooterLineUpMath(double robotPosX, double robotPosZ, double robotFacingAngle) {
		
		setRobotPosition(robotPosX, robotPosZ);
		setRobotFacing(robotFacingAngle);
		
		hoopPosX = new double[3];
		hoopPosX[0] = 134.625;
		hoopPosX[1] = 162;
		hoopPosX[2] = 189.375;
		
		hoopPosZ = 15;
	}
	
	void setRobotPosition(double robotPosX, double robotPosZ) {
		this.robotPosX = robotPosX;
		this.robotPosZ = 648 - robotPosZ;
	}
	
	void setRobotFacing(double robotFacingAngle) {
		this.robotFacingAngle = -robotFacingAngle + 90;
	}
	
	public int[] robotIsLinedUp() {
		int hoopLinedUpWith = 0;
		int quality = 0;
		
		boolean[] hoopLinedUp = new boolean[2];
		
		hoopLinedUp = testRobotInLine(2);
		if(hoopLinedUp[0]) {
			hoopLinedUpWith = 2;
			if(hoopLinedUp[1]) {
				quality = 2;
			} else {
				quality = 1;
			}
		} else {
			hoopLinedUp = testRobotInLine(1);
			if(hoopLinedUp[0]) {
				hoopLinedUpWith = 1;
				if(hoopLinedUp[1]) {
					quality = 2;
				} else {
					quality = 1;
				}
			} else {
				hoopLinedUp = testRobotInLine(3);
				if(hoopLinedUp[0]) {
					hoopLinedUpWith = 1;
					if(hoopLinedUp[1]) {
						quality = 2;
					} else {
						quality = 1;
					}
				}
			}
		}
		
		int[] returnInt = new int[2];
		returnInt[0] = hoopLinedUpWith;
		returnInt[1] = quality;
		
		return returnInt;
	}
	
	boolean[] testRobotInLine(int hoopNumber) {
		boolean insideHoop = false;
		boolean bestShot = false;
		double intersection;
		intersection = ( robotPosX * pow( sin(robotFacingAngle), 2 ) ) - ( hoopPosX[hoopNumber] * pow( cos(robotFacingAngle), 2 ) ) + ( (hoopPosZ - robotPosZ) * ( sin(robotFacingAngle) * cos(robotFacingAngle) ) );
		
		if( intersection <= (hoopPosX[hoopNumber] + 9) || intersection >= (hoopPosX[hoopNumber] - 9) ) {
			insideHoop = true;
			if( intersection <= (hoopPosX[hoopNumber] + 12/PI) || intersection >= (hoopPosX[hoopNumber] - 12/PI) ) {
				bestShot = true;
			}
		}
		
		boolean[] returnBool = new boolean[2];
		returnBool[0] = insideHoop;
		returnBool[1] = bestShot;
		
		
		return returnBool;
	}
}
