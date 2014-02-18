package ilite.display.modules.display2012;


public class ShooterLineUpCalc {
	final int LEFTHOOP = 0;
	final int CENTERHOOP = 1;
	final int RIGHTHOOP = 2;
	
	final int XVALUE = 0;
	final int ZVALUE = 1;
	
	double mRobotPosX; //Inches
	double mRobotPosZ; //Inches
	
	double[][] mTargetPos; //Inches
	
	double mRobotFacingAngle; //Degrees
	
	public ShooterLineUpCalc(double robotPosX, double robotPosZ, double robotFacingAngle) {
		
		setRobotPosition(robotPosX, robotPosZ);
		setRobotFacing(robotFacingAngle);
		
		mTargetPos = new double[5][2];
		configNewTarget(LEFTHOOP, 134.625, 633);
		configNewTarget(CENTERHOOP, 162, 633);
		configNewTarget(RIGHTHOOP, 189.375, 633);
	}
	
	void configNewTarget(int aTargetNumber, double aTargetPosX, double aTargetPosZ ) {
		mTargetPos[aTargetNumber][0] = aTargetPosX;
		mTargetPos[aTargetNumber][1] = aTargetPosZ;
	}
	
	void setRobotPosition(double aRobotPosX, double aRobotPosZ) {
		this.mRobotPosX = aRobotPosX;
		this.mRobotPosZ = aRobotPosZ;
	}
	
	void setRobotFacing(double aRobotFacingAngle) {
		this.mRobotFacingAngle = aRobotFacingAngle;
	}
	
	public double getOffsetDegrees(int aTargetNumber) {
		double aXDifference = mTargetPos[aTargetNumber][XVALUE] - mRobotPosX;
		double aZDifference = mTargetPos[aTargetNumber][ZVALUE] - mRobotPosZ;
		
		double aPartialOrientation = 90 - ( Math.toDegrees(Math.atan(aZDifference / aXDifference)) );
		double aOrientationOffset = aPartialOrientation - mRobotFacingAngle;
		
		return aOrientationOffset;
	}
	
	public double getOffsetMagnitude(int aTargetNumber) {
		double aXDifference = mTargetPos[aTargetNumber][XVALUE] - mRobotPosX;
		double aZDifference = mTargetPos[aTargetNumber][ZVALUE] - mRobotPosZ;
		
		double aDistanceFromTarget = Math.sqrt(Math.pow(aZDifference, 2) + Math.pow(aXDifference, 2));
		double aAngleOffset = Math.toRadians(getOffsetDegrees(aTargetNumber));
		double aOffsetMagnitude = aDistanceFromTarget * Math.sin(aAngleOffset) / Math.sin(Math.PI - aAngleOffset - Math.atan(aZDifference / aXDifference));
		 
		return aOffsetMagnitude;
	}
	
	public boolean onTarget(int aTargetNumber) {
		return getOffsetDegrees(aTargetNumber) == 0;
	}
	
	public double horizontalDistanceToTarget(int aTargetNumber) {
		double aDistanceFromTarget;
		if(onTarget(aTargetNumber)) {
			double aXDifference = mTargetPos[aTargetNumber][XVALUE] - mRobotPosX;
			double aZDifference = mTargetPos[aTargetNumber][ZVALUE] - mRobotPosZ;
			
			aDistanceFromTarget = Math.sqrt(Math.pow(aZDifference, 2) + Math.pow(aXDifference, 2));
		} else {
			aDistanceFromTarget = -1;
		}
		
		return aDistanceFromTarget;
	}
	
	public static void main(String[] args) {
		/*JFrame frame = new JFrame();
		
		frame.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				System.out.println("Terminated.");
				System.exit(0);
			}
		});
		
		frame.setSize(400, 600);
		frame.setVisible(true);
		frame.setResizable(true);
		
		TestView view = new TestView();
		
		
		view.setRobotPosition(100, 200);
		view.setRobotFacing(20);
		
		frame.add(view);*/
		ShooterLineUpCalc calculator = new ShooterLineUpCalc(162, 100, 45);
		System.out.println(calculator.getOffsetDegrees(1));
		System.out.println(calculator.getOffsetMagnitude(1));
	}
}
