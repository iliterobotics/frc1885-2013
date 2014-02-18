package ilite.display.modules.display2012;

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;

@SuppressWarnings("serial")
public class ZYSideView extends Component {
	final int zMax = 486;
	final int yMax = 300;
	
	final int robotZSize = 10;
	final int robotYSize = 44;
	final int robotYPoint = yMax - robotYSize;
	
	final int hoopZSize = 9;
	final int hoopZPoint = zMax - hoopZSize - 6;
	
	final int hoopYSize = 4;
	final int hoopY1Point = (int) (yMax - 28 + .5 * hoopYSize);
	final int hoopY2Point = (int) (yMax - 61 + .5 * hoopYSize);
	final int hoopY3Point = (int) (yMax - 98 + .5 * hoopYSize);
	
	boolean error;
	
	int robotZPoint;
	int robotZCenter;
	int mainHoop;
	
	double baseBallVelocity;
	double robotFacing;
	double shooterAngle;
	
	Graphics graphicsEngine;
	
	ZYSideView() {
		update();
	}
	
	public void paint(Graphics g) {
		graphicsEngine = g;
		
		if(error) {
			drawError();
			drawHoops();
		} else {
			drawBackground();
			drawHoops();
			drawRobot();
			drawArc();
		}
		
	}
	
	void drawBackground() {
		graphicsEngine.setColor(Color.WHITE);
		graphicsEngine.fillRect(0, 0, zMax, yMax);
	}
	
	void drawError() {
		graphicsEngine.setColor(Color.PINK);
		graphicsEngine.fillRect(0, 0, zMax, yMax);
	}
	
	void drawHoops() {
		graphicsEngine.setColor(new Color(0, 255, 0, 85));
		
		switch(mainHoop) {
		case 1:
			graphicsEngine.fillRect(hoopZPoint, hoopY1Point, hoopZSize, hoopYSize);
			
			graphicsEngine.setColor(Color.GRAY);
			graphicsEngine.fillRect(hoopZPoint, hoopY2Point, hoopZSize, hoopYSize);
			graphicsEngine.fillRect(hoopZPoint, hoopY3Point, hoopZSize, hoopYSize);
			break;
		case 2:
			graphicsEngine.fillRect(hoopZPoint, hoopY2Point, hoopZSize, hoopYSize);
			
			graphicsEngine.setColor(Color.GRAY);
			graphicsEngine.fillRect(hoopZPoint, hoopY1Point, hoopZSize, hoopYSize);
			graphicsEngine.fillRect(hoopZPoint, hoopY3Point, hoopZSize, hoopYSize);
			break;
		case 3:
			graphicsEngine.fillRect(hoopZPoint, hoopY3Point, hoopZSize, hoopYSize);
			
			graphicsEngine.setColor(Color.GRAY);
			graphicsEngine.fillRect(hoopZPoint, hoopY1Point, hoopZSize, hoopYSize);
			graphicsEngine.fillRect(hoopZPoint, hoopY2Point, hoopZSize, hoopYSize);
			break;
		case -1:
			graphicsEngine.setColor(Color.GRAY);
			graphicsEngine.fillRect(hoopZPoint, hoopY1Point, hoopZSize, hoopYSize);
			graphicsEngine.fillRect(hoopZPoint, hoopY2Point, hoopZSize, hoopYSize);
			graphicsEngine.fillRect(hoopZPoint, hoopY3Point, hoopZSize, hoopYSize);
		}
	}
	
	void drawRobot() {
		graphicsEngine.setColor(new Color(160, 0, 255));
		graphicsEngine.fillRect(robotZPoint, robotYPoint, robotZSize, robotYSize);
	}
	
	void drawArc() {
		graphicsEngine.setColor(Color.DARK_GRAY);
		double lineZCoordinate = robotZPoint;
		double lineYCoordinate = robotYPoint;
		
		double currentRobotFacing = Math.PI * robotFacing / 180;
		double currentShooterAngle = Math.PI * shooterAngle / 180;
		
		double current2DBallVelocity = compute2DVelocity(baseBallVelocity, currentRobotFacing);
		double currentYBallVelocity = computeYVelocity(current2DBallVelocity, currentShooterAngle);
		double currentZBallVelocity = computeZVelocity(current2DBallVelocity, currentShooterAngle);
		
		int lineZCoordinateChange = 1;
		int totalZChange = 0;
		
		for(;lineZCoordinate <= zMax && lineZCoordinate >= 0 && lineYCoordinate <= yMax; lineZCoordinate += lineZCoordinateChange) {
			totalZChange += 1;
			double newZ = lineZCoordinate;
			double newY = computeYDistancePerZPx(currentZBallVelocity, totalZChange, currentYBallVelocity);
			graphicsEngine.drawLine((int)lineZCoordinate, (int)lineYCoordinate, (int)newZ, (int)newY);
			lineZCoordinate = newZ;
			lineYCoordinate = newY;
		}
	}
	
	double compute2DVelocity(double aBaseBallVelocity, double aCurrentRobotFacing) {
		return aBaseBallVelocity * Math.sin(aCurrentRobotFacing);
	}
	
	double computeYVelocity(double a2DBallVelocity, double aCurrentShooterAngle) {
		return a2DBallVelocity * Math.sin(aCurrentShooterAngle);
	}
	
	double computeZVelocity(double a2DBallVelocity, double aCurrentShooterAngle) {
		return a2DBallVelocity * Math.cos(aCurrentShooterAngle);
	}
	
	double computeYDistancePerZPx(double aZBallVelocity, double alineZCoordinateChange, double aYBallVelocity) {
		double time = alineZCoordinateChange/aZBallVelocity;
		double gravity = -386.0892;
		double yDistance = (aYBallVelocity * time) + (gravity * time * time / 2);
		
		return yMax - yDistance - robotYSize;
	}
	
	public Dimension getPreferredSize() {
		return new Dimension(zMax, yMax);
	}
	
	public void update()
	{
		robotZCenter = (int) (zMax - GlobalData2012.gmRobotZCoord);
		robotZPoint = robotZCenter - robotZSize/2;
		baseBallVelocity = GlobalData2012.gmComputedBallVelocity;
		robotFacing = -GlobalData2012.gmHorizontalGyroAngle + 90;
		shooterAngle = GlobalData2012.gmShooterAngle;
		
		if(GlobalData2012.gmTopHoopDetected) {
			mainHoop = 3;
		} else if(GlobalData2012.gmMidRightHoopDetected || GlobalData2012.gmMidLeftHoopDetected) {
			mainHoop = 2;
		} else if(GlobalData2012.gmBottomHoopDetected) {
			mainHoop = 1;
		} else {
			mainHoop = -1;
			error = true;
		}
		
		repaint();
	}
}
