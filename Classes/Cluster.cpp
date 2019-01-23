#include "../Headers/Cluster.h"

Cluster::Cluster(int q) {
	for (int temp = 0;temp < q;temp++)
	{
		string t_name = "Owl ";
		t_name.push_back(char(temp + 65));
		characterGroup.push_back(new Enemy(t_name, rand() % 10 + 1, rand() % 10 + 1, rand() % 10 + 1, rand() % 2 + 1, rand() % 10 + 1));
		//characterGroup.push_back(new Enemy(t_name, 10, 10, 10, 10, 10));
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

	targetPlayer = NULL;
	targetX = -1;
	targetY = -1;
	pursuitRange = 300;
}
Cluster::Cluster(std::string n)
{
	name = n;
	Cluster(1);
}
Cluster::Cluster() : Cluster("Sample"){}


std::string Cluster::ptoString()
{
	//previous attempt, new attempt uses a stream
	//std::string pString = name + " " + std::to_string(currentFrame) + " " + std::to_string(xPosition) + " " +
	//	std::to_string(yPosition) + " " + std::to_string(xVelocity) + " " + std::to_string(yVelocity);
	std::stringstream st;
	st << xPosition << " " << yPosition << " " <<xVelocity << " " <<yVelocity << " " << clusterSize << " Z";
	std::cout << st.str() << std::endl;
	return st.str();
}
int Cluster::isAlive()
{
	for (auto z : characterGroup)
	{
		if (z->isAlive())
			return 1;
	}
	return 0;
}
void Cluster::fromString(std::string in)
{
	std::vector<std::string> vars;
	size_t pos = 0;
	std::string token;
	while ((pos = in.find(" ")) != std::string::npos) {
		token = in.substr(0, pos);
		//std::cout << "Parsed: " << token << endl;
		vars.push_back(token);
		in.erase(0, pos + 1);
	}
	if (vars.size() > 4)
	{
		//name = vars[0];
		xPosition = atof(vars[0].c_str());
		yPosition = atof(vars[1].c_str());
		xVelocity = atof(vars[2].c_str());
		yVelocity = atof(vars[3].c_str());
		clusterSize = (int) atof(vars[4].c_str());

		return;
	}

}
void Cluster::setTarget(Character* p)
{
	targetPlayer = p;
	targetX = p->xPosition;
	targetY = p->yPosition;
}
void Cluster::clearTarget()
{
	targetPlayer = NULL;
	targetX = -1;
	targetY = -1;
}
void Cluster::findPath(Tile* map[MAX_HORIZONTAL_TILES][MAX_VERTICAL_TILES])
{
	Path p(map);
	pathOffset = 0;
	currentPath = p.makePath(xPosition/MAX_INSIDE_TILE, yPosition/ MAX_INSIDE_TILE, targetX/ MAX_INSIDE_TILE, targetY/ MAX_INSIDE_TILE);
}
void Cluster::moveSteps(double time)
{
	pathOffset += (int)(time * (acceleration/MAX_INSIDE_TILE));
	Point* loc = currentPath.at(pathOffset);
	xPosition = loc->x;
	yPosition = loc->y;
}