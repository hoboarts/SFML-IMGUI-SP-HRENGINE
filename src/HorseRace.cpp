#include "HorseRace.hpp"

#include "App.hpp"

namespace
{
	const float timePerFrame = 1.f / 60.f;
	int raceCounter = 0;
	bool horseSwap = false;
	bool raceFinished = false;
	bool isGameFinished = false;
	bool areYouWin = false;
}

HorseRace::HorseRace(StateStack& stateStack, Context context)
	: State(stateStack, context),
	m_messageBus(context.appInstance.getMessageBus()),
	m_generatedStable(),
	m_chosenForRace(0),
	m_history(),
	selArr()
{
	auto& rw = getContext().renderWindow;
	ImGui::SFML::Init(rw);
	
	for (int i = 0; i < sizeof(selArr); i++)
	{
		selArr[i] = false;
	}
}

HorseRace::~HorseRace()
{
	ImGui::SFML::Shutdown();
}

//public
bool HorseRace::update(float dt)
{
	//updateDrawImgui(dt);
	return true;
}

void HorseRace::draw()
{
	drawHorseRace(timePerFrame);
}

bool HorseRace::handleEvent(const sf::Event& evt)
{
	ImGui::SFML::ProcessEvent(evt);

	return false; //consume events
}

void HorseRace::handleMessage(const Message& msg)
{
}

//private

