#include "../Headers/Cluster.h"
#include "../Headers/Helper.h"
#include "../Headers/Globals.h"



Cluster::Cluster(int q) {
	for (int temp = 0;temp < q;temp++)
	{
		characterGroup.push_back(new Enemy("Owl", 1, 1, 1, 1, 1));
	}
	clusterSize = q;
	pixelShiftAmountForAnimationInSpriteSheet = 200;
	imageWidth = 200;
	imageHeight = 148;

	numIdleAnimationFrames = 6;
	currentFrame = 0;
	timeBetweenIdleAnimations = 120;
	imageIdleResource = "Images/Enemies/shadow_cluster/owl.png";

	Helper helper = Helper();
	textureIdle = helper.loadImage(imageIdleResource.c_str(), gRenderer);
	textureActive = NULL;

	xPosition = 400.0;
	yPosition = 200.0;
	rectangle = { (int)xPosition, (int)yPosition, imageWidth, imageHeight };
	drawRectangle = { 0, 0, imageWidth, imageHeight };
	speedMax = 250.0;
	acceleration = 2000.0;
}
Cluster::Cluster() {
	Cluster("Sample");
}
Cluster::Cluster(std::string n) {
	name = n;
	Cluster(1);
}
/*
Cluster::Cluster(std::string n, int q) {
	name = n;
	Cluster(q);
}
*/