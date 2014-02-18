package ilite.display.modules.display2013;

import ilite.display.modules.display2013.data.RobotData2013;
import ilite.util.lang.IUpdate;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;

@SuppressWarnings("serial")
public class AutonomousPanel extends JPanel implements IUpdate<RobotData2013>
{
	int height, width;
	Font font;
	Color colorComplete;
	Color colorCurrent;
	Color colorIncomplete;
	
	JLabel s1Angle;
	JLabel s1HopState;
	JLabel s1PneumaticState;
	
	JLabel s2Wheel;
	JLabel s2HopState;
	JLabel s2SpatulaState;
	
	JLabel s3Speed;
	JLabel s3KickerState;
	
	JLabel s4Encoder;
	JLabel s4HopState;
	JLabel s4SpatulaState;
	
	JLabel s5SpatulaState;
	
	JLabel s6Encoder;
	JLabel s6SpatulaState;
	
	JLabel s7SpatulaState;
	
	//Removed step 8 because it was obsolete.
	
	JLabel s9MoveBack;
	JLabel s9Angle;
	JLabel s9Speed;
	JLabel s9HopState;
	
	JLabel s10HopState;
	
	JLabel s11KickerSpeed;
	
	public AutonomousPanel()
	{
		height = 430;
		width = 640;
		font = new Font("Comic Sans MS", Font.PLAIN, 16);
		colorComplete = Color.GREEN;
		colorCurrent = Color.BLUE;
		colorIncomplete = Color.red;
		setPreferredSize(new Dimension(width, height));
		setVisible(true);
		setBorder(BorderFactory.createLineBorder(Color.DARK_GRAY, 5));
		underGoal7();
	}
	
