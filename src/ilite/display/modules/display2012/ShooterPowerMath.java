package ilite.display.modules.display2012;

public class ShooterPowerMath {

	double robotPosZ; //Inches
	double robotPosY; //Inches
	
	double robotHorizontalVelocity; //Inches
	double robotShooterVelocity; //Inches
	
	double[] hoopPosY; //Inches
	double hoopPosZ; //Inches
	
	double robotFacingAngle; //Degrees
	double shooterAngle; //Degrees
	
	public ShooterPowerMath (double robotPosZ, double robotPosY, double robotShooterVelocity, double robotFacingAngle, double shooterAngle) {
		
		setRobotPosition(robotPosZ, robotPosY);
		setShooterVelocity(robotShooterVelocity);
		setRobotFacing(robotFacingAngle);
		setShooterAngle(shooterAngle);
		
		hoopPosY = new double[3];
		hoopPosY[1] = 28;
		hoopPosY[2] = 61;
		hoopPosY[3] = 98;
		
		hoopPosZ = 15;
	}
	
	void setRobotPosition(double robotPosZ, double robotPosY) {
		this.robotPosZ = robotPosZ;
		this.robotPosZ = 648 - robotPosY;
	}
	
	void setShooterVelocity(double robotShooterVelocity) {
		this.robotShooterVelocity = robotShooterVelocity;
	}
	
	void setRobotFacing(double robotFacingAngle) {
		this.robotFacingAngle = -robotFacingAngle + 90;
	}
	
	void setShooterAngle(double shooterAngle) {
		this.shooterAngle = shooterAngle;
	}
	
	
}
