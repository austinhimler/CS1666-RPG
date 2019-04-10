#include "../Headers/Cluster.h"

Cluster::Cluster(int q) {
	for (int temp = 0;temp < q;temp++)
	{
		string t_name = "Owl ";
		t_name.push_back(char(temp + 65));
		characterGroup.push_back(new Enemy(t_name, rand() % 10 + 1, rand() % 10 + 1, rand() % 10 + 1, rand() % 2 + 1, rand() % 10 + 1));
	}
	clusterSize = q;
	currentFrame = 0;
	timeBetweenAnimations = 120;

	std::vector<string> tempSprite(13);
	std::vector<int> tempWidth(13);
	std::vector<int> tempHeight(13);
	std::vector<int> tempImages(13);

	tempSprite.at(NOT_READY) = "Images/Enemies/shadow_cluster/OWL_BROWN_NOT_READY.png";
	tempWidth.at(NOT_READY) = 112;
	tempHeight.at(NOT_READY) = 136;
	tempImages.at(NOT_READY) = 6;

	tempSprite.at(IDLE_DOWN) = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";
	tempWidth.at(IDLE_DOWN) = 112;
	tempHeight.at(IDLE_DOWN) = 136;
	tempImages.at(IDLE_DOWN) = 6;

	tempSprite.at(IDLE_RIGHT) = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";
	tempWidth.at(IDLE_RIGHT) = 112;
	tempHeight.at(IDLE_RIGHT) = 136;
	tempImages.at(IDLE_RIGHT) = 6;

	tempSprite.at(IDLE_LEFT) = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";
	tempWidth.at(IDLE_LEFT) = 112;
	tempHeight.at(IDLE_LEFT) = 136;
	tempImages.at(IDLE_LEFT) = 6;

	tempSprite.at(IDLE_UP) = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";
	tempWidth.at(IDLE_UP) = 112;
	tempHeight.at(IDLE_UP) = 136;
	tempImages.at(IDLE_UP) = 7;

	tempSprite.at(RUN_DOWN) = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";
	tempWidth.at(RUN_DOWN) = 112;
	tempHeight.at(RUN_DOWN) = 136;
	tempImages.at(RUN_DOWN) = 6;

	tempSprite.at(RUN_RIGHT) = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";
	tempWidth.at(RUN_RIGHT) = 112;
	tempHeight.at(RUN_RIGHT) = 136;
	tempImages.at(RUN_RIGHT) = 6;

	tempSprite.at(RUN_LEFT) = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";
	tempWidth.at(RUN_LEFT) = 112;
	tempHeight.at(RUN_LEFT) = 136;
	tempImages.at(RUN_LEFT) = 6;

	tempSprite.at(RUN_UP) = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";
	tempWidth.at(RUN_UP) = 112;
	tempHeight.at(RUN_UP) = 136;
	tempImages.at(RUN_UP) = 6;

	tempSprite.at(RUN_DOWN_RIGHT) = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";
	tempWidth.at(RUN_DOWN_RIGHT) = 112;
	tempHeight.at(RUN_DOWN_RIGHT) = 136;
	tempImages.at(RUN_DOWN_RIGHT) = 6;

	tempSprite.at(RUN_DOWN_LEFT) = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";
	tempWidth.at(RUN_DOWN_LEFT) = 112;
	tempHeight.at(RUN_DOWN_LEFT) = 136;
	tempImages.at(RUN_DOWN_LEFT) = 6;

	tempSprite.at(RUN_UP_RIGHT) = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";
	tempWidth.at(RUN_UP_RIGHT) = 112;
	tempHeight.at(RUN_UP_RIGHT) = 136;
	tempImages.at(RUN_UP_RIGHT) = 6;

	tempSprite.at(RUN_UP_LEFT) = "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png";
	tempWidth.at(RUN_UP_LEFT) = 112;
	tempHeight.at(RUN_UP_LEFT) = 136;
	tempImages.at(RUN_UP_LEFT) = 6;

	Helper helper = Helper();
	for (int index = 0; index < tempSprite.size(); index++)
	{
		spriteTextures.push_back(helper.loadImage(tempSprite.at(index).c_str(), gRenderer));
		spriteWidths.push_back(tempWidth.at(index));
		spriteHeights.push_back(tempHeight.at(index));
		spriteImages.push_back(tempImages.at(index));
	}

	spriteSheetNumber = IDLE_DOWN;



	rectangle = { (int)xPosition, (int)yPosition, getImageWidth(), getImageHeight() };
	drawRectangle = { 0, 0, getImageWidth(), getImageHeight() };

	xPosition = 400.0;
	yPosition = 200.0;
	speedMax = 250.0;
	acceleration = 180;
	combatReady = true;
	readyTimeLeft = -1;

	targetPlayer = NULL;
	targetTileX = -1;
	targetTileY = -1;
	pursuitRange = 400;
	partialSteps = 0;
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
	targetTileX = (int)(p->xTile);
	targetTileY = (int)(p->yTile);
}
void Cluster::clearTarget()
{
	targetPlayer = NULL;
	targetTileX = -1;
	targetTileY = -1;
	currentPath.clear();
}
void Cluster::findRandom(Tile* map[MAX_HORIZONTAL_TILES][MAX_VERTICAL_TILES])
{
	Path p(map);
	pathOffset = 0;
	currentPath = p.makeRandomPath(xTile, yTile);
}
void Cluster::findPath(Tile* map[MAX_HORIZONTAL_TILES][MAX_VERTICAL_TILES])
{
	Path p(map);
	pathOffset = 0;
	currentPath = p.makePath(xTile, yTile, targetTileX, targetTileY);
}
void Cluster::moveSteps(double time)
{
	if (currentPath.size() == 0 || (currentPath.size() <= pathOffset))
	{
		clearTarget();
	}
	else
	{
		partialSteps += (time * acceleration);
		int steps = (int)(partialSteps);
		partialSteps -= steps;
		for (int stepsTaken = 0;stepsTaken < steps && currentPath.size() > pathOffset;stepsTaken++)
		{
			Point* next = currentPath.at(pathOffset);
			if (next->x > xTile)
				xPosition++;
			else if (next->x < xTile)
				xPosition--;
			else if (next->y < yTile)
				yPosition--;
			else if (next->y > yTile)
				yPosition++;
			else cout << "ERR" << endl;
			xTile = (int)((xPosition + rectangle.w / 2) / TILE_WIDTH);
			yTile = (int)((yPosition + rectangle.h) / TILE_HEIGHT);
			if (xTile == next->x && yTile == next->y)
				pathOffset++;
		}
	}
}