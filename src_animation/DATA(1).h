#pragma once
const GLfloat wait_color[3] = { 0.4f, 0.5f, 0.8f };
const GLfloat warning_color[3] = {0.8f, 0.2f, 0.2f};

char m_bmpPath[100] = "";
int m_bmpTextureId = -1;

Qtcon* m_qtCon = NULL;

Kinect* m_kinect;

int screenshot_time = -1;
clock_t clock_past = -1;
clock_t wait_past = -1;

bool FIRST_TO_START = false;
bool GET_GESTURE = true;

NUI_SKELETON_DATA m_skeletonData;
GestureTracker m_gestureTracker;
guesture_state m_gestrueState;

Costume hat(costumeType::HAT);
Costume cloth(costumeType::CLOTH);
Costume trouser(costumeType::TROUSER);
Costume shoes(costumeType::SHOES);

const std::string dataPath("../data/");
const std::string Separate("/");
const std::string materialSuffix(".material");
const std::string meshSuffix(".mesh.xml");
const std::string skeletonSuffix(".skeleton.xml");
std::string material("../data/p33/p33.material");
std::string mesh("../data/p33/p33.mesh.xml");
std::string skeleton("../data/p33/p33.skeleton.xml");