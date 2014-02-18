#include "bhs_ColorTargetAnalysis.h"

bhs_ColorTargetAnalysis::bhs_ColorTargetAnalysis()
:mpTargetImage(NULL),
mbRemoveExistingImage(false)
{
	mcLearnOptions.learnMode = IMAQ_LEARN_ALL;
	mcLearnOptions.featureMode = IMAQ_COLOR_AND_SHAPE_FEATURES;
	mcLearnOptions.threshold = 0;
	mcLearnOptions.ignoreMode = IMAQ_IGNORE_BLACK_AND_WHITE;
	mcLearnOptions.colorsToIgnore = NULL;
	mcLearnOptions.numColorsToIgnore = 0;
	
	maMatchRotationAngles[0].lower = -10;
	maMatchRotationAngles[0].upper = 10;
	maMatchRotationAngles[1].lower = 170;
	maMatchRotationAngles[1].upper = 190;
	

	mcMatchTargetOptions.matchMode = IMAQ_MATCH_ROTATION_INVARIANT;
	mcMatchTargetOptions.featureMode = IMAQ_COLOR_AND_SHAPE_FEATURES;
	mcMatchTargetOptions.minContrast = 20;
	mcMatchTargetOptions.subpixelAccuracy = 0;
	mcMatchTargetOptions.angleRanges = maMatchRotationAngles;
	mcMatchTargetOptions.numRanges = 2;
	mcMatchTargetOptions.colorWeight = 500;
	mcMatchTargetOptions.sensitivity = IMAQ_SENSITIVITY_LOW;
	mcMatchTargetOptions.strategy = IMAQ_AGGRESSIVE;
	mcMatchTargetOptions.numMatchesRequested = 5;
	mcMatchTargetOptions.minMatchScore = 500;
}

bhs_ColorTargetAnalysis::~bhs_ColorTargetAnalysis()
{
	removeExistingImage();
}

void bhs_ColorTargetAnalysis::removeExistingImage()
{
	if(mpTargetImage != NULL && mbRemoveExistingImage == true)
	{
		imaqDispose(mpTargetImage);
		mpTargetImage = NULL;
	}
}

int bhs_ColorTargetAnalysis::setTargetImage(char* apFileName)
{
	removeExistingImage();
	mpTargetImage = imaqCreateImage(IMAQ_IMAGE_RGB,3);
	int lnReturn = imaqReadFile(mpTargetImage,apFileName,NULL,NULL);
	
	//we created image object
	//if object needs to be replace, class instance needs to clean 
	//up memory
	mbRemoveExistingImage = true;

	if(lnReturn != 0){
		lnReturn = learnTarget();
	}
	
	return lnReturn;
	
}

int bhs_ColorTargetAnalysis::setTargetImage(Image* apImage)
{
	removeExistingImage();
	
	mpTargetImage = apImage;
	
	//no need to remove, class instance did not create it
	mbRemoveExistingImage = false;
	
	
	
	return learnTarget(); 
}

int bhs_ColorTargetAnalysis::learnTarget()
{
	int lnReturn = 0;
	if(mpTargetImage != NULL){	
		lnReturn = imaqLearnColorPattern(mpTargetImage,
				&mcLearnOptions);
	}
	
	return lnReturn;
}

PatternMatch* bhs_ColorTargetAnalysis::FindTargets(Image* apCurrentImage,
		int& anNumMatches)
{
	PatternMatch* lpMatches = NULL;
	if(apCurrentImage != NULL && mpTargetImage != NULL)
	{	
		lpMatches = imaqMatchColorPattern(apCurrentImage,
				mpTargetImage,
				&mcMatchTargetOptions,
				IMAQ_NO_RECT,
				&anNumMatches);
	}
	
	return lpMatches;
	
}

