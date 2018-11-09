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
	currentFrame = 0;
	timeBetweenIdleAnimations = 120;
	imageIdleResource = "Images/Enemies/shadow_cluster/OWL_BROWN.png";

	Helper helper = Helper();
	textureIdle = helper.loadImage(imageIdleResource.c_str(), gRenderer);
	textureActive = NULL;

	xPosition = 400.0;
	yPosition = 200.0;
	rectangle = { (int)xPosition, (int)yPosition, imageWidth, imageHeight };
	drawRectangle = { 0, 0, imageWidth, imageHeight };
	speedMax = 250.0;
	acceleration = 2000.0;
	alive = true;
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