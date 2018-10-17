#include "../Headers/Cluster.h"
#include "../Headers/Helper.h"


Cluster::Cluster(int q) {
	for (int temp = 0;temp < q;temp++)
	{
		Character c = Character("Owl");
		characterGroup.push_back(c);
	}
	clusterSize = q;
	pixelShiftAmountForAnimationInSpriteSheet = 160;
	imageWidth = 160;
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