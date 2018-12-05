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
	// create ASM
	TaskLevelModifiers.push_back(new MPS_CSModifier((Character*)Self, MPS_Resource::tASM));
	// create PSM
	for (auto& p : Players) {
		TaskLevelModifiers.push_back(new MPS_CSModifier((Character*)p, MPS_Resource::tPSM));
	}
	// create FSM
	for (auto& f : Friends) {
		TaskLevelModifiers.push_back(new MPS_CSModifier((Character*)f, MPS_Resource::tFSM));
	}
}

void MPS_Main::createTasks(Enemy* Self, std::vector<Player*> Players, std::vector<Enemy*> Friends) {
	for (int i = 0; i < MPS_Resource::tMPS_TASK_TYPE_NUM; i++) {
		// for testing 
		//std::cout << "Creating Tasks: " << i << " / " << MPS_Resource::tMPS_TASK_TYPE_NUM << std::endl;
		////////////////////////////////////

		std::vector<Ability*> UsableAbilities;
		for (auto& a : Abilities) {
			if (a->getMPSTaskType() == i) UsableAbilities.push_back(a);
		}
		std::vector<Character*> tar;
		std::vector<MPS_Modifier*> ms;
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
			for (auto& tlm : TaskLevelModifiers) {
				if (tlm->getType() == MPS_Resource::tPSM) ms.push_back(tlm);
			}
			break;
		case tBUFF_SELF_DEF:
		case tBUFF_SELF_OFF:
		case tRECOVER_SELF_HP:
		case tADD_SELF_RE:
		case tMPS_TASK_ESCAPE:
			tar.push_back((Character*)Self);
			ms.push_back(TaskLevelModifiers[0]);
			break;
		default:
			for (auto& f : Friends) {
				tar.push_back((Character*)f);
			}
			for (auto& tlm : TaskLevelModifiers) {
				if (tlm->getType() == MPS_Resource::tFSM) ms.push_back(tlm);
			}
			break;
		}
		Tasks.push_back(MPS_Task(i, UsableAbilities, tar, TaskBasePriority[i], ms));
	}
}

void MPS_Main::findBestAction() {
	float MaxScore = 0;
	for (auto& t : Tasks) {
		//std::cerr << t.getBestAssignment().getAction().getAbil()->getName() << std::endl;
		//std::cerr << t.getBestAssignment().getScore() << std::endl;
		//std::cerr << "Current MaxScore is: " << MaxScore << std::endl;
		if (t.getBestScore() > MaxScore)
		{
			BestAction = t.getBestAssignment().getAction();
			MaxScore = t.getBestScore();
		}
		else if (t.getBestScore() == MaxScore) {
			if (rand() % 100 > 49) {
				BestAction = t.getBestAssignment().getAction();
				MaxScore = t.getBestScore();
			}
		}
	}
}

MPS_Main::MPS_Main() {}

MPS_Main::MPS_Main(Enemy* Self, std::vector<Player*> Players, std::vector<Enemy*> Friends) {
	createAbilities(Self);
	readTBP();
	createTLMs(Self, Players, Friends);
	createTasks(Self, Players, Friends);
	findBestAction();
}

/*
MPS_Main::~MPS_Main() {
	for (int i = 0; i < TaskLevelModifiers.size(); i++) {
		std::cout << "TLMs Size: "<< TaskLevelModifiers.size() << std::endl;
		MPS_Modifier* tlm = TaskLevelModifiers[i];
		delete tlm;
		tlm = nullptr;
	}
	
	TaskLevelModifiers.clear();
	std::cout << "TLMs Size: " << TaskLevelModifiers.size() << std::endl;
}
//*/

Action MPS_Main::getBestAction() {
	return BestAction;
}

std::vector<MPS_Modifier*> MPS_Main::getTLMs() {
	return TaskLevelModifiers;
}