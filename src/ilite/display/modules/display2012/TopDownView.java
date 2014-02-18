package ilite.display.modules.display2012;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;

import javax.swing.JComponent;

@SuppressWarnings("serial")
public class TopDownView extends JComponent {
	final int maxX = 324;
	final int maxZ = 648;
	final int robotXSize = 27;
	final int robotZSize = 37;
	final int hoopRadius = 9;
	final double hoopDisplacement = 27.375;
	final int rampXSize = 48;
	final int rampZSize = 88;
	final int lineOffset = 180;
	
	double rotation;
	double robotX;
	double robotZ;
	
	int[] robotXVertices;
	int[] robotYVertices;
	
	Graphics graphicsEngine;
	
	public TopDownView()
	{
		robotXVertices = new int[4];
		robotYVertices = new int[4];
	}
	
	public void paint(Graphics g) {
		graphicsEngine = g;
		
		drawField();
		drawRobot();
		drawHoops();
		drawAimLine();
	}
	
	public void update()
	{
		robotX = GlobalData2012.gmRobotXCoord;
		robotZ = GlobalData2012.gmRobotZCoord;
		rotation = -GlobalData2012.gmHorizontalGyroAngle + 90;
		repaint();
	}
	
	void drawField() {
		graphicsEngine.setColor(Color.WHITE);
		graphicsEngine.fillRect(0, 0, maxX, maxZ);
		
		graphicsEngine.setColor(Color.BLACK);
		graphicsEngine.fillRect(0, maxZ/2, maxX, 2);
		
		graphicsEngine.setColor(Color.BLUE);
		graphicsEngine.fillRect(0, maxZ/2 - rampZSize/2, rampXSize, rampZSize);
		graphicsEngine.setColor(Color.BLACK);
		graphicsEngine.fillRect(maxX/2 - rampXSize/2, 0 + maxZ/2 - rampZSize/2, rampXSize, rampZSize);
		graphicsEngine.setColor(Color.RED);
		graphicsEngine.fillRect(maxX - rampXSize, 0 + maxZ/2 - rampZSize/2, rampXSize, rampZSize);
	}
	
	void drawHoops() {
		graphicsEngine.setColor(Color.GREEN);
		graphicsEngine.fillOval((int) (maxX/2 + hoopDisplacement), 0, hoopRadius, hoopRadius);
		graphicsEngine.fillOval(maxX/2, 0, hoopRadius, hoopRadius);
		graphicsEngine.fillOval((int) (maxX/2 - hoopDisplacement), 0, hoopRadius, hoopRadius);
	}
	
	void drawRobot()
	{
		robotXVertices[0] = (int) (robotX - (Math.cos (Math.PI * rotation / 180) * (robotZSize / 2) ) );
		robotYVertices[0] = (int) (robotZ + (Math.sin (Math.PI * rotation / 180) * (robotZSize / 2) ) );
		
		robotXVertices[1] = (int) (robotX + (Math.cos (Math.PI * rotation / 180) * (robotZSize / 2) ) );
		robotYVertices[1] = (int) (robotZ - (Math.sin (Math.PI * rotation / 180) * (robotZSize / 2) ) );
		
		robotXVertices[2] = robotXVertices[1] + (int)(Math.sin (Math.PI * rotation / 180) * (robotXSize) );
		robotYVertices[2] = robotYVertices[1] + (int)(Math.cos (Math.PI * rotation / 180) * (robotXSize) );
		
		robotXVertices[3] = robotXVertices[0] + (int)(Math.sin (Math.PI * rotation / 180) * (robotXSize) );
		robotYVertices[3] = robotYVertices[0] + (int)(Math.cos (Math.PI * rotation / 180) * (robotXSize) );		
		
		//robot polygon
		graphicsEngine.setColor(new Color(160, 0, 255));
		graphicsEngine.fillPolygon(robotXVertices, robotYVertices, 4);
		
	}
	
	void drawAimLine() {
		graphicsEngine.setColor(Color.BLACK);
		for(int i = 0; i < 2000; i += 15)
		{
			graphicsEngine.drawLine( (int)( robotX - ( Math.cos ( Math.PI * ( -rotation + lineOffset / 2 ) / 180 ) * i ) ) ,
					(int)( robotZ - ( Math.sin ( Math.PI * ( -rotation + lineOffset / 2 ) / 180 ) * i ) ) ,
					(int)( robotX - ( Math.cos ( Math.PI * ( -rotation + lineOffset / 2 ) / 180 ) * (i-5) ) ) ,
					(int)( robotZ - ( Math.sin ( Math.PI * ( -rotation + lineOffset / 2 ) / 180 ) * (i-5) ) ) 
						);
		}
	}
	
	public Dimension getPreferredSize() {
		return new Dimension(maxX, maxZ);
	}
}