void HorseRace::drawHorseRace(float dt)
{
	auto& rw = getContext().renderWindow;
	ImGui::SFML::Update(rw, static_cast<sf::Time>(sf::seconds(dt)));
	int selected = 0;
	static int clicked = 0;

	for (bool s : selArr)
	{
		if (s)
		{
			selected++;
		}
	}

	ImGui::Begin("HORSE GEN");

	ImGui::GetIO().FontGlobalScale = 1.81f;

	if (selected < 5 && !horseSwap && !isGameFinished)
	{
		for (int i = 0; i < m_generatedStable.getStable().size(); i++)
		{
			const std::string tempName = std::to_string(i + 1) + ". " + m_generatedStable.getHorse(i).getName();
			ImGui::Selectable(tempName.c_str(), &selArr[i]);
			
			//ImGui::SameLine();
			
			//ImGui::Text("%d. %s ", i + 1, m_generatedStable.getHorse(i).getName().c_str());
			//ImGui::SameLine();

			//ImGui::Checkbox(" ", &selArr[i]);

			ImGui::Separator();
		}
	}
	else if (!horseSwap)
	{
		for (int i = 0; i < m_generatedStable.getStable().size(); i++)
		{
			const std::string tempName = std::to_string(i + 1) + ". " + m_generatedStable.getHorse(i).getName();
			if (selArr[i])
			{
				ImGui::Selectable(tempName.c_str(), &selArr[i]);
				ImGui::Separator();
			}
			else
			{
				ImGui::Text("%s", tempName.c_str());
				ImGui::Separator();
			}
		}
	}
	else if (selected < 2 && horseSwap)
	{
		for (int i = 0; i < m_generatedStable.getStable().size(); i++)
		{
			const std::string tempName = std::to_string(i + 1) + ". " + m_generatedStable.getHorse(i).getName();
			ImGui::Selectable(tempName.c_str(), &selArr[i]);

			//ImGui::SameLine();

			//ImGui::Text("%d. %s ", i + 1, m_generatedStable.getHorse(i).getName().c_str());
			//ImGui::SameLine();

			//ImGui::Checkbox(" ", &selArr[i]);

			ImGui::Separator();
		}
	}
	else if (horseSwap)
	{
		for (int i = 0; i < m_generatedStable.getStable().size(); i++)
		{
			const std::string tempName = std::to_string(i + 1) + ". " + m_generatedStable.getHorse(i).getName();
			if (selArr[i])
			{
				ImGui::Selectable(tempName.c_str(), &selArr[i]);
				ImGui::Separator();
			}
			else
			{
				ImGui::Text("%s", tempName.c_str());
				ImGui::Separator();
			}
		}
	}
	
	if (horseSwap && selected == 2)
	{
		if (ImGui::Button("SWAP"))
			clicked = clicked + 5;
		if (clicked & 5)
		{
			for (int i = 0; i < sizeof(selArr); i++)
			{
				if (selArr[i])
				{
					selArr[i] = false;
					for (int j = 0; j < sizeof(selArr); j++)
					{
						if (selArr[j])
						{
							m_generatedStable.swapHorses(i,j);
						}
					}
				}
			}
			clicked = 0;
		}
	}

	if (isGameFinished)
	{
		if (areYouWin)
		{
			std::string temp = "YOU WIN IN RACE " + std::to_string(raceCounter);
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), temp.c_str());
		}
		else
		{
			ImGui::TextColored(ImVec4(0.0f, 0.5f, 1.0f, 1.0f), "YOU LOSE");
		}
	}
	
	if (!horseSwap && !isGameFinished)
	{
		if (ImGui::Button("FINISH GAME"))
			clicked++;
		if (clicked & 1)
		{
			HorseGeneration tempGen(0);
			const std::vector<Horse> tempStable = m_generatedStable.getStable();
			tempGen.setStable(tempStable);
			m_generatedStable.sortStable();

			for (int i = 0; i < 3; i++)
			{ 
				if (tempGen.getHorse(i).getName() == m_generatedStable.getHorse(i).getName())
				{
					areYouWin = true;
				}
				else
				{
					areYouWin = false;
				}
			}

			isGameFinished = true;

			clicked = 0;
		}
	}

	if (!isGameFinished)
	{
		ImGui::Checkbox("Horse swap", &horseSwap);
	}

	if (selected == 5 && m_chosenForRace.getStable().size() == 0 && !isGameFinished)
	{
		if (ImGui::Button("CHOOSE HORSES"))
			clicked = clicked + 2;
		if (clicked & 2)
		{
			std::vector<int> bufferArr;
			for (int i = 0; i < sizeof(selArr); i++)
			{
				if (selArr[i])
				{
					//Horse temp = m_generatedStable.replaceHorse(i);
					//m_chosenForRace.addHorse(temp);
					bufferArr.push_back(i);
					selArr[i] = false;
				}
			}

			for (int i = 0; i < bufferArr.size(); i++)
			{
				Horse temp = m_generatedStable.getHorse(bufferArr[i]);
				m_chosenForRace.addHorse(temp);
			}
			
			selected = 0;
			clicked = 0;
		}
	}
	
	/*
	ImGui::Text("selar 1 : %d", selArr[0]);
	*/

	ImGui::End();

	if (m_chosenForRace.getStable().size() > 0)
	{
		ImGui::Begin("CHOOSEN HORSES");

		for (int i = 0; i < m_chosenForRace.getStable().size(); i++)
		{
			ImGui::Text("%d. %s ", i + 1, m_chosenForRace.getHorse(i).getName().c_str());

			ImGui::Separator();
		}

		if (!raceFinished)
		{
			if (ImGui::Button("RACE GO!"))
				clicked = clicked + 3;
			if (clicked & 3)
			{
				raceCounter++;
				m_chosenForRace.sortStable();
				raceFinished = true;
				clicked = 0;
			}
		}
		else
		{
			if (ImGui::Button("CLOSE"))
				clicked = clicked + 4;
			if (clicked & 4)
			{
				writeHistory(raceCounter, &m_chosenForRace);
				m_chosenForRace.clearStable();
				raceFinished = false;
				clicked = 0;
			}
		}

		ImGui::End();
	}

	ImGui::Begin("HISTORY");

	for (std::string s : m_history.textVec)
	{
		if(s[0] == 49)//49-54
		{
			std::string tempStr = s.c_str();
			tempStr.erase(0,2);
			ImGui::Text(tempStr.c_str());
		}
		if (s[0] == 50)
		{
			std::string tempStr = s.c_str();
			tempStr.erase(0, 2);
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), tempStr.c_str());
		}
		if (s[0] == 51)
		{
			std::string tempStr = s.c_str();
			tempStr.erase(0, 2);
			ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), tempStr.c_str());
		}
		if (s[0] == 52)
		{
			std::string tempStr = s.c_str();
			tempStr.erase(0, 2);
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), tempStr.c_str());
		}
		if (s[0] == 53)
		{
			std::string tempStr = s.c_str();
			tempStr.erase(0, 2);
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), tempStr.c_str());
		}
		if (s[0] == 54)
		{
			std::string tempStr = s.c_str();
			tempStr.erase(0, 2);
			ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), tempStr.c_str());
		}
		if (s[0] == 55)
		{
			ImGui::Separator();
		}
	}

	ImGui::End();

	rw.clear(sf::Color
	(
		(sf::Uint8)(0),
		(sf::Uint8)(100),
		(sf::Uint8)(180)
	));

	ImGui::SFML::Render(rw);
}

void HorseRace::writeHistory(int rC, HorseGeneration* stable)
{
	//std::string textRaceCount = "Race # " + rC;
	//std::string winnerName = "Winner : " + stable->getHorse(0).getName();
	//std::string secondName = "Second place : " + stable->getHorse(1).getName();
	//std::string thirdName = "Third place : " + stable->getHorse(2).getName();
	//std::string fourthName = "Fourth place : " + stable->getHorse(3).getName();
	//std::string fifthName = "Fifth place : " + stable->getHorse(4).getName();
	m_history.addLine("1 Race # " + std::to_string(rC));
	m_history.addLine("2 Winner : " + stable->getHorse(0).getName());
	m_history.addLine("3 Second place : " + stable->getHorse(1).getName());
	m_history.addLine("4 Third place : " + stable->getHorse(2).getName());
	m_history.addLine("5 Looser 1 : " + stable->getHorse(3).getName());
	m_history.addLine("6 Looser 2 : " + stable->getHorse(4).getName());
	m_history.addLine("7");
}