#ifndef BHS_COLORTARGETANALYSIS_H
#define BHS_COLORTARGETANALYSIS_H
/* CLASS NAME: 
 * 				bhs_ColorTargetAnalysis
 * DESCRIPTION:
 * 				This class performs taqrget analysis for finding the target in
 * 				the 2009 FRC Robotics competition.  This class is designed to
 * 				work with RGB image objects
 * 
 * 				Usage:  
 * 						1 - Instantiate class instance
 * 						2 - setTargetImage
 * 						3 - Call FindTargets to find target(s) in the passed in image
 * 
 */

#include "nivision.h"

class bhs_ColorTargetAnalysis {
public:
	bhs_ColorTargetAnalysis();

	~bhs_ColorTargetAnalysis();

	/* FUNCTION NAME:	setTargetImage
	 * DESCRIPTION	:	
	 * PARAMETERS	:
	 * 	apFileName - Name of file containing the target image
	 * RETURN		:	NA
	 */
	int setTargetImage(char* apFileName);

	/* FUNCTION NAME:	setTargetImage
	 * DESCRIPTION	:	
	 * PARAMETERS	:
	 * 	apImage - pointer to target image
	 * RETURN		:	NA
	 */
	int setTargetImage(Image* apImage);

	/* FUNCTION NAME:	FindTargets
	 * DESCRIPTION	:	Find target(s) in an image
	 * PARAMETERS	:
	 * 	apImage - Image to find Targets in
	 *  anNumMatches - How many matches in the return pointer. 
	 * 					This is set by the FindTarget Function (Pass by reference)
	 * RETURN		:	NULL on a failure or no targets 		OR
	 * 					struct with targets that match the Image in setTargetImage
	 */
	PatternMatch* FindTargets(Image* apImage, int& anNumMatches);

private:

	/* FUNCTION NAME:	removeExistingImage
	 * DESCRIPTION	:	Cleans up Image objects if needed
	 * PARAMETERS	:	NA
	 * RETURN		:	NA
	 */
	void removeExistingImage();

	/* FUNCTION NAME:	learnTarget
	 * DESCRIPTION	:	sets up the NI vision layer to learn how to find the
	 * 					target in an image
	 * PARAMETERS	:	NA
	 * RETURN		:	NA
	 */
	int learnTarget();
	
	Image* mpTargetImage; //pointer to target image
	
	LearnColorPatternOptions mcLearnOptions;//struct describing how to process target image
	
	//Array containing the valid rotation angles for the target image
	RotationAngleRange maMatchRotationAngles[2];
	
	//struct describing how to find the target image in another image
	MatchColorPatternOptions mcMatchTargetOptions;

	//internal flag indicating if mpTargetImage needs to be garbage collected
	bool mbRemoveExistingImage;



};

#endif