	public void underGoal7()
	{
		Dimension d = new Dimension(640, 30);
		JPanel stp1 = new JPanel();
		JPanel stp2 = new JPanel();
		JPanel stp3 = new JPanel();
		JPanel stp4 = new JPanel();
		JPanel stp5 = new JPanel();
		JPanel stp6 = new JPanel();
		JPanel stp7 = new JPanel();
		JPanel stp9 = new JPanel();
		JPanel stp10 = new JPanel();
		JPanel stp11 = new JPanel();
		
		stp1.setPreferredSize(d);
		stp2.setPreferredSize(d);
		stp3.setPreferredSize(d);
		stp4.setPreferredSize(d);
		stp5.setPreferredSize(d);
		stp6.setPreferredSize(d);
		stp7.setPreferredSize(d);
		stp9.setPreferredSize(d);
		stp10.setPreferredSize(d);
		stp11.setPreferredSize(d);
		
		JLabel s1 = new JLabel("Step 1: Initialization -- ");
		JLabel s2 = new JLabel("Step 2: Begin Play -- ");
		JLabel s3 = new JLabel("Step 3: Kicker Speed -- ");
		JLabel s4 = new JLabel("Step 4: Move and Collect -- ");
		JLabel s5 = new JLabel("Step 5: Spatula -- ");
		JLabel s6 = new JLabel("Step 6: Move and Collect -- ");
		JLabel s7 = new JLabel("Step 7: Collection -- ");
		JLabel s9 = new JLabel("Step 8: Prepare for Launch -- ");
		JLabel s10 = new JLabel("Step 9: Disc Prep -- ");
		JLabel s11 = new JLabel("Step 10: FIRE! -- ");
		
		//create the labels, by panel.
		s1Angle = new JLabel("[ANGLE]");
		s1HopState = new JLabel("[HOP STATE]");
		s1PneumaticState = new JLabel("[FINGER STATE]");
		
		s2Wheel = new JLabel("[WHEEL SPEED]");
		s2HopState = new JLabel("[HOP STATE]");
		s2SpatulaState = new JLabel("[SPATULA STATE]");
		
		s3Speed = new JLabel("[SPEED]");
		s3KickerState = new JLabel("[KICKER STATE]");
		
		s4Encoder = new JLabel("[ENCODER DIST]");
		s4HopState = new JLabel("[HOP STATE]");
		s4SpatulaState = new JLabel("[SPATULA STATE]");
		
		s5SpatulaState = new JLabel("[SPATULA STATE]");
		
		s6Encoder = new JLabel("[ENCODER DIST]");
		s6SpatulaState = new JLabel("[SPATULA STATE]");
		
		s7SpatulaState = new JLabel("[SPATULA STATE]");
		
		s9MoveBack = new JLabel("[MOVE BACK]");
		s9Angle = new JLabel("[ANGLE]");
		s9Speed = new JLabel("[SPEED]");
		s9HopState = new JLabel("[HOP STATE]");
		
		s10HopState = new JLabel("[HOP STATE]");
		
		s11KickerSpeed = new JLabel("[KICKER SPEED]");
		
		//setting color and size
		s1Angle.setFont(font);
		s1HopState.setFont(font);
		s1PneumaticState.setFont(font);
		s1Angle.setForeground(colorCurrent);
		s1HopState.setForeground(colorCurrent);
		s1PneumaticState.setForeground(colorCurrent);
		
		s2Wheel.setFont(font);
		s2HopState.setFont(font);
		s2SpatulaState.setFont(font);
		s2Wheel.setForeground(colorIncomplete);
		s2HopState.setForeground(colorIncomplete);
		s2SpatulaState.setForeground(colorIncomplete);
		
		s3KickerState.setFont(font);
		s3Speed.setFont(font);
		s3KickerState.setForeground(colorIncomplete);
		s3Speed.setForeground(colorIncomplete);
		
		s4Encoder.setFont(font);
		s4HopState.setFont(font);
		s4SpatulaState.setFont(font);
		s4Encoder.setForeground(colorIncomplete);
		s4HopState.setForeground(colorIncomplete);
		s4SpatulaState.setForeground(colorIncomplete);
		
		s5SpatulaState.setFont(font);
		s5SpatulaState.setForeground(colorIncomplete);
		
		s6Encoder.setFont(font);
		s6SpatulaState.setFont(font);
		s6Encoder.setForeground(colorIncomplete);
		s6SpatulaState.setForeground(colorIncomplete);
		
		s7SpatulaState.setFont(font);
		s7SpatulaState.setForeground(colorIncomplete);
		
		s9MoveBack.setFont(font);
		s9Angle.setFont(font);
		s9Speed.setFont(font);
		s9HopState.setFont(font);
		s9MoveBack.setForeground(colorIncomplete);
		s9Angle.setForeground(colorIncomplete);
		s9Speed.setForeground(colorIncomplete);
		s9HopState.setForeground(colorIncomplete);
		
		s10HopState.setFont(font);
		s10HopState.setForeground(colorIncomplete);
		
		s11KickerSpeed.setFont(font);
		s11KickerSpeed.setForeground(colorIncomplete);
		
		//add the labels, by panel. 
		stp1.add(s1);
		stp1.add(s1Angle);
		stp1.add(s1HopState);
		stp1.add(s1PneumaticState);
		
		stp2.add(s2);
		stp2.add(s2Wheel);
		stp2.add(s2HopState);
		stp2.add(s2SpatulaState);
		
		stp3.add(s3);
		stp3.add(s3KickerState);
		stp3.add(s3Speed);
		
		stp4.add(s4);
		stp4.add(s4Encoder);
		stp4.add(s4HopState);
		stp4.add(s4SpatulaState);
		
		stp5.add(s5);
		stp5.add(s5SpatulaState);
		
		stp6.add(s6);
		stp6.add(s6SpatulaState);
		stp6.add(s6Encoder);
		
		stp7.add(s7);
		stp7.add(s7SpatulaState);
		
		stp9.add(s9);
		stp9.add(s9Speed);
		stp9.add(s9HopState);
		stp9.add(s9Angle);
		stp9.add(s9MoveBack);
		
		stp10.add(s10);
		stp10.add(s10HopState);
		
		stp11.add(s11);
		stp11.add(s11KickerSpeed);
		
		
		//adding the various panels to the containing panel
		add(stp1);
		add(stp2);
		add(stp3);
		add(stp4);
		add(stp5);
		add(stp6);
		add(stp7);
		add(stp9);
		add(stp10);
		add(stp11);
		
		update(new RobotData2013());
	}

