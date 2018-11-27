#include "../../Headers/AI/MPS_Main.h"

std::vector<int> MPS_Main::TaskBasePriority = std::vector<int>();

void MPS_Main::createAbilities(Enemy* Self) {
	Abilities = Self->getAbilityPointers();
}

void MPS_Main::readTBP() {
	if (TaskBasePriority.size() != 0) return;
	std::string Path = "Data/AI/MPS_TBP.csv";
	MPS_Data TBP = MPS_Data(Path);
	TaskBasePriority = TBP.getTBP();
}

void MPS_Main::createTLMs(Enemy* Self, std::vector<Player*> Players, std::vector<Enemy*> Friends) {
	TaskLevelModifiers.push_back(new MPS_ASModifier(Self));
}

void MPS_Main::createTasks(Enemy* Self, std::vector<Player*> Players, std::vector<Enemy*> Friends) {
	for (int i = 0; i < MPS_Resource::tMPS_TASK_TYPE_NUM; i++) {
		std::vector<Ability*> UsableAbilities;
		for (auto& a : Abilities) {
			if (a->getMPSTaskType() == i) UsableAbilities.push_back(a);
		}
		std::vector<Character*> tar;
		switch (i) {
			using namespace MPS_Resource;
		case tMPS_TASK_DAMAGE:
		case tDEBUFF_PLAYER_DEF:
		case tDEBUFF_PLAYER_OFF:
		case tDISPEL_PLAYER_HPBUFF:
		case tDEC_PLAYER_RE:
		case tSTOP_PLAYER_ACT:
			for (auto& p : Players) {
				tar.push_back((Character*)p);
			}
			break;
		case tBUFF_SELF_DEF:
		case tBUFF_SELF_OFF:
		case tRECOVER_SELF_HP:
		case tADD_SELF_RE:
		case tMPS_TASK_ESCAPE:
			tar.push_back((Character*)Self);
			break;
		default:
			for (auto& f : Friends) {
				tar.push_back((Character*)f);
			}
			break;
		}
		Tasks.push_back(MPS_Task(i, UsableAbilities, tar, TaskBasePriority[i], TaskLevelModifiers));
	}
}

void MPS_Main::findBestAction() {
	float MaxScore = 0;
	for (auto& t : Tasks) {
		//std::cerr << t.getBestAssignment().getAction().getAbil()->getName() << std::endl;
		//std::cerr << t.getBestAssignment().getScore() << std::endl;
		//std::cerr << "Current MaxScore is: " << MaxScore << std::endl;
		if (t.getBestScore() > MaxScore) 
			BestAction = t.getBestAssignment().getAction();
		else if (t.getBestScore() == MaxScore) {
			if (rand() % 100 > 49) BestAction = t.getBestAssignment().getAction();
		}
	}
}

MPS_Main::MPS_Main(Enemy* Self, std::vector<Player*> Players, std::vector<Enemy*> Friends) {
	createAbilities(Self);
	readTBP();
	createTLMs(Self, Players, Friends);
	createTasks(Self, Players, Friends);
	findBestAction();
}

MPS_Main::~MPS_Main() {
	for (auto& tlm : TaskLevelModifiers) {
		delete tlm;
	}
}

Action MPS_Main::getBestAction() {
	return BestAction;
}