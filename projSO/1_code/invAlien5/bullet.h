
//void Bullet(Setup setup, GameObject* bullet);

void Bullet(int* p, int* pipe_astronave,Setup setup, GameObject* astronaveMadre, int id_bullet);
void setBullet(Setup setup, GameObject* bullet);
void hideBullet(Setup setup, GameObject* bullet);
void hideAllBullet(Setup setup, GameObject* bullet);
bool bulletTimer(int* old_time, int* new_time, int timer);
