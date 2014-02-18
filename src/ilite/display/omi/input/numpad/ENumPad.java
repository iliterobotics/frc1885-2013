package ilite.display.omi.input.numpad;


public enum ENumPad {
	//Order Matters!
	SEVEN("7"),
	EIGHT("8"),
	NINE("9"),
	FOUR("4"),
	FIVE("5"),
	SIX("6"),
	ONE("1"),
	TWO("2"),
	THREE("3"),
	BKSPC("<"),
	ZERO("0"),
	NULL("C");
	
	@Override
  public String toString(){
		return mLabel;
	}
	
	private String mLabel;
	
	private ENumPad(String pLabel){
		mLabel = pLabel;
	}
}
