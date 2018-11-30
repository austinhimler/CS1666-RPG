#include "../Headers/Cluster.h"
#include "../Headers/Helper.h"
#include "../Headers/Globals.h"

Cluster::Cluster(int q) {
	for (int temp = 0;temp < q;temp++)
	{
		string t_name = "Owl ";
		t_name.push_back(char(temp + 65));
		characterGroup.push_back(new Enemy(t_name, 1, 1, 1, 1, 1));
	}
	clusterSize = q;
	pixelShiftAmountForAnimationInSpriteSheet = 144;
	imageWidth = 144;
	imageHeight = 144;

	numIdleAnimationFrames = 6 ;
	numRunAnimatonFrames = 6;
	currentFrame = 0;
	timeBetweenIdleAnimations = 120;
	timeBetweenRunAnimations = 120;
	imageIdleResource = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";
	imageIdleNotReadyResource = "Images/Enemies/shadow_cluster/OWL_BROWN_NOT_READY.png";
	imageRightIdleResource = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";;
	imageRunResource  = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";
	imageDownRunResource = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";
	imageDownRightRunResource = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";
	imageUpRightRunResource = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";
	imageUpRunResource = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";
	lastDirection = NULL;

	Helper helper = Helper();
	textureIdle = helper.loadImage(imageIdleResource.c_str(), gRenderer);
	textureRightIdle = helper.loadImage(imageRightIdleResource.c_str(), gRenderer);
	textureRun = helper.loadImage(imageRunResource.c_str(), gRenderer);
	textureDownRun = helper.loadImage(imageDownRunResource.c_str(), gRenderer);
	textureDownRightRun = helper.loadImage(imageDownRightRunResource.c_str(), gRenderer);
	textureUpRightRun = helper.loadImage(imageUpRightRunResource.c_str(), gRenderer);
	textureUpRun = helper.loadImage(imageUpRunResource.c_str(), gRenderer);
	textureIdleNotReady = helper.loadImage(imageIdleNotReadyResource.c_str(), gRenderer);
	textureActive = NULL;

	xPosition = 400.0;
	yPosition = 200.0;
	rectangle = { (int)xPosition, (int)yPosition, imageWidth, imageHeight };
	drawRectangle = { 0, 0, imageWidth, imageHeight };
	speedMax = 250.0;
	acceleration = 2000.0;
	combatReady = true;
	readyTimeLeft = -1;
}
Cluster::Cluster(std::string n) {
	name = n;
	Cluster(1);
}
Cluster::Cluster() : Cluster("Sample"){}

/*
Cluster::Cluster(std::string n, int q) {
	name = n;
	Cluster(q);
}
*/