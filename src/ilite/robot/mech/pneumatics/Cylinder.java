package ilite.robot.mech.pneumatics;

import ilite.util.math.concepts.measurements.Area;
import ilite.util.math.concepts.measurements.Length;
import ilite.util.math.concepts.measurements.Volume;
import ilite.util.physics.concepts.Force;
import ilite.util.physics.concepts.Pressure;

public class Cylinder
{
  private final Length mStroke;
  private final Area mBore;
  private final Area mRodArea;
  private Pressure mWorkingPressurePSI = Pressure.fromPSI(60d);
  
  private Volume mInVol;
  private Volume mOutVol;
  private Force mInForce;
  private Force mOutForce;
  
  public Cylinder(Length pStroke, Length pBoreDiameter, Length pRodDiameter)
  {
    mStroke = pStroke;
    mBore = Area.fromCircleDiameter(pBoreDiameter);
    mRodArea = Area.fromCircleDiameter(pRodDiameter);
    recalculate();
  }

  public void setWorkingPressure(Pressure pPressure)
  {
    mWorkingPressurePSI = pPressure;
    recalculate();
  }
  
  public Force getForceExtend()
  {
    return mOutForce;
  }
  
  public Volume getVolumeExtend()
  {
    return mOutVol;
  }
  
  public Force getForceRetract()
  {
    return mInForce;
  }
  
  public Volume getVolumeRetract()
  {
    return mInVol;
  }
  
  private void recalculate()
  {
    double outforce = mWorkingPressurePSI.getPSI() * mBore.getSqInches();
    double inforce = mWorkingPressurePSI.getPSI() * mBore.subtract(mRodArea).getSqInches();
    
    mInForce = Force.fromPounds(inforce);
    mOutForce = Force.fromPounds(outforce);
    mOutVol = Volume.fromAreaAndLength(mBore, mStroke);
    mInVol = Volume.fromAreaAndLength(mBore.subtract(mRodArea), mStroke);
    
  }
}