	@Override
	public void update(RobotData2013 pObject) {
		// TODO Auto-generated method stub
		s1Angle.setText(pObject.sensors.mTiltPotentiometerValue + "");
		s1HopState.setText(pObject.pneumatics.mHopperState + "");
//		s1PneumaticState.setText(!(pObject.mCamForward & pObject.mCamReverse) + ""); // TODO fix this to use pneumatics
		if(pObject.pneumatics.mHopperState)
		{
	
			s1Angle.setForeground(colorComplete);
			s1HopState.setForeground(colorComplete);
			s1PneumaticState.setForeground(colorComplete);
			
			s2Wheel.setForeground(colorCurrent);
			s2HopState.setForeground(colorCurrent);
			s2SpatulaState.setForeground(colorCurrent);
		}
		
		s2Wheel.setText(pObject.motors.mDrivetrainPowerLeft + pObject.motors.mDrivetrainPowerRight + "");
		s2HopState.setText(pObject.pneumatics.mHopperState + "");
		s2SpatulaState.setText(pObject.pneumatics.mSpatulaState + "");
		if(pObject.pneumatics.mSpatulaState)
		{
	
			s2Wheel.setForeground(colorComplete);
			s2HopState.setForeground(colorComplete);
			s2SpatulaState.setForeground(colorComplete);
			
			s3KickerState.setForeground(colorCurrent);
			s3Speed.setForeground(colorCurrent);
		}
		
		s3Speed.setText(pObject.motors.mLaunchWheelMotorPower + "");
		s3KickerState.setText(pObject.pneumatics.mLaunchKickerState + "");
		if(pObject.pneumatics.mLaunchKickerState)
		{
			s3KickerState.setForeground(colorComplete);
			s3Speed.setForeground(colorComplete);
			
			s4Encoder.setForeground(colorCurrent);
			s4HopState.setForeground(colorCurrent);
			s4SpatulaState.setForeground(colorCurrent);
		}
		
		s4Encoder.setText(pObject.sensors.mDrivetrainLeftEncoderValue + pObject.sensors.mDrivetrainRightEncoderValue + "");
		s4HopState.setText(pObject.pneumatics.mHopperState + "");
		s4SpatulaState.setText(pObject.pneumatics.mSpatulaState + "");
		if(pObject.pneumatics.mHopperState)
		{
			s4Encoder.setForeground(colorComplete);
			s4HopState.setForeground(colorComplete);
			s4SpatulaState.setForeground(colorComplete);
			
			s5SpatulaState.setForeground(colorCurrent);
		}
		
		s5SpatulaState.setText(pObject.pneumatics.mSpatulaState + "");
		if(pObject.pneumatics.mSpatulaState)
		{
			s5SpatulaState.setForeground(colorComplete);
			
			s6Encoder.setForeground(colorCurrent);
			s6SpatulaState.setForeground(colorCurrent);
		}
		
		s6Encoder.setText(pObject.sensors.mDrivetrainLeftEncoderValue + pObject.sensors.mDrivetrainRightEncoderValue + "");
		s6SpatulaState.setText(pObject.pneumatics.mSpatulaState + "");
		if(pObject.pneumatics.mSpatulaState)
		{
			s6Encoder.setForeground(colorComplete);
			s6SpatulaState.setForeground(colorComplete);

			s7SpatulaState.setForeground(colorCurrent);
		}
		
		s7SpatulaState.setText(pObject.pneumatics.mSpatulaState + "");
		if(pObject.pneumatics.mSpatulaState)
		{
			s7SpatulaState.setForeground(colorComplete);
			
			s9MoveBack.setForeground(colorCurrent);
			s9Angle.setForeground(colorCurrent);
			s9Speed.setForeground(colorCurrent);
			s9HopState.setForeground(colorCurrent);
		}
		
		s9MoveBack.setText(pObject.sensors.mDrivetrainLeftEncoderValue + pObject.sensors.mDrivetrainRightEncoderValue + "");
		s9Angle.setText(pObject.sensors.mTiltPotentiometerValue + "");
		s9Speed.setText(pObject.motors.mDrivetrainPowerLeft + pObject.motors.mDrivetrainPowerRight + "");
		s9HopState.setText(pObject.pneumatics.mHopperState + "");
		if(pObject.pneumatics.mHopperState)
		{
			s9MoveBack.setForeground(colorComplete);
			s9Angle.setForeground(colorComplete);
			s9Speed.setForeground(colorComplete);
			s9HopState.setForeground(colorComplete);

			s10HopState.setForeground(colorCurrent);
		}
		
		s10HopState.setText(pObject.pneumatics.mHopperState + "");
		if(pObject.pneumatics.mHopperState)
		{
			s10HopState.setForeground(colorComplete);

			s11KickerSpeed.setForeground(colorCurrent);
		}
		
		s11KickerSpeed.setText(pObject.motors.mLaunchWheelMotorPower + "");
		s11KickerSpeed.setForeground(colorComplete);
	}
	
}